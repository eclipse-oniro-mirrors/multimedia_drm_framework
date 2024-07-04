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
#include "media_decrypt_module_service_stub.h"
#include "v1_0/media_decrypt_module_proxy.h"

namespace OHOS {
namespace DrmStandard {
using namespace OHOS::HDI::Drm::V1_0;
using namespace OHOS::HDI;
struct DecryptStatustics {
    uint32_t decryptTimes = 0;
    uint64_t decryptSumSize = 0;
    uint64_t decryptSumDuration = 0;
    uint32_t decryptMaxSize = 0;
    uint32_t decryptMaxDuration = 0;
};

class MediaDecryptModuleService : public MediaDecryptModuleServiceStub {
public:
    MediaDecryptModuleService(sptr<OHOS::HDI::Drm::V1_0::IMediaDecryptModule> hdiMediaDecryptModule);
    ~MediaDecryptModuleService();
    int32_t Release() override;
    int32_t DecryptMediaData(bool secureDecodrtState, IMediaDecryptModuleService::CryptInfo &cryptInfo,
        IMediaDecryptModuleService::DrmBuffer &srcBuffer, IMediaDecryptModuleService::DrmBuffer &dstBuffer) override;

private:
    void SetCryptInfo(OHOS::HDI::Drm::V1_0::CryptoInfo &cryptInfoTmp,
        IMediaDecryptModuleService::CryptInfo &cryptInfo, uint32_t &bufLen);
    void SetDrmBufferInfo(OHOS::HDI::Drm::V1_0::DrmBuffer* drmSrcBuffer, OHOS::HDI::Drm::V1_0::DrmBuffer* drmDstBuffer,
        IMediaDecryptModuleService::DrmBuffer &srcBuffer, IMediaDecryptModuleService::DrmBuffer &dstBuffer,
        uint32_t bufLen);
    void ReportDecryptionStatisticEvent();
    std::mutex moduleLock_;
    sptr<OHOS::HDI::Drm::V1_0::IMediaDecryptModule> hdiMediaDecryptModule_;
    DecryptStatustics decryptStatustics_;
    int32_t errCode_ = 0;
    std::string errMessage_;
    uint64_t instanceId_;
};
} // DrmStandard
} // OHOS

#endif // OHOS_DRM_MediaDecryptModuleService_H_