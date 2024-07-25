/*
 * Copyright (c) 2023  Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <dlfcn.h>
#include <dirent.h>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sys/file.h>
#include <fcntl.h>
#include "iremote_broker.h"
#include "ipc_skeleton.h"
#include "drm_dfx.h"
#include "drm_dfx_utils.h"
#include "drm_log.h"
#include "drm_error_code.h"
#include "napi_param_utils.h"
#include "drm_host_manager.h"

namespace OHOS {
namespace DrmStandard {
using namespace OHOS::HiviewDFX;
std::queue<Message> DrmHostManager::messageQueue;
std::mutex DrmHostManager::queueMutex;
std::condition_variable DrmHostManager::cv;

const int32_t LAZY_UNLOAD_TIME_CHECK_IN_MINUTES = 1;
const int32_t LAZY_UNLOAD_TIME_IN_MINUTES = 3;
const int32_t NOT_LAZY_LOADDED = -65536;

DrmHostManager::DrmHostDeathRecipient::DrmHostDeathRecipient(
    const sptr<DrmHostManager> &drmHostManager, std::string &name)
    : drmHostManager_(drmHostManager)
{
    name_ = name;
    DRM_DEBUG_LOG("DrmHostDeathRecipient");
}

DrmHostManager::DrmHostDeathRecipient::~DrmHostDeathRecipient()
{
    DRM_DEBUG_LOG("DrmHostManager::~DrmHostDeathRecipient");
}
void DrmHostManager::OnDrmPluginDied(std::string &name)
{
}
void DrmHostManager::DrmHostDeathRecipient::OnRemoteDied(const wptr<IRemoteObject> &remote)
{
    DRM_ERR_LOG("Remote service died, do clean works.");
    drmHostManager_->ClearDeathService(name_);
}

void DrmHostManager::ClearDeathService(std::string &name)
{
    DRM_INFO_LOG("ClearDeathService enter.");
    std::lock_guard<std::recursive_mutex> lock(drmHostMapMutex);
    if (lazyLoadPluginInfoMap.count(name) <= 0) {
        DRM_DEBUG_LOG("PluginCountInfo is empty, name:%{public}s",
            name.c_str());
        return;
    }
    if (lazyLoadPluginCountMap.empty()) {
        DRM_DEBUG_LOG("PluginCountMap is empty.");
        return;
    }
    if (statusCallback_ != nullptr) {
        statusCallback_->OnDrmPluginDied(name);
    }
    lazyLoadPluginCountMap[name] = NOT_LAZY_LOADDED;
    lazyLoadPluginTimeoutMap[name] = NOT_LAZY_LOADDED;
    for (auto it = hdiMediaKeySystemFactoryAndPluginNameMap.begin(); it != hdiMediaKeySystemFactoryAndPluginNameMap.end();) { 
        if (it->second == name) {
            it = hdiMediaKeySystemFactoryAndPluginNameMap.erase(it);
        } else {
            ++it;
        }
    }
}

DrmHostManager::DrmHostManager(StatusCallback *statusCallback) : statusCallback_(statusCallback)
{}

DrmHostManager::~DrmHostManager()
{
    DRM_INFO_LOG("~DrmHostManager enter.");
    statusCallback_ = nullptr;
    if (serviceThreadRunning) {
        StopServiceThread();
    }
}

void DrmHostManager::StopServiceThread()
{
    DRM_INFO_LOG("StopServiceThread enter.");
    serviceThreadRunning = false;
    if (serviceThread.joinable()) {
        serviceThread.join();
    }
    for (auto libHandle : loadedLibs) {
        StopThreadFuncType StopThread = (StopThreadFuncType)dlsym(libHandle, "StopThread");
        if (StopThread) {
            StopThread();
        }
        dlclose(libHandle);
        libHandle = nullptr;
    }
    UnloadAllServices();
    loadedLibs.clear();

    ReportServiceBehaviorEvent("DRM_OEM_SERVICE", "end");
}

void DrmHostManager::DelayedLazyUnLoad()
{
    DRM_INFO_LOG("DelayedLazyUnLoad enter.");
    sptr<IDeviceManager> deviceMgr = IDeviceManager::Get();
    if (deviceMgr == nullptr || lazyLoadPluginInfoMap.empty()) {
        return;
    }
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    for (auto pluginInfoIt = lazyLoadPluginInfoMap.begin(); pluginInfoIt != lazyLoadPluginInfoMap.end();
        pluginInfoIt++) {
        DRM_DEBUG_LOG("ProcessMessage check lazy unload, name:%{public}s, Count:%{public}d,"
            "Timeout:%{public}d", pluginInfoIt->second.c_str(), lazyLoadPluginCountMap[pluginInfoIt->first],
            lazyLoadPluginTimeoutMap[pluginInfoIt->first]);
        if (lazyLoadPluginCountMap[pluginInfoIt->first] <= NOT_LAZY_LOADDED ||
            lazyLoadPluginTimeoutMap[pluginInfoIt->first] <= NOT_LAZY_LOADDED) {
            DRM_DEBUG_LOG("ProcessMessage not need to unload");
            continue;
        }
        if (lazyLoadPluginCountMap[pluginInfoIt->first] <= 0) {
            if (lazyLoadPluginTimeoutMap[pluginInfoIt->first] <= LAZY_UNLOAD_TIME_CHECK_IN_MINUTES) {
                deviceMgr->UnloadDevice(pluginInfoIt->second);
                DRM_INFO_LOG("ProcessMessage UnloadDevice:%{public}s.", pluginInfoIt->second.c_str());
                lazyLoadPluginCountMap[pluginInfoIt->first] = NOT_LAZY_LOADDED;
                lazyLoadPluginTimeoutMap[pluginInfoIt->first] = NOT_LAZY_LOADDED;
            } else {
                lazyLoadPluginTimeoutMap[pluginInfoIt->first] -= LAZY_UNLOAD_TIME_CHECK_IN_MINUTES;
            }
        }
    }
}

void DrmHostManager::ProcessMessage()
{
    DRM_INFO_LOG("ProcessMessage enter.");
    std::thread([this] {
        while (serviceThreadRunning) {
            std::unique_lock<std::mutex> queueMutexLock(queueMutex);
            cv.wait_for(queueMutexLock, std::chrono::minutes{LAZY_UNLOAD_TIME_CHECK_IN_MINUTES}, [this] {
                return (!this->messageQueue.empty() || !serviceThreadRunning);
            });
            while (!messageQueue.empty()) {
                auto message = messageQueue.front();
                DRM_DEBUG_LOG("ProcessMessage message type:%{public}d.", message.type);
                messageQueue.pop();
                if (message.type == Message::UnLoadOEMCertifaicateService) {
                    std::lock_guard<std::recursive_mutex> lock(drmHostMapMutex);
                    void *libHandle = pluginNameAndHandleMap[message.name];
                    if (libHandle != nullptr) {
                        ReleaseHandleAndKeySystemMap(libHandle);
                        loadedLibs.erase(std::remove(loadedLibs.begin(), loadedLibs.end(), libHandle),
                            loadedLibs.end());
                        DRM_DEBUG_LOG("ProcessMessage UnLoadOEMCertifaicateService success.");
                    }
                }
            }
            queueMutexLock.unlock();
            if (!serviceThreadRunning) {
                break;
            }
            DRM_DEBUG_LOG("DrmHostManager::ProcessMessage lazy unload start.");
            DelayedLazyUnLoad();
        }
    }).detach();
}

void DrmHostManager::ReleaseHandleAndKeySystemMap(void *handle)
{
    DRM_INFO_LOG("ReleaseHandleAndKeySystemMap enter.");
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    sptr<IMediaKeySystem> hdiMediaKeySystem = handleAndKeySystemMap[handle];
    if (hdiMediaKeySystem != nullptr) {
        ReleaseMediaKeySystem(hdiMediaKeySystem);
        hdiMediaKeySystem->Destroy();
        hdiMediaKeySystem = nullptr;
        DRM_INFO_LOG("ReleaseHandleAndKeySystemMap ReleaseMediaKeySystem end");
    } else {
        DRM_INFO_LOG("ReleaseHandleAndKeySystemMap handle not found.");
    }
    handleAndKeySystemMap.erase(handle);

    if (handle != nullptr) {
        loadedLibs.erase(std::remove(loadedLibs.begin(), loadedLibs.end(), handle), loadedLibs.end());
        dlclose(handle);
        handle = nullptr;
        DRM_INFO_LOG("ReleaseHandleAndKeySystemMap handle closed");
    }
}

void DrmHostManager::GetOemLibraryPath(std::vector<std::string> &libsToLoad)
{
    DIR *dir = nullptr;
    struct dirent *entry = nullptr;
    if ((dir = opendir(OEM_CERTIFICATE_PATH)) != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            std::string fileName = entry->d_name;
            DRM_DEBUG_LOG("ServiceThreadMain fileName:%{public}s.", fileName.c_str());
            if (fileName.find(".so") == std::string::npos) {
                continue;
            }
            std::string fullPath = OEM_CERTIFICATE_PATH + fileName;
            DRM_DEBUG_LOG("ServiceThreadMain fullPath:%{public}s.", fullPath.c_str());
            libsToLoad.push_back(fullPath);
        }
        closedir(dir);
        dir = nullptr;
    }
}

void DrmHostManager::ServiceThreadMain() __attribute__((no_sanitize("cfi")))
{
    DRM_INFO_LOG("ServiceThreadMain enter.");
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    std::vector<std::string> libsToLoad;
    GetOemLibraryPath(libsToLoad);
    for (const auto &libpath : libsToLoad) {
        void *handle = dlopen(libpath.c_str(), RTLD_LAZY);
        if (handle == nullptr) {
            continue;
        }
        loadedLibs.push_back(handle);
        auto QueryMediaKeySystemName = (QueryMediaKeySystemNameFuncType)dlsym(handle, "QueryMediaKeySystemName");
        auto IsProvisionRequired = (IsProvisionRequiredFuncType)dlsym(handle, "IsProvisionRequired");
        auto SetMediaKeySystem = (SetMediaKeySystemFuncType)dlsym(handle, "SetMediaKeySystem");
        auto ThreadExitNotify = (ThreadExitNotifyFuncType)dlsym(handle, "ThreadExitNotify");
        auto StartThread = (StartThreadFuncType)dlsym(handle, "StartThread");
        if (QueryMediaKeySystemName && IsProvisionRequired && SetMediaKeySystem && ThreadExitNotify &&
            StartThread) {
            std::string pluginName;
            int32_t ret = QueryMediaKeySystemName(pluginName);
            if (ret != DRM_OK) {
                ReleaseHandleAndKeySystemMap(handle);
                DRM_ERR_LOG("QueryMediaKeySystemName error!");
                continue;
            }
            sptr<IMediaKeySystem> hdiMediaKeySystem = nullptr;
            ret = CreateMediaKeySystem(pluginName, hdiMediaKeySystem);
            if (ret != DRM_OK || hdiMediaKeySystem == nullptr) {
                ReleaseHandleAndKeySystemMap(handle);
                DRM_ERR_LOG("CreateMediaKeySystem error!");
                continue;
            }
            pluginNameAndHandleMap[pluginName] = handle;
            handleAndKeySystemMap[handle] = hdiMediaKeySystem;
            ret = SetMediaKeySystem(hdiMediaKeySystem);
            if (ret != DRM_OK) {
                ReleaseHandleAndKeySystemMap(handle);
                DRM_ERR_LOG("SetMediaKeySystem error!");
                continue;
            }
            if (IsProvisionRequired() == false) {
                ReleaseHandleAndKeySystemMap(handle);
                DRM_DEBUG_LOG("Provision not required!");
                continue;
            }
            ret = ThreadExitNotify(DrmHostManager::UnLoadOEMCertifaicateService);
            if (ret != DRM_OK) {
                ReleaseHandleAndKeySystemMap(handle);
                DRM_ERR_LOG("ThreadExitNotify error!");
                continue;
            }
            ret = StartThread();
            if (ret != DRM_OK) {
                ReleaseHandleAndKeySystemMap(handle);
                DRM_ERR_LOG("StartThread error!");
            }
        }
    }
    ReportServiceBehaviorEvent("DRM_OEM_SERVICE", "start");
}

void DrmHostManager::UnLoadOEMCertifaicateService(std::string &name, ExtraInfo info)
{
    DRM_INFO_LOG("UnLoadOEMCertifaicateService enter.");
    std::lock_guard<std::mutex> lock(queueMutex);
    Message message(Message::UnLoadOEMCertifaicateService, name, info);
    messageQueue.push(message);
    cv.notify_all();
}

void DrmHostManager::OemCertificateManager()
{
    DRM_INFO_LOG("OemCertificateManager enter.");
    serviceThreadRunning = true;
    serviceThread = std::thread([this] {
        this->ServiceThreadMain();
    });
    ProcessMessage();
}

int32_t DrmHostManager::Init(void)
{
    DRM_INFO_LOG("Init enter.");
    LoadPluginInfo(PLUGIN_LAZYLOAD_CONFIG_PATH);
    InitGetMediaKeySystems();
    OemCertificateManager();
    return DRM_OK;
}

void DrmHostManager::DeInit(void)
{
    DRM_INFO_LOG("DeInit");
}

void DrmHostManager::OnReceive(const HDI::ServiceManager::V1_0::ServiceStatus &status)
{}

std::string DrmHostManager::StringTrim(const std::string &str)
{
    DRM_INFO_LOG("StringTrim enter.");
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

void DrmHostManager::parseLazyLoadService(
    std::ifstream &file, std::map<std::string, std::string> &lazyLoadPluginInfoMap)
{
    DRM_INFO_LOG("parseLazyLoadService enter.");
    std::string line;
    while (getline(file, line)) {
        line = StringTrim(line);
        if (line == "]" || line == "],") {
            break;
        }
        if (!line.empty() && line.front() == '"') {
            /* Remove front quotation marks */
            line = line.substr(1);
            if (!line.empty() && (line.back() == '"' || line.back() == ',')) {
                /* Remove trailing quotation marks or commas */
                line.pop_back();
            }
        }
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string key = StringTrim(line.substr(0, colonPos));
            std::string value = StringTrim(line.substr(colonPos + 1));
            /* Further StringTrim the value */
            if (!value.empty() && value.back() == '"') {
                /* Remove the last closing quotation mark of value */
                value.pop_back();
            }
            lazyLoadPluginInfoMap[key] = value;
            lazyLoadPluginCountMap[key] = NOT_LAZY_LOADDED;
            lazyLoadPluginTimeoutMap[key] = NOT_LAZY_LOADDED;
        }
    }
}

int32_t DrmHostManager::LoadPluginInfo(const std::string &filePath)
{
    DRM_INFO_LOG("LoadPluginInfo enter.");
    lazyLoadPluginInfoMap.clear();
    int fd = open(filePath.c_str(), O_RDONLY);
    if (fd == -1) {
        DRM_ERR_LOG("LoadPluginInfo unable to open file:%{public}s.", filePath.c_str());
        return DRM_HOST_ERROR;
    }

    std::ifstream file(filePath);
    if (!file.is_open()) {
        DRM_ERR_LOG("LoadPluginInfo unable to open file:%{public}s.", filePath.c_str());
        close(fd);
        return DRM_HOST_ERROR;
    }

    std::string line;
    bool inPluginServices = false;
    while (getline(file, line)) {
        line = StringTrim(line);
        if (line == "\"plugin_services\": {") {
            inPluginServices = true;
        } else if (inPluginServices) {
            if (line == "}") {
                break;
            } else if (line == "\"lazy_load_service\": [") {
                parseLazyLoadService(file, lazyLoadPluginInfoMap);
                /* Exit after parsing lazy_load_service */
                break;
            }
        }
    }
    file.close();
    close(fd);
    return DRM_OK;
}

void DrmHostManager::UnloadAllServices()
{
    DRM_INFO_LOG("UnloadAllServices enter.");
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    sptr<IDeviceManager> deviceMgr = IDeviceManager::Get();
    if (deviceMgr == nullptr) {
        DRM_ERR_LOG("deviceMgr == nullptr");
        return;
    }
    for (auto pluginInfoIt = lazyLoadPluginInfoMap.begin(); pluginInfoIt != lazyLoadPluginInfoMap.end();
         pluginInfoIt++) {
        deviceMgr->UnloadDevice(pluginInfoIt->second);
    }
}

void DrmHostManager::ReleaseSevices(sptr<IMediaKeySystemFactory> drmHostServieProxy)
{
    DRM_INFO_LOG("ReleaseSevices enter.");
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    std::string name = hdiMediaKeySystemFactoryAndPluginNameMap[drmHostServieProxy];
    /* No need to release non lazy loading */
    if (lazyLoadPluginInfoMap.count(name) <= 0) {
        DRM_DEBUG_LOG("ReleaseSevices PluginCountInfo is empty, name:%{public}s",
            name.c_str());
        return;
    }
    if (lazyLoadPluginCountMap.empty()) {
        DRM_DEBUG_LOG("ReleaseSevices PluginCountMap is empty");
        return;
    }
    lazyLoadPluginCountMap[name]--;
    DRM_DEBUG_LOG("Lazy unLoad plugin name:%{public}s,count:%{public}d", name.c_str(), lazyLoadPluginCountMap[name]);
    if (lazyLoadPluginCountMap[name] == 0) {
        lazyLoadPluginTimeoutMap[name] = LAZY_UNLOAD_TIME_IN_MINUTES;
        DRM_DEBUG_LOG(
            "ReleaseSevices device need to unload: %{public}s.", lazyLoadPluginInfoMap[name].c_str());
    }
    hdiMediaKeySystemFactoryAndPluginNameMap.erase(drmHostServieProxy);
}

int32_t DrmHostManager::LazyLoadPlugin(std::string &name, std::vector<std::string> &serviceName,
    sptr<IDeviceManager> &deviceMgr, sptr<IServiceManager> &servmgr)
{
    DRM_INFO_LOG("LazyLoadPlugin enter, name:%{public}s.", name.c_str());
    deviceMgr = IDeviceManager::Get();
    if (deviceMgr == nullptr) {
        DRM_ERR_LOG("LazyLoadPlugin deviceMgr == nullptr");
        return DRM_SERVICE_ERROR;
    }
    int32_t ret = deviceMgr->LoadDevice(lazyLoadPluginInfoMap[name]);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("LazyLoadPlugin loadDevice failed return Code:%{public}d", ret);
        ret = servmgr->ListServiceByInterfaceDesc(serviceName, "ohos.hdi.drm.v1_0.IMediaKeySystemFactory");
        if (ret != DRM_OK) {
            DRM_ERR_LOG("ListServiceByInterfaceDesc faild, return Code:%{public}d", ret);
            return DRM_SERVICE_ERROR;
        }
    } else {
        lazyLoadPluginCountMap[name] = 0;
        DRM_INFO_LOG("LazyLoadPlugin LoadDevice: %{public}s.", lazyLoadPluginInfoMap[name].c_str());
        serviceName.push_back(lazyLoadPluginInfoMap[name]);
    }
    return DRM_OK;
}

int32_t DrmHostManager::ProcessLazyLoadPlugin(std::string &name, std::vector<std::string> &serviceName,
    sptr<IDeviceManager> &deviceMgr, sptr<IServiceManager> &servmgr)
{
    DRM_INFO_LOG("ProcessLazyLoadPlugin enter, name:%{public}s.", name.c_str());
    /*
     * If the plugin is configured with lazy loading, read the service name from the configuration file.
     * If lazy loading is not configured, traverse the service bound by the interface descriptor, and obtain
     * the plugin service instance through the uuid and issuport interfaces
     */
    if (lazyLoadPluginInfoMap.count(name) > 0) {
        auto it = std::find(serviceName.begin(), serviceName.end(), lazyLoadPluginInfoMap[name]);
        if (it == serviceName.end()) {
            int32_t ret = LazyLoadPlugin(name, serviceName, deviceMgr, servmgr);
            if (ret != DRM_OK) {
                DRM_ERR_LOG("ProcessLazyLoadPlugin LazyLoadPlugin faild, return Code:%{public}d",
                    ret);
                return DRM_SERVICE_ERROR;
            }
        }
    }
    if (serviceName.empty()) {
        DRM_INFO_LOG("ProcessLazyLoadPlugin exit, No DRM driver service named:%{public}s configured.",
            name.c_str());
        return DRM_SERVICE_ERROR;
    }
    return DRM_OK;
}

int32_t DrmHostManager::ProcessLazyLoadInfomation(std::string &name, sptr<IMediaKeySystemFactory> &drmHostServieProxy,
    sptr<IMediaKeySystemFactory> &drmHostServieProxys)
{
    DRM_INFO_LOG("ProcessLazyLoadInfomation enter, name:%{public}s.", name.c_str());
    drmHostServieProxys = drmHostServieProxy;
    sptr<DrmHostDeathRecipient> drmHostDeathRecipient = nullptr;
    drmHostDeathRecipient = new DrmHostDeathRecipient(this, name);
    const sptr<IRemoteObject> &remote = OHOS::HDI::hdi_objcast<IMediaKeySystemFactory>(drmHostServieProxys);
    if (remote != nullptr) {
        bool result = remote->AddDeathRecipient(drmHostDeathRecipient);
        if (!result) {
            ReleaseSevices(drmHostServieProxys);
            DRM_ERR_LOG("AddDeathRecipient for drm Host failed.");
            return DRM_HOST_ERROR;
        }
    }
    if (!lazyLoadPluginCountMap.empty()) {
        lazyLoadPluginCountMap[name]++;
        hdiMediaKeySystemFactoryAndPluginNameMap[drmHostServieProxys] = name;
        DRM_DEBUG_LOG("Lazy load plugin name:%{public}s,count:%{public}d",
            name.c_str(), lazyLoadPluginCountMap[name]);
    }
    DRM_INFO_LOG("ProcessLazyLoadInfomation exit.");
    return DRM_OK;
}

int32_t DrmHostManager::GetServices(std::string &name, bool *isSurpported,
    sptr<IMediaKeySystemFactory> &drmHostServieProxys)
{
    DRM_INFO_LOG("GetServices enter, name:%{public}s.", name.c_str());
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    std::vector<std::string> serviceName;
    drmHostServieProxys = nullptr;
    sptr<IServiceManager> servmgr = IServiceManager::Get();
    if (servmgr == nullptr) {
        DRM_ERR_LOG("GetServices IServiceManager::Get() failed!");
        return DRM_HOST_ERROR;
    }
    int32_t ret = servmgr->ListServiceByInterfaceDesc(serviceName, "ohos.hdi.drm.v1_0.IMediaKeySystemFactory");
    if (ret != DRM_OK) {
        DRM_ERR_LOG("ListServiceByInterfaceDesc faild, return Code:%{public}d", ret);
        return ret;
    }
    sptr<IDeviceManager> deviceMgr = nullptr;
    ret = ProcessLazyLoadPlugin(name, serviceName, deviceMgr, servmgr);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("GetServices ProcessLazyLoadPlugin faild, return Code:%{public}d", ret);
        return ret;
    }

    for (auto hdiServiceName : serviceName) {
        sptr<IMediaKeySystemFactory> drmHostServieProxy =
            OHOS::HDI::Drm::V1_0::IMediaKeySystemFactory::Get(hdiServiceName, false);
        if (drmHostServieProxy == nullptr) {
            DRM_ERR_LOG("GetServices failed.");
            continue;
        }
        ret = drmHostServieProxy->IsMediaKeySystemSupported(name, "", SECURE_UNKNOWN, *isSurpported);
        if (ret != DRM_OK) {
            DRM_ERR_LOG("IsMediaKeySystemSupported return Code:%{public}d", ret);
            continue;
        } else if (*isSurpported) {
            ret = ProcessLazyLoadInfomation(name, drmHostServieProxy, drmHostServieProxys);
            if (ret != DRM_OK) {
                DRM_ERR_LOG("GetServices ProcessLazyLoadInfomation faild, return Code:%{public}d", ret);
                return ret;
            }
            break;
        }
    }

    if (lazyLoadPluginCountMap[name] == 0 && deviceMgr != nullptr) {
        lazyLoadPluginTimeoutMap[name] = LAZY_UNLOAD_TIME_IN_MINUTES;
        DRM_ERR_LOG("GetServices error, serive unsupported, unload device name:%{public}s",
            name.c_str());
        return DRM_SERVICE_ERROR;
    }
    return DRM_OK;
}

int32_t DrmHostManager::IsMediaKeySystemSupported(std::string &name, bool *isSurpported)
{
    DRM_INFO_LOG("IsMediaKeySystemSupported one parameter enter, name:%{public}s.", name.c_str());
    sptr<IMediaKeySystemFactory> drmHostServieProxys;
    /* Lock will be released when lock goes out of scope */
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    int32_t ret = GetServices(name, isSurpported, drmHostServieProxys);
    if (ret != DRM_OK || drmHostServieProxys == nullptr) {
        *isSurpported = false;
        DRM_ERR_LOG("IsMediaKeySystemSupported one parameter GetServices error");
        return DRM_SERVICE_ERROR;
    }
    ReleaseSevices(drmHostServieProxys);
    return DRM_OK;
}

int32_t DrmHostManager::IsMediaKeySystemSupported(std::string &name, std::string &mimeType, bool *isSurpported)
{
    DRM_INFO_LOG(
        "IsMediaKeySystemSupported two parameters enter, name:%{public}s, mimeType:%{public}s.",
        name.c_str(),
        mimeType.c_str());
    sptr<IMediaKeySystemFactory> drmHostServieProxys;
    /* Lock will be released when lock goes out of scope */
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    int32_t ret = GetServices(name, isSurpported, drmHostServieProxys);
    if (ret != DRM_OK || drmHostServieProxys == nullptr) {
        *isSurpported = false;
        DRM_ERR_LOG("IsMediaKeySystemSupported two parameters GetServices error.");
        return DRM_SERVICE_ERROR;
    }
    if (mimeType.length() == 0) {
        *isSurpported = false;
        ReleaseSevices(drmHostServieProxys);
        DRM_ERR_LOG("IsMediaKeySystemSupported mimeType is null!");
        return DRM_SERVICE_ERROR;
    }
    ret = drmHostServieProxys->IsMediaKeySystemSupported(name, mimeType, SECURE_UNKNOWN, *isSurpported);
    if (ret != 0) {
        DRM_ERR_LOG("IsMediaKeySystemSupported return Code:%{public}d.", ret);
    }
    ReleaseSevices(drmHostServieProxys);
    DRM_INFO_LOG(
        "IsMediaKeySystemSupported two parameters exit, isSurpported:%{public}d.", *isSurpported);
    return DRM_OK;
}

int32_t DrmHostManager::IsMediaKeySystemSupported(
    std::string &name, std::string &mimeType, int32_t securityLevel, bool *isSurpported)
{
    DRM_INFO_LOG("IsMediaKeySystemSupported three parameters enter, name:%{public}s, "
                 "mimeType:%{public}s, securityLevel:%{public}d.",
        name.c_str(),
        mimeType.c_str(),
        securityLevel);
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    sptr<IMediaKeySystemFactory> drmHostServieProxys;
    /* Lock will be released when lock goes out of scope */
    int32_t ret = GetServices(name, isSurpported, drmHostServieProxys);
    if (ret != DRM_OK || drmHostServieProxys == nullptr) {
        *isSurpported = false;
        DRM_ERR_LOG("IsMediaKeySystemSupported three parameters GetServices error");
        return DRM_SERVICE_ERROR;
    }
    if (mimeType.length() == 0) {
        *isSurpported = false;
        ReleaseSevices(drmHostServieProxys);
        DRM_ERR_LOG("IsMediaKeySystemSupported mimeType is null!");
        return DRM_SERVICE_ERROR;
    }
    ret = drmHostServieProxys->IsMediaKeySystemSupported(
        name, mimeType, (OHOS::HDI::Drm::V1_0::ContentProtectionLevel)securityLevel, *isSurpported);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("IsMediaKeySystemSupported return Code:%{public}d", ret);
    }
    ReleaseSevices(drmHostServieProxys);
    return DRM_OK;
}

void DrmHostManager::ReleaseMediaKeySystem(sptr<IMediaKeySystem> &hdiMediaKeySystem)
{
    DRM_INFO_LOG("ReleaseMediaKeySystem enter.");
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    sptr<IMediaKeySystemFactory> drmHostServieProxys = hdiMediaKeySystemAndFactoryMap[hdiMediaKeySystem];
    if (drmHostServieProxys != nullptr) {
        ReleaseSevices(drmHostServieProxys);
    } else {
        DRM_INFO_LOG("ReleaseMediaKeySystem proxys not found.");
    }
    hdiMediaKeySystemAndFactoryMap.erase(hdiMediaKeySystem);
}

int32_t DrmHostManager::CreateMediaKeySystem(std::string &name, sptr<IMediaKeySystem> &hdiMediaKeySystem)
{
    DRM_INFO_LOG("CreateMediaKeySystem enter.");
    /* Lock will be released when lock goes out of scope */
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    bool isSurpported = false;
    sptr<IMediaKeySystemFactory> drmHostServieProxys;
    int32_t ret = GetServices(name, &isSurpported, drmHostServieProxys);
    if (ret != DRM_OK || drmHostServieProxys == nullptr) {
        DRM_ERR_LOG("CreateMediaKeySystem faild.");
        return DRM_HOST_ERROR;
    }

    ret = drmHostServieProxys->CreateMediaKeySystem(hdiMediaKeySystem);
    if (ret != DRM_OK) {
        hdiMediaKeySystem = nullptr;
        ReleaseSevices(drmHostServieProxys);
        DRM_ERR_LOG("CreateMediaKeySystem return Code:%{public}d", ret);
        return DRM_HOST_ERROR;
    }
    hdiMediaKeySystemAndFactoryMap[hdiMediaKeySystem] = drmHostServieProxys;
    return DRM_OK;
}

int32_t DrmHostManager::GetMediaKeySystemUuid(std::string &name, std::string &uuid)
{
    DRM_INFO_LOG("GetMediaKeySystemUuid enter.");
    bool isSurpported = false;
    sptr<IMediaKeySystemFactory> drmHostServieProxys;
    /* Lock will be released when lock goes out of scope */
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    int32_t ret = GetServices(name, &isSurpported, drmHostServieProxys);
    if (ret != DRM_OK || drmHostServieProxys == nullptr) {
        DRM_INFO_LOG("GetMediaKeySystemUuid faild.");
        return DRM_HOST_ERROR;
    }
    ret = drmHostServieProxys->GetMediaKeySystemDescription(name, uuid);
    if (ret != DRM_OK) {
        ReleaseSevices(drmHostServieProxys);
        DRM_ERR_LOG("GetMediaKeySystemUuid return Code:%{public}d", ret);
        return DRM_HOST_ERROR;
    }
    ReleaseSevices(drmHostServieProxys);
    return DRM_OK;
}

int32_t DrmHostManager::GetMediaKeySystems(std::map<std::string, std::string> &mediaKeySystemDescription)
{
    mediaKeySystemDescription.clear();
    mediaKeySystemDescription.insert(mediaKeySystemDescription_.begin(), mediaKeySystemDescription_.end());
    DRM_DEBUG_LOG("MetMediaKeySystems size:%{public}zu\n", mediaKeySystemDescription.size());
    return DRM_OK;
}

int32_t DrmHostManager::InitGetMediaKeySystems()
{
    DRM_INFO_LOG("InitGetMediaKeySystems enter.");
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    int32_t ret;
    std::vector<std::string> pluginServiceNames;
    /*
     * If the plugin is configured with lazy loading, read the service name from the configuration file.
     * If lazy loading is not configured, traverse the service bound by the interface descriptor, and obtain
     * the plugin service instance through the uuid and issuport interfaces
     */
    sptr<IDeviceManager> deviceMgr = IDeviceManager::Get();
    if (deviceMgr == nullptr) {
        DRM_ERR_LOG("InitGetMediaKeySystems deviceMgr == nullptr");
        return DRM_HOST_ERROR;
    }

    for (auto pluginInfoIt = lazyLoadPluginInfoMap.begin(); pluginInfoIt != lazyLoadPluginInfoMap.end();
         pluginInfoIt++) {
        ret = deviceMgr->LoadDevice(pluginInfoIt->second);
        if (ret != DRM_OK) {
            DRM_ERR_LOG("InitGetMediaKeySystems LoadDevice return Code:%{public}d", ret);
            continue;
        }
    }

    auto servmgr = IServiceManager::Get();
    if (servmgr == nullptr) {
        DRM_ERR_LOG("InitGetMediaKeySystems IServiceManager::Get() failed!");
        return DRM_HOST_ERROR;
    }
    ret = servmgr->ListServiceByInterfaceDesc(pluginServiceNames, "ohos.hdi.drm.v1_0.IMediaKeySystemFactory");
    if (ret != DRM_OK) {
        DRM_ERR_LOG(
            "InitGetMediaKeySystems ListServiceByInterfaceDesc faild, return Code:%{public}d", ret);
        return ret;
    }
    for (auto hdiServiceName : pluginServiceNames) {
        std::string pluginName = "";
        std::string pluginUuid = "";
        sptr<IMediaKeySystemFactory> drmHostServieProxy =
            OHOS::HDI::Drm::V1_0::IMediaKeySystemFactory::Get(hdiServiceName, false);
        if (drmHostServieProxy == nullptr) {
            continue;
        }
        ret = drmHostServieProxy->GetMediaKeySystemDescription(pluginName, pluginUuid);
        if (ret != DRM_OK) {
            continue;
        }
        mediaKeySystemDescription_.insert(std::pair<std::string, std::string>(pluginName, pluginUuid));
    }
    for (auto pluginInfoIt = lazyLoadPluginInfoMap.begin(); pluginInfoIt != lazyLoadPluginInfoMap.end();
         pluginInfoIt++) {
        ret = deviceMgr->UnloadDevice(pluginInfoIt->second);
        if (ret != DRM_OK) {
            DRM_ERR_LOG("UnloadDevice return Code:%{public}d", ret);
            continue;
        }
    }
    return DRM_OK;
}
}  // namespace DrmStandard
}  // namespace OHOS