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
    int errCode = DRM_UNKNOWN_ERROR;
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

int MediaKeySessionImpl::GenerateLicenseRequest(IMediaKeySessionService::LicenseRequestInfo &licenseRequestInfo,
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

int MediaKeySessionImpl::ProcessLicenseResponse(std::vector<uint8_t> &licenseId, std::vector<uint8_t> &licenseResponse)
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

int MediaKeySessionImpl::GenerateOfflineReleaseRequest(std::vector<uint8_t> &licenseId,
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

int MediaKeySessionImpl::ProcessOfflineReleaseResponse(std::vector<uint8_t> &licenseId,
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

int MediaKeySessionImpl::GetSecurityLevel(IMediaKeySessionService::SecurityLevel *securityLevel)
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

    int retCode = DRM_OK;

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
    IMediaKeySessionService::MediaKeySessionKeyStatus>& licenseStatus)
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

sptr<MediaKeySessionImplCallback> MediaKeySessionImpl::GetMediaKeySessionApplicationCallback()
{
    DRM_INFO_LOG("MediaKeySessionImpl::GetMediaKeySessionApplicationCallback enter.");
    return keySessionNapiCallback_;
}

int32_t MediaKeySessionImpl::SetMediaKeySessionServiceCallback(sptr<IMediaKeySessionServiceCallback> &callback)
{
    DRM_INFO_LOG("MediaKeySessionImpl::SetMediaKeySessionServiceCallback");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;
    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySessionImpl::SetMediaKeySessionServiceCallback keySessionServiceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = keySessionServiceProxy_->SetMediaKeySessionServiceCallback(callback);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySessionImpl::SetMediaKeySessionServiceCallback failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySessionImpl::SetMediaKeySessionServiceCallback exit.");
    return DRM_OK;
}

int32_t MediaKeySessionImpl::SetMediaKeySessionCallback(const sptr<MediaKeySessionImplCallback> &callback)
{
    DRM_DEBUG_LOG("MediaKeySessionImpl:0x%{public}06" PRIXPTR " SetMediaKeySessionCallback in", FAKE_POINTER(this));
    DRM_CHECK_AND_RETURN_RET_LOG(callback != nullptr, DRM_INVALID_ARG, "callback is nullptr");
    keySessionNapiCallback_ = callback;

    int32_t errCode = DRM_ERROR;
    keySessionServiceCallback_ = new (std::nothrow) MediaKeySessionStatusCallback(this);
    if (keySessionServiceCallback_ == nullptr) {
        DRM_ERR_LOG("MediaKeySessionImpl:: MediaKeySessionStatusCallback alloc failed");
        return errCode;
    }
    errCode = this->SetMediaKeySessionServiceCallback(keySessionServiceCallback_);
    return errCode;
}

int32_t MediaKeySessionStatusCallback::OnMediaKeySessionKeyExpired(const KeyStatus status)
{
    DRM_INFO_LOG("MediaKeySessionStatusCallback::OnMediaKeySessionKeyExpired enter.");
    if (keySessionImpl_ != nullptr) {
        sptr<MediaKeySessionImplCallback> callback = keySessionImpl_->GetMediaKeySessionApplicationCallback();
        if (callback != nullptr) {
            callback->OnMediaKeySessionKeyExpired(DrmEvent::DRM_EVENT_KEY_EXPIRED, status);
            return DRM_OK;
        }
    }
    DRM_ERR_LOG("MediaKeySessionStatusCallback:: OnMediaKeySessionKeyExpired failed");
    return DRM_ERROR;
}

int32_t MediaKeySessionStatusCallback::OnMediaKeySessionReclaimed(const SessionStatus status)
{
    DRM_INFO_LOG("MediaKeySessionStatusCallback::OnMediaKeySessionReclaimed enter.");
    if (keySessionImpl_ != nullptr) {
        sptr<MediaKeySessionImplCallback> callback = keySessionImpl_->GetMediaKeySessionApplicationCallback();
        if (callback != nullptr) {
            callback->OnMediaKeySessionReclaimed(DrmEvent::DRM_EVENT_KEY_SESSION_RECLAIMED, status);
            return DRM_OK;
        }
    }
    DRM_ERR_LOG("MediaKeySessionStatusCallback:: OnMediaKeySessionReclaimed failed");
    return DRM_ERROR;
}
} // DrmStandard
} // OHOS