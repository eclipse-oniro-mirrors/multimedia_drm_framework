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

#include "media_decrypt_module_service_stub.h"
#include "drm_error_code.h"
#include "drm_log.h"
#include "drm_napi_utils.h"
#include "ipc_skeleton.h"
#include "xcollie/xcollie.h"
#include "xcollie/xcollie_define.h"
#include "remote_request_code.h"

namespace OHOS {
namespace DrmStandard {
MediaDecryptModuleServiceStub::MediaDecryptModuleServiceStub()
{
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " Instances create", (POINTER_MASK & reinterpret_cast<uintptr_t>(this)));
}

MediaDecryptModuleServiceStub::~MediaDecryptModuleServiceStub()
{
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " Instances destroy", (POINTER_MASK & reinterpret_cast<uintptr_t>(this)));
}

int32_t MediaDecryptModuleServiceStub::OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("OnRemoteRequest, cmd = %{public}u", code);
    if (data.ReadInterfaceToken() != GetDescriptor()) {
        DRM_ERR_LOG("MediaDecryptModuleServiceStub: ReadInterfaceToken failed");
        return -1;
    }
    switch (code) {
        case DECRYPT_MODULE_DECRYPT_DATA: {
            DRM_INFO_LOG("MediaDecryptModuleServiceStub DECRYPT_MODULE_DECRYPT_DATA enter.");
            IMediaDecryptModuleService::CryptInfo cryptInfo;
            bool secureDecodrtState = data.ReadBool();
            cryptInfo.type = (OHOS::DrmStandard::IMediaDecryptModuleService::CryptAlgorithmType)data.ReadUint32();
            uint32_t keyIdSize = data.ReadUint32();
            for (uint32_t i = 0; i < keyIdSize; i++) {
                cryptInfo.keyId.push_back(data.ReadUint8());
            }
            uint32_t ivSize = data.ReadUint32();
            for (uint32_t i = 0; i < ivSize; i++) {
                cryptInfo.iv.push_back(data.ReadUint8());
            }
            cryptInfo.pattern.encryptBlocks = data.ReadUint32();
            cryptInfo.pattern.skipBlocks = data.ReadUint32();
            uint32_t subSampleNumber = data.ReadUint32();
            cryptInfo.subSample.resize(subSampleNumber);
            for (int32_t i = 0; i < subSampleNumber; i++) {
                cryptInfo.subSample[i].clearHeaderLen = data.ReadUint32();
                cryptInfo.subSample[i].payLoadLen = data.ReadUint32();
            }
            uint64_t srcBuffer = data.ReadFileDescriptor();
            uint64_t dstBuffer = data.ReadFileDescriptor();
            int32_t ret = DecryptMediaData(secureDecodrtState, cryptInfo, srcBuffer, dstBuffer);
            if (ret != 0) {
                DRM_ERR_LOG("DecryptMediaData faild.");
                return ret;
            }
            DRM_INFO_LOG("MediaDecryptModuleServiceStub DECRYPT_MODULE_DECRYPT_DATA exit.");
            return ret;
        }
        case DECRYPT_MODULE_RELEASE: {
            DRM_INFO_LOG("MediaDecryptModuleServiceStub DECRYPT_MODULE_RELEASE enter.");
            int32_t ret = Release();
            DRM_INFO_LOG("MediaDecryptModuleServiceStub DECRYPT_MODULE_RELEASE exit.");
            return ret;
        }
        default: {
            DRM_ERR_LOG("default case, need check Decrypt Module");
            return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
        }
    }
    return -1;
}
} // DrmStandard
} // OHOS