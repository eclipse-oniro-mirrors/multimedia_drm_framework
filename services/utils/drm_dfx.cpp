/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include "drm_dfx.h"
#include "drm_dfx_utils.h"
#include <unistd.h>
#include "drm_log.h"
#include "nlohmann/json.hpp"
#include "native_drm_err.h"
#include "securec.h"
#include "ipc_skeleton.h"
#include "hitrace/tracechain.h"
#include "meta/any.h"
#include <cstdint>

namespace {
    constexpr int64_t HOURS_BETWEEN_REPORTS = 4;
    constexpr int64_t MAX_MAP_SIZE = 100;
    std::mutex collectMut_;
    std::mutex reportMut_;
    std::chrono::system_clock::time_point currentTime_ = std::chrono::system_clock::now();
    std::map<uint64_t, int32_t> idMap_;
    std::map<int32_t, std::list<std::pair<uint64_t, std::shared_ptr<OHOS::Media::Meta>>>> mediaInfoMap_;
    std::map<int32_t, std::list<std::pair<uint64_t, std::shared_ptr<OHOS::Media::Meta>>>> reportMediaInfoMap_;
    bool g_reachMaxMapSize {false};
}
namespace OHOS {
namespace DrmStandard {
using namespace OHOS::HiviewDFX;
using json = nlohmann::json;

#ifdef ENABLE_DRM_SYSEVENT_CONTROL

DrmEvent& DrmEvent::GetInstance()
{
    static DrmEvent instance;
    return instance;
}

void DrmEvent::DrmStatisicsEventWrite(OHOS::HiviewDFX::HiSysEvent::EventType type,
    const std::map<int32_t, std::list<std::pair<uint64_t, std::shared_ptr<Media::Meta>>>>& infoMap)
{
    DRM_INFO_LOG("DrmEvent::DrmStatisicsEventWrite.");
    if (infoMap.empty()) {
        DRM_INFO_LOG("DrmEvent infoMap is empty.");
        return;
    }
    std::vector<std::string> infoArr;
    for (const auto& kv : infoMap) {
        json jsonArray;
        json eventInfoJson;
        json drmEvents;
        for (const auto& listPair : kv.second) {
            json metaInfoJson;
            ParseOneEvent(listPair, metaInfoJson);
            drmEvents.push_back(metaInfoJson);
        }
        eventInfoJson["appName"] = GetClientBundleName(kv.first);
        eventInfoJson["drmStatistics"] = drmEvents;
        jsonArray.push_back(eventInfoJson);
        infoArr.push_back(jsonArray.dump());
    }
    StatisicsHiSysEventWrite(type, infoArr);
}

void DrmEvent::ParseOneEvent(const std::pair<uint64_t, std::shared_ptr<OHOS::Media::Meta>> &listPair,
    json& metaInfoJson)
{
    for (auto it = listPair.second->begin(); it != listPair.second->end(); ++it) {
        Media::Any valueType = OHOS::Media::GetDefaultAnyValue(it->first);
        if (Media::Any::IsSameTypeWith<int32_t>(valueType)) {
            int32_t intVal;
            if (listPair.second->GetData(it->first, intVal)) {
                metaInfoJson[it->first] = std::to_string(intVal);
            }
        } else if (Media::Any::IsSameTypeWith<uint32_t>(valueType)) {
            uint32_t uintVal;
            if (listPair.second->GetData(it->first, uintVal)) {
                metaInfoJson[it->first] = std::to_string(uintVal);
            }
        } else if (Media::Any::IsSameTypeWith<std::string>(valueType)) {
            metaInfoJson[it->first] = Media::AnyCast<std::string>(it->second);
        } else if (Media::Any::IsSameTypeWith<int8_t>(valueType)) {
            int8_t intVal;
            if (listPair.second->GetData(it->first, intVal)) {
                metaInfoJson[it->first] = std::to_string(intVal);
            }
        } else if (Media::Any::IsSameTypeWith<bool>(valueType)) {
            bool isTrue;
            if (listPair.second->GetData(it->first, isTrue)) {
                metaInfoJson[it->first] = isTrue ? "true" : "false";
            }
        } else {
            DRM_INFO_LOG("not found type matched with it->first: %{public}s.", it->first.c_str());
        }
    }
}

void DrmEvent::StatisicsHiSysEventWrite(OHOS::HiviewDFX::HiSysEvent::EventType type,
    const std::vector<std::string>& infoArr)
{
    DRM_INFO_LOG("DrmEvent::StatisicsHiSysEventWrite.");
    std::string eventName = "DRM_STATISTICS";
    HiSysEventWrite(OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MEDIA, eventName, type,
                    "EVENTS", infoArr);
}

int32_t DrmEvent::AppendMediaInfo(const std::shared_ptr<Media::Meta>& meta)
{
    DRM_INFO_LOG("AppendMediaInfo.");
    if (meta == nullptr || meta->Empty()) {
        DRM_INFO_LOG("Insert meta is empty.");
        return DRM_ERR_INVALID_STATE;
    }
    uint64_t instanceId = HiTraceChain::GetId().GetChainId();

    std::lock_guard<std::mutex> lock(collectMut_);
    auto idMapIt = idMap_.find(instanceId);
    if (idMapIt == idMap_.end()) {
        DRM_INFO_LOG("Not found instanceId.");
        return DRM_ERR_INVALID_VAL;
    }
    int32_t uid = idMapIt->second;
    auto it = mediaInfoMap_.find(uid);
    if (it == mediaInfoMap_.end()) {
        DRM_INFO_LOG("Not found uid.");
        return DRM_ERR_INVALID_STATE;
    }
    auto& instanceList = it->second;
    for (const auto& instancePair : instanceList) {
        if (instancePair.first == instanceId) {
            auto arg = meta->begin();
            while (arg != meta->end()) {
                instancePair.second->SetData(arg->first, arg->second);
                ++arg;
            }
            break;
        }
    }
    return DRM_ERR_OK;
}

int32_t DrmEvent::CreateMediaInfo(int32_t uid)
{
    DRM_INFO_LOG("CreateMediaInfo.");
    std::lock_guard<std::mutex> lock(collectMut_);
    uint64_t instanceId = HiTraceChain::GetId().GetChainId();
    DRM_INFO_LOG("CreateMediaInfo uid is: %{public}" PRId32 " instanceId is: %{public}." PRIu64, uid, instanceId);
    auto instanceIdMap = idMap_.find(instanceId);
    if (instanceIdMap != idMap_.end()) {
        DRM_INFO_LOG("instanceId already exists.");
        return DRM_ERR_INVALID_VAL;
    } else {
        idMap_[instanceId] = uid;
    }

    std::shared_ptr<Media::Meta> meta = std::make_shared<Media::Meta>();
    std::pair<uint64_t, std::shared_ptr<Media::Meta>> metaAppIdPair(instanceId, meta);
    auto it = mediaInfoMap_.find(uid);
    if (it != mediaInfoMap_.end()) {
        it->second.push_back(metaAppIdPair);
    } else {
        mediaInfoMap_[uid].push_back(metaAppIdPair);
    }
    return DRM_ERR_OK;
}

int32_t DrmEvent::ReportMediaInfo()
{
    uint64_t instanceId = HiTraceChain::GetId().GetChainId();
    auto currentTime = std::chrono::system_clock::now();
    auto diff = currentTime - currentTime_;
    auto hour = std::chrono::duration_cast<std::chrono::hours>(diff).count();
    CollectReportMediaInfo(instanceId);
    std::lock_guard<std::mutex> lock(reportMut_);
    if (g_reachMaxMapSize) {
        DRM_INFO_LOG("Event data size exceeds 100, report the event");
        g_reachMaxMapSize = false;
        return StatisticsEventReport();
    }
    if (hour >= HOURS_BETWEEN_REPORTS) {
        DRM_INFO_LOG("Over 4 hours, report the event.");
        return StatisticsEventReport();
    }
    return DRM_ERR_OK;
}

void DrmEvent::CollectReportMediaInfo(uint64_t instanceId)
{
    int32_t uid;
    std::pair<uint64_t, std::shared_ptr<OHOS::Media::Meta>> metaAppIdPair;
    {
        std::lock_guard<std::mutex> lock(collectMut_);
        DRM_INFO_LOG("CollectReportMediaInfo.");
        auto idMapIt = idMap_.find(instanceId);
        if (idMapIt == idMap_.end()) {
            DRM_INFO_LOG("Not found instanceId in idMap, instanceId is : %{public}." PRIu64, instanceId);
            return;
        }
        uid = idMapIt->second;
        idMap_.erase(idMapIt);
        auto uidToMediaInfo = mediaInfoMap_.find(uid);
        if (uidToMediaInfo == mediaInfoMap_.end()) {
            return;
        }
        auto& instanceList = uidToMediaInfo->second;
        for (const auto& instancePair : instanceList) {
            if (instancePair.first == instanceId) {
                metaAppIdPair = instancePair;
                instanceList.remove(instancePair);
                break;
            }
        }
    }

    std::lock_guard<std::mutex> lock(reportMut_);
    auto reportUidToMediaInfo = reportMediaInfoMap_.find(uid);
    if (reportUidToMediaInfo != reportMediaInfoMap_.end()) {
        reportUidToMediaInfo->second.push_back(metaAppIdPair);
    } else {
        reportMediaInfoMap_[uid].push_back(metaAppIdPair);
    }
    g_reachMaxMapSize = (reportMediaInfoMap_.size() >= MAX_MAP_SIZE);
}

int32_t DrmEvent::StatisticsEventReport()
{
    DRM_INFO_LOG("StatisticsEventReport.");
    if (reportMediaInfoMap_.empty()) {
        DRM_INFO_LOG("reportMediaInfoMap_ is empty, can't report.");
        return DRM_ERR_INVALID_STATE;
    }

    DrmStatisicsEventWrite(OHOS::HiviewDFX::HiSysEvent::EventType::STATISTIC, reportMediaInfoMap_);
    auto currentTime = std::chrono::system_clock::now();
    currentTime_ = currentTime;
    reportMediaInfoMap_.clear();
    return DRM_ERR_OK;
}
#endif
} // namespace DrmStandard
} // namespace OHOS