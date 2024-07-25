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

#include <memory>
#include <unistd.h>
#include <securec.h>
#include <algorithm>
#include "ashmem.h"
#include "system_ability_definition.h"
#include "mem_mgr_client.h"
#include "mem_mgr_proxy.h"
#include "drm_trace.h"
#include "drm_dfx_utils.h"
#include "drm_error_code.h"
#include "hitrace/tracechain.h"
#include "ipc_skeleton.h"
#include "media_decrypt_module_service.h"

namespace OHOS {
namespace DrmStandard {

const uint32_t TOP_THREE_SIZE = 3;
const uint32_t TOP_ONE = 0;
const uint32_t TOP_SEC = 1;
const uint32_t TOP_THD = 2;

MediaDecryptModuleService::MediaDecryptModuleService(
    sptr<OHOS::HDI::Drm::V1_0::IMediaDecryptModule> hdiMediaDecryptModule)
{
    DRM_INFO_LOG("0x%{public}06" PRIXPTR " Instances create.", FAKE_POINTER(this));
    hdiMediaDecryptModule_ = hdiMediaDecryptModule;
}

MediaDecryptModuleService::MediaDecryptModuleService(
    sptr<OHOS::HDI::Drm::V1_0::IMediaDecryptModule> hdiMediaDecryptModule,
    StatisticsInfo statisticsInfo)
{
    DRM_INFO_LOG("0x%{public}06" PRIXPTR " Instances create.", FAKE_POINTER(this));
    std::lock_guard<std::mutex> lock(moduleLock_);
    hdiMediaDecryptModule_ = hdiMediaDecryptModule;
    statisticsInfo_ = statisticsInfo;
    instanceId_ = HiTraceChain::GetId().GetChainId();
}

MediaDecryptModuleService::~MediaDecryptModuleService()
{
    DRM_INFO_LOG("0x%{public}06" PRIXPTR " Instances destroy.", FAKE_POINTER(this));
    std::lock_guard<std::mutex> lock(moduleLock_);
    if (hdiMediaDecryptModule_ != nullptr) {
        Release();
    }
    std::lock_guard<std::mutex> statisticsLock(statisticsMutex_);
    ReportDecryptionStatisticsEvent(instanceId_, statisticsInfo_.bundleName, decryptStatistics_);
}

int32_t MediaDecryptModuleService::Release()
{
    DrmTrace trace("Release");
    DRM_INFO_LOG("Release enter.");
    int32_t errCode = DRM_OK;
    if (hdiMediaDecryptModule_ != nullptr) {
        DRM_INFO_LOG("hdiMediaDecryptModule_ call Close");
        hdiMediaDecryptModule_ = nullptr;
    }
    return errCode;
}

int32_t MediaDecryptModuleService::DecryptMediaData(bool secureDecodrtState,
    IMediaDecryptModuleService::CryptInfo &cryptInfo, IMediaDecryptModuleService::DrmBuffer &srcBuffer,
    IMediaDecryptModuleService::DrmBuffer &dstBuffer)
{
    DrmTrace trace("DecryptMediaData");
    DRM_INFO_LOG("DecryptMediaData enter.");
    int32_t ret = DRM_OK;
    uint32_t bufLen = 0;
    OHOS::HDI::Drm::V1_0::CryptoInfo cryptInfoTmp;
    SetCryptInfo(cryptInfoTmp, cryptInfo, bufLen);
    OHOS::HDI::Drm::V1_0::DrmBuffer drmSrcBuffer;
    OHOS::HDI::Drm::V1_0::DrmBuffer drmDstBuffer;
    memset_s(&drmSrcBuffer, sizeof(drmSrcBuffer), 0, sizeof(drmSrcBuffer));
    memset_s(&drmDstBuffer, sizeof(drmSrcBuffer), 0, sizeof(drmDstBuffer));
    SetDrmBufferInfo(&drmSrcBuffer, &drmDstBuffer, srcBuffer, dstBuffer, bufLen);
    auto timeBefore = std::chrono::system_clock::now();
    std::lock_guard<std::mutex> lock(moduleLock_);
    ret = hdiMediaDecryptModule_->DecryptMediaData(secureDecodrtState, cryptInfoTmp, drmSrcBuffer, drmDstBuffer);
    uint32_t decryptDuration = CalculateTimeDiff(timeBefore, std::chrono::system_clock::now());
    UpdateDecryptionStatistics(ret, bufLen, decryptDuration);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("DecryptMediaData failed.");
        ReportDecryptionFaultEvent(ret, "DecryptMediaData failed",
            std::to_string(static_cast<int32_t>(cryptInfoTmp.type)),
            CastToHexString(cryptInfoTmp.keyId), CastToHexString(cryptInfoTmp.iv));
    }
    (void)::close(srcBuffer.fd);
    (void)::close(dstBuffer.fd);
    return ret;
}

void MediaDecryptModuleService::SetCryptInfo(OHOS::HDI::Drm::V1_0::CryptoInfo &cryptInfoTmp,
    IMediaDecryptModuleService::CryptInfo &cryptInfo, uint32_t &bufLen)
{
    cryptInfoTmp.type = (OHOS::HDI::Drm::V1_0::CryptoAlgorithmType)cryptInfo.type;
    cryptInfoTmp.keyId.assign(cryptInfo.keyId.begin(), cryptInfo.keyId.end());
    cryptInfoTmp.iv.assign(cryptInfo.iv.begin(), cryptInfo.iv.end());
    cryptInfoTmp.pattern.encryptBlocks = cryptInfo.pattern.encryptBlocks;
    cryptInfoTmp.pattern.skipBlocks = cryptInfo.pattern.skipBlocks;
    cryptInfoTmp.subSamples.resize(cryptInfo.subSample.size());
    for (size_t i = 0; i < cryptInfo.subSample.size(); i++) {
        cryptInfoTmp.subSamples[i].clearHeaderLen = cryptInfo.subSample[i].clearHeaderLen;
        bufLen += cryptInfoTmp.subSamples[i].clearHeaderLen;
        cryptInfoTmp.subSamples[i].payLoadLen = cryptInfo.subSample[i].payLoadLen;
        bufLen += cryptInfoTmp.subSamples[i].payLoadLen;
    }
}

void MediaDecryptModuleService::SetDrmBufferInfo(OHOS::HDI::Drm::V1_0::DrmBuffer* drmSrcBuffer,
    OHOS::HDI::Drm::V1_0::DrmBuffer* drmDstBuffer, IMediaDecryptModuleService::DrmBuffer &srcBuffer,
    IMediaDecryptModuleService::DrmBuffer &dstBuffer, uint32_t bufLen)
{
    DRM_INFO_LOG("SetDrmBufferInfo");
    drmSrcBuffer->bufferType = srcBuffer.bufferType;
    drmSrcBuffer->fd = srcBuffer.fd;
    drmSrcBuffer->bufferLen = bufLen;
    drmSrcBuffer->allocLen = srcBuffer.allocLen;
    drmSrcBuffer->filledLen = srcBuffer.filledLen;
    drmSrcBuffer->offset = srcBuffer.offset;
    drmSrcBuffer->fd = srcBuffer.fd;

    drmDstBuffer->bufferType = dstBuffer.bufferType;
    drmDstBuffer->fd = dstBuffer.fd;
    drmDstBuffer->bufferLen = bufLen;
    drmDstBuffer->allocLen = dstBuffer.allocLen;
    drmDstBuffer->filledLen = dstBuffer.filledLen;
    drmDstBuffer->offset = dstBuffer.offset;
    drmDstBuffer->sharedMemType = dstBuffer.sharedMemType;
}

void MediaDecryptModuleService::UpdateDecryptionStatistics(int32_t decryptionResult,
    uint32_t bufLen, uint32_t curDuration)
{
    std::lock_guard<std::mutex> statisticsLock(statisticsMutex_);
    decryptStatistics_.topThree.push(curDuration);
    if (decryptStatistics_.topThree.size() > TOP_THREE_SIZE) {
        decryptStatistics_.topThree.pop();
    }

    decryptStatistics_.decryptMaxSize = std::max(decryptStatistics_.decryptMaxSize, bufLen);
    decryptStatistics_.decryptMaxDuration = std::max(decryptStatistics_.decryptMaxDuration, curDuration);

    if (decryptionResult != DRM_OK) {
        decryptStatistics_.errorDecryptTimes++;
    }
    if (decryptStatistics_.decryptTimes == UINT32_MAX) {
        decryptStatistics_.decryptTimes = 0;
        decryptStatistics_.decryptSumSize = 0;
        decryptStatistics_.decryptSumDuration = 0;
    }
    decryptStatistics_.decryptTimes++;
    decryptStatistics_.decryptSumSize += bufLen;
    decryptStatistics_.decryptSumDuration += curDuration;
}

const std::string MediaDecryptModuleService::GetTopThreeDecryptionDurations()
{
    DRM_INFO_LOG("GetTopThreeDecryptionDurations");
    std::vector<uint32_t> topThreeDurations(TOP_THREE_SIZE, 0);
    std::lock_guard<std::mutex> statisticsLock(statisticsMutex_);
    uint32_t currentTopThreeSize = decryptStatistics_.topThree.size();
    for (uint32_t i = 0; i < currentTopThreeSize; i++) {
        uint32_t tmp = decryptStatistics_.topThree.top();
        decryptStatistics_.topThree.pop();
        topThreeDurations[i] = tmp;
    }
    for (uint32_t i = 0; i < currentTopThreeSize; i++) {
        decryptStatistics_.topThree.push(topThreeDurations[i]);
    }
    return std::to_string(topThreeDurations[TOP_ONE]) + " " +
           std::to_string(topThreeDurations[TOP_SEC]) + " " +
           std::to_string(topThreeDurations[TOP_THD]) + "\n";
}

std::string MediaDecryptModuleService::GetDumpInfo()
{
    DRM_INFO_LOG("GetDumpInfo");
    std::string dumpInfo = "Total Decryption Times: " + std::to_string(decryptStatistics_.decryptTimes) + "\n"
                           "Error Decryption Times: " + std::to_string(decryptStatistics_.errorDecryptTimes) + "\n"
                           "Top3 Decryption Duration: " + GetTopThreeDecryptionDurations();
    return dumpInfo;
}

} // DrmStandard
} // OHOS