/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include "key_session_service.h"
#include "drm_dfx.h"
#include "drm_dfx_utils.h"
#include "drm_host_manager.h"
#include "drm_log.h"
#include "drm_trace.h"
#include "drm_error_code.h"
#include "ipc_skeleton.h"
#include "hitrace/tracechain.h"

namespace OHOS {
namespace DrmStandard {
using namespace OHOS::HiviewDFX;

MediaKeySessionService::MediaKeySessionService(sptr<OHOS::HDI::Drm::V1_0::IMediaKeySession> hdiMediaKeySession)
{
    DRM_INFO_LOG("0x%{public}06" PRIXPTR " Instances create.", FAKE_POINTER(this));
    sessionOperatorsCallback_ = nullptr;
    hdiMediaKeySession_ = hdiMediaKeySession;
}

MediaKeySessionService::MediaKeySessionService(sptr<OHOS::HDI::Drm::V1_0::IMediaKeySession> hdiMediaKeySession,
    StatisticsInfo statisticsInfo)
{
    DRM_INFO_LOG("0x%{public}06" PRIXPTR " Instances create.", FAKE_POINTER(this));
    sessionOperatorsCallback_ = nullptr;
    hdiMediaKeySession_ = hdiMediaKeySession;
    statisticsInfo_ = statisticsInfo;
}

MediaKeySessionService::~MediaKeySessionService()
{
    DRM_INFO_LOG("~MediaKeySessionService 0x%{public}06" PRIXPTR " Instances destroy.", FAKE_POINTER(this));
    std::lock_guard<std::recursive_mutex> lock(callbackMutex_);
    if (sessionOperatorsCallback_ != nullptr) {
        sessionOperatorsCallback_ = nullptr;
    }
}

int32_t MediaKeySessionService::CloseMediaKeySessionServiceByCallback()
{
    DRM_INFO_LOG("CloseMediaKeySessionServiceByCallback enter.");
    {
        std::lock_guard<std::recursive_mutex> lock(callbackMutex_);
        if (sessionOperatorsCallback_ != nullptr) {
            sessionOperatorsCallback_ = nullptr;
        }
    }
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    int32_t currentPid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("GetCallingPID: %{public}d", currentPid);
    if (hdiMediaKeySession_ != nullptr) {
        DRM_INFO_LOG("hdiMediaKeySession_ CloseHdiMediaKeySession");
        hdiMediaKeySession_->Destroy();
        hdiMediaKeySession_ = nullptr;
    }

    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySessionService::Release()
{
    DRM_INFO_LOG("Release enter.");
    std::lock_guard<std::recursive_mutex> lock(callbackMutex_);
    int32_t currentPid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySessionService GetCallingPID: %{public}d", currentPid);
    if (sessionOperatorsCallback_ != nullptr) {
        sessionOperatorsCallback_->CloseMediaKeySessionService(this);
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySessionService::SetMediaKeySessionServiceOperatorsCallback(
    wptr<IMediaKeySessionServiceOperatorsCallback> callback)
{
    DRM_INFO_LOG("SetMediaKeySessionServiceOperatorsCallback enter.");
    std::lock_guard<std::recursive_mutex> lock(callbackMutex_);
    if (callback.promote() == nullptr) {
        DRM_ERR_LOG("SetMediaKeySessionServiceOperatorsCallback callback is null");
        return DRM_INNER_ERR_INVALID_VAL;
    }
    sessionOperatorsCallback_ = callback;
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySessionService::GenerateMediaKeyRequest(
    IMediaKeySessionService::MediaKeyRequestInfo &licenseRequestInfo,
    IMediaKeySessionService::MediaKeyRequest &licenseRequest)
{
    DrmTrace trace("GenerateMediaKeyRequest");
    DRM_INFO_LOG("GenerateMediaKeyRequest enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    OHOS::HDI::Drm::V1_0::MediaKeyRequestInfo hdiMediaKeyRequestInfo;
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiMediaKeySession_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiMediaKeySession_ is nullptr!");

    hdiMediaKeyRequestInfo.mediaKeyType = (OHOS::HDI::Drm::V1_0::MediaKeyType)licenseRequestInfo.mediaKeyType;
    hdiMediaKeyRequestInfo.mimeType = licenseRequestInfo.mimeType;
    hdiMediaKeyRequestInfo.initData.assign(licenseRequestInfo.initData.begin(), licenseRequestInfo.initData.end());
    for (std::map<std::string, std::string>::iterator it = licenseRequestInfo.optionalData.begin();
        it != licenseRequestInfo.optionalData.end(); ++it) {
        hdiMediaKeyRequestInfo.optionalData.insert(std::make_pair(it->first, it->second));
    }
    OHOS::HDI::Drm::V1_0::MediaKeyRequest hdiMediaKeyRequest;
    mediaKeyType_ = std::to_string(static_cast<int32_t>(licenseRequestInfo.mediaKeyType));
    auto timeBefore = std::chrono::system_clock::now();
    ret = hdiMediaKeySession_->GenerateMediaKeyRequest(hdiMediaKeyRequestInfo, hdiMediaKeyRequest);
    generationDuration_ = CalculateTimeDiff(timeBefore, std::chrono::system_clock::now());
    if (ret != DRM_INNER_ERR_OK) {
        generationResult_ = "failed";
        DRM_ERR_LOG("GenerateMediaKeyRequest failed.");
        ReportFaultEvent(ret, "GenerateMediaKeyRequest failed", "");
        return ret;
    }
    generationResult_ = "success";
    licenseRequest.requestType = (IMediaKeySessionService::RequestType)hdiMediaKeyRequest.requestType;
    licenseRequest.mData.assign(hdiMediaKeyRequest.data.begin(), hdiMediaKeyRequest.data.end());
    licenseRequest.mDefaultURL = hdiMediaKeyRequest.defaultUrl;
    return ret;
}

int32_t MediaKeySessionService::ProcessMediaKeyResponse(std::vector<uint8_t> &licenseId,
    std::vector<uint8_t> &licenseResponse)
{
    DrmTrace trace("ProcessMediaKeyResponse");
    DRM_INFO_LOG("ProcessMediaKeyResponse enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiMediaKeySession_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiMediaKeySession_ is nullptr!");
    auto timeBefore = std::chrono::system_clock::now();
    ret = hdiMediaKeySession_->ProcessMediaKeyResponse(licenseResponse, licenseId);
    uint32_t processDuration = CalculateTimeDiff(timeBefore, std::chrono::system_clock::now());
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("ProcessMediaKeyResponse failed.");
        std::string responseString = std::string(reinterpret_cast<const char*>(licenseResponse.data()),
            licenseResponse.size());
        ReportFaultEvent(ret, "ProcessMediaKeyResponse failed", responseString);
        return ret;
    }
    struct DownLoadInfo downLoadInfo = InitDownLoadInfo(generationDuration_, generationResult_, processDuration,
        "success");
    ReportLicenseBehaviorEvent(statisticsInfo_, mediaKeyType_, downLoadInfo);
    return ret;
}

int32_t MediaKeySessionService::GenerateOfflineReleaseRequest(std::vector<uint8_t> &licenseId,
    std::vector<uint8_t> &releaseRequest)
{
    DRM_INFO_LOG("GenerateOfflineReleaseRequest enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiMediaKeySession_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiMediaKeySession_ is nullptr!");
    ret = hdiMediaKeySession_->GetOfflineReleaseRequest(licenseId, releaseRequest);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GenerateOfflineReleaseRequest failed.");
        return ret;
    }
    return ret;
}

int32_t MediaKeySessionService::ProcessOfflineReleaseResponse(std::vector<uint8_t> &licenseId,
    std::vector<uint8_t> &releaseResponse)
{
    DRM_INFO_LOG("ProcessOfflineReleaseResponse enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiMediaKeySession_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiMediaKeySession_ is nullptr!");

    ret = hdiMediaKeySession_->ProcessOfflineReleaseResponse(licenseId, releaseResponse);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("ProcessOfflineReleaseResponse failed.");
        return ret;
    }
    return ret;
}

int32_t MediaKeySessionService::CheckMediaKeyStatus(std::map<std::string, std::string> &licenseStatus)
{
    DRM_INFO_LOG("CheckMediaKeyStatus enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiMediaKeySession_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiMediaKeySession_ is nullptr!");
    std::map<std::string, std::string> mp;
    ret = hdiMediaKeySession_->CheckMediaKeyStatus(mp);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("CheckMediaKeyStatus failed.");
        return ret;
    }
    for (auto m : mp) {
        std::string name = m.first;
        std::string status = m.second;
        licenseStatus.insert(std::make_pair(name, status));
    }
    if (licenseStatus.size() == 0) {
        DRM_ERR_LOG("CheckMediaKeyStatus licenseStatus is empty.");
        return DRM_INNER_ERR_BASE;
    }
    return ret;
}

int32_t MediaKeySessionService::RestoreOfflineMediaKeys(std::vector<uint8_t> &licenseId)
{
    DRM_INFO_LOG("RestoreOfflineMediaKeys enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiMediaKeySession_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiMediaKeySession_ is nullptr!");
    ret = hdiMediaKeySession_->RestoreOfflineMediaKeys(licenseId);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("RestoreOfflineMediaKeys failed.");
        return ret;
    }
    return ret;
}

int32_t MediaKeySessionService::ClearMediaKeys()
{
    DRM_INFO_LOG("ClearMediaKeys enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiMediaKeySession_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiMediaKeySession_ is nullptr!");
    ret = hdiMediaKeySession_->ClearMediaKeys();
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("ClearMediaKeys failed.");
        return ret;
    }
    return ret;
}

int32_t MediaKeySessionService::GetContentProtectionLevel(
    IMediaKeySessionService::ContentProtectionLevel *securityLevel)
{
    DRM_INFO_LOG("GetContentProtectionLevel enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiMediaKeySession_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiMediaKeySession_ is nullptr!");
    OHOS::HDI::Drm::V1_0::ContentProtectionLevel level;
    ret = hdiMediaKeySession_->GetContentProtectionLevel(level);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GetContentProtectionLevel failed.");
        return ret;
    }
    *securityLevel = (IMediaKeySessionService::ContentProtectionLevel)level;
    return ret;
}

int32_t MediaKeySessionService::GetMediaDecryptModule(sptr<IMediaDecryptModuleService> &decryptModule)
{
    DrmTrace trace("GetMediaDecryptModule");
    DRM_INFO_LOG("GetMediaDecryptModule enter.");
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    if (decryptModule_ != nullptr) {
        DRM_INFO_LOG("decryptModule already exists.");
        decryptModule = decryptModule_;
        return DRM_INNER_ERR_OK;
    }
    sptr<OHOS::HDI::Drm::V1_0::IMediaDecryptModule> hdiDecryptModule = nullptr;
    if (hdiMediaKeySession_ == nullptr) {
        DRM_ERR_LOG("hdiMediaKeySession_ == nullptr");
        return DRM_INNER_ERR_INVALID_KEY_SESSION;
    } else {
        int32_t retCode = DRM_INNER_ERR_OK;
        retCode = hdiMediaKeySession_->GetMediaDecryptModule(hdiDecryptModule);
        if (retCode != DRM_INNER_ERR_OK || hdiDecryptModule == nullptr) {
            DRM_ERR_LOG("hdiDecryptModule allocation failed.");
            return DRM_INNER_ERR_BASE;
        }
        decryptModule_ = new (std::nothrow) MediaDecryptModuleService(hdiDecryptModule, statisticsInfo_);
        if (decryptModule_ == nullptr) {
            DRM_ERR_LOG("New MediaDecryptModuleService allocation failed.");
            return DRM_INNER_ERR_NO_MEMORY;
        }
        decryptModule = decryptModule_;
        return DRM_INNER_ERR_OK;
    }
}

int32_t MediaKeySessionService::RequireSecureDecoderModule(std::string &mimeType, bool *status)
{
    DRM_INFO_LOG("RequireSecureDecoderModule enter.");

    int32_t ret = DRM_INNER_ERR_OK;
    (void)mimeType;
    (void)status;
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiMediaKeySession_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiMediaKeySession_ is nullptr!");
    bool decoderModuleStatus = false;
    ret = hdiMediaKeySession_->RequiresSecureDecoderModule(mimeType, decoderModuleStatus);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("RequireSecureDecoderModule failed.");
        return ret;
    }
    *status = decoderModuleStatus;
    return ret;
}

int32_t MediaKeySessionService::SetCallback(sptr<IMediaKeySessionServiceCallback> &callback)
{
    DRM_INFO_LOG("SetCallback enter.");
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    if (callback == nullptr) {
        DRM_ERR_LOG("SetCallback nullptr , failed.");
        return DRM_INNER_ERR_BASE;
    }
    callback_ = callback;
    if (hdiMediaKeySession_ != nullptr) {
        return hdiMediaKeySession_->SetCallback(this);
    }
    DRM_ERR_LOG("SetCallback hdiMediaKeySession_ is nullptr , failed.");
    return DRM_INNER_ERR_OPERATION_NOT_PERMITTED;
}

int32_t MediaKeySessionService::SendEvent(OHOS::HDI::Drm::V1_0::EventType eventType, int32_t extra,
    const std::vector<uint8_t> &data)
{
    DRM_INFO_LOG("SendEvent.");
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    DrmEventType event = static_cast<DrmEventType>(eventType);
    if (callback_ != nullptr) {
        return callback_->SendEvent(event, extra, data);
    }
    return DRM_INNER_ERR_OPERATION_NOT_PERMITTED;
}

int32_t MediaKeySessionService::SendEventKeyChange(
    const std::map<std::vector<uint8_t>, OHOS::HDI::Drm::V1_0::MediaKeySessionKeyStatus> &keyStatus,
    bool hasNewGoodLicense)
{
    DRM_INFO_LOG("SendEventKeyChange.");
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    std::map<std::vector<uint8_t>, MediaKeySessionKeyStatus> keyStatusMap;
    for (auto item : keyStatus) {
        keyStatusMap.insert({ item.first, static_cast<MediaKeySessionKeyStatus>(item.second) });
    }
    if (callback_ != nullptr) {
        return callback_->SendEventKeyChanged(keyStatusMap, hasNewGoodLicense);
    }
    return DRM_INNER_ERR_OPERATION_NOT_PERMITTED;
}

std::string MediaKeySessionService::GetDecryptModuleDumpInfo()
{
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    if (decryptModule_ != nullptr) {
        return decryptModule_->GetDumpInfo();
    }
    return "";
}
} // DrmStandard
} // OHOS