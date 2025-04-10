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

#include "media_key_system_impl.h"
#include "i_mediakeysystem_service.h"
#include "drm_error_code.h"
#include "drm_trace.h"
#include "napi_param_utils.h"

namespace OHOS {
namespace DrmStandard {
MediaKeySystemImpl::MediaKeySystemImpl(sptr<IMediaKeySystemService> &mediaKeysystem) : serviceProxy_(mediaKeysystem)
{
    DRM_DEBUG_LOG("MediaKeySystemImpl:0x %{public}06" PRIXPTR "MediaKeySystemImpl Instances create",
        FAKE_POINTER(this));

    sptr<IRemoteObject> object = serviceProxy_->AsObject();
    pid_t pid = 0;
    deathRecipient_ = new(std::nothrow) DrmDeathRecipient(pid);
    DRM_CHECK_AND_RETURN_LOG(deathRecipient_ != nullptr, "failed to new DrmDeathRecipient.");

    deathRecipient_->SetNotifyCb([this] (pid_t pid) {
        this->MediaKeySystemServerDied(pid);
    });
    bool result = object->AddDeathRecipient(deathRecipient_);
    if (!result) {
        DRM_ERR_LOG("failed to add deathRecipient");
        return;
    }
}

MediaKeySystemImpl::~MediaKeySystemImpl()
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    serviceProxy_ = nullptr;
}

void MediaKeySystemImpl::MediaKeySystemServerDied(pid_t pid)
{
    DRM_ERR_LOG("MediaKeySystem server has died, pid:%{public}d!", pid);
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    if (serviceProxy_ != nullptr && serviceProxy_->AsObject() != nullptr) {
        (void)serviceProxy_->AsObject()->RemoveDeathRecipient(deathRecipient_);
        serviceProxy_ = nullptr;
        deathRecipient_ = nullptr;
    }
}

int32_t MediaKeySystemImpl::Release()
{
    DRM_INFO_LOG("Release enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t ret = DRM_INNER_ERR_UNKNOWN;
    if (serviceProxy_ != nullptr) {
        sptr<IRemoteObject> object = serviceProxy_->AsObject();
        if (object != nullptr && deathRecipient_ != nullptr) {
            object->RemoveDeathRecipient(deathRecipient_);
            deathRecipient_ = nullptr;
        }
        ret = serviceProxy_->Release();
        if (ret != DRM_INNER_ERR_OK) {
            DRM_ERR_LOG("Failed to Release keySystem!, errCode:%{public}d", ret);
            return ret;
        }
        serviceProxy_ = nullptr;
    } else {
        DRM_ERR_LOG("serviceProxy_ is nullptr");
        return ret;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySystemImpl::GenerateKeySystemRequest(std::vector<uint8_t> &request, std::string &defaultUrl)
{
    DrmTrace trace("MediaKeySystemImpl::GenerateKeySystemRequest");
    DRM_INFO_LOG("GenerateKeySystemRequest enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t ret = DRM_INNER_ERR_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("GenerateKeySystemRequest serviceProxy_ is null");
        return DRM_INNER_ERR_SERVICE_FATAL_ERROR;
    }
    ret = serviceProxy_->GenerateKeySystemRequest(request, defaultUrl);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GenerateKeySystemRequest failed, ret: %{public}d", ret);
        return DRM_INNER_ERR_BASE;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySystemImpl::ProcessKeySystemResponse(const std::vector<uint8_t> &response)
{
    DrmTrace trace("MediaKeySystemImpl::ProcessKeySystemResponse");
    DRM_INFO_LOG("ProcessKeySystemResponse enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t ret = DRM_INNER_ERR_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("ProcessKeySystemResponse serviceProxy_ is null");
        return DRM_INNER_ERR_SERVICE_FATAL_ERROR;
    }
    ret = serviceProxy_->ProcessKeySystemResponse(response);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("ProcessKeySystemResponse failed, ret: %{public}d", ret);
        return DRM_INNER_ERR_BASE;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySystemImpl::SetConfigurationString(std::string &configName, std::string &value)
{
    DRM_INFO_LOG("SetConfiguration enter, configName:%{public}s, value:%{public}s.",
        configName.c_str(), value.c_str());
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t ret = DRM_INNER_ERR_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("SetConfiguration serviceProxy_ is null");
        return DRM_INNER_ERR_SERVICE_FATAL_ERROR;
    }
    ret = serviceProxy_->SetConfigurationString(configName, value);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("SetConfiguration failed, ret: %{public}d", ret);
        return DRM_INNER_ERR_BASE;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySystemImpl::GetConfigurationString(std::string &configName, std::string &value)
{
    DRM_INFO_LOG("GetConfiguration enter, configName:%{public}s.", configName.c_str());
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t ret = DRM_INNER_ERR_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("GetConfiguration serviceProxy_ is null");
        return DRM_INNER_ERR_SERVICE_FATAL_ERROR;
    }
    ret = serviceProxy_->GetConfigurationString(configName, value);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GetConfiguration failed, ret: %{public}d", ret);
        return DRM_INNER_ERR_BASE;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySystemImpl::SetConfigurationByteArray(std::string &configName, std::vector<uint8_t> &value)
{
    DRM_INFO_LOG("SetConfiguration enter, configName:%{public}s.", configName.c_str());
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t ret = DRM_INNER_ERR_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("SetConfiguration serviceProxy_ is null");
        return DRM_INNER_ERR_SERVICE_FATAL_ERROR;
    }
    ret = serviceProxy_->SetConfigurationByteArray(configName, value);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("SetConfiguration failed, ret: %{public}d", ret);
        return DRM_INNER_ERR_BASE;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySystemImpl::GetConfigurationByteArray(std::string &configName, std::vector<uint8_t> &value)
{
    DRM_INFO_LOG("GetConfiguration enter, configName:%{public}s.", configName.c_str());
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t ret = DRM_INNER_ERR_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("GetConfiguration serviceProxy_ is null");
        return DRM_INNER_ERR_SERVICE_FATAL_ERROR;
    }
    ret = serviceProxy_->GetConfigurationByteArray(configName, value);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GetConfiguration failed, ret: %{public}d", ret);
        return DRM_INNER_ERR_BASE;
    }

    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySystemImpl::CreateMediaKeySession(IMediaKeySessionService::ContentProtectionLevel securityLevel,
    sptr<MediaKeySessionImpl> *keySessionImpl)
{
    DrmTrace trace("MediaKeySystemImpl::CreateMediaKeySession");
    DRM_INFO_LOG("CreateMediaKeySession enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    sptr<IMediaKeySessionService> keySessionProxy = nullptr;
    sptr<MediaKeySessionImpl> localMediaKeySessionImpl = nullptr;
    int32_t ret = DRM_INNER_ERR_OK;
    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("serviceProxy_ == nullptr");
        return DRM_INNER_ERR_SERVICE_FATAL_ERROR;
    }

    ret = serviceProxy_->CreateMediaKeySession(securityLevel, keySessionProxy);
    if (ret == DRM_INNER_ERR_OK) {
        if (keySessionProxy != nullptr) {
            localMediaKeySessionImpl = new (std::nothrow) MediaKeySessionImpl(keySessionProxy);
            if (localMediaKeySessionImpl == nullptr) {
                DRM_ERR_LOG("Failed to new MediaKeySessionImpl");
                return DRM_INNER_ERR_SERVICE_FATAL_ERROR;
            }
        } else {
            DRM_ERR_LOG("Service faltal error");
            return DRM_INNER_ERR_SERVICE_FATAL_ERROR;
        }
    } else {
        if (ret == DRM_INNER_ERR_MAX_SESSION_NUM_REACHED) {
            DRM_ERR_LOG("The number of MediaKeySession is greater than 64");
            return DRM_INNER_ERR_MAX_SESSION_NUM_REACHED;
        }
        DRM_ERR_LOG("Failed to get session object from mediakeysystem service!, %{public}d", ret);
        return ret;
    }
    *keySessionImpl = localMediaKeySessionImpl;
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySystemImpl::GetStatistics(std::vector<IMediaKeySystemService::MetircKeyValue> &metrics)
{
    DRM_INFO_LOG("GetStatistics enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t ret = DRM_INNER_ERR_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("GetStatistics serviceProxy_ is null");
        return DRM_INNER_ERR_SERVICE_FATAL_ERROR;
    }
    ret = serviceProxy_->GetStatistics(metrics);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GetStatistics failed, ret: %{public}d", ret);
        return DRM_INNER_ERR_BASE;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySystemImpl::GetMaxContentProtectionLevel(IMediaKeySessionService::ContentProtectionLevel *securityLevel)
{
    DRM_INFO_LOG("GetMaxContentProtectionLevel enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t ret = DRM_INNER_ERR_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("GetMaxContentProtectionLevel serviceProxy_ is null");
        return DRM_INNER_ERR_SERVICE_FATAL_ERROR;
    }
    ret =
        serviceProxy_->GetMaxContentProtectionLevel((IMediaKeySessionService::ContentProtectionLevel *)securityLevel);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GetMaxContentProtectionLevel failed, ret: %{public}d", ret);
        return DRM_INNER_ERR_BASE;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySystemImpl::GetOfflineMediaKeyIds(std::vector<std::vector<uint8_t>> &licenseIds)
{
    DRM_INFO_LOG("GetOfflineMediaKeyIds enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t ret = DRM_INNER_ERR_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("GetOfflineMediaKeyIds serviceProxy_ is null");
        return DRM_INNER_ERR_SERVICE_FATAL_ERROR;
    }
    ret = serviceProxy_->GetOfflineMediaKeyIds(licenseIds);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GetOfflineMediaKeyIds failed, ret: %{public}d", ret);
        return DRM_INNER_ERR_BASE;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySystemImpl::GetOfflineMediaKeyStatus(std::vector<uint8_t> &licenseId,
    IMediaKeySessionService::OfflineMediaKeyStatus &status)
{
    DRM_INFO_LOG("GetOfflineMediaKeyStatus enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t ret = DRM_INNER_ERR_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("GetOfflineMediaKeyStatus serviceProxy_ is null");
        return DRM_INNER_ERR_SERVICE_FATAL_ERROR;
    }
    ret = serviceProxy_->GetOfflineMediaKeyStatus(licenseId, status);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GetOfflineMediaKeyStatus failed, ret: %{public}d", ret);
        return DRM_INNER_ERR_BASE;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySystemImpl::ClearOfflineMediaKeys(std::vector<uint8_t> &licenseId)
{
    DRM_INFO_LOG("ClearOfflineMediaKeys enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t ret = DRM_INNER_ERR_OK;
    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("ClearOfflineMediaKeys serviceProxy_ is null");
        return DRM_INNER_ERR_SERVICE_FATAL_ERROR;
    }
    ret = serviceProxy_->ClearOfflineMediaKeys(licenseId);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("ClearOfflineMediaKeys failed, ret: %{public}d", ret);
        return DRM_INNER_ERR_BASE;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySystemImpl::GetCertificateStatus(IMediaKeySystemService::CertificateStatus *certStatus)
{
    DRM_INFO_LOG("GetCertificateStatus enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t ret = DRM_INNER_ERR_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("GetCertificateStatus serviceProxy_ is null");
        return DRM_INNER_ERR_SERVICE_FATAL_ERROR;
    }
    ret = serviceProxy_->GetCertificateStatus((IMediaKeySystemService::CertificateStatus *)certStatus);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GetCertificateStatus failed, ret: %{public}d", ret);
        return DRM_INNER_ERR_BASE;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySystemImpl::SetCallback(const sptr<MediaKeySystemImplCallback> &callback)
{
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " SetCallback in", FAKE_POINTER(this));
    DRM_CHECK_AND_RETURN_RET_LOG(callback != nullptr, DRM_INNER_ERR_INVALID_VAL, "callback is nullptr");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    mediaKeySystemApplicationCallback_ = callback;

    int32_t ret = DRM_INNER_ERR_BASE;
    serviceCallback_ = new (std::nothrow) MediaKeySystemCallback(this);
    if (serviceCallback_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemCallback alloc failed.");
        return ret;
    }

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("SetCallback serviceProxy_ is null");
        return DRM_INNER_ERR_SERVICE_FATAL_ERROR;
    }
    ret = serviceProxy_->SetCallback(serviceCallback_);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("SetCallback failed, ret: %{public}d", ret);
        return DRM_INNER_ERR_BASE;
    }
    return ret;
}

sptr<MediaKeySystemImplCallback> MediaKeySystemImpl::GetApplicationCallback()
{
    DRM_INFO_LOG("GetApplicationCallback");
    return mediaKeySystemApplicationCallback_;
}

MediaKeySystemCallback::~MediaKeySystemCallback()
{
    DRM_INFO_LOG("~MediaKeySystemCallback");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    systemImpl_ = nullptr;
}

void MediaKeySystemCallback::InitEventMap()
{
    DRM_INFO_LOG("MediaKeySystemCallback InitEventMap");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    eventMap_[static_cast<int32_t>(DRM_EVENT_PROVISION_REQUIRED)] = MediaKeySystemEvent::EVENT_STR_PROVISION_REQUIRED;
}

std::string MediaKeySystemCallback::GetEventName(DrmEventType event)
{
    DRM_INFO_LOG("MediaKeySystemCallback GetEventName");
    std::string eventName = "";
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t eventType = static_cast<int32_t>(event);
    if (eventMap_.find(eventType) == eventMap_.end()) {
        return eventName;
    }
    return eventMap_[eventType];
}

int32_t MediaKeySystemCallback::SendEvent(DrmEventType event, int32_t extra, const std::vector<uint8_t> &data)
{
    DRM_INFO_LOG("SendEvent enter");
    std::string eventName = GetEventName(event);
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    if (systemImpl_ != nullptr && eventName.length() != 0) {
        sptr<MediaKeySystemImplCallback> applicationCallback = systemImpl_->GetApplicationCallback();
        if (applicationCallback != nullptr) {
            applicationCallback->SendEvent(eventName, extra, data);
            return DRM_INNER_ERR_OK;
        }
    }
    DRM_DEBUG_LOG("SendEvent failed");
    return DRM_INNER_ERR_BASE;
}
} // namespace DrmStandard
} // namespace OHOS