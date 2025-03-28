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
#include "ipc_skeleton.h"
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
    DRM_INFO_LOG("~MediaKeySystemService 0x%{public}06" PRIXPTR " Instances destroy.", FAKE_POINTER(this));
    {
        std::lock_guard<std::recursive_mutex> lock(callbackMutex_);
        keySystemOperatoersCallback_ = nullptr;
    }
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    if (hdiKeySystem_ != nullptr) {
        DRM_ERR_LOG("hdiKeySystem != nullptr");
    }
}

void MediaKeySystemService::OnDrmPluginDied(std::string &name)
{}

int32_t MediaKeySystemService::CloseMediaKeySystemServiceByCallback()
{
    DRM_INFO_LOG("CloseMediaKeySystemServiceByCallback enter.");
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
    {
        std::lock_guard<std::recursive_mutex> lock(callbackMutex_);
        keySystemOperatoersCallback_ = nullptr;
    }
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    // release itself
    if (hdiKeySystem_ != nullptr) {
        DRM_ERR_LOG("hdiKeySystem_ CloseHdiMediaKeySession");
        hdiKeySystem_->Destroy();
        hdiKeySystem_ = nullptr;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySystemService::Release()
{
    DRM_INFO_LOG("Release enter.");
    int32_t currentPid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySystemService GetCallingPID: %{public}d", currentPid);
    std::lock_guard<std::recursive_mutex> lock(callbackMutex_);
    if (keySystemOperatoersCallback_ != nullptr) {
        keySystemOperatoersCallback_->CloseMediaKeySystemService(this);
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySystemService::SetMediaKeySystemServiceOperatorsCallback(
    wptr<IMediaKeySystemServiceOperatorsCallback> callback)
{
    std::lock_guard<std::recursive_mutex> lock(callbackMutex_);
    if (callback.promote() == nullptr) {
        DRM_ERR_LOG("SetMediaKeySystemServiceOperatorsCallback callback is null");
        return DRM_INNER_ERR_INVALID_VAL;
    }
    keySystemOperatoersCallback_ = callback;
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySystemService::GenerateKeySystemRequest(std::vector<uint8_t> &request, std::string &defaultUrl)
{
    DrmTrace trace("MediaKeySystemService::GenerateKeySystemRequest");
    DRM_INFO_LOG("GenerateKeySystemRequest enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");
    auto timeBefore = std::chrono::system_clock::now();
    ret = hdiKeySystem_->GenerateKeySystemRequest(defaultUrl, request);
    generationDuration_ = CalculateTimeDiff(timeBefore, std::chrono::system_clock::now());
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GenerateKeySystemRequest failed.");
        ReportFaultEvent(ret, "GenerateKeySystemRequest failed", "");
        generationResult_ = "failed";
        return ret;
    }
    generationResult_ = "success";
    return ret;
}

int32_t MediaKeySystemService::ProcessKeySystemResponse(const std::vector<uint8_t> &response)
{
    DrmTrace trace("MediaKeySystemService::ProcessKeySystemResponse");
    DRM_INFO_LOG("ProcessKeySystemResponse enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");
    auto timeBefore = std::chrono::system_clock::now();
    ret = hdiKeySystem_->ProcessKeySystemResponse(response);
    uint32_t processDuration = CalculateTimeDiff(timeBefore, std::chrono::system_clock::now());
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("ProcessKeySystemResponse failed.");
        std::string responseString = std::string(reinterpret_cast<const char*>(response.data()), response.size());
        ReportFaultEvent(ret, "ProcessKeySystemResponse failed", responseString);
        return ret;
    }
    struct DownLoadInfo downLoadInfo = InitDownLoadInfo(generationDuration_, generationResult_, processDuration,
        "success");
    ReportCertificateBehaviorEvent(statisticsInfo_, downLoadInfo, 0, 0, "");
    return ret;
}

int32_t MediaKeySystemService::SetConfigurationString(std::string &configName, std::string &value)
{
    DRM_INFO_LOG("SetConfiguration enter, configName:%{public}s, value:%{public}s.",
        configName.c_str(), value.c_str());
    DRM_CHECK_AND_RETURN_RET_LOG(configName != "bundleName", DRM_INNER_ERR_INVALID_VAL,
        "configuration name is not support");
    DRM_CHECK_AND_RETURN_RET_LOG(configName != "apiTargetVersion", DRM_INNER_ERR_INVALID_VAL,
        "configuration api target version is not support");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");
    ret = hdiKeySystem_->SetConfigurationString(configName, value);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("SetConfiguration failed.");
        return ret;
    }
    return ret;
}

int32_t MediaKeySystemService::GetConfigurationString(std::string &configName, std::string &value)
{
    DRM_INFO_LOG("GetConfiguration enter, configName:%{public}s.", configName.c_str());
    DRM_CHECK_AND_RETURN_RET_LOG(configName != "bundleName", DRM_INNER_ERR_INVALID_VAL,
        "configuration name is not support");
    DRM_CHECK_AND_RETURN_RET_LOG(configName != "apiTargetVersion", DRM_INNER_ERR_INVALID_VAL,
        "configuration api target version is not support");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");

    ret = hdiKeySystem_->GetConfigurationString(configName, value);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GetConfiguration failed.");
        return ret;
    }
    return ret;
}

int32_t MediaKeySystemService::SetConfigurationByteArray(std::string &configName, std::vector<uint8_t> &value)
{
    DRM_INFO_LOG("SetConfiguration enter, configName:%{public}s.", configName.c_str());
    DRM_CHECK_AND_RETURN_RET_LOG(configName != "bundleName", DRM_INNER_ERR_INVALID_VAL,
        "configuration name is not support");
    DRM_CHECK_AND_RETURN_RET_LOG(configName != "apiTargetVersion", DRM_INNER_ERR_INVALID_VAL,
        "configuration api target version is not support");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");
    std::vector<uint8_t> valueVec;
    valueVec.assign(value.begin(), value.end());
    ret = hdiKeySystem_->SetConfigurationByteArray(configName, valueVec);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("SetConfiguration failed.");
        return ret;
    }
    return ret;
}

int32_t MediaKeySystemService::GetConfigurationByteArray(std::string &configName, std::vector<uint8_t> &value)
{
    DRM_INFO_LOG("GetConfiguration enter, configName:%{public}s.", configName.c_str());
    DRM_CHECK_AND_RETURN_RET_LOG(configName != "bundleName", DRM_INNER_ERR_INVALID_VAL,
        "configuration name is not support");
    DRM_CHECK_AND_RETURN_RET_LOG(configName != "apiTargetVersion", DRM_INNER_ERR_INVALID_VAL,
        "configuration api target version is not support");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");
    std::vector<uint8_t> valueVec;
    ret = hdiKeySystem_->GetConfigurationByteArray(configName, valueVec);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GetConfiguration failed.");
        return ret;
    }
    value.assign(valueVec.begin(), valueVec.end());
    return ret;
}

int32_t MediaKeySystemService::CreateMediaKeySession(IMediaKeySessionService::ContentProtectionLevel securityLevel,
    sptr<IMediaKeySessionService> &keySessionProxy)
{
    DrmTrace trace("MediaKeySystemService::CreateMediaKeySession");
    DRM_INFO_LOG("CreateMediaKeySession enter, securityLevel:%{public}d.", securityLevel);
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");
    sptr<MediaKeySessionService> keySessionService = nullptr;
    sptr<OHOS::HDI::Drm::V1_0::IMediaKeySession> hdiMediaKeySession = nullptr;
    if (currentKeySessionNumber >= KEY_SESSION_INSTANCES_MAX_NUMBER) {
        DRM_ERR_LOG("The number of MediaKeySession is greater than 64");
        return DRM_INNER_ERR_MAX_SESSION_NUM_REACHED;
    }
    ret = hdiKeySystem_->CreateMediaKeySession((OHOS::HDI::Drm::V1_0::ContentProtectionLevel)securityLevel,
        hdiMediaKeySession);
    if (hdiMediaKeySession == nullptr) {
        DRM_ERR_LOG("hdiKeySystem_ CreateMediaKeySession failed.");
        ReportFaultEvent(7, "CreateMediaKeySession failed", ""); // 7:SERVICE ERR
        return DRM_INNER_ERR_INVALID_KEY_SESSION;
    }
    keySessionService = new (std::nothrow) MediaKeySessionService(hdiMediaKeySession, statisticsInfo_);
    if (keySessionService == nullptr) {
        DRM_ERR_LOG("CreateMediaKeySession allocation failed.");
        ReportFaultEvent(1, "CreateMediaKeySession failed", ""); // 1:ALLOC ERR
        return DRM_INNER_ERR_NO_MEMORY;
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
    return ret;
}

int32_t MediaKeySystemService::SetBundleName()
{
    DRM_INFO_LOG("SetBundleName");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(!statisticsInfo_.bundleName.empty(), DRM_INNER_ERR_UNKNOWN,
        "bundle name is empty!");
    ret = hdiKeySystem_->SetConfigurationString("bundleName", statisticsInfo_.bundleName);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK, ret, "SetBundleName failed.");
    return ret;
}

int32_t MediaKeySystemService::SetApiTargetVersion()
{
    DRM_INFO_LOG("SetApiTargetVersion targetVersion: %{public}u", statisticsInfo_.targetVersion);
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");
    ret = hdiKeySystem_->SetConfigurationString("apiTargetVersion",
        std::to_string(static_cast<int>(statisticsInfo_.targetVersion)));
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK, ret, "SetApiTargetVersion failed.");
    return ret;
}

int32_t MediaKeySystemService::CloseMediaKeySessionService(sptr<MediaKeySessionService> sessionService)
{
    DRM_INFO_LOG("CloseMediaKeySessionService enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    int32_t currentPid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySystemService GetCallingPID: %{public}d", currentPid);
    std::lock_guard<std::recursive_mutex> lock(mutex_);

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
    return ret;
}

int32_t MediaKeySystemService::GetStatistics(std::vector<IMediaKeySystemService::MetircKeyValue> &metrics)
{
    DRM_INFO_LOG("GetStatistics enter");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");
    std::map<std::string, std::string> tmpStatistics;
    ret = hdiKeySystem_->GetStatistics(tmpStatistics);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GetStatistics failed.");
        return ret;
    }
    for (auto it = tmpStatistics.begin(); it != tmpStatistics.end(); it++) {
        IMediaKeySystemService::MetircKeyValue keyValue;
        keyValue.name = it->first;
        keyValue.value = it->second;
        metrics.push_back(keyValue);
    }
    if (metrics.size() == 0) {
        DRM_ERR_LOG("GetStatistics failed.");
        return DRM_INNER_ERR_BASE;
    }
    return ret;
}

int32_t MediaKeySystemService::GetMaxContentProtectionLevel(
    IMediaKeySessionService::ContentProtectionLevel *securityLevel)
{
    DRM_INFO_LOG("GetMaxContentProtectionLevel enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    OHOS::HDI::Drm::V1_0::ContentProtectionLevel level;
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");

    ret = hdiKeySystem_->GetMaxContentProtectionLevel(level);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GetMaxContentProtectionLevel failed.");
        return ret;
    }
    *securityLevel = (IMediaKeySessionService::ContentProtectionLevel)level;
    return ret;
}

int32_t MediaKeySystemService::GetCertificateStatus(IMediaKeySystemService::CertificateStatus *certStatus)
{
    DRM_INFO_LOG("GetCertificateStatus enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");

    OHOS::HDI::Drm::V1_0::CertificateStatus tmpStatus;
    ret = hdiKeySystem_->GetOemCertificateStatus(tmpStatus);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GetCertificateStatus failed.");
        return ret;
    }
    *certStatus = (IMediaKeySystemService::CertificateStatus)tmpStatus;

    return ret;
}

int32_t MediaKeySystemService::GetOfflineMediaKeyIds(std::vector<std::vector<uint8_t>> &licenseIds)
{
    DRM_INFO_LOG("GetOfflineMediaKeyIds enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");

    ret = hdiKeySystem_->GetOfflineMediaKeyIds(licenseIds);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GetOfflineMediaKeyIds failed.");
        return ret;
    }

    return ret;
}

int32_t MediaKeySystemService::GetOfflineMediaKeyStatus(std::vector<uint8_t> &licenseId,
    IMediaKeySessionService::OfflineMediaKeyStatus &status)
{
    DRM_INFO_LOG("GetOfflineMediaKeyStatus enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");

    OHOS::HDI::Drm::V1_0::OfflineMediaKeyStatus offlineMediaKeyStatus;
    ret = hdiKeySystem_->GetOfflineMediaKeyStatus(licenseId, offlineMediaKeyStatus);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GetOfflineMediaKeyStatus failed.");
        return ret;
    }
    status = (IMediaKeySessionService::OfflineMediaKeyStatus)offlineMediaKeyStatus;

    return ret;
}

int32_t MediaKeySystemService::ClearOfflineMediaKeys(std::vector<uint8_t> &licenseId)
{
    DRM_INFO_LOG("ClearOfflineMediaKeys enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");

    ret = hdiKeySystem_->ClearOfflineMediaKeys(licenseId);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("ClearOfflineMediaKeys failed.");
        return ret;
    }

    return ret;
}

int32_t MediaKeySystemService::SetCallback(sptr<IMediaKeySystemServiceCallback> &callback)
{
    DRM_INFO_LOG("SetCallback enter");
    int32_t ret = DRM_INNER_ERR_BASE;
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(hdiKeySystem_ != nullptr, DRM_INNER_ERR_SERVICE_FATAL_ERROR,
        "hdiKeySystem_ is nullptr!");
    if (callback == nullptr) {
        DRM_ERR_LOG("SetCallback callback is nullptr.");
        return ret;
    }
    callback_ = callback;

    if (hdiKeySystem_ != nullptr) {
        return hdiKeySystem_->SetCallback(this);
    }
    DRM_ERR_LOG("SetCallback hdiKeySystem_ is nullptr.");
    return ret;
}

int32_t MediaKeySystemService::SendEvent(OHOS::HDI::Drm::V1_0::EventType eventType, int32_t extra,
    const std::vector<uint8_t> &data)
{
    DRM_INFO_LOG("SendEvent enter.");
    DrmEventType event = static_cast<DrmEventType>(eventType);
    if (callback_ != nullptr) {
        return callback_->SendEvent(event, extra, data);
    }
    DRM_INFO_LOG("SendEvent failed because callback is nullptr");
    return DRM_INNER_ERR_OPERATION_NOT_PERMITTED;
}

std::string MediaKeySystemService::GetPluginName()
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return statisticsInfo_.pluginName;
}

std::string MediaKeySystemService::GetSessionsDumpInfo()
{
    DRM_INFO_LOG("GetSessionsDumpInfo enter");
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
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    return hdiKeySystem_;
}

} // DrmStandard
} // OHOS