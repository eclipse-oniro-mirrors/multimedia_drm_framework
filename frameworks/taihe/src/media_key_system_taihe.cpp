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

#include "media_key_system_taihe.h"
#include "key_session_taihe.h"
#include "drm_trace.h"
#include "drm_api_operation.h"

using namespace ANI::DrmStandard;

namespace ANI {
namespace DrmStandard {
thread_local OHOS::sptr<OHOS::DrmStandard::MediaKeySystemImpl> MediaKeySystemImpl::sMediaKeySystemTaihe_ = nullptr;

const std::unordered_map<OHOS::DrmStandard::DrmInnerErrCode, OHOS::DrmStandard::DrmTaiheErrCode>
    INNERCODE_TO_API12_TAIHECODE = {
    {OHOS::DrmStandard::DRM_INNER_ERR_OK,                                         OHOS::DrmStandard::DRM_TAIHE_ERR_OK},
    {OHOS::DrmStandard::DRM_INNER_ERR_UNKNOWN,                               OHOS::DrmStandard::DRM_TAIHE_ERR_UNKNOWN},
    {OHOS::DrmStandard::DRM_INNER_ERR_MAX_SYSTEM_NUM_REACHED, OHOS::DrmStandard::DRM_TAIHE_ERR_MAX_SYSTEM_NUM_REACHED},
    {OHOS::DrmStandard::DRM_INNER_ERR_MAX_SESSION_NUM_REACHED,
        OHOS::DrmStandard::DRM_TAIHE_ERR_MAX_SESSION_NUM_REACHED},
    {OHOS::DrmStandard::DRM_INNER_ERR_INVALID_VAL,                       OHOS::DrmStandard::DRM_TAIHE_ERR_INVALID_VAL},
    {OHOS::DrmStandard::DRM_INNER_ERR_SERVICE_DIED,                    OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL},
    {OHOS::DrmStandard::DRM_INNER_ERR_SERVICE_FATAL_ERROR,             OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL},
};

MediaKeySystemImpl::MediaKeySystemImpl()
{
    OHOS::DrmStandard::DrmTrace trace("MediaKeySystemTaihe::MediaKeySystemImpl");
    DRM_INFO_LOG("MediaKeySystemImpl enter.");
    if (sMediaKeySystemTaihe_ == nullptr) {
        DRM_ERR_LOG("sMediaKeySessionImpl_ is null");
        DrmTaiheUtils::ThrowExceptionError("failed to CreateMediaKeySession");
        return;
    }
    mediaKeySystemImpl_ = sMediaKeySystemTaihe_;
    mediaKeySystemCallbackTaihe_ = new (std::nothrow) MediaKeySystemCallbackTaihe();
    mediaKeySystemImpl_->SetCallback(mediaKeySystemCallbackTaihe_);
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " Instances create", FAKE_POINTER(this));
}

MediaKeySystemImpl::~MediaKeySystemImpl()
{
    OHOS::DrmStandard::DrmTrace trace("MediaKeySystemImplDestructor");
    DRM_INFO_LOG("MediaKeySystemImplDestructor enter.");
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " Instances destroy", FAKE_POINTER(this));
    if (mediaKeySystemImpl_) {
        mediaKeySystemImpl_ = nullptr;
    }
    if (mediaKeySystemCallbackTaihe_) {
        mediaKeySystemCallbackTaihe_ = nullptr;
    }
}

string MediaKeySystemImpl::GetConfigurationString(string_view configName)
{
    DRM_INFO_LOG("GetConfiguration enter");
    std::string value;
    std::string name = std::string(configName);
    if (mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemImpl_->GetConfigurationString(name, value);
        if (ret != OHOS::DrmStandard::DRM_INNER_ERR_OK) {
            DRM_ERR_LOG("GetConfigurationString failed!");
            set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
                "GetConfigurationString failed, service error.");
            return DrmTaiheUtils::ToTaiheString("");
        }
    } else {
        DRM_ERR_LOG("mediaKeySystemImpl_ is nullptr!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_UNKNOWN, "Get configuration failed, unknown error");
        return DrmTaiheUtils::ToTaiheString("");
    }
    return DrmTaiheUtils::ToTaiheString(value);
}

void MediaKeySystemImpl::SetConfigurationString(string_view configName, string_view value)
{
    DRM_INFO_LOG("SetConfiguration enter.");
    std::string name = std::string(configName);
    if (value.empty()) {
        DRM_ERR_LOG("String Parameter length cannot be zero!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_INVALID_VAL, "The param value length is 0.");
        return;
    }
    std::string taiheValue = std::string(value);
    if (mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemImpl_->SetConfigurationString(name, taiheValue);
        if (ret != OHOS::DrmStandard::DRM_INNER_ERR_OK) {
            DRM_ERR_LOG("SetConfigurationString failed!");
            set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
                "SetConfigurationString failed, service error.");
        }
    } else {
        DRM_ERR_LOG("mediaKeySystemImpl_ is nullptr!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_UNKNOWN, "SetConfigurationString failed, unknown error.");
    }
}

array<uint8_t> MediaKeySystemImpl::GetConfigurationByteArray(string_view configName)
{
    DRM_INFO_LOG("GetConfigurationByteArray enter");
    std::string name = std::string(configName);
    std::vector<uint8_t> value;
    std::vector<uint8_t> emptyVector;
    if (mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemImpl_->GetConfigurationByteArray(name, value);
        if (ret != OHOS::DrmStandard::DRM_INNER_ERR_OK) {
            DRM_ERR_LOG("GetConfigurationByteArray failed!");
            set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
                "GetConfigurationByteArray failed, service error.");
            return array<uint8_t>(emptyVector);
        }
    } else {
        DRM_ERR_LOG("mediaKeySystemImpl_ is nullptr!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_UNKNOWN,
            "GetConfigurationByteArray failed, unknown error.");
        return array<uint8_t>(emptyVector);
    }
    return array<uint8_t>(value);
}

void MediaKeySystemImpl::SetConfigurationByteArray(string_view configName, array_view<uint8_t> value)
{
    DRM_INFO_LOG("SetConfigurationByteArray enter.");
    std::string name = std::string(configName);
    if (value.empty()) {
        DRM_ERR_LOG("value is empty!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_INVALID_VAL, "The param value is missing");
        return;
    }
    std::vector<uint8_t> taiheValue(value.begin(), value.end());
    if (mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemImpl_->SetConfigurationByteArray(name, taiheValue);
        if (ret != OHOS::DrmStandard::DRM_INNER_ERR_OK) {
            DRM_ERR_LOG("SetConfigurationByteArray failed!");
            set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
                "SetConfigurationByteArray failed, service error.");
        }
    } else {
        DRM_ERR_LOG("mediaKeySystemImpl_ is nullptr!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_UNKNOWN,
            "SetConfigurationByteArray failed, unknown error.");
    }
}

static array<StatisticKeyValue> vectorToTaiheArray(
    std::vector<OHOS::DrmStandard::MetircKeyValue> &metrics)
{
    DRM_INFO_LOG("vectorToJsArray enter.");
    std::vector<StatisticKeyValue> res;
    for (size_t i = 0; i < metrics.size(); i++) {
        StatisticKeyValue statisticKeyValue {
            DrmTaiheUtils::ToTaiheString(metrics[i].name),
            DrmTaiheUtils::ToTaiheString(metrics[i].value),
        };
        res.push_back(statisticKeyValue);
    }
    return array<StatisticKeyValue>(res);
}

static array<StatisticKeyValue> getInvalidTaiheArray()
{
    DRM_INFO_LOG("getInvalidTaiheArray enter.");
    std::vector<StatisticKeyValue> res;
    return array<StatisticKeyValue>(res);
}

array<StatisticKeyValue> MediaKeySystemImpl::GetStatistics()
{
    DRM_INFO_LOG("GetStatistics enter.");
    std::vector<OHOS::DrmStandard::MetircKeyValue> metrics;
    if (mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemImpl_->GetStatistics(metrics);
        if (ret != OHOS::DrmStandard::DRM_INNER_ERR_OK) {
            DRM_ERR_LOG("GetStatistics failed!");
            set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
                "Get statistics failed, service error.");
            return getInvalidTaiheArray();
        }
    } else {
        DRM_ERR_LOG("mediaKeySystemImpl_ is nullptr!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_UNKNOWN, "GetStatistics failed, unknown error.");
        return getInvalidTaiheArray();
    }
    return vectorToTaiheArray(metrics);
}

ContentProtectionLevel MediaKeySystemImpl::GetMaxContentProtectionLevel()
{
    DRM_INFO_LOG("GetMaxContentProtectionLevel enter.");
    OHOS::DrmStandard::ContentProtectionLevel level =
        OHOS::DrmStandard::ContentProtectionLevel::CONTENT_PROTECTION_LEVEL_UNKNOWN;
    ContentProtectionLevel::key_t contentProtectionLevel;
    DrmTaiheUtils::GetEnumKeyByValue<ContentProtectionLevel>(static_cast<int32_t>(level), contentProtectionLevel);
    if (mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemImpl_->GetMaxContentProtectionLevel(&level);
        if (ret != OHOS::DrmStandard::DRM_INNER_ERR_OK) {
            DRM_ERR_LOG("GetMaxContentProtectionLevel failed!");
            set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
                "GetMaxContentProtectionLevel failed, service error.");
            return ContentProtectionLevel(contentProtectionLevel);
        }
    } else {
        DRM_ERR_LOG("mediaKeySystemImpl_ is nullptr!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_UNKNOWN,
            "GetMaxContentProtectionLevel failed, unknown error.");
        return ContentProtectionLevel(contentProtectionLevel);
    }
    DrmTaiheUtils::GetEnumKeyByValue<ContentProtectionLevel>(
        static_cast<int32_t>(level), contentProtectionLevel);
    return ContentProtectionLevel(contentProtectionLevel);
}

static ProvisionRequest getInvalidProvisionRequest()
{
    DRM_INFO_LOG("getInvalidProvisionRequest enter.");
    std::vector<uint8_t> emptyVector;
    ProvisionRequest res {
        array<uint8_t>(emptyVector),
        DrmTaiheUtils::ToTaiheString(""),
    };
    return res;
}

ProvisionRequest MediaKeySystemImpl::GenerateKeySystemRequestSync()
{
    DRM_INFO_LOG("GenerateKeySystemRequest enter");
    OHOS::DrmStandard::DrmTrace trace("MediaKeySystemTaihe::GenerateKeySystemRequest");
    if (mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemImpl_->GenerateKeySystemRequest(
            provisionRequest.data, provisionRequest.defaultURL);
        if (ret != OHOS::DrmStandard::DRM_INNER_ERR_OK) {
            DRM_ERR_LOG("GenerateKeySystemRequestSync failed!");
            set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
                "GenerateKeySystemRequestSync failed, service error.");
            return getInvalidProvisionRequest();
        }
    } else {
        DRM_ERR_LOG("mediaKeySystemImpl_ is nullptr!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
            "GenerateKeySystemRequestSync failed, unknown error.");
        return getInvalidProvisionRequest();
    }
    ProvisionRequest provisionRequestInfo {
        array<uint8_t>(provisionRequest.data),
        DrmTaiheUtils::ToTaiheString(provisionRequest.defaultURL),
    };
    return provisionRequestInfo;
}

void MediaKeySystemImpl::ProcessKeySystemResponseSync(array_view<uint8_t> response)
{
    DRM_INFO_LOG("ProcessKeySystemResponse enter.");
    int64_t beginTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    OHOS::DrmStandard::DrmTrace trace("MediaKeySystemTaihe::ProcessKeySystemResponse");
    std::vector<uint8_t> taiheValue(response.begin(), response.end());
    OHOS::DrmStandard::ConfigParser::WriteEndEvent(0, 0, std::string("processKeySystemResponse"), beginTime);
    if (mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemImpl_->ProcessKeySystemResponse(taiheValue);
        if (ret != OHOS::DrmStandard::DRM_INNER_ERR_OK) {
            DRM_ERR_LOG("ProcessKeySystemResponseSync failed!");
            set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
                "ProcessKeySystemResponseSync failed, service error.");
        }
    } else {
        DRM_ERR_LOG("mediaKeySystemImpl_ is nullptr!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
            "ProcessKeySystemResponseSync failed, unknown error.");
    }
}

CertificateStatus MediaKeySystemImpl::GetCertificateStatus()
{
    OHOS::DrmStandard::DrmTrace trace("MediaKeySystemTaihe::GetCertificateStatus");
    DRM_INFO_LOG("GetCertificateStatus enter.");
    OHOS::DrmStandard::CertificateStatus certStatus =
        OHOS::DrmStandard::CertificateStatus::CERT_STATUS_UNAVAILABLE;
    CertificateStatus::key_t certStatusInfo;
    DrmTaiheUtils::GetEnumKeyByValue<CertificateStatus>(static_cast<int32_t>(certStatus), certStatusInfo);
    if (mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemImpl_->GetCertificateStatus(&certStatus);
        if (ret != OHOS::DrmStandard::DRM_INNER_ERR_OK) {
            DRM_ERR_LOG("GetCertificateStatus failed!");
            set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
                "GetCertificateStatus failed, service error.");
            return CertificateStatus(certStatusInfo);
        }
    } else {
        DRM_ERR_LOG("mediaKeySystemImpl_ is nullptr!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_UNKNOWN, "GetCertificateStatus failed, unknown error.");
        return CertificateStatus(certStatusInfo);
    }
    DrmTaiheUtils::GetEnumKeyByValue<CertificateStatus>(static_cast<int32_t>(certStatus), certStatusInfo);
    return CertificateStatus(certStatusInfo);
}

OHOS::DrmStandard::DrmTaiheErrCode MediaKeySystemImpl::DrmInnerErrToTaiheErrAPI12(
    OHOS::DrmStandard::DrmInnerErrCode errCode)
{
    if (INNERCODE_TO_API12_TAIHECODE.find(errCode) != INNERCODE_TO_API12_TAIHECODE.end()) {
        return INNERCODE_TO_API12_TAIHECODE.at(errCode);
    }
    return OHOS::DrmStandard::DRM_TAIHE_ERR_UNKNOWN;
}


optional<MediaKeySession> MediaKeySystemImpl::CreateMediaKeySessionWithLevel(ContentProtectionLevel level)
{
    DRM_INFO_LOG("CreateMediaKeySessionWithLevel enter.");
    OHOS::DrmStandard::DrmTrace trace("MediaKeySystemTaihe::CreateMediaKeySessionWithLevel");
    auto res = optional<MediaKeySession>(std::nullopt);
    OHOS::sptr<OHOS::DrmStandard::MediaKeySessionImpl> keySessionImpl = nullptr;
    int32_t taiheContentProtectionLevel = level.get_value();
    OHOS::DrmStandard::ContentProtectionLevel securityLevel =
        static_cast<OHOS::DrmStandard::ContentProtectionLevel>(taiheContentProtectionLevel);
    if (securityLevel <= OHOS::DrmStandard::ContentProtectionLevel::
        CONTENT_PROTECTION_LEVEL_UNKNOWN || securityLevel >= OHOS::DrmStandard::
        ContentProtectionLevel::CONTENT_PROTECTION_LEVEL_MAX) {
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_INVALID_VAL,
            "The param ContentProtectionLevel exceeds reasonable range.");
        DRM_ERR_LOG("securityLevel is error!");
        return res;
    }
    if (mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemImpl_->CreateMediaKeySession(
            (OHOS::DrmStandard::ContentProtectionLevel)securityLevel, &keySessionImpl);
        if (ret != OHOS::DrmStandard::DRM_INNER_ERR_OK) {
            set_business_error(DrmInnerErrToTaiheErrAPI12(static_cast<OHOS::DrmStandard::DrmInnerErrCode>(ret)),
                "CreateMediaKeySessionWithLevel failed.");
            DRM_ERR_LOG("CreateMediaKeySessionWithLevel get failed!!!");
            return res;
        }
    } else {
        DRM_ERR_LOG("mediaKeySystemImpl_ is nullptr!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_UNKNOWN, "CreateMediaKeySessionWithLevel failed");
        return res;
    }
    return MediaKeySessionImpl::CreateMediaKeySession(keySessionImpl);
}

optional<MediaKeySession> MediaKeySystemImpl::CreateMediaKeySession()
{
    DRM_INFO_LOG("CreateMediaKeySession enter.");
    OHOS::DrmStandard::DrmTrace trace("MediaKeySystemTaihe::CreateMediaKeySession");
    auto res = optional<MediaKeySession>(std::nullopt);
    OHOS::DrmStandard::ContentProtectionLevel securityLevel =
        OHOS::DrmStandard::ContentProtectionLevel::CONTENT_PROTECTION_LEVEL_UNKNOWN;
    OHOS::sptr<OHOS::DrmStandard::MediaKeySessionImpl> keySessionImpl = nullptr;
    if (mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemImpl_->CreateMediaKeySession(
            (OHOS::DrmStandard::ContentProtectionLevel)securityLevel, &keySessionImpl);
        if (ret != OHOS::DrmStandard::DRM_INNER_ERR_OK) {
            set_business_error(DrmInnerErrToTaiheErrAPI12(static_cast<OHOS::DrmStandard::DrmInnerErrCode>(ret)),
                "CreateMediaKeySession failed.");
            DRM_ERR_LOG("CreateMediaKeySession get failed!!!");
            return res;
        }
    } else {
        DRM_ERR_LOG("mediaKeySystemImpl_ is nullptr!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_UNKNOWN, "CreateMediaKeySession failed");
        return res;
    }
    return MediaKeySessionImpl::CreateMediaKeySession(keySessionImpl);
}

static array<array<uint8_t>> getValidMediaKeyIds(std::vector<std::vector<uint8_t>> vec)
{
    std::vector<array<uint8_t>> res;
    for (size_t i = 0; i < vec.size(); i++) {
        res.push_back(array<uint8_t>(vec[i]));
    }
    return array<array<uint8_t>>(res);
}

static array<array<uint8_t>> getInvalidMediaKeyIds()
{
    std::vector<array<uint8_t>> res;
    return array<array<uint8_t>>(res);
}

array<array<uint8_t>> MediaKeySystemImpl::GetOfflineMediaKeyIds()
{
    DRM_INFO_LOG("GetOfflineMediaKeyIds enter.");
    std::vector<std::vector<uint8_t>> licenseIds;
    if (mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemImpl_->GetOfflineMediaKeyIds(licenseIds);
        if (ret != OHOS::DrmStandard::DRM_INNER_ERR_OK) {
            DRM_ERR_LOG("napi GetOfflineMediaKeyIds failed!");
            set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
                "GetOfflineMediaKeyIds failed, service error.");
            return getInvalidMediaKeyIds();
        }
    } else {
        DRM_ERR_LOG("mediaKeySystemImpl_ is nullptr!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_UNKNOWN, "GetOfflineMediaKeyIds failed, unknown error.");
        return getInvalidMediaKeyIds();
    }
    return getValidMediaKeyIds(licenseIds);
}

OfflineMediaKeyStatus MediaKeySystemImpl::GetOfflineMediaKeyStatus(array_view<uint8_t> mediaKeyId)
{
    DRM_INFO_LOG("GetOfflineMediaKeyStatus enter");
    OHOS::DrmStandard::OfflineMediaKeyStatus offlineMediaKeyStatus =
        OHOS::DrmStandard::OfflineMediaKeyStatus::OFFLINELICENSESTATUS_UNKNOWN;
    OfflineMediaKeyStatus::key_t offlineMediaKeyStatusValue;
    DrmTaiheUtils::GetEnumKeyByValue<OfflineMediaKeyStatus>(static_cast<int32_t>(offlineMediaKeyStatus),
        offlineMediaKeyStatusValue);

    if (mediaKeyId.empty()) {
        DRM_ERR_LOG("mediaKeyId is empty!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_INVALID_VAL, "The param mediaKeyId is invalid.");
        return OfflineMediaKeyStatus(offlineMediaKeyStatusValue);
    }
    std::vector<uint8_t> licenseIdVec(mediaKeyId.begin(), mediaKeyId.end());
    if (mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemImpl_->GetOfflineMediaKeyStatus(licenseIdVec, offlineMediaKeyStatus);
        if (ret != OHOS::DrmStandard::DRM_INNER_ERR_OK) {
            DRM_ERR_LOG("GetOfflineMediaKeyStatus failed!");
            set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
                "GetOfflineMediaKeyStatus failed, service error.");
            return OfflineMediaKeyStatus(offlineMediaKeyStatusValue);
        }
    } else {
        DRM_ERR_LOG("mediaKeySystemImpl_ is nullptr!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_UNKNOWN, "GetOfflineMediaKeyStatus failed, unknown error.");
        return OfflineMediaKeyStatus(offlineMediaKeyStatusValue);
    }
    DrmTaiheUtils::GetEnumKeyByValue<OfflineMediaKeyStatus>(
        static_cast<int32_t>(offlineMediaKeyStatus), offlineMediaKeyStatusValue);
    return OfflineMediaKeyStatus(offlineMediaKeyStatusValue);
}

void MediaKeySystemImpl::ClearOfflineMediaKeys(array_view<uint8_t> mediaKeyId)
{
    DRM_INFO_LOG("ClearOfflineMediaKeys enter");

    if (mediaKeyId.empty()) {
        DRM_ERR_LOG("mediaKeyId is empty!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_UNKNOWN, "ClearOfflineMediaKeys failed, unknown error.");
        return;
    }
    std::vector<uint8_t> licenseIdVec(mediaKeyId.begin(), mediaKeyId.end());
    if (mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemImpl_->ClearOfflineMediaKeys(licenseIdVec);
        if (ret != OHOS::DrmStandard::DRM_INNER_ERR_OK) {
            DRM_ERR_LOG("ClearOfflineMediaKeys failed!");
            set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL,
                "ClearOfflineMediaKeys failed, service error.");
        }
    } else {
        DRM_ERR_LOG("ClearOfflineMediaKeys call Failed!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_UNKNOWN,
            "ClearOfflineMediaKeys failed, unknown error.");
    }
}

void MediaKeySystemImpl::Destroy()
{
    DRM_INFO_LOG("Release enter.");
    int32_t currentPid = OHOS::IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("GetCallingPID: %{public}d", currentPid);
    if (mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemImpl_->Release();
        if (ret != OHOS::DrmStandard::DRM_INNER_ERR_OK) {
            DRM_ERR_LOG("Destroy failed!");
            set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL, "Destroy failed, service error.");
        }
    } else {
        DRM_ERR_LOG("mediaKeySystemImpl_ is nullptr!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_UNKNOWN, "Destroy failed.");
    }
}

void MediaKeySystemImpl::OnKeySystemRequired(callback_view<void(EventInfo const&)> callback)
{
    DRM_INFO_LOG("OnKeySystemRequired enter.");
    ani_env *env = taihe::get_env();
    std::shared_ptr<taihe::callback<void(EventInfo const&)>> taiheCallback =
        std::make_shared<taihe::callback<void(EventInfo const&)>>(callback);
    std::shared_ptr<uintptr_t> cacheCallback = std::reinterpret_pointer_cast<uintptr_t>(taiheCallback);
    std::shared_ptr<AutoRef> callbackPair = std::make_shared<AutoRef>(env, cacheCallback);
    SaveEventCallbackReferrence("keySystemRequired", callbackPair);
    DRM_INFO_LOG("OnKeySystemRequired out");
}

void MediaKeySystemImpl::SaveEventCallbackReferrence(const std::string eventType, std::shared_ptr<AutoRef> callbackPair)
{
    DRM_INFO_LOG("SaveEventCallbackReferrence");
    std::lock_guard<std::mutex> lock(mutex_);
    if (mediaKeySystemCallbackTaihe_ != nullptr) {
        mediaKeySystemCallbackTaihe_->SetCallbackReference(eventType, callbackPair);
    } else {
        DRM_ERR_LOG("SaveEventCallbackReferrence failed.");
    }
}

void MediaKeySystemImpl::OffKeySystemRequired(optional_view<callback<void(
    EventInfo const&)>> callback)
{
    DRM_INFO_LOG("OffKeySystemRequired enter");
    ClearEventCallbackReferrence("keySystemRequired");
    DRM_INFO_LOG("OffKeySystemRequired out");
}

void MediaKeySystemImpl::ClearEventCallbackReferrence(const std::string eventType)
{
    DRM_INFO_LOG("ClearEventCallbackReference");
    if (mediaKeySystemCallbackTaihe_ != nullptr) {
        mediaKeySystemCallbackTaihe_->ClearCallbackReference(eventType);
    } else {
        DRM_ERR_LOG("ClearEventCallbackReference failed.");
    }
}

string GetMediaKeySystemUuid(string_view name)
{
    DRM_INFO_LOG("GetMediaKeySystemUuid enter");
    std::string uuid;
    std::string nameStr = std::string(name);
    int32_t ret = OHOS::DrmStandard::MediaKeySystemFactoryImpl::GetInstance()->GetMediaKeySystemUuid(nameStr, uuid);
    if (ret != OHOS::DrmStandard::DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GetMediaKeySystemUuid failed");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL, "GetMediaKeySystemUuid call failed");
        return DrmTaiheUtils::ToTaiheString("");
    }
    return DrmTaiheUtils::ToTaiheString(uuid);
}

array<MediaKeySystemDescription> GetMediaKeySystems()
{
    DRM_INFO_LOG("GetMediaKeySystems enter.");
    std::map<std::string, std::string> keySystemNames;
    std::vector<MediaKeySystemDescription> mediaKeySystemDescriptions;
    int32_t ret = OHOS::DrmStandard::MediaKeySystemFactoryImpl::GetInstance()->GetMediaKeySystems(keySystemNames);
    if (keySystemNames.size() == 0 || ret != OHOS::DrmStandard::DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("Plugin not exist.");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL, "GetMediaKeySystems call Failed!");
        std::vector<MediaKeySystemDescription> invalidMediaKeySystemDescriptions;
        return array<MediaKeySystemDescription>(invalidMediaKeySystemDescriptions);
    }
    for (auto it = keySystemNames.begin(); it != keySystemNames.end(); it++) {
        MediaKeySystemDescription mediaKeySystemDescription {
            DrmTaiheUtils::ToTaiheString(it->first),
            DrmTaiheUtils::ToTaiheString(it->second),
        };
        mediaKeySystemDescriptions.push_back(mediaKeySystemDescription);
    }
    return array<MediaKeySystemDescription>(mediaKeySystemDescriptions);
}

optional<MediaKeySystem> CreateMediaKeySystem(string_view name)
{
    DRM_INFO_LOG("CreateMediaKeySystem enter.");
    std::string drmSchemaName = std::string(name);
    int32_t ret = OHOS::DrmStandard::MediaKeySystemFactoryImpl::GetInstance()->CreateMediaKeySystem(drmSchemaName,
        &MediaKeySystemImpl::sMediaKeySystemTaihe_);
    if (ret != OHOS::DrmStandard::DRM_INNER_ERR_OK) {
        set_business_error(MediaKeySystemImpl::DrmInnerErrToTaiheErrAPI12(
            static_cast<OHOS::DrmStandard::DrmInnerErrCode>(ret)), "create MediaKeySystem failed.");
        DRM_ERR_LOG("CreateMediaKeySystem failed!");
        return optional<MediaKeySystem>(std::nullopt);
    }
    auto res = make_holder<MediaKeySystemImpl, MediaKeySystem>();
    if (taihe::has_error()) {
        DRM_ERR_LOG("Create CreateMediaKeySystem failed!");
        taihe::reset_error();
        return optional<MediaKeySystem>(std::nullopt);
    }
    return optional<MediaKeySystem>(std::in_place, res);
}

bool IsMediaKeySystemSupportedWithThreeArgs(string_view name, string_view mimeType, ContentProtectionLevel level)
{
    DRM_INFO_LOG("IsMediaKeySystemSupportedWithThreeArgs enter.");
    std::string pluginName = std::string(name);
    if (pluginName.length() == 0 || pluginName.length() > MAX_STRING_SIZE) {
        DRM_ERR_LOG("drm plugin name lenth is not able to zero or more 256!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_INVALID_VAL,
            "drm plugin name length exceeds reasonable range!");
        return false;
    }
    if (OHOS::DrmStandard::MediaKeySystemFactoryImpl::GetInstance() == nullptr) {
        DRM_ERR_LOG("get instance failed!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL, "get instance failed!");
        return false;
    }
    std::string mimeTypeLocal = std::string(mimeType);
    int32_t taiheContentProtectionLevel = level.get_value();
    OHOS::DrmStandard::ContentProtectionLevel securityLevel =
        static_cast<OHOS::DrmStandard::ContentProtectionLevel>(taiheContentProtectionLevel);
    if ((securityLevel <= OHOS::DrmStandard::ContentProtectionLevel::CONTENT_PROTECTION_LEVEL_UNKNOWN) ||
        (securityLevel >= OHOS::DrmStandard::ContentProtectionLevel::CONTENT_PROTECTION_LEVEL_MAX)) {
        DRM_ERR_LOG("ContentProtectionLevel is invalid");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_INVALID_VAL,
            "param ContentProtectionLevel exceeds reasonable range!");
        return false;
    }
    bool isSupported = OHOS::DrmStandard::MediaKeySystemFactoryImpl::GetInstance()->IsMediaKeySystemSupported(
        pluginName, mimeTypeLocal, securityLevel);
    return isSupported;
}

bool IsMediaKeySystemSupportedWithTwoArgs(string_view name, string_view mimeType)
{
    DRM_INFO_LOG("IsMediaKeySystemSupportedWithTwoArgs enter.");
    std::string pluginName = std::string(name);
    if (pluginName.length() == 0 || pluginName.length() > MAX_STRING_SIZE) {
        DRM_ERR_LOG("drm plugin name lenth is not able to zero or more 256!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_INVALID_VAL,
            "drm plugin name length exceeds reasonable range!");
        return false;
    }
    if (OHOS::DrmStandard::MediaKeySystemFactoryImpl::GetInstance() == nullptr) {
        DRM_ERR_LOG("get instance failed!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL, "get instance failed!");
        return false;
    }
    std::string mimeTypeLocal = std::string(mimeType);
    if (mimeTypeLocal.length() != 0) {
        bool isSupported = OHOS::DrmStandard::MediaKeySystemFactoryImpl::GetInstance()->IsMediaKeySystemSupported(
            pluginName, mimeTypeLocal);
        return isSupported;
    }
    set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_UNKNOWN, "mimeType is empty");
    return false;
}

bool IsMediaKeySystemSupported(string_view name)
{
    DRM_INFO_LOG("IsMediaKeySystemSupported enter.");
    std::string pluginName = std::string(name);
    if (pluginName.length() == 0 || pluginName.length() > MAX_STRING_SIZE) {
        DRM_ERR_LOG("drm plugin name lenth is not able to zero or more 256!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_INVALID_VAL,
            "drm plugin name length exceeds reasonable range!");
        return false;
    }
    if (OHOS::DrmStandard::MediaKeySystemFactoryImpl::GetInstance() == nullptr) {
        DRM_ERR_LOG("get instance failed!");
        set_business_error(OHOS::DrmStandard::DRM_TAIHE_ERR_SERVICE_FATAL, "get instance failed!");
        return false;
    }
    bool isSupported = OHOS::DrmStandard::MediaKeySystemFactoryImpl::GetInstance()->IsMediaKeySystemSupported(
        pluginName);
    return isSupported;
}
} // namespace DrmStandard
} // namespace ANI
TH_EXPORT_CPP_API_GetMediaKeySystemUuid(GetMediaKeySystemUuid);
TH_EXPORT_CPP_API_GetMediaKeySystems(GetMediaKeySystems);
TH_EXPORT_CPP_API_CreateMediaKeySystem(CreateMediaKeySystem);
TH_EXPORT_CPP_API_IsMediaKeySystemSupportedWithThreeArgs(IsMediaKeySystemSupportedWithThreeArgs);
TH_EXPORT_CPP_API_IsMediaKeySystemSupportedWithTwoArgs(IsMediaKeySystemSupportedWithTwoArgs);
TH_EXPORT_CPP_API_IsMediaKeySystemSupported(IsMediaKeySystemSupported);