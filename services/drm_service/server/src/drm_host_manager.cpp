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
#include "hdf_device_class.h"
#include "iremote_broker.h"
#include "iservmgr_hdi.h"
#include "idevmgr_hdi.h"
#include "ipc_skeleton.h"
#include "servmgr_hdi.h"
#include "drm_dfx.h"
#include "drm_dfx_utils.h"
#include "drm_log.h"
#include "drm_error_code.h"
#include "napi_param_utils.h"
#include "drm_host_manager.h"

namespace OHOS {
namespace DrmStandard {
using namespace OHOS::HiviewDFX;
using OHOS::HDI::ServiceManager::V1_0::IServiceManager;
using OHOS::HDI::DeviceManager::V1_0::IDeviceManager;
std::queue<Message> DrmHostManager::messageQueue;
std::mutex DrmHostManager::queueMutex;
std::condition_variable DrmHostManager::cv;

DrmHostManager::DrmHostDeathRecipient::DrmHostDeathRecipient()
{
    DRM_DEBUG_LOG("DrmHostManager::DrmHostDeathRecipient");
}

DrmHostManager::DrmHostDeathRecipient::~DrmHostDeathRecipient()
{
    DRM_DEBUG_LOG("DrmHostManager::~DrmHostDeathRecipient");
}

void DrmHostManager::DrmHostDeathRecipient::OnRemoteDied(const wptr<IRemoteObject> &remote)
{
    DRM_ERR_LOG("DrmHostManager Service died, do clean works.");
}

DrmHostManager::DrmHostManager(StatusCallback *statusCallback)
    : statusCallback_(statusCallback)
{}

DrmHostManager::~DrmHostManager()
{
    DRM_INFO_LOG("DrmHostManager::~DrmHostManager enter.");
    statusCallback_ = nullptr;
    if (serviceThreadRunning) {
        StopServiceThread();
    }
    DRM_INFO_LOG("DrmHostManager::~DrmHostManager exit.");
}

void DrmHostManager::StopServiceThread()
{
    DRM_INFO_LOG("DrmHostManager::StopServiceThread enter.");
    serviceThreadRunning = false;
    if (serviceThread.joinable()) {
        serviceThread.join();
    }
    for (auto libHandle : loadedLibs) {
        StopThreadFuncType StopThread = (StopThreadFuncType)dlsym(libHandle, "StopThread");
        if (StopThread) {
            StopThread();
        }
        ReleaseHandleAndKeySystemMap(libHandle);
    }
    loadedLibs.clear();

    ReportServiceBehaviorEvent("DRM_OEM_SERVICE", "end");
    DRM_INFO_LOG("DrmHostManager::StopServiceThread exit.");
}

void DrmHostManager::ProcessMessage()
{
    DRM_INFO_LOG("DrmHostManager::ProcessMessage enter.");
    std::thread([this] {
        while (serviceThreadRunning) {
            std::unique_lock<std::mutex> lock(queueMutex);
            cv.wait(lock, [this] { return !this->messageQueue.empty(); });
            while (!messageQueue.empty()) {
                auto message = messageQueue.front();
                messageQueue.pop();
                if (message.type == Message::UnLoadOEMCertifaicateService) {
                    void *libHandle = nullptr;
                    std::lock_guard<std::mutex> lockLib(libMutex);
                    auto it = libMap.find(message.name);
                    if (it != libMap.end()) {
                        libHandle = it->second;
                    }
                    if (libHandle != nullptr) {
                        ReleaseHandleAndKeySystemMap(libHandle);
                        loadedLibs.erase(std::remove(loadedLibs.begin(), loadedLibs.end(), libHandle),
                            loadedLibs.end());
                    }
                }
            }
            lock.unlock();
            DRM_INFO_LOG("DrmHostManager::ProcessMessage exit.");
        }
    }).detach();
}

void DrmHostManager::ReleaseHandleAndKeySystemMap(void *handle)
{
    std::lock_guard<std::recursive_mutex> lock(handleAndKeySystemMapMutex);
    auto it = handleAndKeySystemMap.find(handle);
    if (it != handleAndKeySystemMap.end()) {
        it->second->Destroy();
        it->second = nullptr;
        /* Release after the OEM service is started */
        std::lock_guard<std::mutex> lockLib(libMutex);
        for (auto libMapIt = libMap.begin(); libMapIt != libMap.end(); libMapIt++) {
            if (libMapIt->second == handle) {
                libMap.erase(libMapIt->first);
                std::unique_lock<std::shared_mutex> lock(lazyLoadPluginInfoMapMtx);
                if (lazyLoadPluginInfoMap.find(libMapIt->first) != lazyLoadPluginInfoMap.end()) {
                    std::string lazyLoadPluginName = libMapIt->first;
                    ReleaseSevices(lazyLoadPluginName);
                }
            }
        }
    }
    handleAndKeySystemMap.erase(handle);
    if (handle != nullptr) {
        dlclose(handle);
        handle = nullptr;
    }
}

void DrmHostManager::GetOemLibraryPath(std::vector<std::string> &libsToLoad)
{
    DIR *dir = nullptr;
    struct dirent *entry = nullptr;
    if ((dir = opendir(OEM_CERTIFICATE_PATH)) != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            std::string fileName = entry->d_name;
            DRM_DEBUG_LOG("DrmHostManager::ServiceThreadMain fileName:%{public}s.", fileName.c_str());
            if (fileName.find(".so") == std::string::npos) {
                continue;
            }
            std::string fullPath = OEM_CERTIFICATE_PATH + fileName;
            DRM_DEBUG_LOG("DrmHostManager::ServiceThreadMain fullPath:%{public}s.", fullPath.c_str());
            libsToLoad.push_back(fullPath);
        }
        closedir(dir);
        dir = nullptr;
    }
}

void DrmHostManager::ServiceThreadMain()
{
    DRM_INFO_LOG("DrmHostManager::ServiceThreadMain enter.");
    std::vector<std::string> libsToLoad;
    GetOemLibraryPath(libsToLoad);
    for (const auto &libpath : libsToLoad) {
        std::lock_guard<std::mutex> lockLib(libMutex);
        void *handle = dlopen(libpath.c_str(), RTLD_LAZY);
        if (handle) {
            loadedLibs.push_back(handle);
            auto QueryMediaKeySystemName = (QueryMediaKeySystemNameFuncType)dlsym(handle, "QueryMediaKeySystemName");
            auto IsProvisionRequired = (IsProvisionRequiredFuncType)dlsym(handle, "IsProvisionRequired");
            auto SetMediaKeySystem = (SetMediaKeySystemFuncType)dlsym(handle, "SetMediaKeySystem");
            auto ThreadExitNotify = (ThreadExitNotifyFuncType)dlsym(handle, "ThreadExitNotify");
            auto StartThread = (StartThreadFuncType)dlsym(handle, "StartThread");
            if (QueryMediaKeySystemName && SetMediaKeySystem && ThreadExitNotify && StartThread) {
                std::string pluginName;
                int32_t ret = QueryMediaKeySystemName(pluginName);
                DRM_CHECK_AND_CONTINUE_LOG(ret == DRM_OK, "DrmHostManager::QueryMediaKeySystemName error!");
                ret = CreateMediaKeySystem(pluginName, hdiMediaKeySystem);
                DRM_CHECK_AND_CONTINUE_LOG(ret == DRM_OK, "DrmHostManager::CreateMediaKeySystem error!");
                std::lock_guard<std::recursive_mutex> lockHandle(handleAndKeySystemMapMutex);
                handleAndKeySystemMap.insert(std::make_pair(handle, hdiMediaKeySystem));
                ret = SetMediaKeySystem(hdiMediaKeySystem);
                DRM_CHECK_AND_CONTINUE_LOG(ret == DRM_OK, "DrmHostManager::SetMediaKeySystem error!");
                if (IsProvisionRequired()) {
                    libMap[pluginName] = handle;
                    ret = ThreadExitNotify(DrmHostManager::UnLoadOEMCertifaicateService);
                    DRM_CHECK_AND_CONTINUE_LOG(ret == DRM_OK, "DrmHostManager::ThreadExitNotify error!");
                    ret = StartThread();
                    DRM_CHECK_AND_CONTINUE_LOG(ret == DRM_OK, "DrmHostManager::StartThread error!");
                } else {
                    loadedLibs.pop_back();
                    ReleaseHandleAndKeySystemMap(handle);
                    DRM_DEBUG_LOG("Faild to obtain the symbol table in the so!");
                }
            }
        }
    }
    ReportServiceBehaviorEvent("DRM_OEM_SERVICE", "start");
    DRM_INFO_LOG("DrmHostManager::ServiceThreadMain exit.");
}

void DrmHostManager::UnLoadOEMCertifaicateService(std::string &name, ExtraInfo info)
{
    DRM_INFO_LOG("DrmHostManager::UnLoadOEMCertifaicateService enter.");
    std::lock_guard<std::mutex> lock(queueMutex);
    Message message(Message::UnLoadOEMCertifaicateService, name, info);
    messageQueue.push(message);
    cv.notify_all();
    DRM_INFO_LOG("DrmHostManager::UnLoadOEMCertifaicateService exit.");
}

void DrmHostManager::OemCertificateManager()
{
    DRM_INFO_LOG("DrmHostManager::OemCertificateManager enter.");
    serviceThreadRunning = true;
    serviceThread = std::thread(&DrmHostManager::ServiceThreadMain, this);
    ProcessMessage();
    DRM_INFO_LOG("DrmHostManager::OemCertificateManager exit.");
}

int32_t DrmHostManager::Init(void)
{
    DRM_INFO_LOG("DrmHostManager::Init enter.");
    OemCertificateManager();
    DRM_INFO_LOG("DrmHostManager::Init exit.");
    return DRM_OK;
}

void DrmHostManager::DeInit(void)
{
    DRM_INFO_LOG("DrmHostManager::DeInit");
}

void DrmHostManager::OnReceive(const HDI::ServiceManager::V1_0::ServiceStatus &status) {}

std::string DrmHostManager::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

void DrmHostManager::parseLazyLoadService(std::ifstream& file,
    std::map<std::string, std::string>& lazyLoadPluginInfoMap) {
    std::string line;
    while (getline(file, line)) {
        line = trim(line);
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
            std::string key = trim(line.substr(0, colonPos));
            std::string value = trim(line.substr(colonPos + 1));
            /* Further trim the value */
            if (!value.empty() && value.back() == '"') {
                /* Remove the last closing quotation mark of value */
                value.pop_back();
            }
            lazyLoadPluginInfoMap[key] = value;
        }
    }
}

int32_t DrmHostManager::loadPluginInfo(const std::string& filePath)
{
    int fd = open(filePath.c_str(), O_RDONLY);
    if (fd == -1) {
        DRM_ERR_LOG("DrmHostManager::loadPluginInfo unable to open file:%{public}s.", filePath.c_str());
        return DRM_HOST_ERROR;
    }

    std::ifstream file(filePath);
    if (!file.is_open()) {
        DRM_ERR_LOG("DrmHostManager::loadPluginInfo unable to open file:%{public}s.", filePath.c_str());
        close(fd);
        return DRM_HOST_ERROR;
    }

    std::string line;
    bool inPluginServices = false;
    while (getline(file, line)) {
        line = trim(line);
        if (line == "\"plugin_services\": {") {
            inPluginServices = true;
        } else if (inPluginServices) {
            if (line == "}") {
                break;
            } else if (line == "\"lazy_load_service\": [") {
                parseLazyLoadService(file, lazyLoadPluginInfoMap);
                break; // Exit after parsing lazy_load_service
            }
        }
    }
    file.close();
    close(fd);
    return DRM_OK;
}

void DrmHostManager::ReleaseSevices(std::string &name)
{
    /* No need to release non lazy loading */
    if (lazyLoadPluginInfoMap.count(name) <= 0) {
        return;
    }
    if (pluginCountMap.empty()) {
        return;
    }
    pluginCountMap[name]--;
    if (pluginCountMap[name] == 0) {
        sptr<IDeviceManager> deviceMgr = IDeviceManager::Get();
        if (deviceMgr == nullptr) {
            DRM_ERR_LOG("DrmHostManager deviceMgr == nullptr");
            return;
        }
        deviceMgr->UnloadDevice(lazyLoadPluginInfoMap[name]);
        lazyLoadPluginInfoMap.erase(name);
        pluginCountMap.erase(name);
    }
}

int32_t DrmHostManager::GetSevices(std::string &name, bool *isSurpported)
{
    DRM_INFO_LOG("DrmHostManager::GetSevices enter, name:%{public}s.", name.c_str());
    int32_t ret;
    std::vector<std::string> serviceName;
    /* The plugin service corresponding to name has been started, no need to reload, just mark the count. */
    if (drmHostServieProxyMap[name] != nullptr) {
        ret = drmHostServieProxyMap[name]->IsMediaKeySystemSupported(name, "", SECURE_UNKNOWN, *isSurpported);
        if (ret == DRM_OK) {
            pluginCountMap[name]++;
            DRM_DEBUG_LOG("DrmHostManager::GetSevices, the plugin service has been pulled up.");
            return DRM_OK;
        }
    }
    auto servmgr = IServiceManager::Get();
    ret = servmgr->ListServiceByInterfaceDesc(serviceName, "ohos.hdi.drm.v1_0.IMediaKeySystemFactory");
    if (ret != DRM_OK) {
        DRM_ERR_LOG("ListServiceByInterfaceDesc faild, return Code:%{public}d", ret);
        return ret;
    }
    /*
     * If the plugin is configured with lazy loading, read the service name from the configuration file.
     * If lazy loading is not configured, traverse the service bound by the interface descriptor, and obtain
     * the plugin service instance through the uuid and issuport interfaces
    */
    ret = loadPluginInfo(PLUGIN_LAZYLOAD_CONFIG_PATH);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("loadPluginInfo faild, return Code:%{public}d", ret);
        return ret;
    }
    if (lazyLoadPluginInfoMap.count(name) > 0) {
        auto it = std::find(serviceName.begin(), serviceName.end(), lazyLoadPluginInfoMap[name]);
        if (it == serviceName.end()) {
            sptr<IDeviceManager> deviceMgr = IDeviceManager::Get();
            if (deviceMgr == nullptr) {
                DRM_ERR_LOG("DrmHostManager:GetSevices deviceMgr == nullptr");
                return DRM_SERVICE_ERROR;
            }
            ret = deviceMgr->LoadDevice(lazyLoadPluginInfoMap[name]);
            if (ret != DRM_OK) {
                DRM_ERR_LOG("DrmHostManager::GetSevices loadDevice return Code:%{public}d", ret);
                return DRM_HOST_ERROR;
            }
            serviceName.push_back(lazyLoadPluginInfoMap[name]);
        }
    }
    for (auto hdiServiceName : serviceName) {
        sptr<IMediaKeySystemFactory> drmHostServieProxy =
            OHOS::HDI::Drm::V1_0::IMediaKeySystemFactory::Get(hdiServiceName, false);
        if (drmHostServieProxy == nullptr) {
            DRM_ERR_LOG("DrmHostManager::GetSevices failed.");
            continue;
        }
        ret = drmHostServieProxy->IsMediaKeySystemSupported(name, "", SECURE_UNKNOWN, *isSurpported);
        if (ret != DRM_OK) {
            DRM_ERR_LOG("IsMediaKeySystemSupported return Code:%{public}d", ret);
            continue;
        }
        drmHostServieProxyMap[name] = drmHostServieProxy;
    }
    if (serviceName.empty()) {
        DRM_INFO_LOG("DrmHostManager::GetSevices exit, No DRM driver service named:%{public}s configured.",
            name.c_str());
        return DRM_SERVICE_ERROR;
    }
    if (pluginCountMap.empty()) {
        pluginCountMap[name] = 1;
    } else {
        pluginCountMap[name]++;
    }

    DRM_INFO_LOG("DrmHostManager::GetSevices exit, name:%{public}s.", name.c_str());
    return DRM_OK;
}

int32_t DrmHostManager::IsMediaKeySystemSupported(std::string &name, bool *isSurpported)
{
    DRM_INFO_LOG("DrmHostManager::IsMediaKeySystemSupported one parameters enter, name:%{public}s.", name.c_str());
    /* Lock will be released when lock goes out of scope */
    std::unique_lock<std::shared_mutex> lock(lazyLoadPluginInfoMapMtx);
    int32_t ret = GetSevices(name, isSurpported);
    if (ret != DRM_OK || drmHostServieProxyMap[name] == nullptr) {
        *isSurpported = false;
        ReleaseSevices(name);
        return DRM_SERVICE_ERROR;
    }
    drmHostServieProxyMap[name] = nullptr;
    ReleaseSevices(name);
    DRM_INFO_LOG("DrmHostManager::IsMediaKeySystemSupported one parameters exit, isSurpported:%{public}d.",
        *isSurpported);
    return DRM_OK;
}

int32_t DrmHostManager::IsMediaKeySystemSupported(std::string &name, std::string &mimeType, bool *isSurpported)
{
    DRM_INFO_LOG(
        "DrmHostManager::IsMediaKeySystemSupported two parameters enter, name:%{public}s, mimeType:%{public}s.",
        name.c_str(), mimeType.c_str());
    /* Lock will be released when lock goes out of scope */
    std::unique_lock<std::shared_mutex> lock(lazyLoadPluginInfoMapMtx);
    int32_t ret = GetSevices(name, isSurpported);
    if (ret != DRM_OK || drmHostServieProxyMap[name] == nullptr) {
        *isSurpported = false;
        ReleaseSevices(name);
        return DRM_SERVICE_ERROR;
    }
    if (mimeType.length() == 0) {
        *isSurpported = false;
        ReleaseSevices(name);
        DRM_ERR_LOG("IsMediaKeySystemSupported mimeType is null!");
        return DRM_SERVICE_ERROR;
    }
    ret = drmHostServieProxyMap[name]->IsMediaKeySystemSupported(name, mimeType, SECURE_UNKNOWN, *isSurpported);
    if (ret != 0) {
        DRM_ERR_LOG("IsMediaKeySystemSupported return Code:%{public}d", ret);
    }
    drmHostServieProxyMap[name] = nullptr;
    ReleaseSevices(name);
    DRM_INFO_LOG("DrmHostManager::IsMediaKeySystemSupported two parameters exit, isSurpported:%{public}d.",
        *isSurpported);
    return DRM_OK;
}

int32_t DrmHostManager::IsMediaKeySystemSupported(std::string &name, std::string &mimeType, int32_t securityLevel,
    bool *isSurpported)
{
    DRM_INFO_LOG("DrmHostManager::IsMediaKeySystemSupported three parameters enter, name:%{public}s, "
        "mimeType:%{public}s, securityLevel:%{public}d.",
        name.c_str(), mimeType.c_str(), securityLevel);
    /* Lock will be released when lock goes out of scope */
    std::unique_lock<std::shared_mutex> lock(lazyLoadPluginInfoMapMtx);
    int32_t ret = GetSevices(name, isSurpported);
    if (ret != DRM_OK || drmHostServieProxyMap[name] == nullptr) {
        *isSurpported = false;
        ReleaseSevices(name);
        return DRM_SERVICE_ERROR;
    }
    if (mimeType.length() == 0) {
        *isSurpported = false;
        ReleaseSevices(name);
        DRM_ERR_LOG("mimeType is null!");
        return DRM_SERVICE_ERROR;
    }
    ret = drmHostServieProxyMap[name]->IsMediaKeySystemSupported(name, mimeType,
        (OHOS::HDI::Drm::V1_0::ContentProtectionLevel)securityLevel, *isSurpported);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("IsMediaKeySystemSupported return Code:%{public}d", ret);
    }
    drmHostServieProxyMap[name] = nullptr;
    ReleaseSevices(name);
    DRM_INFO_LOG("DrmHostManager::IsMediaKeySystemSupported three parameters exit, isSurpported:%{public}d.",
        *isSurpported);
    return DRM_OK;
}

void DrmHostManager::ReleaseMediaKeySystem(std::string &name)
{
    std::unique_lock<std::shared_mutex> lock(lazyLoadPluginInfoMapMtx);
    ReleaseSevices(name);
}

int32_t DrmHostManager::CreateMediaKeySystem(std::string &name, sptr<IMediaKeySystem> &hdiMediaKeySystem)
{
    DRM_INFO_LOG("DrmHostManager::CreateMediaKeySystem enter.");
    bool isSurpported = false;
    /* Lock will be released when lock goes out of scope */
    std::unique_lock<std::shared_mutex> lock(lazyLoadPluginInfoMapMtx);
    int32_t ret = GetSevices(name, &isSurpported);
    if (ret != DRM_OK || drmHostServieProxyMap[name] == nullptr) {
        DRM_ERR_LOG("DrmHostManager::CreateMediaKeySystem faild.");
        return ret;
    }
    drmHostDeathRecipient_ = new DrmHostDeathRecipient();
    const sptr<IRemoteObject> &remote = OHOS::HDI::hdi_objcast<IMediaKeySystemFactory>(drmHostServieProxyMap[name]);
    if (remote != nullptr) {
        bool result = remote->AddDeathRecipient(drmHostDeathRecipient_);
        if (!result) {
            DRM_ERR_LOG("AddDeathRecipient for drm Host failed.");
            return DRM_HOST_ERROR;
        }
    }
    ret = drmHostServieProxyMap[name]->CreateMediaKeySystem(hdiMediaKeySystem);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("drmHostServieProxyMap CreateMediaKeySystem return Code:%{public}d", ret);
        return ret;
    }
    drmHostServieProxyMap[name] = nullptr;
    DRM_INFO_LOG("DrmHostManager::CreateMediaKeySystem exit.");
    return DRM_OK;
}

int32_t DrmHostManager::GetMediaKeySystemUuid(std::string &name, std::string &uuid)
{
    DRM_INFO_LOG("DrmHostManager::GetMediaKeySystemUuid enter.");
    bool isSurpported = false;
    /* Lock will be released when lock goes out of scope */
    std::unique_lock<std::shared_mutex> lock(lazyLoadPluginInfoMapMtx);
    int32_t ret = GetSevices(name, &isSurpported);
    if (ret != DRM_OK || drmHostServieProxyMap[name] == nullptr) {
        DRM_INFO_LOG("DrmHostManager::GetMediaKeySystemUuid faild.");
        return ret;
    }
    drmHostDeathRecipient_ = new DrmHostDeathRecipient();
    const sptr<IRemoteObject> &remote = OHOS::HDI::hdi_objcast<IMediaKeySystemFactory>(drmHostServieProxyMap[name]);
    if (remote != nullptr) {
        bool result = remote->AddDeathRecipient(drmHostDeathRecipient_);
        if (!result) {
            DRM_ERR_LOG("AddDeathRecipient for drm Host failed.");
            return DRM_HOST_ERROR;
        }
    }

    ret = drmHostServieProxyMap[name]->GetMediaKeySystemDescription(name, uuid);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("drmHostServieProxyMap GetMediaKeySystemUuid return Code:%{public}d", ret);
        return ret;
    }
    drmHostServieProxyMap[name] = nullptr;
    DRM_INFO_LOG("DrmHostManager::GetMediaKeySystemUuid exit.");
    return DRM_OK;
}

int32_t DrmHostManager::GetMediaKeySystems(std::map<std::string, std::string> &mediaKeySystemDescription)
{
    DRM_INFO_LOG("DrmHostManager::GetMediaKeySystems enter.");
    int32_t ret;
    std::vector<std::string> lazyLoadServiceNames;
    std::vector<std::string> pluginServiceNames;
    std::unique_lock<std::shared_mutex> lock(lazyLoadPluginInfoMapMtx);
    auto servmgr = IServiceManager::Get();
    ret = servmgr->ListServiceByInterfaceDesc(pluginServiceNames, "ohos.hdi.drm.v1_0.IMediaKeySystemFactory");
    if (ret != DRM_OK) {
        DRM_ERR_LOG("ListServiceByInterfaceDesc faild, return Code:%{public}d", ret);
        return ret;
    }
    /*
     * If the plugin is configured with lazy loading, read the service name from the configuration file.
     * If lazy loading is not configured, traverse the service bound by the interface descriptor, and obtain
     * the plugin service instance through the uuid and issuport interfaces
    */
    loadPluginInfo(PLUGIN_LAZYLOAD_CONFIG_PATH);
    sptr<IDeviceManager> deviceMgr = IDeviceManager::Get();
    if (deviceMgr == nullptr) {
        DRM_ERR_LOG("DrmHostManager:GetMediaKeySystems deviceMgr == nullptr");
    }
    for (auto pluginInfoIt = lazyLoadPluginInfoMap.begin(); pluginInfoIt != lazyLoadPluginInfoMap.end();
        pluginInfoIt++) {
        ret = deviceMgr->LoadDevice(pluginInfoIt->second);
        if (ret != DRM_OK) {
            DRM_ERR_LOG("DrmHostManager LoadDevice return Code:%{public}d", ret);
            continue;
        }
        lazyLoadServiceNames.push_back(pluginInfoIt->second);
        if (pluginCountMap.empty()) {
            pluginCountMap[pluginInfoIt->first] = 1;
        } else {
            pluginCountMap[pluginInfoIt->first]++;
        }
    }
    for (uint32_t i = 0; i < lazyLoadServiceNames.size(); i++) {
        bool found = false;
        for (uint32_t j = 0; j < pluginServiceNames.size(); j++) {
            if (lazyLoadServiceNames[i] == pluginServiceNames[j]) {
                found = true;
                break;
            }
        }
        if (found == false) {
            pluginServiceNames.push_back(lazyLoadServiceNames[i]);
        }
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
        mediaKeySystemDescription.insert(std::pair<std::string, std::string>(pluginName, pluginUuid));
    }
    for (auto lazyLoadServiceName : lazyLoadServiceNames) {
        ReleaseSevices(lazyLoadServiceName);
    }
    DRM_INFO_LOG("DrmHostManager::GetMediaKeySystems exit.");
    return DRM_OK;
}
} // namespace DrmStandard
} // namespace OHOS
