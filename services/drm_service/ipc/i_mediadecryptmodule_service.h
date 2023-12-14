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

#ifndef OHOS_DRM_I_MEDIA_DECRYPT_MODULE_SERVICE_H
#define OHOS_DRM_I_MEDIA_DECRYPT_MODULE_SERVICE_H

#include "i_mediadecryptmodule_service.h"
#include "ipc_types.h"
#include "iremote_broker.h"
#include "iremote_proxy.h"
#include "iremote_stub.h"

namespace OHOS {
namespace DrmStandard {
class IMediaDecryptModuleService : public IRemoteBroker {
public:
    struct SubSample {
        int32_t clearHeaderLen;
        int32_t payLoadLen;
    };

    struct Pattern {
        int32_t encryptBlocks;
        int32_t skipBlocks;
    };

    enum CryptAlgorithmType {
        ALGTYPE_UNENCRYPTED = 0,
        ALGTYPE_AES_CTR,
        ALGTYPE_AES_WV,
        ALGTYPE_AES_CBC,
        ALGTYPE_SM4_CBC,
        ALGTYPE_SM4_CTR,
    };

    struct CryptInfo {
        CryptAlgorithmType type;
        std::vector<uint8_t> keyId;
        std::vector<uint8_t> iv;
        Pattern pattern;
        std::vector<SubSample> subSample;
    };

    struct DrmBuffer {
        uint32_t bufferType;
        uint32_t bufferLen;
        uint32_t fd;
    };

    virtual ~IMediaDecryptModuleService() = default;
    virtual int32_t Release() = 0;
    virtual int32_t DecryptMediaData(bool secureDecodrtState, IMediaDecryptModuleService::CryptInfo &cryptInfo,
        uint64_t srcBuffer, uint64_t dstBuffer) = 0;
    DECLARE_INTERFACE_DESCRIPTOR(u"IMediaDecryptModuleService");
};
} // DrmStandard
} // OHOS
#endif // OHOS_DRM_I_MEDIA_DECRYPT_MODULE_SERVICE_H