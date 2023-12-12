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
#include "drm_log.h"
#include "ipc_skeleton.h"

namespace OHOS {
namespace DrmStandard {
static std::mutex sessionMutex_;

MediaKeySessionService::MediaKeySessionService(sptr<OHOS::HDI::Drm::V1_0::IMediaKeySession> hdiMediaKeySession)
{
    DRM_DEBUG_LOG("MediaKeySessionService::MediaKeySessionService.");
    sessionOperatorsCallback_ = nullptr;
    hdiMediaKeySession_ = hdiMediaKeySession;
}

MediaKeySessionService::~MediaKeySessionService()
{
    DRM_INFO_LOG("MediaKeySessionService::~MediaKeySessionService enter.");
    std::lock_guard<std::mutex> lock(sessionMutex_);
    if (sessionOperatorsCallback_ != nullptr) {
        sessionOperatorsCallback_ = nullptr;
    }
    if (hdiMediaKeySession_ != nullptr) {
        DRM_ERR_LOG("hdiMediaKeySession_ != nullptr");
    }
    DRM_INFO_LOG("MediaKeySessionService::~MediaKeySessionService exit.");
}

int32_t MediaKeySessionService::CloseMediaKeySessionServiceByCallback()
{
    DRM_INFO_LOG("MediaKeySessionService::CloseMediaKeySessionServiceByCallback enter.");
    int32_t currentPid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySessionService GetCallingPID: %{public}d", currentPid);
    if (hdiMediaKeySession_ != nullptr) {
        DRM_INFO_LOG("hdiMediaKeySession_ CloseHdiMediaKeySession");
        hdiMediaKeySession_->Destroy();
        hdiMediaKeySession_ = nullptr;
    }
    if (sessionOperatorsCallback_ != nullptr) {
        sessionOperatorsCallback_ = nullptr;
    }
    DRM_INFO_LOG("MediaKeySessionService::CloseMediaKeySessionServiceByCallback exit.");
    return DRM_OK;
}

int32_t MediaKeySessionService::Release()
{
    DRM_INFO_LOG("MediaKeySessionService::Release enter.");
    int32_t currentPid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySessionService GetCallingPID: %{public}d", currentPid);
    if (sessionOperatorsCallback_ != nullptr) {
        sessionOperatorsCallback_->CloseMediaKeySessionService(this);
    }
    DRM_INFO_LOG("MediaKeySessionService::Release exit.");
    return DRM_OK;
}

int32_t MediaKeySessionService::SetMediaKeySessionServiceOperatorsCallback(
    wptr<IMediaKeySessionServiceOperatorsCallback> callback)
{
    DRM_INFO_LOG("MediaKeySessionService::SetMediaKeySessionServiceOperatorsCallback enter.");
    if (callback.promote() == nullptr) {
        DRM_ERR_LOG("SetMediaKeySessionServiceOperatorsCallback callback is null");
        return DRM_INVALID_ARG;
    }
    sessionOperatorsCallback_ = callback;
    DRM_INFO_LOG("MediaKeySessionService::SetMediaKeySessionServiceOperatorsCallback exit.");
    return DRM_OK;
}

int32_t MediaKeySessionService::GenerateLicenseRequest(IMediaKeySessionService::LicenseRequestInfo &licenseRequestInfo,
    IMediaKeySessionService::LicenseRequest &licenseRequest)
{
    DRM_INFO_LOG("MediaKeySessionService::GenerateLicenseRequest enter.");
    int32_t ret = DRM_OK;
    OHOS::HDI::Drm::V1_0::LicenseRequestInfo hdiLicenseRequestInfo;

    hdiLicenseRequestInfo.licenseType = (OHOS::HDI::Drm::V1_0::LicenseType)licenseRequestInfo.licenseType;
    hdiLicenseRequestInfo.mimeType = licenseRequestInfo.mimeType;
    hdiLicenseRequestInfo.initData.assign(licenseRequestInfo.initData.begin(), licenseRequestInfo.initData.end());
    for (std::map<std::string, std::string>::iterator it = licenseRequestInfo.optionalData.begin(); it !=
	    licenseRequestInfo.optionalData.end(); ++it) {
        hdiLicenseRequestInfo.optionalData.insert(std::make_pair(it->first, it->second));
    }
    OHOS::HDI::Drm::V1_0::LicenseRequest hdiLicenseRequest;
    ret = hdiMediaKeySession_->GenerateLicenseRequest(hdiLicenseRequestInfo, hdiLicenseRequest);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySessionService::GenerateLicenseRequest failed");
        return ret;
    }
    licenseRequest.requestType = (IMediaKeySessionService::RequestType)hdiLicenseRequest.requestType;
    licenseRequest.mData.assign(hdiLicenseRequest.mData.begin(), hdiLicenseRequest.mData.end());
    licenseRequest.mDefaultURL = hdiLicenseRequest.mDefaultUrl;
    DRM_INFO_LOG("MediaKeySessionService::GenerateLicenseRequest exit.");
    return ret;
}

int32_t MediaKeySessionService::ProcessLicenseResponse(std::vector<uint8_t> &licenseId,
    std::vector<uint8_t> &licenseResponse)
{
    DRM_INFO_LOG("MediaKeySessionService::ProcessLicenseResponse enter.");
    int32_t ret = DRM_OK;
    ret = hdiMediaKeySession_->ProcessLicenseResponse(licenseResponse, licenseId);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySessionService::ProcessLicenseResponse failed");
        return ret;
    }
    DRM_INFO_LOG("bMediaKeySessionService::ProcessLicenseResponse exit.");
    return ret;
}

int32_t MediaKeySessionService::GenerateOfflineReleaseRequest(std::vector<uint8_t> &licenseId,
    std::vector<uint8_t> &releaseRequest)
{
    DRM_INFO_LOG("MediaKeySessionService::GenerateOfflineReleaseRequest enter.");
    int32_t ret = DRM_OK;
    ret = hdiMediaKeySession_->GetOfflineReleaseRequest(licenseId, releaseRequest);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySessionService::GenerateOfflineReleaseRequest failed");
        return ret;
    }
    DRM_INFO_LOG("MediaKeySessionService::GenerateOfflineReleaseRequest exit.");
    return ret;
}

int32_t MediaKeySessionService::ProcessOfflineReleaseResponse(std::vector<uint8_t> &licenseId,
    std::vector<uint8_t> &releaseResponse)
{
    DRM_INFO_LOG("MediaKeySessionService::ProcessOfflineReleaseResponse enter.");
    int32_t ret = DRM_OK;

    ret = hdiMediaKeySession_->ProcessOfflineReleaseResponse(licenseId, releaseResponse);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySessionService::ProcessOfflineReleaseResponse failed");
        return ret;
    }
    DRM_INFO_LOG("MediaKeySessionService::ProcessOfflineReleaseResponse exit.");
    return ret;
}

int32_t MediaKeySessionService::CheckLicenseStatus(std::map<std::string, MediaKeySessionKeyStatus> &licenseStatus)
{
    DRM_INFO_LOG("MediaKeySessionService::CheckLicenseStatus enter.");
    int32_t ret = DRM_OK;
    std::map<std::string, OHOS::HDI::Drm::V1_0::MediaKeySessionKeyStatus> mp;
    ret = hdiMediaKeySession_->CheckLicenseStatus(mp);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySessionService::CheckLicenseStatus failed");
        return ret;
    }
    for (auto m : mp) {
        std::string name = m.first;
        MediaKeySessionKeyStatus status = (MediaKeySessionKeyStatus)m.second;
        licenseStatus.insert(std::make_pair(name, status));
    }
    if (licenseStatus.size() == 0) {
        DRM_ERR_LOG("MediaKeySessionService::CheckLicenseStatus licenseStatus is empty.");
        return DRM_ERROR;
    }
    DRM_INFO_LOG("MediaKeySessionService::CheckLicenseStatus exit.");
    return ret;
}

int32_t MediaKeySessionService::RestoreOfflineLicense(std::vector<uint8_t> &licenseId)
{
    DRM_INFO_LOG("MediaKeySessionService::RestoreOfflineLicense enter.");
    int32_t ret = DRM_OK;
    ret = hdiMediaKeySession_->RestoreOfflineLicense(licenseId);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySessionService::RestoreOfflineLicense failed");
        return ret;
    }
    DRM_INFO_LOG("MediaKeySessionService::RestoreOfflineLicense exit.");
    return ret;
}

int32_t MediaKeySessionService::RemoveLicense()
{
    DRM_INFO_LOG("MediaKeySessionService::RemoveLicense enter.");
    int32_t ret = DRM_OK;
    ret = hdiMediaKeySession_->RemoveLicense();
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySessionService::RemoveLicense failed");
        return ret;
    }
    DRM_INFO_LOG("MediaKeySessionService::RemoveLicense exit.");
    return ret;
}

int32_t MediaKeySessionService::GetSecurityLevel(IMediaKeySessionService::SecurityLevel *securityLevel)
{
    DRM_INFO_LOG("MediaKeySessionService::GetSecurityLevel enter.");
    int32_t ret = DRM_OK;

    OHOS::HDI::Drm::V1_0::SecurityLevel level;
    ret = hdiMediaKeySession_->GetSecurityLevel(level);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySessionService::GetSecurityLevel failed");
        return ret;
    }
    *securityLevel = (IMediaKeySessionService::SecurityLevel)level;

    DRM_INFO_LOG("MediaKeySessionService::GetSecurityLevel exit.");
    return ret;
}

int32_t MediaKeySessionService::CreateMediaDecryptModule(sptr<IMediaDecryptModuleService> &decryptModule)
{
    DRM_INFO_LOG("MediaKeySessionService::CreateMediaDecryptModule enter.");
    std::lock_guard<std::mutex> lock(sessionMutex_);
    sptr<MediaDecryptModuleService> mediaDecryptService = nullptr;
    sptr<OHOS::HDI::Drm::V1_0::IMediaDecryptModule> hdiDecryptModule = nullptr;
    if (hdiMediaKeySession_ == nullptr) {
        DRM_ERR_LOG("MediaKeySessionService:: hdiMediaKeySession_ == nullptr");
        return DRM_SERVICE_ERROR;
    } else {
        int32_t retCode = DRM_OK;
        retCode = hdiMediaKeySession_->GetMediaDecryptModule(hdiDecryptModule);
        if (retCode != DRM_OK || hdiDecryptModule == nullptr) {
            DRM_ERR_LOG("MediaKeySessionService:: hdiDecryptModule allocation failed");
            return DRM_SERVICE_ERROR;
        }
        mediaDecryptService = new (std::nothrow) MediaDecryptModuleService(hdiDecryptModule);
        if (mediaDecryptService == nullptr) {
            DRM_ERR_LOG("MediaKeySessionService:: new MediaDecryptModuleService allocation failed");
            return DRM_ALLOC_ERROR;
        }
        decryptModule = mediaDecryptService;
        DRM_INFO_LOG("MediaKeySessionService::CreateMediaDecryptModule enter.");
        return DRM_OK;
    }
}

int32_t MediaKeySessionService::RequireSecureDecoderModule(std::string &mimeType, bool *status)
{
    DRM_INFO_LOG("MediaKeySessionService::RequireSecureDecoderModule enter.");

    int32_t ret = DRM_OK;
    (void)mimeType;
    (void)status;

    bool decoderModuleStatus = false;
    ret = hdiMediaKeySession_->RequiresSecureDecoderModule(mimeType, decoderModuleStatus);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySessionService::RequireSecureDecoderModule failed");
        return ret;
    }
    *status = decoderModuleStatus;

    DRM_INFO_LOG("MediaKeySessionService::RequireSecureDecoderModule exit.");
    return ret;
}

int32_t MediaKeySessionService::SetCallback(sptr<IMediaKeySessionServiceCallback> &callback)
{
    DRM_INFO_LOG("MediaKeySessionService::SetCallback enter.");
    std::lock_guard<std::mutex> lock(sessionMutex_);
    if (callback == nullptr) {
        DRM_ERR_LOG("MediaKeySessionService::SetCallback nullptr , failed.");
        return DRM_ERROR;
    }
    callback_ = callback;
    if (hdiMediaKeySession_ != nullptr) {
        return hdiMediaKeySession_->SetCallback(this);
    }
    DRM_ERR_LOG("MediaKeySessionService::SetCallback hdiMediaKeySession_ is nullptr , failed.");
    return DRM_OPERATION_NOT_ALLOWED;
}

int32_t MediaKeySessionService::SendEvent(OHOS::HDI::Drm::V1_0::EventType eventType, int32_t extra,
    const std::vector<uint8_t> &data)
{
    DRM_INFO_LOG("MediaKeySessionService:: SendEvent.");
    DrmEventType event = static_cast<DrmEventType>(eventType);
    if (callback_ != nullptr) {
        return callback_->SendEvent(event, extra, data);
    }
    DRM_INFO_LOG("MediaKeySystemService:: SendEvent failed because callback is nullptr");
    return DRM_OPERATION_NOT_ALLOWED;
}

int32_t MediaKeySessionService::SendEventKeyChange(
    const std::map<std::vector<uint8_t>, OHOS::HDI::Drm::V1_0::MediaKeySessionKeyStatus> &keyStatus,
    bool hasNewGoodLicense)
{
    DRM_INFO_LOG("MediaKeySessionService:: SendEventKeyChange.");
    std::map<std::vector<uint8_t>, MediaKeySessionKeyStatus> keyStatusMap;
    for (auto item : keyStatus) {
        keyStatusMap.insert({ item.first, static_cast<MediaKeySessionKeyStatus>(item.second) });
    }
    if (callback_ != nullptr) {
        return callback_->SendEventKeyChanged(keyStatusMap, hasNewGoodLicense);
    }
    DRM_INFO_LOG("MediaKeySessionService:: SendEvent failed because callback is nullptr");
    return DRM_OPERATION_NOT_ALLOWED;
}
} // DrmStandard
} // OHOS