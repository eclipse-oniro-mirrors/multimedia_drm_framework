/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
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
KeySessionImpl::KeySessionImpl(sptr<IKeySessionService> &keySession)
{
    DRM_DEBUG_LOG("KeySessionImpl::KeySessionImpl enter.");
    keySessionServiceProxy_ = keySession;
    mediaDecryptModuleImpl_ = nullptr;
    keySessionServiceCallback_ = nullptr;
}

KeySessionImpl::~KeySessionImpl()
{
    DRM_INFO_LOG("KeySessionImpl::~KeySessionImpl enter.");
    keySessionServiceProxy_ = nullptr;
    mediaDecryptModuleImpl_ = nullptr;
    keySessionServiceCallback_ = nullptr;
}

int32_t KeySessionImpl::Release()
{
    DRM_INFO_LOG("KeySessionImpl Release enter.");
    int errCode = DRM_UNKNOWN_ERROR;
    if (keySessionServiceProxy_) {
        errCode = keySessionServiceProxy_->Release();
        if (errCode != DRM_OK) {
            DRM_ERR_LOG("Failed to Release key session!, %{public}d", errCode);
        }
    } else {
        DRM_ERR_LOG("KeySessionServiceProxy_ == nullptr");
    }
    keySessionServiceProxy_ = nullptr;
    mediaDecryptModuleImpl_ = nullptr;
    keySessionServiceCallback_ = nullptr;
    DRM_INFO_LOG("KeySessionImpl Release exit.");
    return errCode;
}

int KeySessionImpl::GenerateLicenseRequest(IKeySessionService::DrmInfo &drmInfo,
    IKeySessionService::LicenseInfo &licenseInfo)
{
    DRM_INFO_LOG("KeySessionImpl::GenerateLicenseRequest enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;
    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("KeySessionImpl::GenerateLicenseRequest keySessionServiceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = keySessionServiceProxy_->GenerateLicenseRequest(drmInfo, licenseInfo);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("KeySessionImpl::GenerateLicenseRequest failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("KeySessionImpl::GenerateLicenseRequest exit");
    return DRM_OK;
}

int KeySessionImpl::ProcessLicenseResponse(std::vector<uint8_t> &keyId, std::vector<uint8_t> &licenseResponse)
{
    DRM_INFO_LOG("KeySessionImpl::ProcessLicenseResponse enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("KeySessionImpl::ProcessLicenseResponse keySessionServiceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = keySessionServiceProxy_->ProcessLicenseResponse(keyId, licenseResponse);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("KeySessionImpl::ProcessLicenseResponse failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("KeySessionImpl::ProcessLicenseResponse exit");
    return DRM_OK;
}

int KeySessionImpl::GenerateOfflineReleaseRequest(std::vector<uint8_t> &keyId, std::vector<uint8_t> &releaseRequest)
{
    DRM_INFO_LOG("KeySessionImpl::GenerateOfflineReleaseRequest enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("KeySessionImpl::GenerateOfflineReleaseRequest keySessionServiceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }

    retCode = keySessionServiceProxy_->GenerateOfflineReleaseRequest(keyId, releaseRequest);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("KeySessionImpl::GenerateOfflineReleaseRequest failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }

    DRM_INFO_LOG("KeySessionImpl::GenerateOfflineReleaseRequest exit");
    return DRM_OK;
}

int KeySessionImpl::ProcessOfflineReleaseResponse(std::vector<uint8_t> &keyId, std::vector<uint8_t> &releaseResponse)
{
    DRM_INFO_LOG("KeySessionImpl::ProcessOfflineReleaseResponse enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("KeySessionImpl::ProcessOfflineReleaseResponse keySessionServiceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = keySessionServiceProxy_->ProcessOfflineReleaseResponse(keyId, releaseResponse);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("KeySessionImpl::ProcessOfflineReleaseResponse failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("KeySessionImpl::ProcessOfflineReleaseResponse exit");
    return DRM_OK;
}

sptr<MediaDecryptModuleImpl> KeySessionImpl::GetDecryptModule()
{
    DRM_INFO_LOG("KeySessionImpl::GetDecryptModule enter.");
    if (mediaDecryptModuleImpl_ == nullptr) {
        sptr<IMediaDecryptModuleService> decryptModuleProxy = nullptr;
        sptr<MediaDecryptModuleImpl> localDecryptModuleImpl = nullptr;

        int retCode = DRM_OK;
        if (keySessionServiceProxy_ == nullptr) {
            DRM_ERR_LOG("MediaDecryptModuleImpl::keySessionServiceProxy_ == nullptr");
            return nullptr;
        }
        retCode = keySessionServiceProxy_->CreateMediaDecryptModule(decryptModuleProxy);
        if (retCode == DRM_OK) {
            if (decryptModuleProxy == nullptr) {
                DRM_ERR_LOG("Failed to Create MediaDecryptModuleImpl because decryptModuleProxy nullptr");
                return nullptr;
            }
            localDecryptModuleImpl = new(std::nothrow) MediaDecryptModuleImpl(decryptModuleProxy);
            if (localDecryptModuleImpl == nullptr) {
                DRM_ERR_LOG("Failed to alloc new MediaDecryptModuleImpl");
                return nullptr;
            } else {
                mediaDecryptModuleImpl_ = localDecryptModuleImpl;
            }
        } else {
            DRM_ERR_LOG("Failed to keySessionServiceProxy_->CreateMediaDecryptModule!, %{public}d", retCode);
            return nullptr;
        }
    }
    DRM_INFO_LOG("KeySessionImpl::GetDecryptModule exit.");
    return mediaDecryptModuleImpl_;
}

int32_t KeySessionImpl::CheckLicenseStatus(std::vector<IKeySessionService::KeyValue> &infoMap)
{
    DRM_INFO_LOG("KeySessionImpl::CheckLicenseStatus enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("KeySessionImpl::CheckLicenseStatus keySessionServiceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = keySessionServiceProxy_->CheckLicenseStatus(infoMap);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("KeySessionImpl::CheckLicenseStatus failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("KeySessionImpl::CheckLicenseStatus exit");
    return DRM_OK;
}

int32_t KeySessionImpl::RestoreOfflineKeys(std::vector<uint8_t> &keyId)
{
    DRM_ERR_LOG("KeySessionImpl::RestoreOfflineKeys enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("KeySessionImpl::RestoreOfflineKeys keySessionServiceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = keySessionServiceProxy_->RestoreOfflineKeys(keyId);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("KeySessionImpl::RestoreOfflineKeys failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("KeySessionImpl::RestoreOfflineKeys exit");
    return DRM_OK;
}

int32_t KeySessionImpl::RemoveOfflineKeys(std::vector<uint8_t> &keyId)
{
    DRM_INFO_LOG("KeySessionImpl::RemoveOfflineKeys enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;
    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("KeySessionImpl::RemoveOfflineKeys keySessionServiceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = keySessionServiceProxy_->RemoveOfflineKeys(keyId);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("KeySessionImpl::RemoveOfflineKeys failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("KeySessionImpl::RemoveOfflineKeys exit");
    return DRM_OK;
}

int32_t KeySessionImpl::GetOfflineKeyIds(std::vector<std::vector<uint8_t>> &keyIds)
{
    DRM_INFO_LOG("KeySessionImpl::GetOfflineKeyIds enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("KeySessionImpl::GetOfflineKeyIds keySessionServiceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = keySessionServiceProxy_->GetOfflineKeyIds(keyIds);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("KeySessionImpl::GetOfflineKeyIds failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("KeySessionImpl::GetOfflineKeyIds exit");
    return DRM_OK;
}

int32_t KeySessionImpl::RemoveLicenses()
{
    DRM_INFO_LOG("KeySessionImpl::RemoveLicenses enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("KeySessionImpl::RemoveLicenses keySessionServiceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = keySessionServiceProxy_->RemoveLicenses();
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("KeySessionImpl::RemoveLicenses failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("KeySessionImpl::RemoveLicenses exit");
    return DRM_OK;
}

int KeySessionImpl::GetOfflineKeyState(std::vector<uint8_t> &keyId, IKeySessionService::OfflineKeyState &state)
{
    DRM_INFO_LOG("KeySessionImpl::GetOfflineKeyState enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("KeySessionImpl::GetOfflineKeyState keySessionServiceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = keySessionServiceProxy_->GetOfflineKeyState(keyId, state);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("KeySessionImpl::GetOfflineKeyState failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("KeySessionImpl::GetOfflineKeyState exit.");
    return DRM_OK;
}

sptr<IKeySessionService> KeySessionImpl::GetKeySessionServiceProxy()
{
    DRM_INFO_LOG("KeySessionImpl::GetKeySessionServiceProxy enter.");
    if (keySessionServiceProxy_ != nullptr) {
        DRM_DEBUG_LOG("KeySessionImpl KeySessionServiceProxy is not nullptr");
        DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " is keySessionServiceProxy's native pointer in KeySessionImpl",
            FAKE_POINTER(keySessionServiceProxy_.GetRefPtr()));
        DRM_DEBUG_LOG("And it's count is, %{public}d", keySessionServiceProxy_->GetSptrRefCount());
    }
    DRM_INFO_LOG("KeySessionImpl::GetKeySessionServiceProxy enter.");
    return keySessionServiceProxy_;
}

sptr<KeySessionImplCallback> KeySessionImpl::GetKeySessionApplicationCallback()
{
    DRM_INFO_LOG("KeySessionImpl::GetKeySessionApplicationCallback enter.");
    return keySessionNapiCallback_;
}

int32_t KeySessionImpl::SetKeySessionServiceCallback(sptr<IKeySessionServiceCallback> &callback)
{
    DRM_INFO_LOG("KeySessionImpl::SetKeySessionServiceCallback");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;
    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("KeySessionImpl::SetKeySessionServiceCallback keySessionServiceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = keySessionServiceProxy_->SetKeySessionServiceCallback(callback);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("KeySessionImpl::SetKeySessionServiceCallback failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("KeySessionImpl::SetKeySessionServiceCallback exit.");
    return DRM_OK;
}

int32_t KeySessionImpl::SetKeySessionCallback(const sptr<KeySessionImplCallback> &callback)
{
    DRM_DEBUG_LOG("KeySessionImpl:0x%{public}06" PRIXPTR " SetKeySessionCallback in", FAKE_POINTER(this));
    CHECK_AND_RETURN_RET_LOG(callback != nullptr, INVALID_ARGUMENT, "callback is nullptr");
    keySessionNapiCallback_ = callback;

    int32_t errCode = DRM_ERROR;
    keySessionServiceCallback_ = new(std::nothrow) KeySessionStatusCallback(this);
    if (keySessionServiceCallback_ == nullptr) {
        DRM_ERR_LOG("KeySessionImpl:: KeySessionStatusCallback alloc failed");
        return errCode;
    }
    errCode = this->SetKeySessionServiceCallback(keySessionServiceCallback_);
    return errCode;
}

int32_t KeySessionStatusCallback::OnKeySessionKeyExpired(const KeyStatus status)
{
    DRM_INFO_LOG("KeySessionStatusCallback::OnKeySessionKeyExpired enter.");
    if (keySessionImpl_ != nullptr) {
        sptr<KeySessionImplCallback> callback = keySessionImpl_->GetKeySessionApplicationCallback();
        if (callback != nullptr) {
            callback->OnKeySessionKeyExpired("keyExpired", status);
            return DRM_SUCCESS;
        }
    }
    DRM_ERR_LOG("KeySessionStatusCallback:: OnKeySessionKeyExpired failed");
    return DRM_ERROR;
}

int32_t KeySessionStatusCallback::OnKeySessionReclaimed(const SessionStatus status)
{
    DRM_INFO_LOG("KeySessionStatusCallback::OnKeySessionReclaimed enter.");
    if (keySessionImpl_ != nullptr) {
        sptr<KeySessionImplCallback> callback = keySessionImpl_->GetKeySessionApplicationCallback();
        if (callback != nullptr) {
            callback->OnKeySessionReclaimed("keySessionReclaimed", status);
            return DRM_SUCCESS;
        }
    }
    DRM_ERR_LOG("KeySessionStatusCallback:: OnKeySessionReclaimed failed");
    return DRM_ERROR;
}
} // DrmStandard
} // OHOS