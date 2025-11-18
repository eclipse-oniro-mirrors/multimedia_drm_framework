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
#include "drm_dfx_utils.h"
#include "drm_log.h"
#include "drm_dfx.h"
#include "drm_error_code.h"
#include "drm_helper.h"
#include "dump_usage.h"
#include "hitrace/tracechain.h"
#include "mediakeysystem_service.h"
#include "mediakeysystemfactory_service.h"

namespace OHOS {
namespace DrmStandard {
using namespace OHOS::HiviewDFX;
namespace {
constexpr uint32_t MAX_LISTNER_NUM = 64;
}
const std::string TV_DEVICE = "tv";
const std::string SPLIT_LINE =
    "----------------------------------------------------------------------------------------\n";

REGISTER_SYSTEM_ABILITY_BY_ID(MediaKeySystemFactoryService, MEDIA_KEY_SYSTEM_SERVICE_ID, true)


void MediaKeySystemFactoryService::OnDrmPluginDied(std::string &name)
{
    DRM_INFO_LOG("OnDrmPluginDied enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    for (auto pidIt = mediaKeySystemForPid_.begin(); pidIt != mediaKeySystemForPid_.end();) {
        std::set<sptr<MediaKeySystemService>> mediaKeySystemServiceSet = pidIt->second;
        for (auto keySystem = mediaKeySystemServiceSet.begin(); keySystem != mediaKeySystemServiceSet.end();) {
            std::string pluginName = (*keySystem)->GetPluginName();
            if (name == pluginName) {
                CloseMediaKeySystemService(*keySystem);
                mediaKeySystemServiceSet.erase(keySystem++);
            } else {
                ++keySystem;
            }
        }
        if (mediaKeySystemServiceSet.empty()) {
            pidIt = mediaKeySystemForPid_.erase(pidIt);
        } else {
            pidIt++;
        }
    }
    currentMediaKeySystemNum_.erase(name);
}

MediaKeySystemFactoryService::MediaKeySystemFactoryService(int32_t systemAbilityId, bool runOnCreate)
    : SystemAbility(systemAbilityId, runOnCreate)
{
    DRM_INFO_LOG("MediaKeySystemFactoryService enter.");
    deathRecipientMap_.clear();
    drmHostManager_ = new (std::nothrow) DrmHostManager(this);
    if (drmHostManager_ == nullptr) {
        DRM_ERR_LOG("create drmHostManager_ failed.");
        return;
    }
}

MediaKeySystemFactoryService::~MediaKeySystemFactoryService()
{
    DRM_INFO_LOG("~MediaKeySystemFactoryService enter.");
}

void MediaKeySystemFactoryService::OnStart()
{
    DRM_INFO_LOG("OnStart enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_LOG(drmHostManager_ != nullptr && drmHostManager_->Init() == DRM_INNER_ERR_OK,
        "OnStart failed to init drm host manager.");
    StartDrmNetObserver();
    bool res = Publish(this);
    DRM_DEBUG_LOG("MediaKeySystemFactoryService OnStart res=%{public}d", res);
    AddSystemAbilityListener(MEMORY_MANAGER_SA_ID);
    ReportServiceBehaviorEvent("DRM_SERVICE", "start");
}

void MediaKeySystemFactoryService::OnDump()
{
}

void MediaKeySystemFactoryService::OnStop()
{
    DRM_INFO_LOG("OnStop enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);

    if (drmHostManager_) {
        drmHostManager_->DeInit();
        drmHostManager_ = nullptr;
    }

    int pid = getpid();
    /* 3012 is the saId of drm_service */
    Memory::MemMgrClient::GetInstance().NotifyProcessStatus(pid, 1, 0, 3012);
    OHOS::HiviewDFX::DumpUsage dumpUse;
    ReportServiceBehaviorEvent("DRM_SERVICE", "end");
}

int32_t MediaKeySystemFactoryService::OnIdle(const SystemAbilityOnDemandReason& idleReason)
{
    (void)idleReason;
    DRM_INFO_LOG("OnIdle enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    if (!mediaKeySystemForPid_.empty()) {
        return -1; // -1:reject unload
    }
    return 0;
}

void MediaKeySystemFactoryService::OnAddSystemAbility(int32_t systemAbilityId, const std::string &deviceId)
{
    DRM_INFO_LOG("OnAddSystemAbility enter.");
    if (systemAbilityId == MEMORY_MANAGER_SA_ID) {
        int32_t pid = getpid();
        /* 3012 is the said of drm service */
        Memory::MemMgrClient::GetInstance().NotifyProcessStatus(pid, 1, 1, 3012);
    }
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
    DRM_INFO_LOG("DistroyForClientDied pid: %{public}d.", pid);
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_LOG(mediaKeySystemForPid_.find(pid) != mediaKeySystemForPid_.end(), "pid not found");
    for (auto it = mediaKeySystemForPid_[pid].begin(); it != mediaKeySystemForPid_[pid].end();) {
        if ((*it) != nullptr) {
            // decrease the total count in drm host manager.
            sptr<IMediaKeySystem> hdiMediaKeySystem = (*it)->getMediaKeySystem();
            (*it)->CloseMediaKeySystemServiceByCallback();
            if (hdiMediaKeySystem != nullptr) {
                drmHostManager_->ReleaseMediaKeySystem(hdiMediaKeySystem);
            }
            if (currentMediaKeySystemNum_.find((*it)->GetPluginName()) != currentMediaKeySystemNum_.end()) {
                currentMediaKeySystemNum_[(*it)->GetPluginName()]--;
            }
        }
        it = mediaKeySystemForPid_[pid].erase(it);
    }
    mediaKeySystemForPid_[pid].clear();
    mediaKeySystemForPid_.erase(pid);
}

void MediaKeySystemFactoryService::MediaKeySystemFactoryClientDied(pid_t pid)
{
    DRM_ERR_LOG("MediaKeySystemFactory client has died, pid:%{public}d", pid);
    std::lock_guard<std::recursive_mutex> lock(factoryServiceMutex_);
    if (clientListenerMap_.find(pid) != clientListenerMap_.end()) {
        if (clientListenerMap_[pid] != nullptr && clientListenerMap_[pid]->AsObject() != nullptr &&
            deathRecipientMap_.find(pid) != deathRecipientMap_.end() && deathRecipientMap_[pid] != nullptr) {
            (void)clientListenerMap_[pid]->AsObject()->RemoveDeathRecipient(deathRecipientMap_[pid]);
        }
        deathRecipientMap_.erase(pid);
        clientListenerMap_.erase(pid);
    }
    DistroyForClientDied(pid);
}

int32_t MediaKeySystemFactoryService::SetListenerObject(const sptr<IRemoteObject> &object)
{
    pid_t pid = IPCSkeleton::GetCallingPid();
    std::lock_guard<std::recursive_mutex> lock(factoryServiceMutex_);
    if (clientListenerMap_.find(pid) != clientListenerMap_.end()) {
        if (clientListenerMap_[pid] != nullptr && clientListenerMap_[pid]->AsObject() != nullptr &&
            deathRecipientMap_.find(pid) != deathRecipientMap_.end() && deathRecipientMap_[pid] != nullptr) {
            (void)clientListenerMap_[pid]->AsObject()->RemoveDeathRecipient(deathRecipientMap_[pid]);
        }
        deathRecipientMap_.erase(pid);
        clientListenerMap_.erase(pid);
    }
    DRM_CHECK_AND_RETURN_RET_LOG(clientListenerMap_.size() < MAX_LISTNER_NUM,
        DRM_INNER_ERR_OPERATION_NOT_PERMITTED, "the number of listeners exceeds MAX_LISTNER_NUM: 64");
    DRM_CHECK_AND_RETURN_RET_LOG(object != nullptr, DRM_INNER_ERR_MEMORY_ERROR, "set listener object is nullptr");
    sptr<IDrmListener> clientListener = iface_cast<IDrmListener>(object);
    DRM_CHECK_AND_RETURN_RET_LOG(
        clientListener != nullptr, DRM_INNER_ERR_MEMORY_ERROR, "failed to convert IDrmListener");
    sptr<DrmDeathRecipient> deathRecipient = new (std::nothrow) DrmDeathRecipient(pid);
    DRM_CHECK_AND_RETURN_RET_LOG(deathRecipient != nullptr, DRM_INNER_ERR_MEMORY_ERROR,
        "failed to new DrmDeathRecipient");
    deathRecipient->SetNotifyCb([this] (pid_t pid) {
        this->MediaKeySystemFactoryClientDied(pid);
    });
    if (clientListener->AsObject() != nullptr) {
        (void)clientListener->AsObject()->AddDeathRecipient(deathRecipient);
    }
    DRM_DEBUG_LOG("MediaKeySystem client pid:%{public}d", pid);
    deathRecipientMap_[pid] = deathRecipient;
    clientListenerMap_[pid] = clientListener;
    return DRM_INNER_ERR_OK;
}

bool MediaKeySystemFactoryService::IsListenerObjectSet()
{
    std::lock_guard<std::recursive_mutex> lock(factoryServiceMutex_);
    pid_t pid = IPCSkeleton::GetCallingPid();
    bool ret = false;
    if (clientListenerMap_.find(pid) != clientListenerMap_.end()) {
        ret = true;
    }
    return ret;
}

int32_t MediaKeySystemFactoryService::CancelAbilityIdle()
{
    if (GetAbilityState() == SystemAbilityState::IDLE) {
        bool result = CancelIdle();
        DRM_CHECK_AND_RETURN_RET_LOG(result, DRM_INNER_ERR_SERVICE_DIED, "CancelIdle failed");
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySystemFactoryService::CreateMediaKeySystem(const std::string &name,
    sptr<IMediaKeySystemService> &mediaKeySystemProxy)
{
    DRM_INFO_LOG("CreateMediaKeySystem enter.");
    bool res = IsListenerObjectSet();
    DRM_CHECK_AND_RETURN_RET_LOG(res, DRM_INNER_ERR_OPERATION_NOT_PERMITTED, "Not Set Listener.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(
        CancelAbilityIdle() == DRM_INNER_ERR_OK, DRM_INNER_ERR_SERVICE_DIED, "Cancel Idle failed");
    sptr<MediaKeySystemService> mediaKeySystemService = nullptr;
    sptr<IMediaKeySystem> hdiMediaKeySystem = nullptr;
    if (currentMediaKeySystemNum_[name] >= KEY_SYSTEM_INSTANCES_MAX_NUMBER) {
        DRM_ERR_LOG("The number of MediaKeySystem is greater than 64");
        return DRM_INNER_ERR_MAX_SYSTEM_NUM_REACHED;
    }
    std::string systemName = name;
    int32_t ret = drmHostManager_->CreateMediaKeySystem(systemName, hdiMediaKeySystem);
    if (hdiMediaKeySystem == nullptr || ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("drmHostManager_ return hdiMediaKeySystem nullptr.");
        ReportFaultEvent(7, "CreateMediaKeySystem failed", ""); // 7:SERVICE ERR
        return DRM_INNER_ERR_INVALID_MEDIA_KEY_SYSTEM;
    }

    StatisticsInfo statisticsInfo;
    InitStatisticsInfo(hdiMediaKeySystem, name, statisticsInfo);
    mediaKeySystemService = new(std::nothrow) MediaKeySystemService(hdiMediaKeySystem, statisticsInfo);
    if (mediaKeySystemService == nullptr) {
        DRM_ERR_LOG("CreateMediaKeySystem allocation failed.");
        ReportFaultEvent(1, "CreateMediaKeySystem failed", ""); // 1:ALLOC ERR
        return DRM_INNER_ERR_NO_MEMORY;
    }
    mediaKeySystemService->SetMediaKeySystemServiceOperatorsCallback(this);
    (void)mediaKeySystemService->SetBundleName();
    (void)mediaKeySystemService->SetApiTargetVersion();

    int32_t pid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("CreateMediaKeySystem GetCallingPID: %{public}d.", pid);
    mediaKeySystemForPid_[pid].insert(mediaKeySystemService);
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR "  Current mediaKeySystemService",
        FAKE_POINTER(mediaKeySystemService.GetRefPtr()));
    mediaKeySystemProxy = mediaKeySystemService;
    if (currentMediaKeySystemNum_.find(name) != currentMediaKeySystemNum_.end()) {
        currentMediaKeySystemNum_[name]++;
    } else {
        currentMediaKeySystemNum_[name] = 1;
    }
    return ret;
}

int32_t MediaKeySystemFactoryService::CloseMediaKeySystemService(sptr<MediaKeySystemService> mediaKeySystemService)
{
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_INFO_LOG("CloseMediaKeySystemService enter.");
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
    if (currentMediaKeySystemNum_.find(pluginName) != currentMediaKeySystemNum_.end() &&
        currentMediaKeySystemNum_[pluginName] > 0) {
        currentMediaKeySystemNum_[pluginName]--;
    }
    if (hdiMediaKeySystem != NULL) {
        drmHostManager_->ReleaseMediaKeySystem(hdiMediaKeySystem);
    }
    mediaKeySystemService = nullptr;
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySystemFactoryService::IsMediaKeySystemSupported(const std::string &name, bool &isSupported)
{
    DRM_INFO_LOG("IsMediaKeySystemSupported one parameters enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    bool isSystemSupported = false;
    DRM_CHECK_AND_RETURN_RET_LOG(
        CancelAbilityIdle() == DRM_INNER_ERR_OK, DRM_INNER_ERR_SERVICE_DIED, "Cancel Idle failed");
    std::string systemName = name;
    int32_t ret = drmHostManager_->IsMediaKeySystemSupported(systemName, &isSystemSupported);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("IsMediaKeySystemSupported failed.");
        return ret;
    }
    isSupported = isSystemSupported;
    return ret;
}

int32_t MediaKeySystemFactoryService::IsMediaKeySystemSupported(const std::string &name, const std::string &mimeType,
    bool &isSupported)
{
    DRM_INFO_LOG("IsMediaKeySystemSupported two parameters enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    bool isSystemSupported = false;
    DRM_CHECK_AND_RETURN_RET_LOG(
        CancelAbilityIdle() == DRM_INNER_ERR_OK, DRM_INNER_ERR_SERVICE_DIED, "Cancel Idle failed");
    std::string systemName = name;
    std::string systemMimeType = mimeType;
    int32_t ret = drmHostManager_->IsMediaKeySystemSupported(systemName, systemMimeType, &isSystemSupported);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("IsMediaKeySystemSupported failed.");
        return ret;
    }
    isSupported = isSystemSupported;
    return ret;
}

int32_t MediaKeySystemFactoryService::IsMediaKeySystemSupported(const std::string &name, const std::string &mimeType,
    int32_t securityLevel, bool &isSupported)
{
    DRM_INFO_LOG("IsMediaKeySystemSupported three parameters enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    bool isSystemSupported = false;
    DRM_CHECK_AND_RETURN_RET_LOG(
        CancelAbilityIdle() == DRM_INNER_ERR_OK, DRM_INNER_ERR_SERVICE_DIED, "Cancel Idle failed");
    std::string systemName = name;
    std::string systemMimeType = mimeType;
    int32_t ret = drmHostManager_->IsMediaKeySystemSupported(systemName,
        systemMimeType, securityLevel, &isSystemSupported);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("IsMediaKeySystemSupported failed.");
        return ret;
    }
    isSupported = isSystemSupported;
    return ret;
}

int32_t MediaKeySystemFactoryService::GetMediaKeySystems(std::map<std::string, std::string> &mediaKeySystemNames)
{
    DRM_INFO_LOG("GetMediaKeySystems enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(
        CancelAbilityIdle() == DRM_INNER_ERR_OK, DRM_INNER_ERR_SERVICE_DIED, "Cancel Idle failed");
    int32_t ret = drmHostManager_->GetMediaKeySystems(mediaKeySystemNames);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK, ret, "GetMediaKeySystems failed.");
    return ret;
}

int32_t MediaKeySystemFactoryService::GetMediaKeySystemUuid(const std::string &name, std::string &uuid)
{
    DRM_INFO_LOG("GetMediaKeySystemUuid enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    DRM_CHECK_AND_RETURN_RET_LOG(
        CancelAbilityIdle() == DRM_INNER_ERR_OK, DRM_INNER_ERR_SERVICE_DIED, "Cancel Idle failed");
    std::string systemName = name;
    int32_t ret = drmHostManager_->GetMediaKeySystemUuid(systemName, uuid);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK, ret, "GetMediaKeySystemUuid failed.");
    return ret;
}

void MediaKeySystemFactoryService::InitStatisticsInfo(const sptr<IMediaKeySystem> &hdiMediaKeySystem,
    std::string pluginName, StatisticsInfo &statisticsInfo)
{
    DRM_INFO_LOG("InitStatisticsInfo enter.");
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    statisticsInfo.pluginName = pluginName;
    if (drmHostManager_ != nullptr) {
        std::map<std::string, std::string> pluginNameUuidMap;
        drmHostManager_->GetMediaKeySystems(pluginNameUuidMap);
        if (pluginNameUuidMap.find(pluginName) != pluginNameUuidMap.end()) {
            statisticsInfo.pluginUuid = pluginNameUuidMap[pluginName];
        }
    }
    statisticsInfo.bundleName = GetClientBundleName(IPCSkeleton::GetCallingUid());
    if (hdiMediaKeySystem != nullptr) {
        (void)hdiMediaKeySystem->GetConfigurationString("vendor", statisticsInfo.vendorName);
        (void)hdiMediaKeySystem->GetConfigurationString("version", statisticsInfo.versionName);
    }
    statisticsInfo.targetVersion = GetClientBundleInfoTargetVersion(statisticsInfo.bundleName,
        IPCSkeleton::GetCallingUid());
    DRM_INFO_LOG("uid: %{public}d, appName: %{public}s, targetVersion: %{public}u.",
        IPCSkeleton::GetCallingUid(), statisticsInfo.bundleName.c_str(), statisticsInfo.targetVersion);
    DRM_INFO_LOG("pluginName: %{public}s, pluginUUID: %{public}s",
        statisticsInfo.pluginName.c_str(), statisticsInfo.pluginUuid.c_str());
    DRM_INFO_LOG("vendorName: %{public}s, versionName: %{public}s",
        statisticsInfo.vendorName.c_str(), statisticsInfo.versionName.c_str());
}

int32_t MediaKeySystemFactoryService::WriteDumpInfo(int32_t fd, std::string &dumpString)
{
    OHOS::HiviewDFX::DumpUsage dumpUse;
    std::lock_guard<std::recursive_mutex> lock(mutex_);
    dumpString += "MediaKeySystem MemoryUsage: " + std::to_string(dumpUse.GetPss(getpid())) + "\n";
    std::map<std::string, std::string> mediaKeySystemInfo;
    drmHostManager_->GetMediaKeySystems(mediaKeySystemInfo);
    for (auto &iter : mediaKeySystemInfo) {
        dumpString += SPLIT_LINE;
        std::string tmpStr = "Plugin Name: " + iter.first + "\n" +
                             "Plugin UUID: " + iter.second + "\n" +
                             "Total MediaKeySystem Num: ";
        int32_t systemNum = 0;
        if (currentMediaKeySystemNum_.find(iter.first) != currentMediaKeySystemNum_.end()) {
            systemNum = currentMediaKeySystemNum_[iter.first];
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
            CertificateStatus certStatus = CertificateStatus::CERT_STATUS_UNAVAILABLE;
            system->GetCertificateStatus(certStatus);
            dumpString += "Plugin Name: " + system->GetPluginName() + "\n";
            dumpString += "Certificate Status: " + std::to_string(static_cast<int32_t>(certStatus)) + "\n";
            dumpString += system->GetSessionsDumpInfo();
        }
    }
    dumpString += SPLIT_LINE;
    DRM_CHECK_AND_RETURN_RET_LOG(fd != -1, OHOS::NO_ERROR, "%{public}s", dumpString.c_str());
    ssize_t writeLen = write(fd, dumpString.c_str(), dumpString.size());
    DRM_CHECK_AND_RETURN_RET_LOG(writeLen != -1, OHOS::NO_ERROR, "Dump write error!");
    return OHOS::NO_ERROR;
}

void MediaKeySystemFactoryService::StartDrmNetObserver()
{
    DRM_CHECK_AND_RETURN_LOG(DrmHelper::GetDeviceType() == TV_DEVICE,
                             "not TV,StartDrmNetObserver stop");
    sptr<DrmNetObserver> drmNetObserver_ = new (std::nothrow)DrmNetObserver();
    DRM_CHECK_AND_RETURN_LOG(drmNetObserver_ != nullptr, "Drm Alloc Memory Failed");
    this->drmNetObserver_ = drmNetObserver_;
    int32_t ret = drmNetObserver_->SetDrmHostManager(drmHostManager_);
    DRM_CHECK_AND_RETURN_LOG(ret == DRM_INNER_ERR_OK, "Set drmHostManager failed");
    drmNetObserver_->StartObserver();
}
} // DrmStandard
} // OHOS
