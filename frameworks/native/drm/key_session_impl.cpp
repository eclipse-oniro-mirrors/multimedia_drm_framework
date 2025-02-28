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
#include "drm_trace.h"

namespace OHOS {
namespace DrmStandard {

MediaKeySessionImpl::MediaKeySessionImpl(sptr<IMediaKeySessionService> &keySession)
    : keySessionServiceCallback_(nullptr), keySessionServiceProxy_(keySession)
{
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR "MediaKeySessionImpl Instances create",
        FAKE_POINTER(this));

    sptr<IRemoteObject> object = keySessionServiceProxy_->AsObject();
    pid_t pid = 0;
    deathRecipient_ = new(std::nothrow) DrmDeathRecipient(pid);
    DRM_CHECK_AND_RETURN_LOG(deathRecipient_ != nullptr, "failed to new DrmDeathRecipient.");
    deathRecipient_->SetNotifyCb([this] (pid_t pid) {
        this->MediaKeySessionServerDied(pid);
    });
    bool result = object->AddDeathRecipient(deathRecipient_);
    if (!result) {
        DRM_ERR_LOG("failed to add deathRecipient");
        return;
    }
}

MediaKeySessionImpl::~MediaKeySessionImpl()
{
    DRM_INFO_LOG("~MediaKeySessionImpl enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    keySessionServiceProxy_ = nullptr;
    keySessionServiceCallback_ = nullptr;
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR "MediaKeySessionImpl Instances release",
        FAKE_POINTER(this));
}

void MediaKeySessionImpl::MediaKeySessionServerDied(pid_t pid)
{
    DRM_ERR_LOG("MediaKeySession server has died, pid:%{public}d!", pid);
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    if (keySessionServiceProxy_ != nullptr && keySessionServiceProxy_->AsObject() != nullptr
        && deathRecipient_ != nullptr) {
        (void)keySessionServiceProxy_->AsObject()->RemoveDeathRecipient(deathRecipient_);
        keySessionServiceProxy_ = nullptr;
    }
    deathRecipient_ = nullptr;
}

int32_t MediaKeySessionImpl::Release()
{
    DRM_INFO_LOG("MediaKeySessionImpl Release enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t ret = DRM_INNER_ERR_UNKNOWN;
    if (keySessionServiceProxy_ != nullptr) {
        sptr<IRemoteObject> object = keySessionServiceProxy_->AsObject();
        if (object != nullptr && deathRecipient_ != nullptr) {
            object->RemoveDeathRecipient(deathRecipient_);
            deathRecipient_ = nullptr;
        }
        ret = keySessionServiceProxy_->Release();
        if (ret != DRM_INNER_ERR_OK) {
            DRM_ERR_LOG("Failed to Release key session!, %{public}d", ret);
        }
    } else {
        DRM_ERR_LOG("MediaKeySessionServiceProxy_ == nullptr");
    }
    keySessionServiceProxy_ = nullptr;
    keySessionServiceCallback_ = nullptr;
    return ret;
}

int32_t MediaKeySessionImpl::GenerateMediaKeyRequest(IMediaKeySessionService::MediaKeyRequestInfo &licenseRequestInfo,
    IMediaKeySessionService::MediaKeyRequest &licenseRequest)
{
    DrmTrace trace("GenerateMediaKeyRequest");
    DRM_INFO_LOG("GenerateMediaKeyRequest enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t retCode = DRM_INNER_ERR_OK;
    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("GenerateMediaKeyRequest keySessionServiceProxy_ is null");
        return DRM_INNER_ERR_INVALID_KEY_SESSION;
    }
    retCode = keySessionServiceProxy_->GenerateMediaKeyRequest(licenseRequestInfo, licenseRequest);
    if (retCode != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GenerateMediaKeyRequest failed, retCode: %{public}d", retCode);
        return DRM_INNER_ERR_BASE;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySessionImpl::ProcessMediaKeyResponse(std::vector<uint8_t> &licenseId,
    std::vector<uint8_t> &licenseResponse)
{
    DrmTrace trace("MediaKeySessionImpl::ProcessMediaKeyResponse");
    DRM_INFO_LOG("ProcessMediaKeyResponse enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t retCode = DRM_INNER_ERR_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("ProcessMediaKeyResponse keySessionServiceProxy_ is null");
        return DRM_INNER_ERR_INVALID_KEY_SESSION;
    }
    retCode = keySessionServiceProxy_->ProcessMediaKeyResponse(licenseId, licenseResponse);
    if (retCode != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("ProcessMediaKeyResponse failed, retCode: %{public}d", retCode);
        return DRM_INNER_ERR_BASE;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySessionImpl::GenerateOfflineReleaseRequest(std::vector<uint8_t> &licenseId,
    std::vector<uint8_t> &releaseRequest)
{
    DRM_INFO_LOG("GenerateOfflineReleaseRequest enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t retCode = DRM_INNER_ERR_OK;
    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("GenerateOfflineReleaseRequest keySessionServiceProxy_ is null");
        return DRM_INNER_ERR_INVALID_KEY_SESSION;
    }

    retCode = keySessionServiceProxy_->GenerateOfflineReleaseRequest(licenseId, releaseRequest);
    if (retCode != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GenerateOfflineReleaseRequest failed, retCode: %{public}d", retCode);
        return DRM_INNER_ERR_BASE;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySessionImpl::ProcessOfflineReleaseResponse(std::vector<uint8_t> &licenseId,
    std::vector<uint8_t> &releaseResponse)
{
    DRM_INFO_LOG("ProcessOfflineReleaseResponse enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t retCode = DRM_INNER_ERR_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("ProcessOfflineReleaseResponse keySessionServiceProxy_ is null");
        return DRM_INNER_ERR_INVALID_KEY_SESSION;
    }
    retCode = keySessionServiceProxy_->ProcessOfflineReleaseResponse(licenseId, releaseResponse);
    if (retCode != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("ProcessOfflineReleaseResponse failed, retCode: %{public}d", retCode);
        return DRM_INNER_ERR_BASE;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySessionImpl::GetContentProtectionLevel(IMediaKeySessionService::ContentProtectionLevel *securityLevel)
{
    DRM_INFO_LOG("GetContentProtectionLevel enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t retCode = DRM_INNER_ERR_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("GetContentProtectionLevel serviceProxy_ is null");
        return DRM_INNER_ERR_INVALID_KEY_SESSION;
    }
    retCode = keySessionServiceProxy_->GetContentProtectionLevel(
        (IMediaKeySessionService::ContentProtectionLevel *)securityLevel);
    if (retCode != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GetContentProtectionLevel failed, retCode: %{public}d", retCode);
        return DRM_INNER_ERR_BASE;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySessionImpl::CheckMediaKeyStatus(std::map<std::string, std::string> &licenseStatus)
{
    DRM_INFO_LOG("CheckMediaKeyStatus enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t retCode = DRM_INNER_ERR_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("CheckMediaKeyStatus keySessionServiceProxy_ is null");
        return DRM_INNER_ERR_INVALID_KEY_SESSION;
    }
    retCode = keySessionServiceProxy_->CheckMediaKeyStatus(licenseStatus);
    if (retCode != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("CheckMediaKeyStatus failed, retCode: %{public}d", retCode);
        return DRM_INNER_ERR_BASE;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySessionImpl::RestoreOfflineMediaKeys(std::vector<uint8_t> &licenseId)
{
    DRM_INFO_LOG("RestoreOfflineMediaKeys enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t retCode = DRM_INNER_ERR_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("RestoreOfflineMediaKeys keySessionServiceProxy_ is null");
        return DRM_INNER_ERR_INVALID_KEY_SESSION;
    }
    retCode = keySessionServiceProxy_->RestoreOfflineMediaKeys(licenseId);
    if (retCode != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("RestoreOfflineMediaKeys failed, retCode: %{public}d", retCode);
        return DRM_INNER_ERR_BASE;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySessionImpl::ClearMediaKeys()
{
    DRM_INFO_LOG("ClearMediaKeys enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t retCode = DRM_INNER_ERR_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("ClearMediaKeys keySessionServiceProxy_ is null");
        return DRM_INNER_ERR_INVALID_KEY_SESSION;
    }
    retCode = keySessionServiceProxy_->ClearMediaKeys();
    if (retCode != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("ClearMediaKeys failed, retCode: %{public}d", retCode);
        return DRM_INNER_ERR_BASE;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySessionImpl::RequireSecureDecoderModule(std::string &mimeType, bool *status)
{
    DRM_INFO_LOG("RequireSecureDecoderModule enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t retCode = DRM_INNER_ERR_OK;

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("RequireSecureDecoderModule keySessionServiceProxy_ is null");
        return DRM_INNER_ERR_INVALID_KEY_SESSION;
    }
    retCode = keySessionServiceProxy_->RequireSecureDecoderModule(mimeType, status);
    if (retCode != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("status: %{public}d", *status);
        return retCode;
    }
    return DRM_INNER_ERR_OK;
}

sptr<IMediaKeySessionService> MediaKeySessionImpl::GetMediaKeySessionServiceProxy()
{
    DRM_INFO_LOG("GetMediaKeySessionServiceProxy enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    if (keySessionServiceProxy_ != nullptr) {
        DRM_DEBUG_LOG("MediaKeySessionImpl MediaKeySessionServiceProxy is not nullptr");
    }
    return keySessionServiceProxy_;
}

sptr<MediaKeySessionImplCallback> MediaKeySessionImpl::GetApplicationCallback()
{
    DRM_INFO_LOG("GetApplicationCallback enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return keySessionApplicationCallback_;
}

int32_t MediaKeySessionImpl::SetCallback(const sptr<MediaKeySessionImplCallback> &callback)
{
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " SetCallback in", FAKE_POINTER(this));
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(callback != nullptr, DRM_INNER_ERR_INVALID_VAL, "callback is nullptr");
    keySessionApplicationCallback_ = callback;

    int32_t ret = DRM_INNER_ERR_BASE;
    keySessionServiceCallback_ = new (std::nothrow) MediaKeySessionServiceCallback(this);
    if (keySessionServiceCallback_ == nullptr) {
        DRM_ERR_LOG("MediaKeySessionServiceCallback alloc failed.");
        return ret;
    }

    if (keySessionServiceProxy_ == nullptr) {
        DRM_ERR_LOG("SetCallback keySessionServiceProxy_ is null");
        return DRM_INNER_ERR_INVALID_KEY_SESSION;
    }
    ret = keySessionServiceProxy_->SetCallback(keySessionServiceCallback_);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("SetCallback failed, retCode: %{public}d", ret);
        return DRM_INNER_ERR_BASE;
    }
    return ret;
}

void MediaKeySessionServiceCallback::InitEventMap()
{
    DRM_INFO_LOG("InitEventMap enter");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    eventMap_[static_cast<int32_t>(DRM_EVENT_KEY_NEEDED)] = MediaKeySessionEvent::EVENT_STR_KEY_NEEDED;
    eventMap_[static_cast<int32_t>(DRM_EVENT_KEY_EXPIRED)] = MediaKeySessionEvent::EVENT_STR_KEY_EXPIRED;
    eventMap_[static_cast<int32_t>(DRM_EVENT_EXPIRATION_UPDATED)] = MediaKeySessionEvent::EVENT_STR_EXPIRATION_UPDATED;
    eventMap_[static_cast<int32_t>(DRM_EVENT_KEY_CHANGED)] = MediaKeySessionEvent::EVENT_STR_KEY_CHANGED;
    eventMap_[static_cast<int32_t>(DRM_EVENT_VENDOR_DEFINED)] = MediaKeySessionEvent::EVENT_STR_VENDOR_DEFINED;
}

std::string MediaKeySessionServiceCallback::GetEventName(DrmEventType event)
{
    DRM_INFO_LOG("GetEventName enter");
    std::string eventName = "";
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    int32_t eventType = static_cast<int32_t>(event);
    if (eventMap_.find(eventType) == eventMap_.end()) {
        return eventName;
    }
    return eventMap_[eventType];
}

int32_t MediaKeySessionServiceCallback::SendEvent(DrmEventType event, int32_t extra, const std::vector<uint8_t> &data)
{
    DRM_INFO_LOG("SendEvent enter");
    std::string eventName = GetEventName(event);
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    if (keySessionImpl_ != nullptr && eventName.length() != 0) {
        sptr<MediaKeySessionImplCallback> applicationCallback = keySessionImpl_->GetApplicationCallback();
        if (applicationCallback != nullptr) {
            applicationCallback->SendEvent(eventName, extra, data);
            return DRM_INNER_ERR_OK;
        }
    }
    DRM_DEBUG_LOG("SendEvent failed.");
    return DRM_INNER_ERR_BASE;
}

int32_t MediaKeySessionServiceCallback::SendEventKeyChanged(
    std::map<std::vector<uint8_t>, MediaKeySessionKeyStatus> statusTable, bool hasNewGoodLicense)
{
    DRM_INFO_LOG("SendEventKeyChanged enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    if (keySessionImpl_ != nullptr) {
        sptr<MediaKeySessionImplCallback> callback = keySessionImpl_->GetApplicationCallback();
        if (callback != nullptr) {
            callback->SendEventKeyChanged(statusTable, hasNewGoodLicense);
            return DRM_INNER_ERR_OK;
        }
    }
    DRM_ERR_LOG("SendEventKeyChanged failed.");
    return DRM_INNER_ERR_BASE;
}
} // DrmStandard
} // OHOS