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
#include "hdf_device_class.h"
#include "iremote_broker.h"
#include "iservmgr_hdi.h"
#include "servmgr_hdi.h"
#include "drm_log.h"
#include "drm_error_code.h"
#include "drm_napi_utils.h"
#include "drm_host_manager.h"

namespace OHOS {
namespace DrmStandard {
using OHOS::HDI::ServiceManager::V1_0::IServiceManager;
std::queue<Message> DrmHostManager::messageQueue;
std::mutex DrmHostManager::queueMutex;
std::mutex DrmHostManager::libMutex;
std::condition_variable DrmHostManager::cv;
std::mutex DrmHostManager::libMapMutex;
std::map<std::string, void *> DrmHostManager::libMap;
std::mutex DrmHostManager::handleAndKeySystemMapMutex;
std::map<void *, sptr<IMediaKeySystem>> DrmHostManager::handleAndKeySystemMap;

void DrmHostManager::DrmHostDeathRecipient::OnRemoteDied(const wptr<IRemoteObject> &remote)
{
    DRM_DEBUG_LOG("Remote died, do clean works.");
}

DrmHostManager::DrmHostManager(StatusCallback *statusCallback)
    : statusCallback_(statusCallback), drmHostServieProxy_(nullptr)
{}

DrmHostManager::~DrmHostManager()
{
    DRM_INFO_LOG("DrmHostManager::~DrmHostManager enter.");
    if (drmHostServieProxy_ != nullptr) {
        drmHostServieProxy_ = nullptr;
    }
    if (statusCallback_ != nullptr) {
        statusCallback_ = nullptr;
    }
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
        dlclose(libHandle);
        ReleaseHandleAndKeySystemMap(libHandle);
    }
    loadedLibs.clear();
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
                    std::lock_guard<std::mutex> lockLibMap(libMapMutex);
                    auto it = libMap.find(message.uuid);
                    if (it != libMap.end()) {
                        libHandle = it->second;
                    }
                    if (libHandle) {
                        std::lock_guard<std::mutex> lockLib(libMutex);
                        dlclose(libHandle);
                        ReleaseHandleAndKeySystemMap(libHandle);
                        loadedLibs.erase(std::remove(loadedLibs.begin(), loadedLibs.end(), libHandle),
                            loadedLibs.end());
                        libHandle = nullptr;
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
    std::lock_guard<std::mutex> lock(handleAndKeySystemMapMutex);
    auto it = handleAndKeySystemMap.find(handle);
    if (it != handleAndKeySystemMap.end()) {
        it->second->Destroy();
        it->second = nullptr;
    }
    handleAndKeySystemMap.erase(handle);
}

void DrmHostManager::ServiceThreadMain()
{
    DRM_INFO_LOG("DrmHostManager::ServiceThreadMain enter.");
    DIR *dir = nullptr;
    struct dirent *entry = nullptr;
    if ((dir = opendir("/system/lib64/oem_certificate_service")) != nullptr) {
        while ((entry = readdir(dir)) != nullptr) {
            std::string fileName = entry->d_name;
            DRM_DEBUG_LOG("DrmHostManager::ServiceThreadMain fileName:%{public}s.", fileName.c_str());
            if (fileName.find(".so") == std::string::npos) {
                continue;
            }
            std::string fullPath = "/system/lib64/oem_certificate_service/" + fileName;
            DRM_DEBUG_LOG("DrmHostManager::ServiceThreadMain fullPath:%{public}s.", fullPath.c_str());
            libsToLoad.push_back(fullPath);
        }
    }
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
            auto StopThread = (StopThreadFuncType)dlsym(handle, "StopThread");
            if (QueryMediaKeySystemName && SetMediaKeySystem && ThreadExitNotify && StartThread && StopThread) {
                std::string uuid;
                int32_t ret = QueryMediaKeySystemName(uuid);
                DRM_CHECK_AND_RETURN_LOG(ret == DRM_OK, "QueryMediaKeySystemName faild.");
                ret = CreateMediaKeySystem(uuid, hdiMediaKeySystem);
                std::lock_guard<std::mutex> lockHandle(handleAndKeySystemMapMutex);
                handleAndKeySystemMap.insert(std::make_pair(handle, hdiMediaKeySystem));
                DRM_CHECK_AND_RETURN_LOG(ret == DRM_OK && hdiMediaKeySystem != nullptr, "CreateMediaKeySystem faild.");
                ret = SetMediaKeySystem(hdiMediaKeySystem);
                if (ret != DRM_OK) {
                    ReleaseHandleAndKeySystemMap(handle);
                    DRM_ERR_LOG("DrmHostManager::SetMediaKeySystem error!");
                    return;
                }
                if (IsProvisionRequired()) {
                    std::lock_guard<std::mutex> lockLibMap(libMapMutex);
                    libMap[uuid] = handle;
                    ret = ThreadExitNotify(DrmHostManager::UnLoadOEMCertifaicateService);
                    if (ret != DRM_OK) {
                        ReleaseHandleAndKeySystemMap(handle);
                        DRM_ERR_LOG("DrmHostManager::ThreadExitNotify error!");
                        return;
                    }
                    ret = StartThread();
                    if (ret != DRM_OK) {
                        ReleaseHandleAndKeySystemMap(handle);
                        DRM_ERR_LOG("DrmHostManager::StartThread error!");
                        return;
                    }
                } else {
                    dlclose(handle);
                    loadedLibs.pop_back();
                    ReleaseHandleAndKeySystemMap(handle);
                    DRM_DEBUG_LOG("OEM certificate exist!");
                }
            }
        }
    }
    DRM_INFO_LOG("DrmHostManager::ServiceThreadMain exit.");
}

void DrmHostManager::UnLoadOEMCertifaicateService(std::string &uuid, ExtraInfo info)
{
    DRM_INFO_LOG("DrmHostManager::UnLoadOEMCertifaicateService enter.");
    std::lock_guard<std::mutex> lock(queueMutex);
    Message message(Message::UnLoadOEMCertifaicateService, uuid, info);
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
    if (drmHostServieProxy_ != nullptr) {
        DRM_ERR_LOG("DrmHostManager::Init, no drm host proxy");
        return DRM_HOST_ERROR;
    }
    OemCertificateManager();
    DRM_INFO_LOG("DrmHostManager::Init exit.");
    return DRM_OK;
}

void DrmHostManager::DeInit(void)
{
    DRM_INFO_LOG("DrmHostManager::DeInit");
}

void DrmHostManager::OnReceive(const HDI::ServiceManager::V1_0::ServiceStatus &status) {}

int32_t DrmHostManager::GetSevices(std::string &uuid, bool *isSurpported)
{
    DRM_INFO_LOG("DrmHostManager::GetSevices enter, uuid:%{public}s.", uuid.c_str());
    std::vector<std::string> serviceName;
    auto servmgr = IServiceManager::Get();
    int32_t ret = servmgr->ListServiceByInterfaceDesc(serviceName, "ohos.hdi.drm.v1_0.IMediaKeySystemFactory");
    if (ret != 0) {
        DRM_ERR_LOG("ListServiceByInterfaceDesc faild, return Code:%{public}d", ret);
        return ret;
    }

    for (auto hdiServiceName : serviceName) {
        drmHostServieProxy_ = OHOS::HDI::Drm::V1_0::IMediaKeySystemFactory::Get(hdiServiceName, false);
        if (drmHostServieProxy_ == nullptr) {
            DRM_ERR_LOG("Failed to GetSevices");
            return DRM_HOST_ERROR;
        }
        ret = drmHostServieProxy_->IsMediaKeySystemSupported(uuid, "", SECURE_UNKNOWN, *isSurpported);
        if (ret != 0) {
            DRM_ERR_LOG("drmHostServieProxy_ return Code:%{public}d", ret);
            return DRM_HOST_ERROR;
        }
        if (*isSurpported == true) {
            DRM_ERR_LOG("DrmHostManager::GetSevices exit, uuid:%{public}s.", uuid.c_str());
            return DRM_OK;
        }
    }
    if (*isSurpported == false) {
        DRM_ERR_LOG("The drm for uuid is not support");
        return DRM_HOST_ERROR;
    }
    DRM_INFO_LOG("DrmHostManager::GetSevices exit, uuid:%{public}s.", uuid.c_str());
    return DRM_OK;
}

int32_t DrmHostManager::IsMediaKeySystemSupported(std::string &uuid, bool *isSurpported)
{
    DRM_INFO_LOG("DrmHostManager::IsMediaKeySystemSupported one parameters enter, uuid:%{public}s.", uuid.c_str());
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t ret = GetSevices(uuid, isSurpported);
    if (ret != DRM_OK) {
        *isSurpported = false;
        return ret;
    }
    *isSurpported = true;
    DRM_INFO_LOG("DrmHostManager::IsMediaKeySystemSupported one parameters exit, isSurpported:%{public}d.",
        *isSurpported);
    return DRM_OK;
}

int32_t DrmHostManager::IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType, bool *isSurpported)
{
    DRM_INFO_LOG(
        "DrmHostManager::IsMediaKeySystemSupported two parameters enter, uuid:%{public}s, mimeType:%{public}s.",
        uuid.c_str(), mimeType.c_str());
    int32_t ret = GetSevices(uuid, isSurpported);
    if (ret != DRM_OK) {
        *isSurpported = false;
        return ret;
    }
    if (mimeType.length() == 0) {
        *isSurpported = false;
        DRM_ERR_LOG("mimeType is null!");
        return DRM_SERVICE_ERROR;
    }
    ret = drmHostServieProxy_->IsMediaKeySystemSupported(uuid, mimeType, SECURE_UNKNOWN, *isSurpported);
    if (ret != 0) {
        DRM_ERR_LOG("drmHostServieProxy_ return Code:%{public}d", ret);
    }
    DRM_INFO_LOG("DrmHostManager::IsMediaKeySystemSupported two parameters exit, isSurpported:%{public}d.",
        *isSurpported);
    return DRM_OK;
}

int32_t DrmHostManager::IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType, int32_t securityLevel,
    bool *isSurpported)
{
    DRM_INFO_LOG("DrmHostManager::IsMediaKeySystemSupported three parameters enter, uuid:%{public}s, "
        "mimeType:%{public}s, securityLevel:%{public}d.",
        uuid.c_str(), mimeType.c_str(), securityLevel);
    int32_t ret = GetSevices(uuid, isSurpported);
    if (ret != DRM_OK) {
        *isSurpported = false;
        return ret;
    }
    if (mimeType.length() == 0) {
        *isSurpported = false;
        DRM_ERR_LOG("mimeType is null!");
        return DRM_SERVICE_ERROR;
    }
    ret = drmHostServieProxy_->IsMediaKeySystemSupported(uuid, mimeType,
        (OHOS::HDI::Drm::V1_0::SecurityLevel)securityLevel, *isSurpported);
    if (ret != 0) {
        DRM_ERR_LOG("drmHostServieProxy_ return Code:%{public}d", ret);
    }
    DRM_INFO_LOG("DrmHostManager::IsMediaKeySystemSupported three parameters exit, isSurpported:%{public}d.",
        *isSurpported);
    return DRM_OK;
}

int32_t DrmHostManager::CreateMediaKeySystem(std::string &uuid, sptr<IMediaKeySystem> &hdiMediaKeySystem)
{
    DRM_INFO_LOG("DrmHostManager::CreateMediaKeySystem enter.");
    bool sup = false;
    bool *isSurpported = &sup;
    int32_t ret = GetSevices(uuid, isSurpported);
    if (ret != DRM_OK) {
        DRM_INFO_LOG("DrmHostManager::CreateMediaKeySystem faild.");
        return ret;
    }
    sptr<DrmHostDeathRecipient> drmHostDeathRecipient = new DrmHostDeathRecipient();
    const sptr<IRemoteObject> &remote = OHOS::HDI::hdi_objcast<IMediaKeySystemFactory>(drmHostServieProxy_);
    bool result = remote->AddDeathRecipient(drmHostDeathRecipient);
    if (!result) {
        DRM_ERR_LOG("AddDeathRecipient for drm Host failed.");
        return DRM_HOST_ERROR;
    }

    ret = drmHostServieProxy_->CreateMediaKeySystem(hdiMediaKeySystem);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("drmHostServieProxy_ CreateMediaKeySystem return Code:%{public}d", ret);
        return ret;
    }
    DRM_INFO_LOG("DrmHostManager::CreateMediaKeySystem exit.");
    return DRM_OK;
}
} // namespace DrmStandard
} // namespace OHOS
