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
#include "ipc_skeleton.h"
#include "xcollie/xcollie.h"
#include "xcollie/xcollie_define.h"
#include "remote_request_code.h"

namespace OHOS {
namespace DrmStandard {
MediaDecryptModuleServiceStub::MediaDecryptModuleServiceStub()
{
}

MediaDecryptModuleServiceStub::~MediaDecryptModuleServiceStub()
{
    DRM_INFO_LOG("~MediaDecryptModuleServiceStub");
}

void MediaDecryptModuleServiceStub::MediaDecryptModuleClientDied(pid_t pid)
{
    DRM_ERR_LOG("MediaDecryptModule client has died, pid:%{public}d", pid);
    std::lock_guard<std::recursive_mutex> lock(decryptModuleStubMutex_);
    if (clientListener_ != nullptr && clientListener_->AsObject() != nullptr && deathRecipient_ != nullptr) {
        DRM_DEBUG_LOG("MediaDecryptModuleServiceStub release listener!");
        (void)clientListener_->AsObject()->RemoveDeathRecipient(deathRecipient_);
        deathRecipient_ = nullptr;
        clientListener_ = nullptr;
    }
}

int32_t MediaDecryptModuleServiceStub::OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_DEBUG_LOG("OnRemoteRequest, cmd = %{public}u", code);
    if (data.ReadInterfaceToken() != GetDescriptor()) {
        DRM_ERR_LOG("MediaDecryptModuleServiceStub: ReadInterfaceToken failed.");
        return -1;
    }
    switch (code) {
        case DECRYPT_MODULE_DECRYPT_DATA: {
            DRM_DEBUG_LOG("DECRYPT_MODULE_DECRYPT_DATA enter.");
            IMediaDecryptModuleService::CryptInfo cryptInfo;
            bool secureDecodrtState = data.ReadBool();
            cryptInfo.type = (OHOS::DrmStandard::IMediaDecryptModuleService::CryptAlgorithmType)data.ReadUint32();
            uint32_t keyIdSize = data.ReadUint32();
            DRM_CHECK_AND_RETURN_RET_LOG(keyIdSize < KEYID_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
                "The size of keyId is too large.");
            if (keyIdSize != 0) {
                const uint8_t *keyIdBuf = static_cast<const uint8_t *>(data.ReadUnpadBuffer(keyIdSize));
                if (keyIdBuf == nullptr) {
                    DRM_ERR_LOG("DECRYPT_MODULE_DECRYPT_DATA read keyId failed.");
                    return IPC_STUB_WRITE_PARCEL_ERR;
                }
                cryptInfo.keyId.assign(keyIdBuf, keyIdBuf + keyIdSize);
            }
            uint32_t ivSize = data.ReadUint32();
            DRM_CHECK_AND_RETURN_RET_LOG(ivSize < IV_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
                "The size of iv is too large.");
            if (ivSize != 0) {
                const uint8_t *ivBuf = static_cast<const uint8_t *>(data.ReadUnpadBuffer(ivSize));
                if (ivBuf == nullptr) {
                    DRM_ERR_LOG("MediaDecryptModuleServiceStub DECRYPT_MODULE_DECRYPT_DATA read ivSize failed.");
                    return IPC_STUB_WRITE_PARCEL_ERR;
                }
                cryptInfo.iv.assign(ivBuf, ivBuf + ivSize);
            }
            cryptInfo.pattern.encryptBlocks = data.ReadUint32();
            cryptInfo.pattern.skipBlocks = data.ReadUint32();
            uint32_t subSampleNumber = data.ReadUint32();
            DRM_CHECK_AND_RETURN_RET_LOG(subSampleNumber < SUBSAMPLE_MAX_NUM, DRM_INNER_ERR_MEMORY_ERROR,
                "The number of subSample is too large.");
            cryptInfo.subSample.resize(subSampleNumber);
            for (uint32_t i = 0; i < subSampleNumber; i++) {
                cryptInfo.subSample[i].clearHeaderLen = data.ReadUint32();
                cryptInfo.subSample[i].payLoadLen = data.ReadUint32();
            }
            IMediaDecryptModuleService::DrmBuffer srcBuffer;
            IMediaDecryptModuleService::DrmBuffer dstBuffer;
            srcBuffer.bufferType  = data.ReadUint32();
            srcBuffer.fd  = data.ReadFileDescriptor();
            srcBuffer.bufferLen  = data.ReadUint32();
            srcBuffer.allocLen  = data.ReadUint32();
            srcBuffer.filledLen  = data.ReadUint32();
            srcBuffer.offset  = data.ReadUint32();
            srcBuffer.sharedMemType  = data.ReadUint32();
            dstBuffer.bufferType  = data.ReadUint32();
            dstBuffer.fd  = data.ReadFileDescriptor();
            dstBuffer.bufferLen  = data.ReadUint32();
            dstBuffer.allocLen  = data.ReadUint32();
            dstBuffer.filledLen  = data.ReadUint32();
            dstBuffer.offset  = data.ReadUint32();
            dstBuffer.sharedMemType  = data.ReadUint32();
            int32_t ret = DecryptMediaData(secureDecodrtState, cryptInfo, srcBuffer, dstBuffer);
            if (ret != 0) {
                DRM_ERR_LOG("DecryptMediaData faild.");
                return ret;
            }
            return ret;
        }
        case DECRYPT_MODULE_RELEASE: {
            DRM_INFO_LOG("MediaDecryptModuleServiceStub DECRYPT_MODULE_RELEASE enter.");
            int32_t ret = Release();
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