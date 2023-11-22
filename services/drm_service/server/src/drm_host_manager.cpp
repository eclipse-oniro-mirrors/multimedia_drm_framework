/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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
void DrmHostManager::DrmHostDeathRecipient::OnRemoteDied(const wptr<IRemoteObject> &remote)
{
    DRM_DEBUG_LOG("Remote died, do clean works.");
}

DrmHostManager::DrmHostManager(StatusCallback *statusCallback)
    : statusCallback_(statusCallback), drmHostServieProxy_(nullptr)
{}

DrmHostManager::~DrmHostManager()
{
    if (drmHostServieProxy_ != nullptr) {
        drmHostServieProxy_ = nullptr;
    }
    if (statusCallback_ != nullptr) {
        statusCallback_ = nullptr;
    }
}

int32_t DrmHostManager::Init(void)
{
    DRM_INFO_LOG("DrmHostManager::Init enter.");
    if (drmHostServieProxy_ != nullptr) {
        DRM_ERR_LOG("DrmHostManager::Init, no drm host proxy");
        return DRM_HOST_ERROR;
    }
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
    DRM_INFO_LOG("DrmHostManager::IsMediaKeySystemSupported one parameters exit, isSurpported:%{public}d.", *isSurpported);
    return DRM_OK;
}

int32_t DrmHostManager::IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType, bool *isSurpported)
{
    DRM_INFO_LOG("DrmHostManager::IsMediaKeySystemSupported two parameters enter, uuid:%{public}s, mimeType:%{public}s.", uuid.c_str(), mimeType.c_str());
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
    DRM_INFO_LOG("DrmHostManager::IsMediaKeySystemSupported two parameters exit, isSurpported:%{public}d.", *isSurpported);
    return DRM_OK;
}

int32_t DrmHostManager::IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType,
    int32_t securityLevel, bool *isSurpported)
{
    DRM_INFO_LOG("DrmHostManager::IsMediaKeySystemSupported three parameters enter, uuid:%{public}s, mimeType:%{public}s, securityLevel:%{public}d.", uuid.c_str(), mimeType.c_str(), securityLevel);
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
    DRM_INFO_LOG("DrmHostManager::IsMediaKeySystemSupported three parameters exit, isSurpported:%{public}d.", *isSurpported);
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
