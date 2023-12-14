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

#include "ashmem.h"
#include "media_decrypt_module_service_proxy.h"

namespace OHOS {
namespace DrmStandard {
MediaDecryptModuleServiceProxy::MediaDecryptModuleServiceProxy(const sptr<IRemoteObject> &impl)
    : IRemoteProxy<IMediaDecryptModuleService>(impl)
{
    DRM_DEBUG_LOG("MediaDecryptModuleServiceProxy::MediaKeySystemServiceProxy Initialized.");
}

int32_t MediaDecryptModuleServiceProxy::Release()
{
    DRM_INFO_LOG("MediaDecryptModuleServiceProxy::Release enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaDecryptModuleServiceProxy Release Write interface token failed");
        return IPC_PROXY_ERR;
    }

    int32_t error = Remote()->SendRequest(DECRYPT_MODULE_RELEASE, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaDecryptModuleServiceProxy::Release failed, error: %{public}d", error);
        return error;
    }

    DRM_INFO_LOG("MediaDecryptModuleServiceProxy::Release exit.");
    return reply.ReadInt32();
}

int32_t MediaDecryptModuleServiceProxy::DecryptMediaData(bool secureDecodrtState,
    IMediaDecryptModuleService::CryptInfo &cryptInfo, uint64_t srcBuffer, uint64_t dstBuffer)
{
    DRM_INFO_LOG("MediaDecryptModuleServiceProxy::DecryptMediaData enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    DRM_CHECK_AND_RETURN_RET_LOG(data.WriteInterfaceToken(GetDescriptor()), IPC_PROXY_ERR,
        "MediaDecryptModuleServiceProxy DecryptMediaData Write interface token failed");

    DRM_CHECK_AND_RETURN_RET_LOG(data.WriteBool(secureDecodrtState), IPC_PROXY_ERR,
        "MediaDecryptModuleServiceProxy DecryptMediaData Write secureDecodrtState failed");

    DRM_CHECK_AND_RETURN_RET_LOG(data.WriteUint32(cryptInfo.type), IPC_PROXY_ERR,
        "MediaDecryptModuleServiceProxy DecryptMediaData Write cryptInfo.type failed");

    DRM_CHECK_AND_RETURN_RET_LOG(data.WriteUint32(cryptInfo.keyId.size()), IPC_PROXY_ERR,
        "MediaDecryptModuleServiceProxy DecryptMediaData Write cryptInfo.keyId size failed");
    for (auto keyId : cryptInfo.keyId) {
        DRM_CHECK_AND_RETURN_RET_LOG(data.WriteUint8(keyId), IPC_PROXY_ERR,
            "MediaDecryptModuleServiceProxy DecryptMediaData Write cryptInfo.keyId failed");
    }

    DRM_CHECK_AND_RETURN_RET_LOG(data.WriteUint32(cryptInfo.iv.size()), IPC_PROXY_ERR,
        "MediaDecryptModuleServiceProxy DecryptMediaData Write cryptInfo.iv size failed");

    for (auto iv : cryptInfo.iv) {
        DRM_CHECK_AND_RETURN_RET_LOG(data.WriteUint8(iv), IPC_PROXY_ERR,
            "MediaDecryptModuleServiceProxy DecryptMediaData Write cryptInfo.iv failed");
    }

    DRM_CHECK_AND_RETURN_RET_LOG(data.WriteUint32(cryptInfo.pattern.encryptBlocks), IPC_PROXY_ERR,
        "MediaDecryptModuleServiceProxy DecryptMediaData Write cryptInfo.pattern.encryptBlocks failed");

    DRM_CHECK_AND_RETURN_RET_LOG(data.WriteUint32(cryptInfo.pattern.skipBlocks), IPC_PROXY_ERR,
        "MediaDecryptModuleServiceProxy DecryptMediaData Write cryptInfo.pattern.skipBlocks failed");

    DRM_CHECK_AND_RETURN_RET_LOG(data.WriteUint32(cryptInfo.subSample.size()), IPC_PROXY_ERR,
        "MediaDecryptModuleServiceProxy DecryptMediaData Write cryptInfo.subSample size failed");
    for (size_t i = 0; i < cryptInfo.subSample.size(); i++) {
        DRM_CHECK_AND_RETURN_RET_LOG(data.WriteUint32(cryptInfo.subSample[i].clearHeaderLen), IPC_PROXY_ERR,
            "MediaDecryptModuleServiceProxy DecryptMediaData Write cryptInfo.subSample.clearHeaderLen failed");

        DRM_CHECK_AND_RETURN_RET_LOG(data.WriteUint32(cryptInfo.subSample[i].payLoadLen), IPC_PROXY_ERR,
            "MediaDecryptModuleServiceProxy DecryptMediaData Write cryptInfo.subSample.payLoadLen failed");
    }
    (void)data.WriteFileDescriptor(srcBuffer);
    (void)data.WriteFileDescriptor(dstBuffer);

    int32_t error = Remote()->SendRequest(DECRYPT_MODULE_DECRYPT_DATA, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaDecryptModuleServiceProxy DecryptMediaData failed, error: %{public}d", error);
        return error;
    }
    DRM_CHECK_AND_RETURN_RET_LOG(error == ERR_NONE, IPC_PROXY_ERR,
        "MediaDecryptModuleServiceProxy DecryptMediaData failed, error: %{public}d", error);

    DRM_INFO_LOG("MediaDecryptModuleServiceProxy::DecryptMediaData exit.");
    return reply.ReadInt32();
}
} // DrmStandard
} // OHOS