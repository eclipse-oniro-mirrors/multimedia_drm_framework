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
#ifndef OHOS_DRM_MEDIADECRYPTMODULESERVICE_H
#define OHOS_DRM_MEDIADECRYPTMODULESERVICE_H

#include <iostream>
#include <refbase.h>
#include "drm_host_manager.h"
#include "drm_log.h"
#include "drm_dfx.h"
#include "drm_dfx_utils.h"
#include "media_decrypt_module_service_stub.h"
#include "v1_0/media_decrypt_module_proxy.h"

namespace OHOS {
namespace DrmStandard {
using namespace OHOS::HDI::Drm::V1_0;
using namespace OHOS::HDI;

class MediaDecryptModuleService : public MediaDecryptModuleServiceStub {
public:
    MediaDecryptModuleService(sptr<OHOS::HDI::Drm::V1_0::IMediaDecryptModule> hdiMediaDecryptModule);
    MediaDecryptModuleService(sptr<OHOS::HDI::Drm::V1_0::IMediaDecryptModule> hdiMediaDecryptModule,
        StatisticsInfo statisticsInfo);
    ~MediaDecryptModuleService();
    int32_t Release() override;
    int32_t DecryptMediaData(bool secureDecodrtState, IMediaDecryptModuleService::CryptInfo &cryptInfo,
        IMediaDecryptModuleService::DrmBuffer &srcBuffer, IMediaDecryptModuleService::DrmBuffer &dstBuffer) override;
    std::string GetDumpInfo();

private:
    void SetCryptInfo(OHOS::HDI::Drm::V1_0::CryptoInfo &cryptInfoTmp,
        IMediaDecryptModuleService::CryptInfo &cryptInfo, uint32_t &bufLen);
    void SetDrmBufferInfo(OHOS::HDI::Drm::V1_0::DrmBuffer* drmSrcBuffer, OHOS::HDI::Drm::V1_0::DrmBuffer* drmDstBuffer,
        IMediaDecryptModuleService::DrmBuffer &srcBuffer, IMediaDecryptModuleService::DrmBuffer &dstBuffer,
        uint32_t bufLen);
    void UpdateDecryptionStatistics(int32_t decryptionResult, uint32_t bufLen, uint32_t curDuration);
    const std::string GetTopThreeDecryptionDurations();

    std::mutex moduleLock_;
    sptr<OHOS::HDI::Drm::V1_0::IMediaDecryptModule> hdiMediaDecryptModule_;
    StatisticsInfo statisticsInfo_;
    std::mutex statisticsMutex_;
    DecryptionStatistics decryptStatistics_;
    uint64_t instanceId_;
};
} // DrmStandard
} // OHOS

#endif // OHOS_DRM_MediaDecryptModuleService_H_