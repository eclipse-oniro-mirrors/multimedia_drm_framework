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

#include <securec.h>
#include <unistd.h>
#include <unordered_set>
#include "system_ability_definition.h"
#include "mem_mgr_client.h"
#include "mem_mgr_proxy.h"
#include "ipc_skeleton.h"
#include "access_token.h"
#include "iservice_registry.h"
#include "drm_dfx_utils.h"
#include "drm_log.h"
#include "drm_dfx.h"
#include "drm_error_code.h"
#include "dump_usage.h"
#include "hitrace/tracechain.h"
#include "iservice_registry.h"
#include "mediakeysystem_service.h"
#include "mediakeysystemfactory_service.h"

namespace OHOS {
namespace DrmStandard {
using namespace OHOS::HiviewDFX;
const std::string SPLIT_LINE =
    "----------------------------------------------------------------------------------------\n";

REGISTER_SYSTEM_ABILITY_BY_ID(MediaKeySystemFactoryService, MEDIA_KEY_SYSTEM_SERVICE_ID, true)

MediaKeySystemFactoryService::MediaKeySystemFactoryService(int32_t systemAbilityId, bool runOnCreate)
    : SystemAbility(systemAbilityId, runOnCreate)
{
    drmHostManager_ = new (std::nothrow) DrmHostManager(this);
    if (drmHostManager_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemFactoryService create drmHostManager_ failed.");
        return;
    }
}

MediaKeySystemFactoryService::~MediaKeySystemFactoryService()
{
    DRM_INFO_LOG("~MediaKeySystemFactoryService.");
}

void MediaKeySystemFactoryService::OnStart()
{
    if (drmHostManager_ == nullptr || drmHostManager_->Init() != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemFactoryService OnStart failed to init drm host manager.");
    }
    bool res = Publish(this);
    if (res) {
        DRM_INFO_LOG("MediaKeySystemFactoryService OnStart res=%{public}d", res);
    }
    AddSystemAbilityListener(MEMORY_MANAGER_SA_ID);
    ReportServiceBehaviorEvent("DRM_SERVICE", "start");
}

void MediaKeySystemFactoryService::OnDump()
{
    DRM_INFO_LOG("MediaKeySystemFactoryService::OnDump called.");
}

void MediaKeySystemFactoryService::OnStop()
{
    DRM_INFO_LOG("MediaKeySystemFactoryService::OnStop called.");

    if (drmHostManager_) {
        drmHostManager_->DeInit();
        delete drmHostManager_;
        drmHostManager_ = nullptr;
    }

    int pid = getpid();
    /* 3012 is the saId of drm_service */
    Memory::MemMgrClient::GetInstance().NotifyProcessStatus(pid, 1, 0, 3012);
    OHOS::HiviewDFX::DumpUsage dumpUse;
    ReportServiceBehaviorEvent("DRM_SERVICE", "end");
}

void MediaKeySystemFactoryService::OnAddSystemAbility(int32_t systemAbilityId, const std::string &deviceId)
{
    DRM_INFO_LOG("MediaKeySystemFactoryService::OnAddSystemAbility called.");
    if (systemAbilityId == MEMORY_MANAGER_SA_ID) {
        int32_t pid = getpid();
        /* 3012 is the said of drm service */
        Memory::MemMgrClient::GetInstance().NotifyProcessStatus(pid, 1, 1, 3012);
    }
    DRM_INFO_LOG("MediaKeySystemFactoryService::OnAddSystemAbility end.");
}

int32_t MediaKeySystemFactoryService::Dump(int32_t fd, const std::vector<std::u16string>& args)
{
    DRM_CHECK_AND_RETURN_RET_LOG(fd > 0, OHOS::INVALID_OPERATION, "Failed to check fd.");
    std::string dumpString;

    auto ret = WriteDumpInfo(fd, dumpString);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == NO_ERROR,
        OHOS::INVALID_OPERATION, "Failed to write framework information");
    return OHOS::NO_ERROR;
}

void MediaKeySystemFactoryService::DistroyForClientDied(pid_t pid)
{
    // destroy all system objects for this pid
    DRM_INFO_LOG("MediaKeySystemFactoryService::DistroyForClientDied pid: %{public}d.", pid);
    if (mediaKeySystemForPid_.find(pid) == mediaKeySystemForPid_.end()) {
        return;
    }
    for (auto it = mediaKeySystemForPid_[pid].begin(); it != mediaKeySystemForPid_[pid].end();) {
        if ((*it) != nullptr) {
            sptr<IMediaKeySystem> hdiMediaKeySystem = (*it)->getMediaKeySystem();
            (*it)->CloseMediaKeySystemServiceByCallback();
            if (hdiMediaKeySystem != nullptr) {
                DRM_DEBUG_LOG("MediaKeySystemFactoryService ReleaseMediaKeySystem.");
                drmHostManager_->ReleaseMediaKeySystem(hdiMediaKeySystem);
            }
        }
        it = mediaKeySystemForPid_[pid].erase(it);
    }
    mediaKeySystemForPid_[pid].clear();
    mediaKeySystemForPid_.erase(pid);
}

int32_t MediaKeySystemFactoryService::CreateMediaKeySystem(std::string &name,
    sptr<IMediaKeySystemService> &mediaKeySystemProxy)
{
    DRM_INFO_LOG("MediaKeySystemFactoryService CreateMediaKeySystem enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    sptr<MediaKeySystemService> mediaKeySystemService = nullptr;
    sptr<IMediaKeySystem> hdiMediaKeySystem = nullptr;
    int32_t ret = drmHostManager_->CreateMediaKeySystem(name, hdiMediaKeySystem);
    if (hdiMediaKeySystem == nullptr || ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemFactoryService::drmHostManager_ return hdiMediaKeySystem nullptr.");
        ReportFaultEvent(DRM_SERVICE_ERROR, "CreateMediaKeySystem failed", "");
        return DRM_SERVICE_ERROR;
    }
    StatisticsInfo statisticsInfo;
    InitStatisticsInfo(hdiMediaKeySystem, statisticsInfo);
    mediaKeySystemService = new (std::nothrow) MediaKeySystemService(hdiMediaKeySystem, statisticsInfo, name);
    if (mediaKeySystemService == nullptr) {
        DRM_ERR_LOG("MediaKeySystemFactoryService::CreateMediaKeySystem allocation failed.");
        ReportFaultEvent(DRM_ALLOC_ERROR, "CreateMediaKeySystem failed", "");
        return DRM_ALLOC_ERROR;
    }
    mediaKeySystemService->SetMediaKeySystemServiceOperatorsCallback(this);
    int32_t pid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySystemFactoryService CreateMediaKeySystem GetCallingPID: %{public}d.", pid);
    mediaKeySystemForPid_[pid].insert(mediaKeySystemService);
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " is Current mediaKeySystemService",
        FAKE_POINTER(mediaKeySystemService.GetRefPtr()));
    mediaKeySystemProxy = mediaKeySystemService;
    if (CurrentMediaKeySystemNum_.find(name) != CurrentMediaKeySystemNum_.end()) {
        CurrentMediaKeySystemNum_[name]++;
    } else {
        CurrentMediaKeySystemNum_[name] = 1;
    }
    DRM_INFO_LOG("MediaKeySystemFactoryService CreateMediaKeySystem exit.");
    return ret;
}

int32_t MediaKeySystemFactoryService::CloseMediaKeySystemService(sptr<MediaKeySystemService> mediaKeySystemService)
{
    DRM_INFO_LOG("MediaKeySystemFactoryService CloseMediaKeySystemService enter.");
    int32_t errCode = DRM_OK;
    int32_t currentPid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySystemFactoryService GetCallingPID: %{public}d", currentPid);
    sptr<IMediaKeySystem> hdiMediaKeySystem = mediaKeySystemService->getMediaKeySystem();

    for (auto &pidSystemsSet : mediaKeySystemForPid_) {
        if (pidSystemsSet.second.find(mediaKeySystemService) != pidSystemsSet.second.end()) {
            mediaKeySystemService->CloseMediaKeySystemServiceByCallback();
            pidSystemsSet.second.erase(mediaKeySystemService);
            break;
        }
    }
    std::string pluginName = mediaKeySystemService->GetPluginName();
    if (CurrentMediaKeySystemNum_.find(pluginName) != CurrentMediaKeySystemNum_.end()) {
        CurrentMediaKeySystemNum_[pluginName]--;
    }
    if (hdiMediaKeySystem != NULL) {
        DRM_DEBUG_LOG("MediaKeySystemFactoryService ReleaseMediaKeySystem.");
        drmHostManager_->ReleaseMediaKeySystem(hdiMediaKeySystem);
    }
    mediaKeySystemService = nullptr;
    DRM_INFO_LOG("MediaKeySystemFactoryService CloseMediaKeySystemService exit.");
    return errCode;
}

int32_t MediaKeySystemFactoryService::IsMediaKeySystemSupported(std::string &name, bool *isSurpported)
{
    DRM_INFO_LOG("MediaKeySystemFactoryService::IsMediaKeySystemSupported one parameters enter.");
    int32_t ret = drmHostManager_->IsMediaKeySystemSupported(name, isSurpported);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemFactoryService::IsMediaKeySystemSupported failed.");
        return ret;
    }
    DRM_INFO_LOG("MediaKeySystemFactoryService::IsMediaKeySystemSupported one parameters exit.");
    return ret;
}

int32_t MediaKeySystemFactoryService::IsMediaKeySystemSupported(std::string &name, std::string &mimeType,
    bool *isSurpported)
{
    DRM_INFO_LOG("MediaKeySystemFactoryService::IsMediaKeySystemSupported two parameters enter.");
    int32_t ret = drmHostManager_->IsMediaKeySystemSupported(name, mimeType, isSurpported);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemFactoryService::IsMediaKeySystemSupported failed.");
        return ret;
    }
    DRM_INFO_LOG("MediaKeySystemFactoryService::IsMediaKeySystemSupported two parameters exit.");
    return ret;
}

int32_t MediaKeySystemFactoryService::IsMediaKeySystemSupported(std::string &name, std::string &mimeType,
    int32_t securityLevel, bool *isSurpported)
{
    DRM_INFO_LOG("MediaKeySystemFactoryService::IsMediaKeySystemSupported three parameters enter.");
    int32_t ret = drmHostManager_->IsMediaKeySystemSupported(name, mimeType, securityLevel, isSurpported);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemFactoryService::IsMediaKeySystemSupported failed.");
        return ret;
    }
    DRM_INFO_LOG("MediaKeySystemFactoryService enter IsMediaKeySystemSupported  three parameters exit.");
    return ret;
}

int32_t MediaKeySystemFactoryService::GetMediaKeySystems(std::map<std::string, std::string> &mediaKeySystemNames)
{
    DRM_INFO_LOG("MediaKeySystemFactoryService::GetMediaKeySystems enter.");
    int32_t ret = drmHostManager_->GetMediaKeySystems(mediaKeySystemNames);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemFactoryService::GetMediaKeySystems failed.");
        return ret;
    }
    DRM_INFO_LOG("MediaKeySystemFactoryService::GetMediaKeySystems exit.");
    return ret;
}

int32_t MediaKeySystemFactoryService::GetMediaKeySystemUuid(std::string &name, std::string &uuid)
{
    DRM_INFO_LOG("MediaKeySystemFactoryService::GetMediaKeySystemUuid enter.");
    int32_t ret = drmHostManager_->GetMediaKeySystemUuid(name, uuid);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemService::GetMediaKeySystemUuid failed.");
        return ret;
    }
    DRM_INFO_LOG("MediaKeySystemFactoryService::GetMediaKeySystemUuid exit.");
    return ret;
}

void MediaKeySystemFactoryService::InitStatisticsInfo(sptr<IMediaKeySystem> hdiMediaKeySystem,
    StatisticsInfo &statisticsInfo)
{
    std::string pluginUuid;
    std::string pluginName;
    std::string vendorName;
    std::string versionName;
    std::map<std::string, std::string> mediaKeySystemNames;
    std::map<std::string, std::string> statistics;
    int32_t ret = GetMediaKeySystems(mediaKeySystemNames);
    if (ret == DRM_OK) {
        auto it = mediaKeySystemNames.begin();
        pluginName = it->first;
        pluginUuid = it->second;
    }
    ret = hdiMediaKeySystem->GetStatistics(statistics);
    if (ret == DRM_OK) {
        vendorName = statistics[vendor];
        versionName = statistics[version];
    }
    std::string bundleName = GetClientBundleName(IPCSkeleton::GetCallingUid());
    statisticsInfo = {
        pluginUuid,
        pluginName,
        vendorName,
        versionName,
        bundleName,
    };
}

int32_t MediaKeySystemFactoryService::WriteDumpInfo(int32_t fd, std::string &dumpString)
{
    OHOS::HiviewDFX::DumpUsage dumpUse;
    dumpString += "MediaKeySystem MemoryUsage: " + std::to_string(dumpUse.GetPss(getpid())) + "\n";
    std::map<std::string, std::string> mediaKeySystemInfo;
    drmHostManager_->GetMediaKeySystems(mediaKeySystemInfo);
    for (auto &iter : mediaKeySystemInfo) {
        dumpString += SPLIT_LINE;
        std::string tmpStr = "Plugin Name: " + iter.first + "\n" +
                             "Plugin UUID: " + iter.second + "\n" +
                             "Total MediaKeySystem Num: ";
        int32_t systemNum = 0;
        if (CurrentMediaKeySystemNum_.find(iter.first) != CurrentMediaKeySystemNum_.end()) {
            systemNum = CurrentMediaKeySystemNum_[iter.first];
        }
        tmpStr += std::to_string(systemNum) + "\n";
        dumpString += tmpStr;
    }
    uint32_t systemNum = 0;
    for (auto &pidIter : mediaKeySystemForPid_) {
        dumpString += SPLIT_LINE;
        systemNum++;
        dumpString += "#### MediaKeySystem " + std::to_string(systemNum) + " ####\n";
        dumpString += "PID: " + std::to_string(pidIter.first) + "\n";
        for (auto &system : pidIter.second) {
            dumpString += "-------------------------------\n";
            IMediaKeySystemService::CertificateStatus certStatus = IMediaKeySystemService::CERT_STATUS_UNAVAILABLE;
            system->GetCertificateStatus(&certStatus);
            dumpString += "Plugin Name: " + system->GetPluginName() + "\n";
            dumpString += "Certificate Status: " + std::to_string(certStatus) + "\n";
            dumpString += system->GetSessionsDumpInfo();
        }
    }
    dumpString += SPLIT_LINE;
    if (fd != -1) {
        write(fd, dumpString.c_str(), dumpString.size());
    } else {
        DRM_INFO_LOG("%{public}s", dumpString.c_str());
    }
    return OHOS::NO_ERROR;
}

} // DrmStandard
} // OHOS