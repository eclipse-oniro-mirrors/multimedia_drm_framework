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

#include <unordered_set>
#include <securec.h>
#include "system_ability_definition.h"
#include "ipc_skeleton.h"
#include "iservice_registry.h"
#include "drm_dfx_utils.h"
#include "drm_host_manager.h"
#include "drm_log.h"
#include "drm_trace.h"
#include "drm_error_code.h"
#include "key_session_service.h"
#include "mediakeysystem_service.h"
#include "hitrace/tracechain.h"

namespace OHOS {
namespace DrmStandard {
using namespace OHOS::HiviewDFX;
MediaKeySystemService::MediaKeySystemService(sptr<OHOS::HDI::Drm::V1_0::IMediaKeySystem> hdiKeySystem)
{
    DRM_INFO_LOG("MediaKeySystemService 0x%{public}06" PRIXPTR " Instances create.", FAKE_POINTER(this));
    keySystemOperatoersCallback_ = nullptr;
    hdiKeySystem_ = hdiKeySystem;
}

MediaKeySystemService::MediaKeySystemService(sptr<OHOS::HDI::Drm::V1_0::IMediaKeySystem> hdiKeySystem,
    StatisticsInfo statisticsInfo)
{
    DRM_INFO_LOG("MediaKeySystemService 0x%{public}06" PRIXPTR " Instances create.", FAKE_POINTER(this));
    keySystemOperatoersCallback_ = nullptr;
    hdiKeySystem_ = hdiKeySystem;
    statisticsInfo_ = statisticsInfo;
}

MediaKeySystemService::~MediaKeySystemService()
{
    DRM_INFO_LOG("MediaKeySystemService 0x%{public}06" PRIXPTR " Instances destroy.", FAKE_POINTER(this));
    std::lock_guard<std::mutex> lock(mutex_);
    keySystemOperatoersCallback_ = nullptr;
    if (hdiKeySystem_ != nullptr) {
        DRM_ERR_LOG("hdiKeySystem != nullptr");
    }
}

void MediaKeySystemService::OnDrmPluginDied(std::string &name)
{}

int32_t MediaKeySystemService::CloseMediaKeySystemServiceByCallback()
{
    DRM_INFO_LOG("MediaKeySystemService::CloseMediaKeySystemServiceByCallback enter.");
    int32_t currentPid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySystemService GetCallingPID: %{public}d", currentPid);
    {
        std::lock_guard<std::mutex> lock(sessionsSetMutex_);
        for (auto it = sessionsSet_.begin(); it != sessionsSet_.end();) {
            if ((*it) != nullptr) {
                (*it)->CloseMediaKeySessionServiceByCallback();
            }
            it = sessionsSet_.erase(it);
        }
        sessionsSet_.clear();
    }

    // release itself
    if (hdiKeySystem_ != nullptr) {
        DRM_ERR_LOG("hdiKeySystem_ CloseHdiMediaKeySession");
        hdiKeySystem_->Destroy();
        hdiKeySystem_ = nullptr;
    }
    keySystemOperatoersCallback_ = nullptr;
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
    DrmTrace trace("MediaKeySystemService::GenerateKeySystemRequest");
    DRM_INFO_LOG("MediaKeySystemService::GenerateKeySystemRequest enter.");
    int32_t ret = DRM_OK;
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");
    auto timeBefore = std::chrono::system_clock::now();
    ret = hdiKeySystem_->GenerateKeySystemRequest(defaultUrl, request);
    generationDuration_ = CalculateTimeDiff(timeBefore, std::chrono::system_clock::now());
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::GenerateKeySystemRequest failed.");
        ReportFaultEvent(ret, "GenerateKeySystemRequest failed", "");
        generationResult_ = "failed";
        return ret;
    }
    generationResult_ = "success";
    DRM_INFO_LOG("MediaKeySystemService::GenerateKeySystemRequest exit.");
    return ret;
}

int32_t MediaKeySystemService::ProcessKeySystemResponse(const std::vector<uint8_t> &response)
{
    DrmTrace trace("MediaKeySystemService::ProcessKeySystemResponse");
    DRM_INFO_LOG("MediaKeySystemService::ProcessKeySystemResponse enter.");
    int32_t ret = DRM_OK;
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");
    auto timeBefore = std::chrono::system_clock::now();
    ret = hdiKeySystem_->ProcessKeySystemResponse(response);
    uint32_t processDuration = CalculateTimeDiff(timeBefore, std::chrono::system_clock::now());
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::ProcessKeySystemResponse failed.");
        std::string responseString = std::string(reinterpret_cast<const char*>(response.data()), response.size());
        ReportFaultEvent(ret, "ProcessKeySystemResponse failed", responseString);
        return ret;
    }
    DRM_INFO_LOG("MediaKeySystemService::ProcessKeySystemResponse exit.");
    struct DownLoadInfo downLoadInfo = InitDownLoadInfo(generationDuration_, generationResult_, processDuration,
        "success");
    ReportCertificateBehaviorEvent(statisticsInfo_, downLoadInfo, 0, 0, "");
    return ret;
}

int32_t MediaKeySystemService::SetConfigurationString(std::string &configName, std::string &value)
{
    DRM_INFO_LOG("MediaKeySystemService::SetConfiguration enter, configName:%{public}s, value:%{public}s.",
        configName.c_str(), value.c_str());
    int32_t ret = DRM_OK;
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");
    ret = hdiKeySystem_->SetConfigurationString(configName, value);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::SetConfiguration failed.");
        return ret;
    }
    DRM_INFO_LOG("MediaKeySystemService::SetConfiguration exit.");
    return ret;
}

int32_t MediaKeySystemService::GetConfigurationString(std::string &configName, std::string &value)
{
    DRM_INFO_LOG("MediaKeySystemService::GetConfiguration enter, configName:%{public}s.", configName.c_str());
    int32_t ret = DRM_OK;
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");

    ret = hdiKeySystem_->GetConfigurationString(configName, value);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::GetConfiguration failed.");
        return ret;
    }
    DRM_INFO_LOG("MediaKeySystemService::GetConfiguration exit, value:%{public}s.", value.c_str());
    return ret;
}

int32_t MediaKeySystemService::SetConfigurationByteArray(std::string &configName, std::vector<uint8_t> &value)
{
    DRM_INFO_LOG("MediaKeySystemService::SetConfiguration enter, configName:%{public}s.", configName.c_str());
    int32_t ret = DRM_OK;
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");
    std::vector<uint8_t> valueVec;
    valueVec.assign(value.begin(), value.end());
    ret = hdiKeySystem_->SetConfigurationByteArray(configName, valueVec);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::SetConfiguration failed.");
        return ret;
    }
    DRM_INFO_LOG("MediaKeySystemService::SetConfiguration exit.");
    return ret;
}

int32_t MediaKeySystemService::GetConfigurationByteArray(std::string &configName, std::vector<uint8_t> &value)
{
    DRM_INFO_LOG("MediaKeySystemService::GetConfiguration enter, configName:%{public}s.", configName.c_str());
    int32_t ret = DRM_OK;
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");
    std::vector<uint8_t> valueVec;
    ret = hdiKeySystem_->GetConfigurationByteArray(configName, valueVec);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::GetConfiguration failed.");
        return ret;
    }
    value.assign(valueVec.begin(), valueVec.end());
    return ret;
}

int32_t MediaKeySystemService::CreateMediaKeySession(IMediaKeySessionService::ContentProtectionLevel securityLevel,
    sptr<IMediaKeySessionService> &keySessionProxy)
{
    DrmTrace trace("MediaKeySystemService::CreateMediaKeySession");
    DRM_INFO_LOG("MediaKeySystemService::CreateMediaKeySession enter, securityLevel:%{public}d.", securityLevel);
    int32_t ret = DRM_OK;
    std::lock_guard<std::mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");
    sptr<MediaKeySessionService> keySessionService = nullptr;
    sptr<OHOS::HDI::Drm::V1_0::IMediaKeySession> hdiMediaKeySession = nullptr;
    if (currentKeySessionNumber >= KEY_SESSION_MAX_NUMBER) {
        DRM_ERR_LOG("The number of MediaKeySession is greater than 64");
        return DRM_MAX_SESSION_NUM_REACHED;
    }
    ret = hdiKeySystem_->CreateMediaKeySession((OHOS::HDI::Drm::V1_0::ContentProtectionLevel)securityLevel,
        hdiMediaKeySession);
    if (hdiMediaKeySession == nullptr) {
        DRM_ERR_LOG("hdiKeySystem_ CreateMediaKeySession failed.");
        ReportFaultEvent(DRM_SERVICE_ERROR, "CreateMediaKeySession failed", "");
        return DRM_SERVICE_ERROR;
    }
    keySessionService = new (std::nothrow) MediaKeySessionService(hdiMediaKeySession, statisticsInfo_);
    if (keySessionService == nullptr) {
        DRM_ERR_LOG("MediaKeySystemService::CreateMediaKeySession allocation failed.");
        ReportFaultEvent(DRM_ALLOC_ERROR, "CreateMediaKeySession failed", "");
        return DRM_ALLOC_ERROR;
    }
    keySessionService->SetMediaKeySessionServiceOperatorsCallback(this);

    int32_t pid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySystemService CreateMediaKeySession GetCallingPID: %{public}d", pid);
    {
        std::lock_guard<std::mutex> lock(sessionsSetMutex_);
        sessionsSet_.insert(keySessionService);
    }

    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " is Current keySessionService", FAKE_POINTER(keySessionService.GetRefPtr()));
    keySessionProxy = keySessionService;
    currentKeySessionNumber++;
    DRM_INFO_LOG("MediaKeySystemService::CreateMediaKeySession exit.");
    return ret;
}

int32_t MediaKeySystemService::CloseMediaKeySessionService(sptr<MediaKeySessionService> sessionService)
{
    DRM_INFO_LOG("MediaKeySystemService::CloseMediaKeySessionService enter.");
    int32_t ret = DRM_OK;
    int32_t currentPid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySystemService GetCallingPID: %{public}d", currentPid);

    if (sessionService != nullptr) {
        DRM_INFO_LOG("MediaKeySystemService call CloseMediaKeySessionServiceByCallback ");
        ret = sessionService->CloseMediaKeySessionServiceByCallback();
    }
    {
        std::lock_guard<std::mutex> lock(sessionsSetMutex_);
        sessionsSet_.erase(sessionService);
        if (currentKeySessionNumber > 0) {
            currentKeySessionNumber--;
        }
    }
    sessionService = nullptr;
    DRM_INFO_LOG("MediaKeySystemService::CloseMediaKeySessionService exit.");
    return ret;
}

int32_t MediaKeySystemService::GetStatistics(std::vector<IMediaKeySystemService::MetircKeyValue> &metrics)
{
    DRM_INFO_LOG("MediaKeySystemService::GetStatistics enter");
    int32_t ret = DRM_OK;
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");
    std::map<std::string, std::string> tmpStatistics;
    ret = hdiKeySystem_->GetStatistics(tmpStatistics);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::GetStatistics failed.");
        return ret;
    }
    for (auto it = tmpStatistics.begin(); it != tmpStatistics.end(); it++) {
        IMediaKeySystemService::MetircKeyValue keyValue;
        keyValue.name = it->first;
        keyValue.value = it->second;
        metrics.push_back(keyValue);
    }
    if (metrics.size() == 0) {
        DRM_ERR_LOG("MediaKeySystemService::GetStatistics failed.");
        return DRM_ERROR;
    }
    DRM_INFO_LOG("MediaKeySystemService::GetStatistics exit");
    return ret;
}

int32_t MediaKeySystemService::GetMaxContentProtectionLevel(
    IMediaKeySessionService::ContentProtectionLevel *securityLevel)
{
    DRM_INFO_LOG("MediaKeySystemService::GetMaxContentProtectionLevel enter.");
    int32_t ret = DRM_OK;
    OHOS::HDI::Drm::V1_0::ContentProtectionLevel level;
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");

    ret = hdiKeySystem_->GetMaxContentProtectionLevel(level);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::GetMaxContentProtectionLevel failed.");
        return ret;
    }
    *securityLevel = (IMediaKeySessionService::ContentProtectionLevel)level;
    DRM_INFO_LOG("MediaKeySystemService::GetMaxContentProtectionLevel exit.");
    return ret;
}

int32_t MediaKeySystemService::GetCertificateStatus(IMediaKeySystemService::CertificateStatus *certStatus)
{
    DRM_INFO_LOG("MediaKeySystemService::GetCertificateStatus enter.");
    int32_t ret = DRM_OK;
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");

    OHOS::HDI::Drm::V1_0::CertificateStatus tmpStatus;
    ret = hdiKeySystem_->GetOemCertificateStatus(tmpStatus);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::GetCertificateStatus failed.");
        return ret;
    }
    *certStatus = (IMediaKeySystemService::CertificateStatus)tmpStatus;

    DRM_INFO_LOG("MediaKeySystemService::GetCertificateStatus exit.");
    return ret;
}

int32_t MediaKeySystemService::GetOfflineMediaKeyIds(std::vector<std::vector<uint8_t>> &licenseIds)
{
    DRM_INFO_LOG("MediaKeySystemService::GetOfflineMediaKeyIds enter.");
    int32_t ret = DRM_OK;
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");

    ret = hdiKeySystem_->GetOfflineMediaKeyIds(licenseIds);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::GetOfflineMediaKeyIds failed.");
        return ret;
    }

    DRM_INFO_LOG("MediaKeySystemService::GetOfflineMediaKeyIds exit.");
    return ret;
}

int32_t MediaKeySystemService::GetOfflineMediaKeyStatus(std::vector<uint8_t> &licenseId,
    IMediaKeySessionService::OfflineMediaKeyStatus &status)
{
    DRM_INFO_LOG("MediaKeySystemService::GetOfflineMediaKeyStatus enter.");
    int32_t ret = DRM_OK;
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");

    OHOS::HDI::Drm::V1_0::OfflineMediaKeyStatus offlineMediaKeyStatus;
    ret = hdiKeySystem_->GetOfflineMediaKeyStatus(licenseId, offlineMediaKeyStatus);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::GetOfflineMediaKeyStatus failed.");
        return ret;
    }
    status = (IMediaKeySessionService::OfflineMediaKeyStatus)offlineMediaKeyStatus;

    DRM_ERR_LOG("MediaKeySystemService::GetOfflineMediaKeyStatus exit.");
    return ret;
}

int32_t MediaKeySystemService::ClearOfflineMediaKeys(std::vector<uint8_t> &licenseId)
{
    DRM_INFO_LOG("MediaKeySystemService::ClearOfflineMediaKeys enter.");
    int32_t ret = DRM_OK;
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");

    ret = hdiKeySystem_->ClearOfflineMediaKeys(licenseId);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::ClearOfflineMediaKeys failed.");
        return ret;
    }

    DRM_INFO_LOG("MediaKeySystemService::ClearOfflineMediaKeys exit.");
    return ret;
}

int32_t MediaKeySystemService::SetCallback(sptr<IMeidaKeySystemServiceCallback> &callback)
{
    DRM_INFO_LOG("MediaKeySystemService:: SetCallback.");
    int32_t ret = DRM_ERROR;
    std::lock_guard<std::mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");
    if (callback == nullptr) {
        DRM_ERR_LOG("MediaKeySystemService::SetCallback callback is nullptr , failed.");
        return ret;
    }
    callback_ = callback;

    if (hdiKeySystem_ != nullptr) {
        return hdiKeySystem_->SetCallback(this);
    }
    DRM_ERR_LOG("MediaKeySystemService::SetCallback hdiKeySystem_ is nullptr , failed.");
    return ret;
}

int32_t MediaKeySystemService::SendEvent(OHOS::HDI::Drm::V1_0::EventType eventType, int32_t extra,
    const std::vector<uint8_t> &data)
{
    DRM_INFO_LOG("MediaKeySystemService:: SendEvent.");
    DrmEventType event = static_cast<DrmEventType>(eventType);
    if (callback_ != nullptr) {
        return callback_->SendEvent(event, extra, data);
    }
    DRM_INFO_LOG("MediaKeySystemService:: SendEvent failed because callback is nullptr");
    return DRM_OPERATION_NOT_ALLOWED;
}

std::string MediaKeySystemService::GetPluginName()
{
    return statisticsInfo_.pluginName;
}

std::string MediaKeySystemService::GetSessionsDumpInfo()
{
    DRM_INFO_LOG("MediaKeySystemService:: GetSessionsDumpInfo.");
    std::string dumpInfo;
    std::lock_guard<std::mutex> lock(sessionsSetMutex_);
    dumpInfo += "Total MediaKeySession Num: " + std::to_string(sessionsSet_.size()) + "\n";
    uint32_t sessionNum = 0;
    for (auto &session : sessionsSet_) {
        sessionNum++;
        dumpInfo += "#### MediaKeySession " + std::to_string(sessionNum) + " ####\n";
        if (session != nullptr) {
            dumpInfo += session->GetDecryptModuleDumpInfo();
        }
    }
    return dumpInfo;
}

sptr<OHOS::HDI::Drm::V1_0::IMediaKeySystem> MediaKeySystemService::getMediaKeySystem()
{
    return hdiKeySystem_;
}

} // DrmStandard
} // OHOS