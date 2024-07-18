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

#ifndef DRM_DFX_H
#define DRM_DFX_H

#include <list>
#include <string>
#include <queue>
#include <refbase.h>
#include "drm_dfx_utils.h"
#include "nocopyable.h"
#include "nlohmann/json.hpp"
#include "hisysevent.h"
#include "hitrace/tracechain.h"
#include <chrono>

namespace OHOS {
namespace DrmStandard {
using namespace OHOS::HiviewDFX;

struct DrmServiveInfo {
    std::string module;
    uint32_t currentTime;
    std::string serviceName;
    std::string action;
    uint32_t memoryUsage;
};

struct DrmLicenseInfo {
    std::string module;
    uint32_t currentTime;
    std::string appName;
    std::string instanceId;
    std::string drmName;
    std::string drmUuid;
    std::string clientVersion;
    std::string licenseType;
    uint32_t generationDuration;
    std::string generationResult;
    uint32_t processDuration;
    std::string processResult;
};

struct DrmCertificateInfo {
    std::string module;
    uint32_t currentTime;
    std::string appName;
    std::string instanceId;
    std::string drmName;
    std::string drmUuid;
    std::string clientVersion;
    uint32_t generationDuration;
    std::string generationResult;
    uint32_t processDuration;
    std::string processResult;
    uint32_t callServerTime;
    uint32_t serverCostDuration;
    std::string serverResult;
};

struct DrmFaultInfo {
    std::string module;
    std::string appName;
    std::string instanceId;
    int32_t errorCode;
    std::string errorMesg;
    std::string extraMesg;
};

struct DrmDecryptionInfo {
    std::string module;
    std::string appName;
    std::string instanceId;
    int32_t errorCode;
    std::string errorMesg;
    std::string decryptAlgo;
    std::string decryptKeyid;
    std::string decryptIv;
};

struct DownLoadInfo {
    uint32_t generationDuration;
    std::string generationResult;
    uint32_t processDuration;
    std::string processResult;
};

struct DecryptionStatistics {
    uint32_t decryptTimes = 0;
    uint64_t decryptSumSize = 0;
    uint64_t decryptSumDuration = 0;
    uint32_t decryptMaxSize = 0;
    uint32_t decryptMaxDuration = 0;
    uint64_t errorDecryptTimes = 0;
    int32_t errCode = 0;
    std::string errMessage = "no error";
    std::priority_queue<int, std::vector<int>, std::greater<int>> topThree;
};

class DrmEvent {
public:
    static DrmEvent& GetInstance();
    void WriteServiceEvent(std::string eventName, OHOS::HiviewDFX::HiSysEvent::EventType type, DrmServiveInfo &info);
    void WriteLicenseEvent(std::string eventName, OHOS::HiviewDFX::HiSysEvent::EventType type, DrmLicenseInfo &info);
    void WriteCertificateEvent(std::string eventName, OHOS::HiviewDFX::HiSysEvent::EventType type,
        DrmCertificateInfo &info);
    void WriteFaultEvent(std::string eventName, OHOS::HiviewDFX::HiSysEvent::EventType type, DrmFaultInfo &info);
    void WriteDecryptionEvent(std::string eventName, OHOS::HiviewDFX::HiSysEvent::EventType type,
        DrmDecryptionInfo &info);
};

__attribute__((visibility("default"))) void ReportServiceBehaviorEvent(std::string serviceName, std::string action);
__attribute__((visibility("default"))) void ReportLicenseBehaviorEvent(StatisticsInfo statisticsInfo,
    std::string licenseType, DownLoadInfo downLoadInfo);
__attribute__((visibility("default"))) void ReportCertificateBehaviorEvent(StatisticsInfo statisticsInfo,
    DownLoadInfo downLoadInfo, uint32_t callServerTime, uint32_t serverCostDuration, std::string serverResult);
__attribute__((visibility("default"))) void ReportFaultEvent(uint32_t errorCode, std::string errorMesg,
    std::string extraMesg);
__attribute__((visibility("default"))) void ReportDecryptionFaultEvent(int32_t errorCode, std::string errorMesg,
    std::string decryptAlgo, std::string decryptKeyid, std::string decryptIv);
__attribute__((visibility("default"))) DownLoadInfo InitDownLoadInfo(uint32_t generationDuration,
    std::string generationResult, uint32_t processDuration, std::string processResult);
__attribute__((visibility("default"))) void ReportDecryptionStatisticsEvent(uint64_t instanceId, std::string appName,
    DecryptionStatistics &statistics);
} // namespace DrmStandard
} // namespace OHOS
#endif // DRM_DFX_H