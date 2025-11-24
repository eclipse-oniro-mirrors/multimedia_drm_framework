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
#include "drm_helper.h"
#include "drm_net_observer.h"

namespace OHOS {
namespace DrmStandard {
using namespace OHOS::HiviewDFX;
std::queue<Message> DrmHostManager::messageQueue;
std::mutex DrmHostManager::queueMutex;
std::condition_variable DrmHostManager::cv;

const int32_t LAZY_UNLOAD_TIME_CHECK_IN_MINUTES = 1;
const int32_t LAZY_UNLOAD_WAIT_IN_MILMINUTES = 100;
const int32_t LAZY_UNLOAD_TIME_IN_MINUTES = 3;
const int32_t NOT_LAZY_LOADDED = -65536;
const int32_t TIME_IN_MS = 60000;
const int32_t QUERY_INTERVAL = 60;
const std::string TV_DEVICE = "tv";
const std::string AGREED_STATEMENT = "1";
const std::string SECURE_TYPE = "secure";
const std::string BASIC_STATEMENT_AGREED = "basic_statement_agreed";
const std::string INVALID_DATA = "invalid_data";

DrmHostManager::DrmHostDeathRecipient::DrmHostDeathRecipient(
    const sptr<DrmHostManager> &drmHostManager, std::string &name)
    : drmHostManager_(drmHostManager)
{
    name_ = name;
    DRM_DEBUG_LOG("DrmHostDeathRecipient");
}

DrmHostManager::DrmHostDeathRecipient::~DrmHostDeathRecipient()
{
    DRM_DEBUG_LOG("~DrmHostDeathRecipient");
}

void DrmHostManager::OnDrmPluginDied(std::string &name)
{
}

void DrmHostManager::DrmHostDeathRecipient::OnRemoteDied(const wptr<IRemoteObject> &remote)
{
    DRM_ERR_LOG("Remote service died, do clean works.");
    if (auto drmHostManagerPtr = drmHostManager_.promote()) {
        DRM_INFO_LOG("OnRemoteDied enter.");
        drmHostManagerPtr->ClearDeathService(name_);
    }
}

void DrmHostManager::ClearDeathService(std::string &name)
{
    DRM_INFO_LOG("ClearDeathService enter.");
    std::lock_guard<std::recursive_mutex> lock(drmHostMapMutex);
    DRM_CHECK_AND_RETURN_LOG(
        lazyLoadPluginInfoMap.count(name) > 0, "PluginCountInfo is empty, name:%{public}s", name.c_str());
    DRM_CHECK_AND_RETURN_LOG(!lazyLoadPluginCountMap.empty(), "PluginCountMap is empty.");
    if (statusCallback_ != nullptr) {
        statusCallback_->OnDrmPluginDied(name);
    }
    lazyLoadPluginCountMap[name] = NOT_LAZY_LOADDED;
    lazyLoadPluginTimeoutMap[name] = NOT_LAZY_LOADDED;

    for (auto it = hdiMediaKeySystemFactoryAndPluginNameMap.begin();
         it != hdiMediaKeySystemFactoryAndPluginNameMap.end();) {
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
    {
        std::lock_guard<std::recursive_mutex> lock(drmHostMapMutex);
        statusCallback_ = nullptr;
    }
    StopServiceThread();
}

void DrmHostManager::StopServiceThread()
{
    DRM_INFO_LOG("StopServiceThread enter.");
    if (!serviceThreadRunning) {
        return;
    }
    serviceThreadRunning = false;
    cv.notify_all();
    if (serviceThread.joinable()) {
        serviceThread.join();
    }
    if (messageQueueThread.joinable()) {
        messageQueueThread.join();
    }
    std::lock_guard<std::recursive_mutex> lock(drmHostMapMutex);
    for (auto libHandle : loadedLibs) {
        StopThreadFuncType StopThread = (StopThreadFuncType)dlsym(libHandle, "StopThread");
        if (StopThread) {
            StopThread();
        }
        dlclose(libHandle);
        libHandle = nullptr;
    }
    for (auto it = hdiMediaKeySystemAndFactoryMap.begin(); it != hdiMediaKeySystemAndFactoryMap.end(); it++) {
        if (it->second != nullptr) {
            ReleaseSevices(it->second);
        } else {
            DRM_INFO_LOG("proxys not found.");
        }
    }
    UnloadAllServices();
    loadedLibs.clear();

    ReportServiceBehaviorEvent("DRM_OEM_SERVICE", "end");
}

void DrmHostManager::DelayedLazyUnLoad()
{
    DRM_DEBUG_LOG("DelayedLazyUnLoad enter.");
    sptr<IDeviceManager> deviceMgr = IDeviceManager::Get();
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    DRM_CHECK_AND_RETURN_LOG(
        deviceMgr != nullptr && !lazyLoadPluginInfoMap.empty(), "devMgr is null or lazyLoadPluginInfoMap is empty");
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
    messageQueueThread = std::thread([this] {
        int32_t counter = TIME_IN_MS;
        while (serviceThreadRunning) {
            std::unique_lock<std::mutex> queueMutexLock(queueMutex);
            cv.wait_for(queueMutexLock, std::chrono::milliseconds(LAZY_UNLOAD_WAIT_IN_MILMINUTES), [this] {
                return (!this->messageQueue.empty() || !this->serviceThreadRunning);
            });
            std::queue<Message> localQueue;
            localQueue.swap(messageQueue);
            queueMutexLock.unlock();
            while (!localQueue.empty()) {
                auto message = localQueue.front();
                localQueue.pop();
                DRM_DEBUG_LOG("ProcessMessage message type:%{public}d.", message.type);
                if (message.type == Message::UnLoadOEMCertifaicateService) {
                    std::lock_guard<std::recursive_mutex> lock(drmHostMapMutex);
                    void *libHandle = pluginNameAndHandleMap[message.name];
                    if (libHandle != nullptr) {
                        ReleaseHandleAndKeySystemMap(libHandle);
                        loadedLibs.erase(std::remove(loadedLibs.begin(), loadedLibs.end(), libHandle),
                            loadedLibs.end());
                        DRM_INFO_LOG("ProcessMessage UnLoadOEMCertifaicateService success.");
                    }
                }
            }
            if (!serviceThreadRunning) {
                break;
            }
            counter -= LAZY_UNLOAD_WAIT_IN_MILMINUTES;
            if (counter <= 0) {
                DRM_DEBUG_LOG("ProcessMessage lazy unload start.");
                DelayedLazyUnLoad();
                counter = TIME_IN_MS;
            }
        }
    });
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
        DRM_DEBUG_LOG("ReleaseHandleAndKeySystemMap ReleaseMediaKeySystem end");
    } else {
        DRM_INFO_LOG("ReleaseHandleAndKeySystemMap handle not found");
    }
    handleAndKeySystemMap.erase(handle);

    if (handle != nullptr) {
        loadedLibs.erase(std::remove(loadedLibs.begin(), loadedLibs.end(), handle), loadedLibs.end());
        StopThreadFuncType StopThread = (StopThreadFuncType)dlsym(handle, "StopThread");
        if (StopThread) {
            StopThread();
        }
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
            libsToLoad.push_back(fullPath);
        }
        closedir(dir);
        dir = nullptr;
    }
}

bool DrmHostManager::ProcessProvision(void *handle)
{
    auto QueryMediaKeySystemName = (QueryMediaKeySystemNameFuncType)dlsym(handle, "QueryMediaKeySystemName");
    auto IsProvisionRequired = (IsProvisionRequiredFuncType)dlsym(handle, "IsProvisionRequired");
    auto SetMediaKeySystem = (SetMediaKeySystemFuncType)dlsym(handle, "SetMediaKeySystem");
    auto ThreadExitNotify = (ThreadExitNotifyFuncType)dlsym(handle, "ThreadExitNotify");
    auto StartThread = (StartThreadFuncType)dlsym(handle, "StartThread");

    DRM_CHECK_AND_RETURN_RET_LOG(
        QueryMediaKeySystemName && IsProvisionRequired && SetMediaKeySystem && ThreadExitNotify && StartThread,
        DRM_INNER_ERR_UNKNOWN,
        "Not all functions found");

    std::string pluginName;
    int32_t ret = QueryMediaKeySystemName(pluginName);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK, ret, "QueryMediaKeySystemName error!");
    sptr<IMediaKeySystem> hdiMediaKeySystem = nullptr;
    ret = CreateMediaKeySystem(pluginName, hdiMediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(
        ret == DRM_INNER_ERR_OK && hdiMediaKeySystem != nullptr, ret, "CreateMediaKeySystem error!");
    {
        std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
        pluginNameAndHandleMap[pluginName] = handle;
        handleAndKeySystemMap[handle] = hdiMediaKeySystem;
    }
    ret = SetMediaKeySystem(hdiMediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK, ret, "SetMediaKeySystem error!");
    DRM_CHECK_AND_RETURN_RET_LOG(IsProvisionRequired(), DRM_INNER_ERR_UNKNOWN, "Provision not required!");

    ret = ThreadExitNotify(DrmHostManager::UnLoadOEMCertifaicateService);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK, ret, "ThreadExitNotify error!");
    ret = StartThread();
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK, ret, "StartThread error!");
    return ret;
}

void DrmHostManager::ServiceThreadMain() __attribute__((no_sanitize("cfi")))
{
    DRM_INFO_LOG("ServiceThreadMain enter.");
    std::vector<std::string> libsToLoad;
    GetOemLibraryPath(libsToLoad);
    for (const auto &libpath : libsToLoad) {
        void *handle = dlopen(libpath.c_str(), RTLD_LAZY);
        if (handle == nullptr) {
            continue;
        }
        {
            std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
            loadedLibs.push_back(handle);
        }
        if (QueryBasicStatement() == INVALID_DATA|| ProcessProvision(handle) != DRM_INNER_ERR_OK) {
            ReleaseHandleAndKeySystemMap(handle);
        }
    }
    ReportServiceBehaviorEvent("DRM_OEM_SERVICE", "start");
}

void DrmHostManager::UnLoadOEMCertifaicateService(std::string &name, ExtraInfo info)
{
    DRM_INFO_LOG("UnLoadOEMCertifaicateService enter.");
    std::unique_lock<std::mutex> queueMutexLock(queueMutex);
    Message message(Message::UnLoadOEMCertifaicateService, name, info);
    messageQueue.push(message);
    cv.notify_all();
}

void DrmHostManager::OemCertificateManager()
{
    DRM_INFO_LOG("OemCertificateManager enter.");
    serviceThreadRunning = true;
    serviceThread = std::thread([this] { this->ServiceThreadMain(); });
    ProcessMessage();
}

int32_t DrmHostManager::Init(void)
{
    DRM_INFO_LOG("Init enter.");
    LoadPluginInfo(PLUGIN_LAZYLOAD_CONFIG_PATH);
    InitGetMediaKeySystems();
    OemCertificateManager();
    return DRM_INNER_ERR_OK;
}

void DrmHostManager::DeInit(void)
{
    DRM_INFO_LOG("DeInit");
}

void DrmHostManager::OnReceive(const HDI::ServiceManager::V1_0::ServiceStatus &status)
{}

std::string DrmHostManager::StringTrim(const std::string &str)
{
    DRM_DEBUG_LOG("StringTrim enter.");
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
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    lazyLoadPluginInfoMap.clear();

    std::ifstream file(filePath);
    DRM_CHECK_AND_RETURN_RET_LOG(
        file.is_open(), DRM_INNER_ERR_BASE, "LoadPluginInfo unable to open file:%{public}s.", filePath.c_str());

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
    return DRM_INNER_ERR_OK;
}

void DrmHostManager::UnloadAllServices()
{
    DRM_INFO_LOG("UnloadAllServices enter.");
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    sptr<IDeviceManager> deviceMgr = IDeviceManager::Get();
    DRM_CHECK_AND_RETURN_LOG(deviceMgr != nullptr, "deviceMgr == nullptr");
    for (auto pluginInfoIt = lazyLoadPluginInfoMap.begin(); pluginInfoIt != lazyLoadPluginInfoMap.end();
         pluginInfoIt++) {
        deviceMgr->UnloadDevice(pluginInfoIt->second);
    }
}

void DrmHostManager::ReleaseSevices(sptr<IMediaKeySystemFactory> drmHostServieProxy)
{
    DRM_INFO_LOG("ReleaseSevices enter.");
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    const sptr<IRemoteObject> &remote = OHOS::HDI::hdi_objcast<IMediaKeySystemFactory>(drmHostServieProxy);
    if (remote != nullptr && drmHostDeathRecipientMap[drmHostServieProxy] != nullptr) {
        remote->RemoveDeathRecipient(drmHostDeathRecipientMap[drmHostServieProxy]);
        drmHostDeathRecipientMap[drmHostServieProxy] = nullptr;
        DRM_INFO_LOG("Release RemoveDeathRecipient.");
    }
    drmHostDeathRecipientMap.erase(drmHostServieProxy);
    std::string name = hdiMediaKeySystemFactoryAndPluginNameMap[drmHostServieProxy];
    /* No need to release non lazy loading */
    DRM_CHECK_AND_RETURN_LOG(lazyLoadPluginInfoMap.count(name) > 0,
        "ReleaseSevices PluginCountInfo is empty, name:%{public}s",
        name.c_str());
    DRM_CHECK_AND_RETURN_LOG(!lazyLoadPluginCountMap.empty(), "ReleaseSevices PluginCountMap is empty");
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
    DRM_CHECK_AND_RETURN_RET_LOG(
        deviceMgr != nullptr, DRM_INNER_ERR_PLUGIN_ERROR, "LazyLoadPlugin deviceMgr == nullptr");
    int32_t ret = deviceMgr->LoadDevice(lazyLoadPluginInfoMap[name]);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("LazyLoadPlugin loadDevice failed return Code:%{public}d", ret);
        ret = servmgr->ListServiceByInterfaceDesc(serviceName, "ohos.hdi.drm.v1_0.IMediaKeySystemFactory");
        DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK,
            DRM_INNER_ERR_PLUGIN_ERROR,
            "ListServiceByInterfaceDesc faild, return Code:%{public}d",
            ret);
    } else {
        lazyLoadPluginCountMap[name] = 0;
        DRM_INFO_LOG("LazyLoadPlugin LoadDevice: %{public}s.", lazyLoadPluginInfoMap[name].c_str());
        serviceName.push_back(lazyLoadPluginInfoMap[name]);
    }
    return DRM_INNER_ERR_OK;
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
            DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK,
                DRM_INNER_ERR_PLUGIN_ERROR,
                "ProcessLazyLoadPlugin LazyLoadPlugin faild, return Code:%{public}d",
                ret);
        }
    }
    DRM_CHECK_AND_RETURN_RET_LOG(!serviceName.empty(),
        DRM_INNER_ERR_PLUGIN_ERROR,
        "No DRM driver service named:%{public}s configured.",
        name.c_str());
    return DRM_INNER_ERR_OK;
}

int32_t DrmHostManager::ProcessLazyLoadInfomation(std::string &name, sptr<IMediaKeySystemFactory> &drmHostServieProxy)
{
    DRM_INFO_LOG("ProcessLazyLoadInfomation enter, name:%{public}s.", name.c_str());
    drmHostDeathRecipientMap[drmHostServieProxy] = new DrmHostDeathRecipient(this, name);
    const sptr<IRemoteObject> &remote = OHOS::HDI::hdi_objcast<IMediaKeySystemFactory>(drmHostServieProxy);
    if (remote != nullptr && drmHostDeathRecipientMap[drmHostServieProxy] != nullptr) {
        bool result = remote->AddDeathRecipient(drmHostDeathRecipientMap[drmHostServieProxy]);
        if (!result) {
            drmHostDeathRecipientMap[drmHostServieProxy] = nullptr;
            ReleaseSevices(drmHostServieProxy);
            DRM_ERR_LOG("AddDeathRecipient for drm Host failed.");
            return DRM_INNER_ERR_BASE;
        }
    }
    DRM_CHECK_AND_RETURN_RET_LOG(!lazyLoadPluginCountMap.empty(), DRM_INNER_ERR_OK, "no puglin in count map");

    lazyLoadPluginCountMap[name]++;
    hdiMediaKeySystemFactoryAndPluginNameMap[drmHostServieProxy] = name;
    DRM_DEBUG_LOG("Lazy load plugin name:%{public}s,count:%{public}d", name.c_str(), lazyLoadPluginCountMap[name]);

    return DRM_INNER_ERR_OK;
}

int32_t DrmHostManager::GetServices(
    std::string &name, bool *isSupported, sptr<IMediaKeySystemFactory> &drmHostServieProxys)
{
    DRM_INFO_LOG("GetServices enter, name:%{public}s.", name.c_str());
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    std::vector<std::string> serviceName;
    drmHostServieProxys = nullptr;
    sptr<IServiceManager> servmgr = IServiceManager::Get();
    *isSupported = false;
    DRM_CHECK_AND_RETURN_RET_LOG(
        servmgr != nullptr, DRM_INNER_ERR_PLUGIN_ERROR, "GetServices IServiceManager::Get() failed!");
    int32_t ret = servmgr->ListServiceByInterfaceDesc(serviceName, "ohos.hdi.drm.v1_0.IMediaKeySystemFactory");
    DRM_CHECK_AND_RETURN_RET_LOG(
        ret == DRM_INNER_ERR_OK, ret, "ListServiceByInterfaceDesc faild, return Code:%{public}d", ret);
    sptr<IDeviceManager> deviceMgr = nullptr;
    ret = ProcessLazyLoadPlugin(name, serviceName, deviceMgr, servmgr);
    DRM_CHECK_AND_RETURN_RET_LOG(
        ret == DRM_INNER_ERR_OK, ret, "GetServices ProcessLazyLoadPlugin faild, return Code:%{public}d", ret);

    for (auto hdiServiceName : serviceName) {
        sptr<IMediaKeySystemFactory> drmHostServieProxy =
            OHOS::HDI::Drm::V1_0::IMediaKeySystemFactory::Get(hdiServiceName, false);
        if (drmHostServieProxy == nullptr) {
            DRM_ERR_LOG("GetServices failed.");
            continue;
        }
        ret = drmHostServieProxy->IsMediaKeySystemSupported(name, "", SECURE_UNKNOWN, *isSupported);
        if (ret != DRM_INNER_ERR_OK) {
            DRM_ERR_LOG("IsMediaKeySystemSupported return Code:%{public}d", ret);
            continue;
        } else if (*isSupported) {
            ret = ProcessLazyLoadInfomation(name, drmHostServieProxy);
            DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK,
                ret,
                "GetServices ProcessLazyLoadInfomation faild, return Code:%{public}d",
                ret);
            drmHostServieProxys = drmHostServieProxy;
            break;
        }
    }

    if (lazyLoadPluginCountMap[name] == 0 && deviceMgr != nullptr) {
        lazyLoadPluginTimeoutMap[name] = LAZY_UNLOAD_TIME_IN_MINUTES;
        DRM_ERR_LOG("GetServices error, serive unsupported, unload device name:%{public}s", name.c_str());
        return DRM_INNER_ERR_UNSUPPORTED;
    }
    return DRM_INNER_ERR_OK;
}

int32_t DrmHostManager::IsMediaKeySystemSupported(std::string &name, bool *isSupported)
{
    DRM_INFO_LOG("IsMediaKeySystemSupported one parameter enter, name:%{public}s.", name.c_str());
    sptr<IMediaKeySystemFactory> drmHostServieProxys;
    /* Lock will be released when lock goes out of scope */
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    int32_t ret;
    *isSupported = false;
    ret = GetServices(name, isSupported, drmHostServieProxys);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK && drmHostServieProxys != nullptr,
        DRM_INNER_ERR_PLUGIN_ERROR,
        "IsMediaKeySystemSupported one parameter GetServices error");
    ReleaseSevices(drmHostServieProxys);
    return DRM_INNER_ERR_OK;
}

int32_t DrmHostManager::IsMediaKeySystemSupported(std::string &name, std::string &mimeType, bool *isSupported)
{
    DRM_INFO_LOG("IsMediaKeySystemSupported two parameters enter, name:%{public}s, mimeType:%{public}s.",
        name.c_str(),
        mimeType.c_str());
    sptr<IMediaKeySystemFactory> drmHostServieProxys;
    /* Lock will be released when lock goes out of scope */
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    int32_t ret;
    *isSupported = false;
    DRM_CHECK_AND_RETURN_RET_LOG(
        mimeType.length() != 0, DRM_INNER_ERR_UNSUPPORTED, "IsMediaKeySystemSupported mimeType is null!");
    ret = GetServices(name, isSupported, drmHostServieProxys);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK && drmHostServieProxys != nullptr,
        DRM_INNER_ERR_PLUGIN_ERROR,
        "IsMediaKeySystemSupported two parameters GetServices error.");
    ret = drmHostServieProxys->IsMediaKeySystemSupported(name, mimeType, SECURE_UNKNOWN, *isSupported);
    ReleaseSevices(drmHostServieProxys);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, DRM_INNER_ERR_OK, "IsMediaKeySystemSupported return Code:%{public}d.", ret);
    return DRM_INNER_ERR_OK;
}

int32_t DrmHostManager::IsMediaKeySystemSupported(
    std::string &name, std::string &mimeType, int32_t securityLevel, bool *isSupported)
{
    DRM_INFO_LOG("IsMediaKeySystemSupported three parameters enter, name:%{public}s, "
                 "mimeType:%{public}s, securityLevel:%{public}d.",
        name.c_str(),
        mimeType.c_str(),
        securityLevel);
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    sptr<IMediaKeySystemFactory> drmHostServieProxys;
    int32_t ret;
    *isSupported = false;
    /* Lock will be released when lock goes out of scope */
    DRM_CHECK_AND_RETURN_RET_LOG(
        mimeType.length() != 0, DRM_INNER_ERR_UNSUPPORTED, "IsMediaKeySystemSupported mimeType is null!");
    ret = GetServices(name, isSupported, drmHostServieProxys);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK && drmHostServieProxys != nullptr,
        DRM_INNER_ERR_PLUGIN_ERROR,
        "IsMediaKeySystemSupported three parameters GetServices error");
    ret = drmHostServieProxys->IsMediaKeySystemSupported(
        name, mimeType, (OHOS::HDI::Drm::V1_0::ContentProtectionLevel)securityLevel, *isSupported);
    ReleaseSevices(drmHostServieProxys);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, DRM_INNER_ERR_OK, "IsMediaKeySystemSupported return Code:%{public}d.", ret);
    return DRM_INNER_ERR_OK;
}

void DrmHostManager::ReleaseMediaKeySystem(sptr<IMediaKeySystem> &hdiMediaKeySystem)
{
    DRM_INFO_LOG("ReleaseMediaKeySystem enter.");
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    sptr<IMediaKeySystemFactory> drmHostServieProxys = hdiMediaKeySystemAndFactoryMap[hdiMediaKeySystem];
    if (drmHostServieProxys != nullptr) {
        ReleaseSevices(drmHostServieProxys);
    }
    hdiMediaKeySystemAndFactoryMap.erase(hdiMediaKeySystem);
}

int32_t DrmHostManager::CreateMediaKeySystem(std::string &name, sptr<IMediaKeySystem> &hdiMediaKeySystem)
{
    DRM_INFO_LOG("CreateMediaKeySystem enter.");
    /* Lock will be released when lock goes out of scope */
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    bool isSupported = false;
    sptr<IMediaKeySystemFactory> drmHostServieProxys;
    int32_t ret = GetServices(name, &isSupported, drmHostServieProxys);
    if (ret != DRM_INNER_ERR_OK || drmHostServieProxys == nullptr) {
        DRM_ERR_LOG("CreateMediaKeySystem faild.");
        return DRM_INNER_ERR_PLUGIN_ERROR;
    }

    ret = drmHostServieProxys->CreateMediaKeySystem(hdiMediaKeySystem);
    if (ret != DRM_INNER_ERR_OK) {
        hdiMediaKeySystem = nullptr;
        ReleaseSevices(drmHostServieProxys);
        DRM_ERR_LOG("CreateMediaKeySystem return Code:%{public}d", ret);
        return DRM_INNER_ERR_INVALID_MEDIA_KEY_SYSTEM;
    }
    hdiMediaKeySystemAndFactoryMap[hdiMediaKeySystem] = drmHostServieProxys;
    return DRM_INNER_ERR_OK;
}

int32_t DrmHostManager::GetMediaKeySystemUuid(std::string &name, std::string &uuid)
{
    DRM_INFO_LOG("GetMediaKeySystemUuid enter.");
    bool isSupported = false;
    sptr<IMediaKeySystemFactory> drmHostServieProxys;
    /* Lock will be released when lock goes out of scope */
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    int32_t ret = GetServices(name, &isSupported, drmHostServieProxys);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK && drmHostServieProxys != nullptr,
        DRM_INNER_ERR_PLUGIN_ERROR,
        "GetMediaKeySystemUuid failed.");
    ret = drmHostServieProxys->GetMediaKeySystemDescription(name, uuid);
    ReleaseSevices(drmHostServieProxys);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK,
        DRM_INNER_ERR_INVALID_MEDIA_KEY_SYSTEM,
        "GetMediaKeySystemUuid return Code:%{public}d",
        ret);
    return DRM_INNER_ERR_OK;
}

int32_t DrmHostManager::GetMediaKeySystems(std::map<std::string, std::string> &mediaKeySystemDescription)
{
    DRM_INFO_LOG("GetMediaKeySystems enter.");
    std::lock_guard<std::recursive_mutex> drmHostMapLock(drmHostMapMutex);
    mediaKeySystemDescription.clear();
    mediaKeySystemDescription.insert(mediaKeySystemDescription_.begin(), mediaKeySystemDescription_.end());
    DRM_DEBUG_LOG("GetMediaKeySystems size:%{public}zu\n", mediaKeySystemDescription.size());
    return DRM_INNER_ERR_OK;
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
    DRM_CHECK_AND_RETURN_RET_LOG(
        deviceMgr != nullptr, DRM_INNER_ERR_PLUGIN_ERROR, "InitGetMediaKeySystems deviceMgr == nullptr");

    for (auto pluginInfoIt = lazyLoadPluginInfoMap.begin(); pluginInfoIt != lazyLoadPluginInfoMap.end();
         pluginInfoIt++) {
        ret = deviceMgr->LoadDevice(pluginInfoIt->second);
        if (ret != DRM_INNER_ERR_OK) {
            DRM_ERR_LOG("InitGetMediaKeySystems LoadDevice return Code:%{public}d", ret);
            continue;
        }
    }

    auto servmgr = IServiceManager::Get();
    DRM_CHECK_AND_RETURN_RET_LOG(
        servmgr != nullptr, DRM_INNER_ERR_PLUGIN_ERROR, "InitGetMediaKeySystems IServiceManager::Get() failed!");
    ret = servmgr->ListServiceByInterfaceDesc(pluginServiceNames, "ohos.hdi.drm.v1_0.IMediaKeySystemFactory");
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK,
        ret,
        "InitGetMediaKeySystems ListServiceByInterfaceDesc faild, return Code:%{public}d",
        ret);
    for (auto hdiServiceName : pluginServiceNames) {
        std::string pluginName = "";
        std::string pluginUuid = "";
        sptr<IMediaKeySystemFactory> drmHostServieProxy =
            OHOS::HDI::Drm::V1_0::IMediaKeySystemFactory::Get(hdiServiceName, false);
        if (drmHostServieProxy == nullptr) {
            continue;
        }
        ret = drmHostServieProxy->GetMediaKeySystemDescription(pluginName, pluginUuid);
        if (ret != DRM_INNER_ERR_OK) {
            continue;
        }
        mediaKeySystemDescription_.insert(std::pair<std::string, std::string>(pluginName, pluginUuid));
    }
    for (auto pluginInfoIt = lazyLoadPluginInfoMap.begin(); pluginInfoIt != lazyLoadPluginInfoMap.end();
         pluginInfoIt++) {
        ret = deviceMgr->UnloadDevice(pluginInfoIt->second);
        if (ret != DRM_INNER_ERR_OK) {
            DRM_ERR_LOG("UnloadDevice return Code:%{public}d", ret);
            continue;
        }
    }
    return DRM_INNER_ERR_OK;
}

std::string DrmHostManager::QueryBasicStatement()
{
    std::string value = "";
    if (DrmHelper::GetDeviceType() == TV_DEVICE) {
        value = DrmHelper::GetSettingDataValue(SECURE_TYPE, BASIC_STATEMENT_AGREED);
        while (value != AGREED_STATEMENT) {
            DRM_CHECK_AND_RETURN_RET_LOG(value != INVALID_DATA, value, "QueryBasicStatement error!");
            DRM_INFO_LOG("DrmHostManager wait for basic statement agreed.");
            sleep(QUERY_INTERVAL);
            value = DrmHelper::GetSettingDataValue(SECURE_TYPE, BASIC_STATEMENT_AGREED);
        }
        WaitForNetwork();
    }
    return value;
}

void DrmHostManager::WaitForNetwork()
{
    DRM_CHECK_AND_RETURN_LOG(this->isNetWork, "DRM Net has Connected");
    DRM_INFO_LOG("DRM Net Is Not Connected - waiting for network...");
    std::unique_lock<std::mutex> lock(drmNetObserverMutex);
    condVar.wait(lock, [this] { return this->isNetWork.load(); });
    DRM_INFO_LOG("DRM Net Connected");
}


void DrmHostManager::SetIsNetWork(const bool &isNetWork)
{
    this->isNetWork = isNetWork;
}

bool DrmHostManager::GetIsNetWork()
{
    return this->isNetWork;
}
}  // namespace DrmStandard
}  // namespace OHOS
