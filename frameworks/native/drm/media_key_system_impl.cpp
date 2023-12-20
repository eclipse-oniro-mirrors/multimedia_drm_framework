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

namespace OHOS {
namespace DrmStandard {
MediaKeySystemImpl::MediaKeySystemImpl(sptr<IMediaKeySystemService> &mediaKeysystem)
{
    DRM_DEBUG_LOG("MediaKeySystemImpl:0x%{public}06" PRIXPTR "MediaKeySystemImpl Instances create", FAKE_POINTER(this));
    serviceProxy_ = mediaKeysystem;
}

MediaKeySystemImpl::~MediaKeySystemImpl()
{
    serviceProxy_ = nullptr;
}

int32_t MediaKeySystemImpl::Release()
{
    DRM_INFO_LOG("MediaKeySystemImpl::Release enter.");
    int32_t errCode = DRM_UNKNOWN_ERROR;
    if (serviceProxy_ != nullptr) {
        errCode = serviceProxy_->Release();
        if (errCode != DRM_OK) {
            DRM_ERR_LOG("Failed to Release keySystem!, %{public}d", errCode);
            return errCode;
        }
        serviceProxy_ = nullptr;
    } else {
        DRM_ERR_LOG("serviceProxy_ is nullptr");
        return errCode;
    }
    DRM_INFO_LOG("MediaKeySystemImpl::Release exit.");
    return DRM_OK;
}

int32_t MediaKeySystemImpl::GenerateKeySystemRequest(std::vector<uint8_t> &request, std::string &defaultUrl)
{
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl::GenerateKeySystemRequest serviceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = serviceProxy_->GenerateKeySystemRequest(request, defaultUrl);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemImpl::GenerateKeySystemRequest failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySystemImpl::GenerateKeySystemRequest exit.");
    return DRM_OK;
}

int32_t MediaKeySystemImpl::ProcessKeySystemResponse(const std::vector<uint8_t> &response)
{
    DRM_INFO_LOG("MediaKeySystemImpl::ProcessKeySystemResponse enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl::ProcessKeySystemResponse serviceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = serviceProxy_->ProcessKeySystemResponse(response);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemImpl::ProcessKeySystemResponse failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySystemImpl::ProcessKeySystemResponse exit.");
    return DRM_OK;
}

int32_t MediaKeySystemImpl::SetConfigurationString(std::string &configName, std::string &value)
{
    DRM_INFO_LOG("MediaKeySystemImpl::SetConfiguration enter, configName:%{public}s, value:%{public}s.",
        configName.c_str(), value.c_str());
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl::SetConfiguration serviceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = serviceProxy_->SetConfigurationString(configName, value);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemImpl::SetConfiguration failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySystemImpl::SetConfiguration exit.");
    return DRM_OK;
}

int32_t MediaKeySystemImpl::GetConfigurationString(std::string &configName, std::string &value)
{
    DRM_INFO_LOG("MediaKeySystemImpl::GetConfiguration enter, configName:%{public}s.", configName.c_str());
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl::GetConfiguration serviceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = serviceProxy_->GetConfigurationString(configName, value);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemImpl::GetConfiguration failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }

    DRM_INFO_LOG("MediaKeySystemImpl::GetConfiguration exit, value:%{public}s.", value.c_str());
    return DRM_OK;
}

int32_t MediaKeySystemImpl::SetConfigurationByteArray(std::string &configName, std::vector<uint8_t> &value)
{
    DRM_INFO_LOG("MediaKeySystemImpl::SetConfiguration enter, configName:%{public}s.", configName.c_str());
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl::SetConfiguration serviceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = serviceProxy_->SetConfigurationByteArray(configName, value);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemImpl::SetConfiguration failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySystemImpl::SetConfiguration exit.");
    return DRM_OK;
}

int32_t MediaKeySystemImpl::GetConfigurationByteArray(std::string &configName, std::vector<uint8_t> &value)
{
    DRM_INFO_LOG("MediaKeySystemImpl::GetConfiguration enter, configName:%{public}s.", configName.c_str());
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl::GetConfiguration serviceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = serviceProxy_->GetConfigurationByteArray(configName, value);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemImpl::GetConfiguration failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }

    return DRM_OK;
}

int32_t MediaKeySystemImpl::CreateMediaKeySession(IMediaKeySessionService::SecurityLevel securityLevel,
    sptr<MediaKeySessionImpl> *keySessionImpl)
{
    DRM_INFO_LOG("MediaKeySystemImpl::CreateMediaKeySession enter.");
    sptr<IMediaKeySessionService> keySessionProxy = nullptr;
    sptr<MediaKeySessionImpl> localMediaKeySessionImpl = nullptr;
    int32_t retCode = DRM_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl:: serviceProxy_ == nullptr");
        return DRM_SERVICE_ERROR;
    }

    retCode = serviceProxy_->CreateMediaKeySession(securityLevel, keySessionProxy);
    if (retCode == DRM_OK) {
        if (keySessionProxy != nullptr) {
            localMediaKeySessionImpl = new (std::nothrow) MediaKeySessionImpl(keySessionProxy);
            if (localMediaKeySessionImpl == nullptr) {
                DRM_ERR_LOG("Failed to new MediaKeySessionImpl");
                return DRM_ALLOC_ERROR;
            }
        } else {
            DRM_ERR_LOG("Failed to CreateMediaKeySessionImpl with session is null");
            return DRM_UNKNOWN_ERROR;
        }
    } else {
        DRM_ERR_LOG("Failed to get session object from mediakeysystem service!, %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    *keySessionImpl = localMediaKeySessionImpl;
    DRM_INFO_LOG("MediaKeySystemImpl::CreateMediaKeySession exit.");
    return DRM_OK;
}

int32_t MediaKeySystemImpl::GetMetrics(std::vector<IMediaKeySystemService::MetircKeyValue> &metrics)
{
    DRM_INFO_LOG("MediaKeySystemImpl::GetMetrics enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl::GetMetrics serviceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = serviceProxy_->GetMetrics(metrics);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemImpl::GetMetrics failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySystemImpl::GetMetrics exit.");
    return DRM_OK;
}

int32_t MediaKeySystemImpl::GetMaxSecurityLevel(IMediaKeySessionService::SecurityLevel *securityLevel)
{
    DRM_INFO_LOG("MediaKeySystemImpl::GetMaxSecurityLevel enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl::GetMaxSecurityLevel serviceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = serviceProxy_->GetMaxSecurityLevel((IMediaKeySessionService::SecurityLevel *)securityLevel);
    DRM_ERR_LOG("MediaKeySystemImpl::GetMaxSecurityLevel 277");
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemImpl::GetMaxSecurityLevel failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySystemImpl::GetMaxSecurityLevel exit.");
    return DRM_OK;
}

int32_t MediaKeySystemImpl::GetOfflineLicenseIds(std::vector<std::vector<uint8_t>> &licenseIds)
{
    DRM_INFO_LOG("MediaKeySystemImpl::GetOfflineLicenseIds enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl::GetOfflineLicenseIds serviceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = serviceProxy_->GetOfflineLicenseIds(licenseIds);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemImpl::GetOfflineLicenseIds failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySystemImpl::GetOfflineLicenseIds exit");
    return DRM_OK;
}

int32_t MediaKeySystemImpl::GetOfflineLicenseStatus(std::vector<uint8_t> &licenseId,
    IMediaKeySessionService::OfflineLicenseStatus &status)
{
    DRM_INFO_LOG("MediaKeySystemImpl::GetOfflineLicenseStatus enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl::GetOfflineLicenseStatus serviceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = serviceProxy_->GetOfflineLicenseStatus(licenseId, status);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemImpl::GetOfflineLicenseStatus failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySystemImpl::GetOfflineLicenseStatus exit.");
    return DRM_OK;
}

int32_t MediaKeySystemImpl::RemoveOfflineLicense(std::vector<uint8_t> &licenseId)
{
    DRM_INFO_LOG("MediaKeySystemImpl::RemoveOfflineLicense enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;
    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl::RemoveOfflineLicense serviceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = serviceProxy_->RemoveOfflineLicense(licenseId);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemImpl::RemoveOfflineLicense failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySystemImpl::RemoveOfflineLicense exit");
    return DRM_OK;
}

int32_t MediaKeySystemImpl::GetCertificateStatus(IMediaKeySystemService::CertificateStatus *certStatus)
{
    DRM_INFO_LOG("MediaKeySystemImpl::GetCertificateStatus enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl::GetCertificateStatus serviceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = serviceProxy_->GetCertificateStatus((IMediaKeySystemService::CertificateStatus *)certStatus);
    DRM_ERR_LOG("MediaKeySystemImpl::GetCertificateStatus 277");
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemImpl::GetCertificateStatus failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySystemImpl::GetCertificateStatus exit.");
    return DRM_OK;
}

int32_t MediaKeySystemImpl::SetCallback(const sptr<MediaKeySystemImplCallback> &callback)
{
    DRM_DEBUG_LOG("MediaKeySystemImpl:0x%{public}06" PRIXPTR " SetCallback in", FAKE_POINTER(this));
    DRM_CHECK_AND_RETURN_RET_LOG(callback != nullptr, DRM_INVALID_ARG, "callback is nullptr");
    mediaKeySystemApplicationCallback_ = callback;

    int32_t retCode = DRM_ERROR;
    serviceCallback_ = new (std::nothrow) MediaKeySystemCallback(this);
    if (serviceCallback_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl:: MediaKeySystemCallback alloc failed");
        return retCode;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl::SetCallback serviceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = serviceProxy_->SetCallback(serviceCallback_);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemImpl::SetCallback failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySystemImpl::SetCallback exit.");
    return retCode;
}

sptr<MediaKeySystemImplCallback> MediaKeySystemImpl::GetApplicationCallback()
{
    DRM_INFO_LOG("MediaKeySystemImpl GetApplicationCallback");
    return mediaKeySystemApplicationCallback_;
}

MediaKeySystemCallback::~MediaKeySystemCallback()
{
    DRM_INFO_LOG("MediaKeySystemCallback ~MediaKeySystemCallback");
    systemImpl_ = nullptr;
}

void MediaKeySystemCallback::InitEventMap()
{
    DRM_INFO_LOG("MediaKeySystemCallback InitEventMap");
    eventMap_[static_cast<int32_t>(DRM_EVENT_PROVISION_REQUIRED)] = MediaKeySystemEvent::EVENT_STR_PROVISION_REQUIRED;
    eventMap_[static_cast<int32_t>(DRM_EVENT_KEYSESSION_LOST)] = MediaKeySystemEvent::EVENT_STR_SESSION_LOST;
}

std::string MediaKeySystemCallback::GetEventName(DrmEventType event)
{
    DRM_INFO_LOG("MediaKeySystemCallback GetEventName");
    std::string eventName;
    int32_t eventType = static_cast<int32_t>(event);
    if (eventMap_.find(eventType) == eventMap_.end()) {
        return eventName;
    }
    return eventMap_[eventType];
}

int32_t MediaKeySystemCallback::SendEvent(DrmEventType event, uint32_t extra, const std::vector<uint8_t> data)
{
    DRM_INFO_LOG("MediaKeySystemCallback SendEvent");
    std::string eventName = GetEventName(event);
    if (systemImpl_ != nullptr) {
        sptr<MediaKeySystemImplCallback> applicationCallback = systemImpl_->GetApplicationCallback();
        if (applicationCallback != nullptr) {
            applicationCallback->SendEvent(eventName, extra, data);
            return DRM_OK;
        }
    }
    DRM_ERR_LOG("MediaKeySystemCallback:: SendEvent failed");
    return DRM_ERROR;
}
} // namespace DrmStandard
} // namespace OHOS