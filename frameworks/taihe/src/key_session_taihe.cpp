/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#include "key_session_taihe.h"
#include "drm_trace.h"
#include "drm_api_operation.h"

using namespace ANI::DrmStandard;

namespace ANI {
namespace DrmStandard {
thread_local OHOS::sptr<OHOS::DrmStandard::MediaKeySessionImpl> MediaKeySessionImpl::sMediaKeySessionImpl_ = nullptr;

MediaKeySessionImpl::MediaKeySessionImpl()
{
    if (sMediaKeySessionImpl_ == nullptr) {
        DRM_ERR_LOG("sMediaKeySessionImpl_ is null");
        DrmTaiheUtils::ThrowExceptionError("failed to CreateMediaKeySession");
        return;
    }
    keySessionImpl_ = sMediaKeySessionImpl_;
    keySessionCallbackTaihe_ = new (std::nothrow) MediaKeySessionCallbackTaihe();
    keySessionImpl_->SetCallback(keySessionCallbackTaihe_);
    SetMediaKeySessionNative(reinterpret_cast<int64_t>(sMediaKeySessionImpl_.GetRefPtr()));
}

MediaKeySessionImpl::~MediaKeySessionImpl()
{
    OHOS::DrmStandard::DrmTrace trace("MediaKeySessionImplDestructor");
    DRM_INFO_LOG("MediaKeySessionImplDestructor enter.");
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " Instances destroy", FAKE_POINTER(this));
    if (keySessionImpl_) {
        keySessionImpl_ = nullptr;
    }
    if (keySessionCallbackTaihe_) {
        keySessionCallbackTaihe_ = nullptr;
    }
}

int64_t MediaKeySessionImpl::GetMediaKeySessionNative()
{
    return mediaKeySessionNative_;
}

void MediaKeySessionImpl::SetMediaKeySessionNative(int64_t mediaKeySessionNative)
{
    mediaKeySessionNative_ = mediaKeySessionNative;
}

MediaKeyRequest MediaKeySessionImpl::getEmptyMediaKeyRequest()
{
    std::vector<uint8_t> emptyData;
    MediaKeyRequest emptyReq{
        MediaKeyRequestType(static_cast<MediaKeyRequestType::key_t>(0)),
        array<uint8_t>(emptyData),
        ""
    };
    return emptyReq;
}

bool MediaKeySessionImpl::CheckMediaKeySessionStatus(MediaKeySessionImpl *keySessionTaihe)
{
    if (keySessionTaihe == nullptr || keySessionTaihe->keySessionImpl_ == nullptr) {
        DRM_ERR_LOG("MediaKeySession state is error.");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL, "keySessionImpl_ is null");
        return false;
    }
    return true;
}

MediaKeyRequest MediaKeySessionImpl::GenerateMediaKeyRequestSync(string_view mimeType,
    array_view<uint8_t> initData, int32_t mediaKeyType,
    optional_view<array<OptionsData>> options)
{
    DRM_INFO_LOG("GenerateMediaKeyRequest enter");
    OHOS::DrmStandard::DrmTrace trace("MediaKeySessionImpl::GenerateMediaKeyRequestSync");

    OHOS::DrmStandard::MediaKeyRequestInfo mediaKeyRequestInfo;
    OHOS::DrmStandard::MediaKeyRequest mediaKeyRequest;
    mediaKeyRequestInfo.mimeType = std::string(mimeType);
    mediaKeyRequestInfo.initData = std::vector<uint8_t> (initData.begin(), initData.end());
    mediaKeyRequestInfo.mediaKeyType = (OHOS::DrmStandard::MediaKeyType)mediaKeyType;
    mediaKeyRequestInfo.optionalData.clear();
    if (options.has_value()) {
        const auto& arr = options.value();
        for (const auto& opt : arr) {
            mediaKeyRequestInfo.optionalData.insert(std::make_pair(std::string(opt.name), std::string(opt.value)));
        }
    }
    if (!CheckMediaKeySessionStatus(this)) {
        return getEmptyMediaKeyRequest();
    }
    int intValue = keySessionImpl_->GenerateMediaKeyRequest(mediaKeyRequestInfo, mediaKeyRequest);
    if (intValue != OHOS::DrmStandard::DRM_TAIHE_ERR_OK) {
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
            "service error.");
        return getEmptyMediaKeyRequest();
    }

    auto reqType = MediaKeyRequestType(
        static_cast<MediaKeyRequestType::key_t>(mediaKeyRequest.requestType)
    );
    std::vector<uint8_t> reqDataVec(mediaKeyRequest.mData.begin(), mediaKeyRequest.mData.end());
    array<uint8_t> reqData(reqDataVec);
    std::string defaultUrl = mediaKeyRequest.mDefaultURL;

    return MediaKeyRequest{
        reqType,
        reqData,
        defaultUrl
    };
}

array<uint8_t> MediaKeySessionImpl::ProcessMediaKeyResponseSync(array_view<uint8_t> response)
{
    DRM_INFO_LOG("ProcessMediaKeyResponse enter.");
    int64_t beginTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    OHOS::DrmStandard::DrmTrace trace("MediaKeySessionImpl::ProcessMediaKeyResponseSync");

    std::vector<uint8_t> licenseId;
    std::vector<uint8_t> resPonse(response.begin(), response.end());
    if (!CheckMediaKeySessionStatus(this)) {
        std::vector<uint8_t> emptyData;
        array<uint8_t> emptyArray(emptyData);
        return emptyArray;
    }
    int32_t intValue = keySessionImpl_->ProcessMediaKeyResponse(licenseId, resPonse);
    OHOS::DrmStandard::ConfigParser::WriteEndEvent(0, 0, std::string("ProcessMediaKeyResponse"), beginTime);
    if (intValue != OHOS::DrmStandard::DRM_TAIHE_ERR_OK) {
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
            "service error.");
        std::vector<uint8_t> emptyData;
        array<uint8_t> emptyArray(emptyData);
        return emptyArray;
    }
    return array<uint8_t>(licenseId);
}

array<MediaKeyStatus> MediaKeySessionImpl::getEmptyMediaKeyStatusArray()
{
    DRM_INFO_LOG("getInvalidTaiheArray enter.");
    std::vector<MediaKeyStatus> res;
    return array<MediaKeyStatus>(res);
}

array<MediaKeyStatus> MediaKeySessionImpl::CheckMediaKeyStatus()
{
    DRM_INFO_LOG("CheckMediaKeyStatus enter");
    std::map<std::string, std::string> licenseStatus;

    if (keySessionImpl_ != nullptr) {
        int32_t ret = keySessionImpl_->CheckMediaKeyStatus(licenseStatus);
        if (ret != OHOS::DrmStandard::DRM_TAIHE_ERR_OK) {
            DRM_ERR_LOG("CheckMediaKeyStatus failed!");
            set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
                "CheckMediaKeyStatus failed");
            return getEmptyMediaKeyStatusArray();
        }
    } else {
        DRM_ERR_LOG("CheckMediaKeyStatus call Failed!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_UNKNOWN,
            "CheckMediaKeyStatus call failed");
        return getEmptyMediaKeyStatusArray();
    }

    if (licenseStatus.size() == 0) {
        DRM_ERR_LOG("Licence not exist.");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
            "CheckMediaKeyStatus call failed");
        return getEmptyMediaKeyStatusArray();
    }
    return vectorToJsArray(licenseStatus);
}

void MediaKeySessionImpl::ClearMediaKeys()
{
    DRM_INFO_LOG("ClearMediaKeys enter.");
    int32_t currentPid = OHOS::IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySessiontaihe GetCallingPID: %{public}d", currentPid);
    if (keySessionImpl_ != nullptr) {
        int32_t ret = keySessionImpl_->ClearMediaKeys();
        if (ret != OHOS::DrmStandard::DRM_TAIHE_ERR_OK) {
            DRM_ERR_LOG("ClearMediaKeys call Failed!");
            set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
                "ClearMediaKeys call failed");
            return;
        }
    } else {
        DRM_ERR_LOG("keySessionImpl_ is null!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
            "ClearMediaKeys call failed");
        return;
    }
}

array<uint8_t> MediaKeySessionImpl::getEmptyArray()
{
    DRM_INFO_LOG("getEmptyArray enter.");
    std::vector<uint8_t> res;
    return array<uint8_t>(res);
}

array<uint8_t> MediaKeySessionImpl::GenerateOfflineReleaseRequestSync(array_view<uint8_t> mediaKeyId)
{
    DRM_INFO_LOG("GenerateOfflineReleaseRequest enter");
    if (!CheckMediaKeySessionStatus(this)) {
        return getEmptyArray();
    }
    std::vector<uint8_t> releaseRequest;
    std::vector<uint8_t> releaseResponseLicenseId(mediaKeyId.begin(), mediaKeyId.end());
    int32_t intValue = keySessionImpl_->GenerateOfflineReleaseRequest(releaseResponseLicenseId,
        releaseRequest);
    if (intValue != OHOS::DrmStandard::DRM_TAIHE_ERR_OK) {
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
            "service error.");
        return getEmptyArray();
    }
    return array<uint8_t>(releaseRequest);
}

void MediaKeySessionImpl::ProcessOfflineReleaseResponseSync(array_view<uint8_t> mediaKeyId,
    array_view<uint8_t> response)
{
    DRM_INFO_LOG("ProcessOfflineReleaseResponse enter.");
    std::vector<uint8_t> releaseResponseLicenseId(mediaKeyId.begin(), mediaKeyId.end());
    std::vector<uint8_t> releaseResponse(response.begin(), response.end());
    if (!CheckMediaKeySessionStatus(this)) {
        return;
    }
    int32_t intValue = keySessionImpl_->ProcessOfflineReleaseResponse(releaseResponseLicenseId,
        releaseResponse);
    if (intValue != OHOS::DrmStandard::DRM_TAIHE_ERR_OK) {
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
            "service error.");
        return;
    }
}

void MediaKeySessionImpl::RestoreOfflineMediaKeysSync(array_view<uint8_t> mediaKeyId)
{
    DRM_INFO_LOG("RestoreOfflineMediaKeys enter.");
    int64_t beginTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    int32_t intValue;
    std::vector<uint8_t> restoreLicenseId(mediaKeyId.begin(), mediaKeyId.end());
    if (!CheckMediaKeySessionStatus(this)) {
        return;
    }
    intValue = keySessionImpl_->RestoreOfflineMediaKeys(restoreLicenseId);
    OHOS::DrmStandard::ConfigParser::WriteEndEvent(0, 0, std::string("restoreOfflineMediaKeys"), beginTime);
    if (intValue != OHOS::DrmStandard::DRM_TAIHE_ERR_OK) {
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
            "service error.");
        return;
    }
}

ContentProtectionLevel MediaKeySessionImpl::GetContentProtectionLevel()
{
    DRM_INFO_LOG("GetContentProtectionLevel enter.");

    OHOS::DrmStandard::ContentProtectionLevel level =
    (OHOS::DrmStandard::ContentProtectionLevel)0;
    ContentProtectionLevel::key_t levelKey;
    DrmTaiheUtils::GetEnumKeyByValue<ContentProtectionLevel>(static_cast<int32_t>(level), levelKey);
    if (keySessionImpl_ != nullptr) {
        int32_t ret = keySessionImpl_->GetContentProtectionLevel(&level);
        if (ret != OHOS::DrmStandard::DRM_TAIHE_ERR_OK) {
            DRM_ERR_LOG("GetContentProtectionLevel call Failed!");
            set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
                "GetContentProtectionLevel failed, service error.");
            return ContentProtectionLevel(levelKey);
        }
    } else {
        DRM_ERR_LOG("taihe_call Failed!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_UNKNOWN,
            "GetContentProtectionLevel failed, service error.");
        return ContentProtectionLevel(levelKey);
    }

    DrmTaiheUtils::GetEnumKeyByValue<ContentProtectionLevel>(static_cast<int32_t>(level), levelKey);
    return ContentProtectionLevel(levelKey);
}

bool MediaKeySessionImpl::RequireSecureDecoderModule(string_view mimeType)
{
    DRM_INFO_LOG("RequireSecureDecoderModule enter.");
    std::string mimeTypeStr(mimeType.data(), mimeType.size());
    bool statusValue = false;
    if (keySessionImpl_ == nullptr) {
        DRM_ERR_LOG("taihe call failed!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_UNKNOWN, "taihe call failed");
        return false;
    }
    int32_t ret = keySessionImpl_->RequireSecureDecoderModule(mimeTypeStr, &statusValue);
    if (ret != OHOS::DrmStandard::DRM_TAIHE_ERR_OK) {
        DRM_ERR_LOG("keySessionImpl_ RequireSecureDecoderModule call Failed!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
            "RequireSecureDecoderModule failed, service error.");
        return false;
    }
    DRM_INFO_LOG("taihe_get_boolean call success!,statusValue:%{public}d.", statusValue);
    return statusValue;
}

void MediaKeySessionImpl::Destroy()
{
    OHOS::DrmStandard::DrmTrace trace("MediaKeySessionImpl::Destroy");
    DRM_INFO_LOG("Destroy enter.");
    int32_t currentPid = OHOS::IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySessionNapi GetCallingPID: %{public}d", currentPid);

    if (keySessionImpl_ != nullptr) {
        int32_t ret = keySessionImpl_->Release();
        if (ret != OHOS::DrmStandard::DRM_TAIHE_ERR_OK) {
            DRM_ERR_LOG("keySessionImpl_ Release call Failed!");
            set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
                "Release failed, service error.");
        }
    } else {
        DRM_ERR_LOG("Release call Failed!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_UNKNOWN,
            "Release failed, unknown error.");
    }
}

optional<MediaKeySession> MediaKeySessionImpl::CreateMediaKeySession(
    OHOS::sptr<OHOS::DrmStandard::MediaKeySessionImpl>keySessionImpl)
{
    OHOS::DrmStandard::DrmTrace trace("MediaKeySessionImpl::CreateMediaKeySession");
    DRM_INFO_LOG("CreateMediaKeySession enter.");
    sMediaKeySessionImpl_ = keySessionImpl;
    if (sMediaKeySessionImpl_ == nullptr) {
        DRM_ERR_LOG("Failed to CreateMediaKeySession instance");
        return optional<MediaKeySession>(std::nullopt);
    }
    auto res = make_holder<MediaKeySessionImpl, MediaKeySession>();
    if (taihe::has_error()) {
        DRM_ERR_LOG("Create CreateMediaKeySession failed!");
        taihe::reset_error();
        return optional<MediaKeySession>(std::nullopt);
    }
    return optional<MediaKeySession>(std::in_place, res);
}

array<MediaKeyStatus> MediaKeySessionImpl::vectorToJsArray(std::map<std::string, std::string> &licenseStatus)
{
    DRM_INFO_LOG("vectorToJsArray enter.");
    std::vector<MediaKeyStatus> result;
    for (const auto &it : licenseStatus) {
        MediaKeyStatus status{
            DrmTaiheUtils::ToTaiheString(it.first),
            DrmTaiheUtils::ToTaiheString(it.second)
        };
        result.push_back(status);
    }
    return array<MediaKeyStatus>(result);
}

void MediaKeySessionImpl::OnKeyRequired(callback_view<void(EventInfo const&)> callback)
{
    DRM_INFO_LOG("OnKeyRequired In");
    ani_env *env = taihe::get_env();
    std::shared_ptr<taihe::callback<void(EventInfo const&)>> taiheCallback =
        std::make_shared<taihe::callback<void(EventInfo const&)>>(callback);
    std::shared_ptr<uintptr_t> cacheCallback = std::reinterpret_pointer_cast<uintptr_t>(taiheCallback);
    std::shared_ptr<AutoRef> callbackPair = std::make_shared<AutoRef>(env, cacheCallback);
    SetEventCallbackReference("keyRequired", callbackPair);
}

void MediaKeySessionImpl::OnKeyExpired(callback_view<void(EventInfo const&)> callback)
{
    DRM_INFO_LOG("OnKeyExpired In");
    ani_env *env = taihe::get_env();
    std::shared_ptr<taihe::callback<void(EventInfo const&)>> taiheCallback =
        std::make_shared<taihe::callback<void(EventInfo const&)>>(callback);
    std::shared_ptr<uintptr_t> cacheCallback = std::reinterpret_pointer_cast<uintptr_t>(taiheCallback);
    std::shared_ptr<AutoRef> callbackPair = std::make_shared<AutoRef>(env, cacheCallback);
    SetEventCallbackReference("keyExpired", callbackPair);
}

void MediaKeySessionImpl::OnVendorDefined(callback_view<void(EventInfo const&)> callback)
{
    DRM_INFO_LOG("OnVendorDefined In");
    ani_env *env = taihe::get_env();
    std::shared_ptr<taihe::callback<void(EventInfo const&)>> taiheCallback =
        std::make_shared<taihe::callback<void(EventInfo const&)>>(callback);
    std::shared_ptr<uintptr_t> cacheCallback = std::reinterpret_pointer_cast<uintptr_t>(taiheCallback);
    std::shared_ptr<AutoRef> callbackPair = std::make_shared<AutoRef>(env, cacheCallback);
    SetEventCallbackReference("vendorDefined", callbackPair);
}

void MediaKeySessionImpl::OnExpirationUpdate(callback_view<void(EventInfo const&)> callback)
{
    DRM_INFO_LOG("OnExpirationUpdate In");
    ani_env *env = taihe::get_env();
    std::shared_ptr<taihe::callback<void(EventInfo const&)>> taiheCallback =
        std::make_shared<taihe::callback<void(EventInfo const&)>>(callback);
    std::shared_ptr<uintptr_t> cacheCallback = std::reinterpret_pointer_cast<uintptr_t>(taiheCallback);
    std::shared_ptr<AutoRef> callbackPair = std::make_shared<AutoRef>(env, cacheCallback);
    SetEventCallbackReference("expirationUpdate", callbackPair);
}

void MediaKeySessionImpl::OnKeysChange(callback_view<void(
    array_view<KeysInfo>, bool)> callback)
{
    DRM_INFO_LOG("OnKeysChange In");
    ani_env *env = taihe::get_env();
    std::shared_ptr<taihe::callback<void(array_view<KeysInfo>, bool)>>
        taiheCallback = std::make_shared<taihe::callback<void(
        array_view<KeysInfo>, bool)>>(callback);
    std::shared_ptr<uintptr_t> cacheCallback = std::reinterpret_pointer_cast<uintptr_t>(taiheCallback);
    std::shared_ptr<AutoRef> callbackPair = std::make_shared<AutoRef>(env, cacheCallback);
    SetEventCallbackReference("keysChange", callbackPair);
}

void MediaKeySessionImpl::OffKeyRequired(optional_view<callback<void(EventInfo const&)>> callback)
{
    DRM_INFO_LOG("OffKeyRequired in");
    ClearEventCallbackReference("keyRequired");
    DRM_INFO_LOG("OffKeyRequired out");
}

void MediaKeySessionImpl::OffKeysChange(optional_view<callback<void(array_view<KeysInfo>, bool)>> callback)
{
    DRM_INFO_LOG("OffKeysChange in");
    ClearEventCallbackReference("keysChange");
    DRM_INFO_LOG("OffKeysChange out");
}

void MediaKeySessionImpl::OffKeyExpired(optional_view<callback<void(EventInfo const&)>> callback)
{
    DRM_INFO_LOG("OffKeyExpired in");
    ClearEventCallbackReference("keyExpired");
    DRM_INFO_LOG("OffKeyExpired out");
}

void MediaKeySessionImpl::OffVendorDefined(optional_view<callback<void(EventInfo const&)>> callback)
{
    DRM_INFO_LOG("OffVendorDefined in");
    ClearEventCallbackReference("vendorDefined");
    DRM_INFO_LOG("OffVendorDefined out");
}

void MediaKeySessionImpl::OffExpirationUpdate(optional_view<callback<void(EventInfo const&)>> callback)
{
    DRM_INFO_LOG("OffExpirationUpdate in");
    ClearEventCallbackReference("expirationUpdate");
    DRM_INFO_LOG("OffExpirationUpdate out");
}

void MediaKeySessionImpl::SetEventCallbackReference(const std::string eventType, std::shared_ptr<AutoRef> callbackPair)
{
    DRM_INFO_LOG("SetEventCallbackReference");
    std::lock_guard<std::mutex> lock(mutex_);
    if (keySessionCallbackTaihe_ != nullptr) {
        keySessionCallbackTaihe_->SetCallbackReference(eventType, callbackPair);
    } else {
        DRM_ERR_LOG("SetEventCallbackReference failed.");
    }
}

void MediaKeySessionImpl::ClearEventCallbackReference(const std::string eventType)
{
    DRM_INFO_LOG("ClearEventCallbackReference");
    if (keySessionCallbackTaihe_ != nullptr) {
        keySessionCallbackTaihe_->ClearCallbackReference(eventType);
    } else {
        DRM_ERR_LOG("ClearEventCallbackReference failed.");
    }
}
} // namespace DrmStandard
} // namespace ANI