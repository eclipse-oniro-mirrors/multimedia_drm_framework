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

#include "key_session_impl.h"
#include "drm_log.h"
#include "drm_error_code.h"

namespace OHOS {
namespace DrmStandard {

MediaKeySessionImpl::MediaKeySessionImpl(sptr<IMediaKeySessionService> &keySession)
{
    DRM_DEBUG_LOG("MediaKeySessionImpl::MediaKeySessionImpl enter.");
    keySessionServiceProxy_ = keySession;
    mediaDecryptModuleImpl_ = nullptr;
    keySessionServiceCallback_ = nullptr;
}

MediaKeySessionImpl::~MediaKeySessionImpl()
{
    DRM_INFO_LOG("MediaKeySessionImpl::~MediaKeySessionImpl enter.");
    keySessionServiceProxy_ = nullptr;
    mediaDecryptModuleImpl_ = nullptr;
    keySessionServiceCallback_ = nullptr;
}

int32_t MediaKeySessionImpl::Release()
{
    DRM_INFO_LOG("MediaKeySessionImpl Release enter.");
    int32_t errCode = DRM_UNKNOWN_ERROR;
    if (keySessionServiceProxy_) {
        errCode = keySessionServiceProxy_->Release();
        if (errCode != DRM_OK) {
            DRM_ERR_LOG("Failed to Release key session!, %{public}d", errCode);
        }
    } else {
        DRM_ERR_LOG("MediaKeySessionServiceProxy_ == nullptr");
    }
    keySessionServiceProxy_ = nullptr;
    mediaDecryptModuleImpl_ = nullptr;
    keySessionServiceCallback_ = nullptr;
    DRM_INFO_LOG("MediaKeySessionImpl Release exit.");
    return errCode;
}

int32_t MediaKeySessionImpl::GenerateLicenseRequest(IMediaKeySessionService::LicenseRequestInfo &licenseRequestInfo,
    IMediaKeySessionService::LicenseRequest &licenseRequest)
{
    DRM_INFO_LOG("MediaKeySessionImpl::GenerateLicenseRequest enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;
    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySessionImpl::GenerateLicenseRequest keySessionServiceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = keySessionServiceProxy_->GenerateLicenseRequest(licenseRequestInfo, licenseRequest);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySessionImpl::GenerateLicenseRequest failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySessionImpl::GenerateLicenseRequest exit");
    return DRM_OK;
}

int32_t MediaKeySessionImpl::ProcessLicenseResponse(std::vector<uint8_t> &licenseId, std::vector<uint8_t> &licenseResponse)
{
    DRM_INFO_LOG("MediaKeySessionImpl::ProcessLicenseResponse enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySessionImpl::ProcessLicenseResponse keySessionServiceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = keySessionServiceProxy_->ProcessLicenseResponse(licenseId, licenseResponse);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySessionImpl::ProcessLicenseResponse failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySessionImpl::ProcessLicenseResponse exit");
    return DRM_OK;
}

int32_t MediaKeySessionImpl::GenerateOfflineReleaseRequest(std::vector<uint8_t> &licenseId,
    std::vector<uint8_t> &releaseRequest)
{
    DRM_INFO_LOG("MediaKeySessionImpl::GenerateOfflineReleaseRequest enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySessionImpl::GenerateOfflineReleaseRequest keySessionServiceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }

    retCode = keySessionServiceProxy_->GenerateOfflineReleaseRequest(licenseId, releaseRequest);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySessionImpl::GenerateOfflineReleaseRequest failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }

    DRM_INFO_LOG("MediaKeySessionImpl::GenerateOfflineReleaseRequest exit");
    return DRM_OK;
}

int32_t MediaKeySessionImpl::ProcessOfflineReleaseResponse(std::vector<uint8_t> &licenseId,
    std::vector<uint8_t> &releaseResponse)
{
    DRM_INFO_LOG("MediaKeySessionImpl::ProcessOfflineReleaseResponse enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySessionImpl::ProcessOfflineReleaseResponse keySessionServiceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = keySessionServiceProxy_->ProcessOfflineReleaseResponse(licenseId, releaseResponse);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySessionImpl::ProcessOfflineReleaseResponse failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySessionImpl::ProcessOfflineReleaseResponse exit");
    return DRM_OK;
}

int32_t MediaKeySessionImpl::GetSecurityLevel(IMediaKeySessionService::SecurityLevel *securityLevel)
{
    DRM_INFO_LOG("MediaKeySessionImpl::GetSecurityLevel enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySessionImpl::GetSecurityLevel serviceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = keySessionServiceProxy_->GetSecurityLevel((IMediaKeySessionService::SecurityLevel *)securityLevel);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySessionImpl::GetSecurityLevel failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySessionImpl::GetSecurityLevel exit.");
    return DRM_OK;
}

sptr<MediaDecryptModuleImpl> MediaKeySessionImpl::GetDecryptModule()
{
    DRM_INFO_LOG("MediaKeySessionImpl::GetDecryptModule enter.");
    if (mediaDecryptModuleImpl_ != nullptr) {
        DRM_INFO_LOG("MediaKeySessionImpl::GetDecryptModule exit.");
        return mediaDecryptModuleImpl_;
    }

    sptr<IMediaDecryptModuleService> decryptModuleProxy = nullptr;
    sptr<MediaDecryptModuleImpl> localDecryptModuleImpl = nullptr;

    int32_t retCode = DRM_OK;

    DRM_CHECK_AND_RETURN_RET_LOG((keySessionServiceProxy_ != nullptr), nullptr,
        "MediaDecryptModuleImpl::keySessionServiceProxy_ == nullptr");
    retCode = keySessionServiceProxy_->CreateMediaDecryptModule(decryptModuleProxy);
    DRM_CHECK_AND_RETURN_RET_LOG(retCode == DRM_OK, nullptr,
        "Failed to keySessionServiceProxy_->CreateMediaDecryptModule!, %{public}d", retCode);
    DRM_CHECK_AND_RETURN_RET_LOG(decryptModuleProxy != nullptr, nullptr,
        "Failed to Create MediaDecryptModuleImpl because decryptModuleProxy nullptr");

    localDecryptModuleImpl = new (std::nothrow) MediaDecryptModuleImpl(decryptModuleProxy);
    DRM_CHECK_AND_RETURN_RET_LOG(localDecryptModuleImpl != nullptr, nullptr,
        "Failed to alloc new MediaDecryptModuleImpl");
    mediaDecryptModuleImpl_ = localDecryptModuleImpl;

    DRM_INFO_LOG("MediaKeySessionImpl::GetDecryptModule exit.");
    return mediaDecryptModuleImpl_;
}

int32_t MediaKeySessionImpl::CheckLicenseStatus(std::map<std::string,
    MediaKeySessionKeyStatus>& licenseStatus)
{
    DRM_INFO_LOG("MediaKeySessionImpl::CheckLicenseStatus enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySessionImpl::CheckLicenseStatus keySessionServiceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = keySessionServiceProxy_->CheckLicenseStatus(licenseStatus);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySessionImpl::CheckLicenseStatus failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySessionImpl::CheckLicenseStatus exit");
    return DRM_OK;
}

int32_t MediaKeySessionImpl::RestoreOfflineLicense(std::vector<uint8_t> &licenseId)
{
    DRM_ERR_LOG("MediaKeySessionImpl::RestoreOfflineLicense enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySessionImpl::RestoreOfflineLicense keySessionServiceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = keySessionServiceProxy_->RestoreOfflineLicense(licenseId);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySessionImpl::RestoreOfflineLicense failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySessionImpl::RestoreOfflineLicense exit");
    return DRM_OK;
}

int32_t MediaKeySessionImpl::RemoveLicense()
{
    DRM_INFO_LOG("MediaKeySessionImpl::RemoveLicense enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySessionImpl::RemoveLicense keySessionServiceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = keySessionServiceProxy_->RemoveLicense();
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySessionImpl::RemoveLicense failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySessionImpl::RemoveLicense exit");
    return DRM_OK;
}

int32_t MediaKeySessionImpl::RequireSecureDecoderModule(std::string &mimeType, bool *status)
{
    DRM_INFO_LOG("MediaKeySessionImpl::RequireSecureDecoderModule enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySessionImpl::RequireSecureDecoderModule keySessionServiceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = keySessionServiceProxy_->RequireSecureDecoderModule(mimeType, status);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("status: %{public}d", *status);
        return retCode;
    }
    DRM_INFO_LOG("MediaKeySessionImpl::RequireSecureDecoderModule exit.");
    return DRM_OK;
}

sptr<IMediaKeySessionService> MediaKeySessionImpl::GetMediaKeySessionServiceProxy()
{
    DRM_INFO_LOG("MediaKeySessionImpl::GetMediaKeySessionServiceProxy enter.");
    if (keySessionServiceProxy_ != nullptr) {
        DRM_DEBUG_LOG("MediaKeySessionImpl MediaKeySessionServiceProxy is not nullptr");
    }
    DRM_INFO_LOG("MediaKeySessionImpl::GetMediaKeySessionServiceProxy enter.");
    return keySessionServiceProxy_;
}

sptr<MediaKeySessionImplCallback> MediaKeySessionImpl::GetApplicationCallback()
{
    DRM_INFO_LOG("MediaKeySessionImpl::GetApplicationCallback enter.");
    return keySessionNapiCallback_;
}

int32_t MediaKeySessionImpl::SetCallback(const sptr<MediaKeySessionImplCallback> &callback)
{
    DRM_DEBUG_LOG("MediaKeySessionImpl:0x%{public}06" PRIXPTR " SetCallback in", FAKE_POINTER(this));
    DRM_CHECK_AND_RETURN_RET_LOG(callback != nullptr, DRM_INVALID_ARG, "callback is nullptr");
    keySessionNapiCallback_ = callback;

    int32_t errCode = DRM_ERROR;
    keySessionServiceCallback_ = new (std::nothrow) MediaKeySessionServiceCallback(this);
    if (keySessionServiceCallback_ == nullptr) {
        DRM_ERR_LOG("MediaKeySessionImpl:: MediaKeySessionServiceCallback alloc failed");
        return errCode;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySessionImpl::SetCallback keySessionServiceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    errCode = keySessionServiceProxy_->SetCallback(keySessionServiceCallback_);
    if (errCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySessionImpl::SetCallback failed, retCode: %{public}d", errCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySessionImpl::SetCallback exit.");
    return errCode;
}

void MediaKeySessionServiceCallback::InitEventMap()
{
    DRM_INFO_LOG("KeySessionImpl::InitEventMap");
    eventMap_[static_cast<int32_t>(DRM_EVENT_KEY_NEEDED)] = "keyNeeded";
    eventMap_[static_cast<int32_t>(DRM_EVENT_KEY_EXPIRED)] = "keyExpired";
    eventMap_[static_cast<int32_t>(DRM_EVENT_EXPIRATION_UPDATED)] = "expirationUpdated";
    eventMap_[static_cast<int32_t>(DRM_EVENT_KEY_CHANGED)] = "keyChanged";
    eventMap_[static_cast<int32_t>(DRM_EVENT_VENDOR_DEFINED)] = "vendorDefined";
}

std::string MediaKeySessionServiceCallback::GetEventName(DrmEventType event)
{
    DRM_INFO_LOG("MediaKeySessionServiceCallback::GetEventName");
    std::string eventName;
    int32_t eventType = static_cast<int32_t>(event);
    if (eventMap_.find(eventType) == eventMap_.end()) {
        return eventName;
    }
    return eventMap_[eventType];
}

int32_t MediaKeySessionServiceCallback::SendEvent(DrmEventType event, uint32_t extra,
    const std::vector<uint8_t> data)
{
    DRM_INFO_LOG("MediaKeySessionServiceCallback SendEvent");
    std::string eventName = GetEventName(event);
    if (keySessionImpl_ != nullptr) {
        sptr<MediaKeySessionImplCallback> napiCallback = keySessionImpl_->GetApplicationCallback();
        if (napiCallback != nullptr) {
            napiCallback->SendEvent(eventName, extra, data);
            return DRM_OK;
        }
    }
    DRM_ERR_LOG("MediaKeySessionServiceCallback:: SendEvent failed");
    return DRM_ERROR;
}

int32_t MediaKeySessionServiceCallback::SendEventKeyChanged(std::map<std::vector<uint8_t>,
    MediaKeySessionKeyStatus> statusTable, bool hasNewGoodLicense)
{
    DRM_INFO_LOG("MediaKeySessionServiceCallback::SendEventKeyChanged enter.");
    if (keySessionImpl_ != nullptr) {
        sptr<MediaKeySessionImplCallback> callback = keySessionImpl_->GetApplicationCallback();
        if (callback != nullptr) {
            callback->SendEventKeyChanged(statusTable, hasNewGoodLicense);
            return DRM_OK;
        }
    }
    DRM_ERR_LOG("MediaKeySessionServiceCallback:: SendEventKeyChanged failed");
    return DRM_ERROR;
}
} // DrmStandard
} // OHOS