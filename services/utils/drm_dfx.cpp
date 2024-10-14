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
#include "dump_usage.h"
#include "nlohmann/json.hpp"
#include "native_drm_err.h"
#include "securec.h"
#include "ipc_skeleton.h"
#include "hitrace/tracechain.h"
#include <cstdint>

namespace OHOS {
namespace DrmStandard {
using namespace OHOS::HiviewDFX;
using json = nlohmann::json;
constexpr uint32_t MIN_DECRYPTION_TIMES = 1000;

DrmEvent& DrmEvent::GetInstance()
{
    static DrmEvent instance;
    return instance;
}

int32_t DrmEvent::WriteServiceEvent(std::string eventName, OHOS::HiviewDFX::HiSysEvent::EventType type,
    DrmServiveInfo &info)
{
    int32_t res = DRM_ERR_OK;
    res = HiSysEventWrite(OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MEDIA, eventName, type,
        "MODULE", info.module,
        "SERVICE_NAME", info.serviceName,
        "ACTION", info.action,
        "MEMORY", info.memoryUsage);
    if (res != DRM_ERR_OK) {
        DRM_ERR_LOG("EventWrite failed, res = %d", res);
        return res;
    } else {
        DRM_INFO_LOG("EventWrite success");
    }
    return DRM_ERR_OK;
}

int32_t DrmEvent::WriteLicenseEvent(std::string eventName, OHOS::HiviewDFX::HiSysEvent::EventType type,
    DrmLicenseInfo &info)
{
    int32_t res = DRM_ERR_OK;
    res = HiSysEventWrite(OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MEDIA, eventName, type,
        "MODULE", info.module,
        "APP_NAME", info.appName,
        "INSTANCE_ID", info.instanceId,
        "DRM_NAME", info.drmName,
        "DRM_UUID", info.drmUuid,
        "CLIENT_VERSION", info.clientVersion,
        "LICENSE_TYPE", info.licenseType,
        "GENERATION_DURATION", info.generationDuration,
        "GENERATION_RESULT", info.generationResult,
        "PROCESS_DURATION", info.processDuration,
        "PROCESS_RESULT", info.processResult);
    if (res != DRM_ERR_OK) {
        DRM_ERR_LOG("EventWrite failed, res = %d", res);
        return res;
    } else {
        DRM_INFO_LOG("EventWrite success");
    }
    return DRM_ERR_OK;
}

int32_t DrmEvent::WriteCertificateEvent(std::string eventName, OHOS::HiviewDFX::HiSysEvent::EventType type,
    DrmCertificateInfo &info)
{
    int32_t res = DRM_ERR_OK;
    res = HiSysEventWrite(OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MEDIA, eventName, type,
        "MODULE", info.module,
        "APP_NAME", info.appName,
        "INSTANCE_ID", info.instanceId,
        "DRM_NAME", info.drmName,
        "DRM_UUID", info.drmUuid,
        "CLIENT_VERSION", info.clientVersion,
        "GENERATION_DURATION", info.generationDuration,
        "GENERATION_RESULT", info.generationResult,
        "PROCESS_DURATION", info.processDuration,
        "PROCESS_RESULT", info.processResult,
        "CALL_SERVER_TIME", info.callServerTime,
        "SERVER_COST_DURATION", info.serverCostDuration,
        "SERVER_RESULT", info.serverResult);
    if (res != DRM_ERR_OK) {
        DRM_ERR_LOG("EventWrite failed, res = %d", res);
        return res;
    } else {
        DRM_INFO_LOG("EventWrite success");
    }
    return DRM_ERR_OK;
}

int32_t DrmEvent::WriteFaultEvent(std::string eventName, OHOS::HiviewDFX::HiSysEvent::EventType type, DrmFaultInfo &info)
{
    int32_t res = DRM_ERR_OK;
    res = HiSysEventWrite(OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MEDIA, eventName, type,
        "MODULE", info.module,
        "APP_NAME", info.appName,
        "INSTANCE_ID", info.instanceId,
        "ERROR_CODE", info.errorCode,
        "ERROR_MESG", info.errorMesg,
        "EXTRA_MESG", info.extraMesg);
    if (res != DRM_ERR_OK) {
        DRM_ERR_LOG("EventWrite failed, res = %d", res);
        return res;
    } else {
        DRM_INFO_LOG("EventWrite success");
    }
    return DRM_ERR_OK;
}

int32_t DrmEvent::WriteDecryptionEvent(std::string eventName, OHOS::HiviewDFX::HiSysEvent::EventType type,
    DrmDecryptionInfo &info)
{
    int32_t res = DRM_ERR_OK;

    res = HiSysEventWrite(OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MEDIA, eventName, type,
        "MODULE", info.module,
        "APP_NAME", info.appName,
        "INSTANCE_ID", info.instanceId,
        "ERROR_CODE", info.errorCode,
        "ERROR_MESG", info.errorMesg,
        "DECRYPT_ALGO", info.decryptAlgo,
        "DECRYPT_KEYID", info.decryptKeyid,
        "DECRYPT_IV", info.decryptIv);
    if (res != DRM_ERR_OK) {
        DRM_ERR_LOG("EventWrite failed, res = %d", res);
        return res;
    } else {
        DRM_INFO_LOG("EventWrite success");
    }
    return DRM_ERR_OK;
}

int32_t ReportServiceBehaviorEvent(std::string serviceName, std::string action)
{
    DrmEvent event;
    OHOS::HiviewDFX::DumpUsage dumpUse;
    uint32_t memoryUsage = dumpUse.GetPss(getpid());
    struct DrmServiveInfo drmServiveInfo = {
        "DRM_SERVICE",
        0,
        serviceName,
        action,
        memoryUsage,
    };
    return event.WriteServiceEvent("DRM_SERVICE_INFO", OHOS::HiviewDFX::HiSysEvent::EventType::BEHAVIOR, drmServiveInfo);
}

int32_t ReportLicenseBehaviorEvent(StatisticsInfo statisticsInfo, std::string licenseType, DownLoadInfo downLoadInfo)
{
    DrmEvent event;
    struct DrmLicenseInfo drmLicenseInfo = {
        "DRM_SERVICE",
        0,
        GetClientBundleName(IPCSkeleton::GetCallingUid()),
        std::to_string(HiTraceChain::GetId().GetChainId()),
        statisticsInfo.pluginName,
        statisticsInfo.pluginUuid,
        statisticsInfo.versionName,
        licenseType,
        downLoadInfo.generationDuration,
        downLoadInfo.generationResult,
        downLoadInfo.processDuration,
        downLoadInfo.processResult,
    };
    return event.WriteLicenseEvent("DRM_LICENSE_DOWNLOAD_INFO", OHOS::HiviewDFX::HiSysEvent::EventType::BEHAVIOR,
        drmLicenseInfo);
}

int32_t ReportCertificateBehaviorEvent(StatisticsInfo statisticsInfo, DownLoadInfo downLoadInfo, uint32_t callServerTime,
    uint32_t serverCostDuration, std::string serverResult)
{
    DrmEvent event;
    struct DrmCertificateInfo drmCertificateInfo = {
        "DRM_SERVICE",
        0,
        GetClientBundleName(IPCSkeleton::GetCallingUid()),
        std::to_string(HiTraceChain::GetId().GetChainId()),
        statisticsInfo.pluginName,
        statisticsInfo.pluginUuid,
        statisticsInfo.versionName,
        downLoadInfo.generationDuration,
        downLoadInfo.generationResult,
        downLoadInfo.processDuration,
        downLoadInfo.processResult,
        callServerTime,
        serverCostDuration,
        serverResult,
    };
    return event.WriteCertificateEvent("DRM_CERTIFICATE_DOWNLOAD_INFO", OHOS::HiviewDFX::HiSysEvent::EventType::BEHAVIOR,
        drmCertificateInfo);
}

int32_t ReportFaultEvent(uint32_t errorCode, std::string errorMesg, std::string extraMesg)
{
    DrmEvent event;
    struct DrmFaultInfo drmFaultInfo = {
        "DRM_SERVICE",
        GetClientBundleName(IPCSkeleton::GetCallingUid()),
        std::to_string(HiTraceChain::GetId().GetChainId()),
        errorCode,
        errorMesg,
        extraMesg,
    };
    return event.WriteFaultEvent("DRM_COMMON_FAILURE", OHOS::HiviewDFX::HiSysEvent::EventType::FAULT, drmFaultInfo);
}

int32_t ReportDecryptionFaultEvent(int32_t errorCode, std::string errorMesg, std::string decryptAlgo,
    std::string decryptKeyid, std::string decryptIv)
{
    DrmEvent event;
    struct DrmDecryptionInfo drmDecryptionInfo = {
        "DRM_SERVICE",
        GetClientBundleName(IPCSkeleton::GetCallingUid()),
        std::to_string(HiTraceChain::GetId().GetChainId()),
        errorCode,
        errorMesg,
        decryptAlgo,
        decryptKeyid,
        decryptIv,
    };
    return event.WriteDecryptionEvent("DRM_DECRYPTION_FAILURE", OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        drmDecryptionInfo);
}

DownLoadInfo InitDownLoadInfo(uint32_t generationDuration, std::string generationResult, uint32_t processDuration,
    std::string processResult)
{
    struct DownLoadInfo downLoadInfo = {
        generationDuration,
        generationResult,
        processDuration,
        processResult,
    };
    return downLoadInfo;
}

void ReportDecryptionStatisticsEvent(uint64_t instanceId, std::string appName,
    DecryptionStatistics &statistics)
{
    DRM_INFO_LOG("ReportDecryptionStatisticsEvent.");

    int32_t res = DRM_ERR_OK;
    if (statistics.decryptTimes < MIN_DECRYPTION_TIMES) {
        return;
    }
    uint32_t avgSize = statistics.decryptSumSize / statistics.decryptTimes;
    uint32_t avgDuration = statistics.decryptSumDuration / statistics.decryptTimes;

    res = HiSysEventWrite(OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MEDIA,
        "DRM_STATISTICS",
        HiviewDFX::HiSysEvent::EventType::STATISTIC,
        "APP_NAME", appName,
        "INSTANCE_ID", instanceId,
        "ERROR_CODE", statistics.errCode,
        "ERROR_MESG", statistics.errMessage,
        "DECRYPT_TIMES", statistics.decryptTimes,
        "DECRYPT_AVG_SIZE", avgSize,
        "DECRYPT_AVG_DURATION", avgDuration,
        "DECRYPT_MAX_SIZE", statistics.decryptMaxSize,
        "DECRYPT_MAX_DURATION", statistics.decryptMaxDuration);
    if (res != DRM_ERR_OK) {
        DRM_ERR_LOG("DRM_STATISTICS Event Write failed, res = %d", res);
    } else {
        DRM_INFO_LOG("DRM_STATISTICS Event Write success");
    }
    DRM_INFO_LOG("ReportDecryptionStatisticsEvent done!");
}
} // namespace DrmStandard
} // namespace OHOS