/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

#include <unordered_set>
#include <securec.h>
#include "system_ability_definition.h"
#include "ipc_skeleton.h"
#include "iservice_registry.h"
#include "access_token.h"
#include "drm_log.h"
#include "drm_napi_utils.h"
#include "key_session_service.h"
#include "mediakeysystem_service.h"

namespace OHOS {
namespace DrmStandard {
MediaKeySystemService::MediaKeySystemService(sptr<OHOS::HDI::Drm::V1_0::IMediaKeySystem> hdiKeySystem)
{
    DRM_DEBUG_LOG("~MediaKeySystemService");
    keySystemOperatoersCallback_ = nullptr;
    hdiKeySystem_ = hdiKeySystem;
}

MediaKeySystemService::~MediaKeySystemService()
{
    DRM_DEBUG_LOG("~MediaKeySystemService");
    std::lock_guard<std::mutex> lock(mutex_);
    if (keySystemOperatoersCallback_ != nullptr) {
        keySystemOperatoersCallback_ = nullptr;
    }
    if (hdiKeySystem_ != nullptr) {
        DRM_ERR_LOG("hdiKeySystem != nullptr");
    }
}

int32_t MediaKeySystemService::CloseMediaKeySystemServiceByCallback()
{
    DRM_INFO_LOG("MediaKeySystemService::CloseMediaKeySystemServiceByCallback enter.");
    int32_t currentPid = IPCSkeleton::GetCallingPid();
    DRM_ERR_LOG("MediaKeySystemService GetCallingPID: %{public}d", currentPid);
    if (hdiKeySystem_ != nullptr) {
        DRM_ERR_LOG("hdiKeySystem_ CloseHdiMediaKeySession");
        hdiKeySystem_->Destroy();
        hdiKeySystem_ = nullptr;
    }
    if (keySystemOperatoersCallback_ != nullptr) {
        keySystemOperatoersCallback_ = nullptr;
    }
    DRM_INFO_LOG("MediaKeySystemService::CloseMediaKeySystemServiceByCallback exit.");
    return DRM_OK;
}

int32_t MediaKeySystemService::Release()
{
    DRM_INFO_LOG("MediaKeySystemService::Release enter.");
    int32_t currentPid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySystemService GetCallingPID: %{public}d", currentPid);
    if (keySystemOperatoersCallback_ != nullptr) {
        keySystemOperatoersCallback_->CloseMediaKeySystemService(this);
    }
    DRM_INFO_LOG("MediaKeySystemService::Release exit.");
    return DRM_OK;
}

int32_t MediaKeySystemService::SetMediaKeySystemServiceOperatorsCallback(
    wptr<IMediaKeySystemServiceOperatorsCallback> callback)
{
    if (callback.promote() == nullptr) {
        DRM_ERR_LOG("SetMediaKeySystemServiceOperatorsCallback callback is null");
        return DRM_INVALID_ARG;
    }
    keySystemOperatoersCallback_ = callback;
    return DRM_OK;
}

int32_t MediaKeySystemService::GenerateKeySystemRequest(std::vector<uint8_t> &request, std::string &defaultUrl)
{
    int32_t ret = DRM_OK;
    ret = hdiKeySystem_->GenerateKeySystemRequest(defaultUrl, request);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::GenerateKeySystemRequest failed");
        return ret;
    }
    DRM_INFO_LOG("MediaKeySystemService::GenerateKeySystemRequest exit.");
    return ret;
}

int32_t MediaKeySystemService::ProcessKeySystemResponse(const std::vector<uint8_t> &response)
{
    int32_t ret = DRM_OK;
    ret = hdiKeySystem_->ProcessKeySystemResponse(response);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::ProcessKeySystemResponse failed");
        return ret;
    }
    DRM_INFO_LOG("MediaKeySystemService::ProcessKeySystemResponse exit.");
    return ret;
}

int32_t MediaKeySystemService::SetConfigurationString(std::string &configName, std::string &value)
{
    DRM_INFO_LOG("MediaKeySystemService::SetConfiguration enter, configName:%{public}s, value:%{public}s.",
        configName.c_str(), value.c_str());
    int32_t ret = DRM_OK;

    ret = hdiKeySystem_->SetConfigurationString(configName, value);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::SetConfiguration failed");
        return ret;
    }
    DRM_INFO_LOG("MediaKeySystemService::SetConfiguration exit.");
    return ret;
}

int32_t MediaKeySystemService::GetConfigurationString(std::string &configName, std::string &value)
{
    DRM_INFO_LOG("MediaKeySystemService::GetConfiguration enter, configName:%{public}s.", configName.c_str());
    int32_t ret = DRM_OK;

    ret = hdiKeySystem_->GetConfigurationString(configName, value);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::GetConfiguration failed");
        return ret;
    }
    DRM_INFO_LOG("MediaKeySystemService::GetConfiguration exit, value:%{public}s.", value.c_str());
    return ret;
}

int32_t MediaKeySystemService::SetConfigurationByteArray(std::string &configName, std::vector<uint8_t> &value)
{
    DRM_INFO_LOG("MediaKeySystemService::SetConfiguration enter, configName:%{public}s.", configName.c_str());
    int32_t ret = DRM_OK;
    std::vector<uint8_t> valueVec;
    valueVec.assign(value.begin(), value.end());
    ret = hdiKeySystem_->SetConfigurationByteArray(configName, valueVec);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::SetConfiguration failed");
        return ret;
    }
    DRM_INFO_LOG("MediaKeySystemService::SetConfiguration exit.");
    return ret;
}

int32_t MediaKeySystemService::GetConfigurationByteArray(std::string &configName, std::vector<uint8_t> &value)
{
    DRM_INFO_LOG("MediaKeySystemService::GetConfiguration enter, configName:%{public}s.", configName.c_str());
    int32_t ret = DRM_OK;
    std::vector<uint8_t> valueVec;
    ret = hdiKeySystem_->GetConfigurationByteArray(configName, valueVec);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::GetConfiguration failed");
        return ret;
    }
    value.assign(valueVec.begin(), valueVec.end());
    return ret;
}

int32_t MediaKeySystemService::CreateMediaKeySession(IMediaKeySessionService::SecurityLevel securityLevel,
    sptr<IMediaKeySessionService> &keySessionProxy)
{
    DRM_INFO_LOG("MediaKeySystemService::CreateMediaKeySession enter, securityLevel:%{public}d.", securityLevel);
    int32_t ret = DRM_OK;
    std::lock_guard<std::mutex> lock(mutex_);
    sptr<MediaKeySessionService> keySessionService = nullptr;
    sptr<OHOS::HDI::Drm::V1_0::IMediaKeySession> hdiMediaKeySession = nullptr;
    ret = hdiKeySystem_->CreateMediaKeySession((OHOS::HDI::Drm::V1_0::SecurityLevel)securityLevel, hdiMediaKeySession);
    if (hdiMediaKeySession == nullptr) {
        DRM_ERR_LOG("MediaKeySystemService:: drmHostManager_ return hdiMediaKeySession nullptr");
        return DRM_SERVICE_ERROR;
    }
    keySessionService = new (std::nothrow) MediaKeySessionService(hdiMediaKeySession);
    if (keySessionService == nullptr) {
        DRM_ERR_LOG("MediaKeySystemService::CreateMediaKeySession allocation failed");
        return DRM_ALLOC_ERROR;
    }
    keySessionService->SetMediaKeySessionServiceOperatorsCallback(this);

    int32_t pid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySystemService CreateMediaKeySession GetCallingPID: %{public}d", pid);
    sessionsForPid_[pid].insert(keySessionService);
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " is Current keySessionService", FAKE_POINTER(keySessionService.GetRefPtr()));
    keySessionProxy = keySessionService;
    DRM_INFO_LOG("MediaKeySystemService::CreateMediaKeySession exit.");
    return ret;
}

int32_t MediaKeySystemService::CloseMediaKeySessionService(sptr<MediaKeySessionService> sessionService)
{
    DRM_INFO_LOG("MediaKeySystemService::CloseMediaKeySessionService enter.");
    int32_t ret = DRM_OK;
    int32_t currentPid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySystemService GetCallingPID: %{public}d", currentPid);

    int32_t pid = currentPid;
    if (sessionService != nullptr) {
        DRM_ERR_LOG("MediaKeySystemService call CloseMediaKeySessionServiceByCallback ");
        ret = sessionService->CloseMediaKeySessionServiceByCallback();
    }

    DRM_DEBUG_LOG("MediaKeySystemService sessionsForPid_ size:%{public}u", sessionsForPid_[pid].size());
    if (sessionsForPid_[pid].find(sessionService) != sessionsForPid_[pid].end()) {
        DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " will be deleted", FAKE_POINTER(sessionService.GetRefPtr()));
        DRM_DEBUG_LOG("MediaKeySystemService before sessionSet size:%{public}u", sessionsForPid_[pid].size());
        sessionsForPid_[pid].erase(sessionService);
        DRM_DEBUG_LOG("MediaKeySystemService after sessionSet size:%{public}u", sessionsForPid_[pid].size());
    } else {
        DRM_DEBUG_LOG("MediaKeySystemService not find sessions for PID:%{public}d", pid);
    }
    DRM_DEBUG_LOG("MediaKeySystemService sessionsForPid_ size:%{public}u", sessionsForPid_[pid].size());

    sessionService = nullptr;
    DRM_INFO_LOG("MediaKeySystemService::CloseMediaKeySessionService exit.");
    return ret;
}

int32_t MediaKeySystemService::GetMetrics(std::vector<IMediaKeySystemService::MetircKeyValue> &metrics)
{
    DRM_INFO_LOG("MediaKeySystemService::GetMetrics enter");
    int32_t ret = DRM_OK;
    std::map<std::string, std::string> tmpMetrics;
    ret = hdiKeySystem_->GetMetrics(tmpMetrics);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::GetMetrics failed");
        return ret;
    }
    for (auto it = tmpMetrics.begin(); it != tmpMetrics.end(); it++) {
        IMediaKeySystemService::MetircKeyValue keyValue;
        keyValue.name = it->first;
        keyValue.value = it->second;
        metrics.push_back(keyValue);
    }
    if (metrics.size() == 0) {
        DRM_ERR_LOG("MediaKeySystemService::GetMetrics failed");
        return DRM_ERROR;
    }
    DRM_INFO_LOG("MediaKeySystemService::GetMetrics exit");
    return ret;
}

int32_t MediaKeySystemService::GetMaxSecurityLevel(IMediaKeySessionService::SecurityLevel *securityLevel)
{
    DRM_INFO_LOG("MediaKeySystemService::GetMaxSecurityLevel enter.");
    int32_t ret = DRM_OK;
    OHOS::HDI::Drm::V1_0::SecurityLevel level;

    ret = hdiKeySystem_->GetMaxSecurityLevel(level);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::GetMaxSecurityLevel failed");
        return ret;
    }
    *securityLevel = (IMediaKeySessionService::SecurityLevel)level;
    DRM_INFO_LOG("MediaKeySystemService::GetMaxSecurityLevel exit.");
    return ret;
}

int32_t MediaKeySystemService::GetCertificateStatus(IMediaKeySystemService::CertificateStatus *certStatus)
{
    DRM_INFO_LOG("MediaKeySystemService::GetCertificateStatus enter.");
    int32_t ret = DRM_OK;

    OHOS::HDI::Drm::V1_0::CertificateStatus tmpStatus;
    ret = hdiKeySystem_->GetOemCertificateStatus(tmpStatus);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::GetCertificateStatus failed");
        return ret;
    }
    *certStatus = (IMediaKeySystemService::CertificateStatus)tmpStatus;

    DRM_INFO_LOG("MediaKeySystemService::GetCertificateStatus exit.");
    return ret;
}

int32_t MediaKeySystemService::GetOfflineLicenseIds(std::vector<std::vector<uint8_t>> &licenseIds)
{
    DRM_INFO_LOG("MediaKeySystemService::GetOfflineLicenseIds enter.");
    int32_t ret = DRM_OK;

    ret = hdiKeySystem_->GetOfflineLicenseIds(licenseIds);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::GetOfflineLicenseIds failed");
        return ret;
    }

    DRM_INFO_LOG("MediaKeySystemService::GetOfflineLicenseIds exit.");
    return ret;
}

int32_t MediaKeySystemService::GetOfflineLicenseStatus(std::vector<uint8_t> &licenseId,
    IMediaKeySessionService::OfflineLicenseStatus &status)
{
    DRM_INFO_LOG("MediaKeySystemService::GetOfflineLicenseStatus enter.");
    int32_t ret = DRM_OK;

    OHOS::HDI::Drm::V1_0::OfflineLicenseStatus offlineLicenseStatus;
    ret = hdiKeySystem_->GetOfflineLicenseStatus(licenseId, offlineLicenseStatus);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::GetOfflineLicenseStatus failed");
        return ret;
    }
    status = (IMediaKeySessionService::OfflineLicenseStatus)offlineLicenseStatus;

    DRM_ERR_LOG("MediaKeySystemService::GetOfflineLicenseStatus exit.");
    return ret;
}

int32_t MediaKeySystemService::RemoveOfflineLicense(std::vector<uint8_t> &licenseId)
{
    DRM_INFO_LOG("MediaKeySystemService::RemoveOfflineLicense enter.");
    int32_t ret = DRM_OK;

    ret = hdiKeySystem_->RemoveOfflineLicense(licenseId);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::RemoveOfflineLicense failed");
        return ret;
    }

    DRM_INFO_LOG("MediaKeySystemService::RemoveOfflineLicense exit.");
    return ret;
}
} // DrmStandard
} // OHOS