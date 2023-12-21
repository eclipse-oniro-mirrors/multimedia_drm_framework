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
#include "ashmem.h"
#include "ipc_skeleton.h"
#include "media_decrypt_module_service.h"

namespace OHOS {
namespace DrmStandard {
MediaDecryptModuleService::MediaDecryptModuleService(
    sptr<OHOS::HDI::Drm::V1_0::IMediaDecryptModule> hdiMediaDecryptModule)
{
    DRM_DEBUG_LOG("MediaDecryptModuleService::MediaDecryptModuleService");
    hdiMediaDecryptModule_ = hdiMediaDecryptModule;
}

MediaDecryptModuleService::~MediaDecryptModuleService()
{
    DRM_INFO_LOG("MediaDecryptModuleService::~MediaDecryptModuleService enter.");
    std::lock_guard<std::mutex> lock(moduleLock_);
    if (hdiMediaDecryptModule_ != nullptr) {
        Release();
    }
    DRM_INFO_LOG("MediaDecryptModuleService::~MediaDecryptModuleService exit.");
}

int32_t MediaDecryptModuleService::Release()
{
    DRM_INFO_LOG("MediaDecryptModuleService::Release enter.");
    int32_t errCode = DRM_OK;
    if (hdiMediaDecryptModule_ != nullptr) {
        DRM_INFO_LOG("hdiMediaDecryptModule_ call Close");
        hdiMediaDecryptModule_ = nullptr;
    }
    DRM_INFO_LOG("MediaDecryptModuleService::Release exit.");
    return errCode;
}

int32_t MediaDecryptModuleService::DecryptMediaData(bool secureDecodrtState,
    IMediaDecryptModuleService::CryptInfo &cryptInfo, uint64_t srcBuffer, uint64_t dstBuffer)
{
    DRM_INFO_LOG("MediaDecryptModuleService::DecryptMediaData enter.");
    int32_t ret = DRM_OK;
    int32_t bufLen = 0;
    OHOS::HDI::Drm::V1_0::CryptoInfo cryptInfoTmp;
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

    OHOS::HDI::Drm::V1_0::DrmBuffer drmSrcBuffer;
    OHOS::HDI::Drm::V1_0::DrmBuffer drmDstBuffer;
    memset_s(&drmSrcBuffer, sizeof(drmSrcBuffer), 0, sizeof(drmSrcBuffer));
    memset_s(&drmDstBuffer, sizeof(drmSrcBuffer), 0, sizeof(drmDstBuffer));
    drmSrcBuffer.fd = srcBuffer;
    drmDstBuffer.fd = dstBuffer;
    drmSrcBuffer.bufferLen = bufLen;
    drmDstBuffer.bufferLen = bufLen;
    ret = hdiMediaDecryptModule_->DecryptMediaData(secureDecodrtState, cryptInfoTmp, drmSrcBuffer, drmDstBuffer);
    if (ret != DRM_OK) {
        (void)::close(srcBuffer);
        (void)::close(dstBuffer);
        DRM_ERR_LOG("MediaDecryptModuleService::DecryptMediaData failed");
        return ret;
    }
    (void)::close(srcBuffer);
    (void)::close(dstBuffer);
    DRM_INFO_LOG("MediaDecryptModuleService::DecryptMediaData exit.");
    return ret;
}
} // DrmStandard
} // OHOS