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
    statisticsInfo_.targetVersion = 0;
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
}

int32_t MediaKeySessionService::CloseMediaKeySessionServiceByCallback()
{
    DRM_INFO_LOG("CloseMediaKeySessionServiceByCallback enter.");
    {
        std::lock_guard<std::recursive_mutex> lock(callbackMutex_);
        sessionOperatorsCallback_ = nullptr;
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
    auto var = sessionOperatorsCallback_.promote();
    if (var != nullptr) {
        var->CloseMediaKeySessionService(this);
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
    const MediaKeyRequestInfo &licenseRequestInfo, MediaKeyRequest &licenseRequest)
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
    for (auto it = licenseRequestInfo.optionalData.begin();
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
    licenseRequest.requestType = (RequestType)hdiMediaKeyRequest.requestType;
    licenseRequest.mData.assign(hdiMediaKeyRequest.data.begin(), hdiMediaKeyRequest.data.end());
    licenseRequest.mDefaultURL = hdiMediaKeyRequest.defaultUrl;
    return ret;
}

int32_t MediaKeySessionService::ProcessMediaKeyResponse(std::vector<uint8_t> &licenseId,
    const std::vector<uint8_t> &licenseResponse)
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

int32_t MediaKeySessionService::GenerateOfflineReleaseRequest(const std::vector<uint8_t> &licenseId,
    std::vector<uint8_t> &releaseRequest)
{
    DRM_INFO_LOG("GenerateOfflineReleaseRequest enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiMediaKeySession_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiMediaKeySession_ is nullptr!");
    ret = hdiMediaKeySession_->GetOfflineReleaseRequest(licenseId, releaseRequest);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK, ret, "GenerateOfflineReleaseRequest failed.");
    return ret;
}

int32_t MediaKeySessionService::ProcessOfflineReleaseResponse(const std::vector<uint8_t> &licenseId,
    const std::vector<uint8_t> &releaseResponse)
{
    DRM_INFO_LOG("ProcessOfflineReleaseResponse enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiMediaKeySession_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiMediaKeySession_ is nullptr!");

    ret = hdiMediaKeySession_->ProcessOfflineReleaseResponse(licenseId, releaseResponse);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK, ret, "ProcessOfflineReleaseResponse failed.");
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
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK, ret, "CheckMediaKeyStatus failed.");
    for (auto m : mp) {
        std::string name = m.first;
        std::string status = m.second;
        licenseStatus.insert(std::make_pair(name, status));
    }
    DRM_CHECK_AND_RETURN_RET_LOG(
        licenseStatus.size() != 0, DRM_INNER_ERR_BASE, "CheckMediaKeyStatus licenseStatus is empty.");
    return ret;
}

int32_t MediaKeySessionService::RestoreOfflineMediaKeys(const std::vector<uint8_t> &licenseId)
{
    DRM_INFO_LOG("RestoreOfflineMediaKeys enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiMediaKeySession_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiMediaKeySession_ is nullptr!");
    ret = hdiMediaKeySession_->RestoreOfflineMediaKeys(licenseId);
    DRM_CHECK_AND_RETURN_RET_LOG(
        ret == DRM_INNER_ERR_OK, ret, "RestoreOfflineMediaKeys failed.");
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
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK, ret, "ClearMediaKeys failed.");
    return ret;
}

int32_t MediaKeySessionService::GetContentProtectionLevel(ContentProtectionLevel &securityLevel)
{
    DRM_INFO_LOG("GetContentProtectionLevel enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiMediaKeySession_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiMediaKeySession_ is nullptr!");
    OHOS::HDI::Drm::V1_0::ContentProtectionLevel level;
    ret = hdiMediaKeySession_->GetContentProtectionLevel(level);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK, ret, "GetContentProtectionLevel failed.");
    securityLevel = (ContentProtectionLevel)level;
    return ret;
}

int32_t MediaKeySessionService::GetMediaDecryptModule(sptr<IMediaDecryptModuleService> &decryptModule)
{
    DrmTrace trace("GetMediaDecryptModule");
    DRM_INFO_LOG("GetMediaDecryptModule enter.");
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    decryptModule = decryptModule_;
    DRM_CHECK_AND_RETURN_RET_LOG(decryptModule_ == nullptr, DRM_INNER_ERR_OK, "decryptModule already exists.");
    sptr<OHOS::HDI::Drm::V1_0::IMediaDecryptModule> hdiDecryptModule = nullptr;
    DRM_CHECK_AND_RETURN_RET_LOG(
        hdiMediaKeySession_ != nullptr, DRM_INNER_ERR_INVALID_KEY_SESSION, "hdiMediaKeySession_ == nullptr");
    int32_t retCode = DRM_INNER_ERR_OK;
    retCode = hdiMediaKeySession_->GetMediaDecryptModule(hdiDecryptModule);
    DRM_CHECK_AND_RETURN_RET_LOG(retCode == DRM_INNER_ERR_OK && hdiDecryptModule != nullptr,
        DRM_INNER_ERR_BASE,
        "hdiDecryptModule allocation failed.");
    decryptModule_ = new (std::nothrow) MediaDecryptModuleService(hdiDecryptModule, statisticsInfo_);
    DRM_CHECK_AND_RETURN_RET_LOG(
        decryptModule_ != nullptr, DRM_INNER_ERR_NO_MEMORY, "New MediaDecryptModuleService allocation failed.");
    decryptModule = decryptModule_;
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySessionService::RequireSecureDecoderModule(const std::string &mimeType, bool &status)
{
    DRM_INFO_LOG("RequireSecureDecoderModule enter.");

    int32_t ret = DRM_INNER_ERR_OK;
    (void)mimeType;
    (void)status;
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiMediaKeySession_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiMediaKeySession_ is nullptr!");
    ret = hdiMediaKeySession_->RequiresSecureDecoderModule(mimeType, status);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK, ret, "RequireSecureDecoderModule failed.");
    return ret;
}

int32_t MediaKeySessionService::SetCallback(const sptr<IMediaKeySessionServiceCallback> &callback)
{
    DRM_INFO_LOG("SetCallback enter.");
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(callback != nullptr, DRM_INNER_ERR_BASE, "SetCallback nullptr failed.");
    callback_ = callback;
    DRM_CHECK_AND_RETURN_RET_LOG(hdiMediaKeySession_ != nullptr,
        DRM_INNER_ERR_OPERATION_NOT_PERMITTED,
        "SetCallback hdiMediaKeySession_ is nullptr , failed.");
    return hdiMediaKeySession_->SetCallback(this);
}

int32_t MediaKeySessionService::SendEvent(OHOS::HDI::Drm::V1_0::EventType eventType, int32_t extra,
    const std::vector<uint8_t> &data)
{
    DRM_INFO_LOG("SendEvent.");
    DrmEventType event = static_cast<DrmEventType>(eventType);
    DRM_CHECK_AND_RETURN_RET_LOG(
        callback_ != nullptr, DRM_INNER_ERR_OPERATION_NOT_PERMITTED, "SendEvent callback not set");
    return callback_->SendEvent(event, extra, data);
}

int32_t MediaKeySessionService::SendEventKeyChange(
    const std::map<std::vector<uint8_t>, OHOS::HDI::Drm::V1_0::MediaKeySessionKeyStatus> &keyStatus,
    bool hasNewGoodLicense)
{
    DRM_INFO_LOG("SendEventKeyChange.");
    std::map<std::vector<uint8_t>, MediaKeySessionKeyStatus> keyStatusMap;
    for (auto item : keyStatus) {
        keyStatusMap.insert({ item.first, static_cast<MediaKeySessionKeyStatus>(item.second) });
    }
    DRM_CHECK_AND_RETURN_RET_LOG(
        callback_ != nullptr, DRM_INNER_ERR_OPERATION_NOT_PERMITTED, "SendEventKeyChange callback not set");
    return callback_->SendEventKeyChanged(keyStatusMap, hasNewGoodLicense);
}

std::string MediaKeySessionService::GetDecryptModuleDumpInfo()
{
    std::lock_guard<std::recursive_mutex> lock(sessionMutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(
        decryptModule_ != nullptr, "", "get decrypt dump info: decrypt module is null");
    return decryptModule_->GetDumpInfo();
}
} // DrmStandard
} // OHOS
