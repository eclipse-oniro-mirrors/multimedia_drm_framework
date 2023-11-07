/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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
#include "media_decrypt_module_service_proxy.h"

namespace OHOS {
namespace DrmStandard {
MediaDecryptModuleServiceProxy::MediaDecryptModuleServiceProxy(const sptr<IRemoteObject> &impl)
    : IRemoteProxy<IMediaDecryptModuleService>(impl)
{
    DRM_DEBUG_LOG("MediaKeySystemServiceProxy Initialized.");
}

int32_t MediaDecryptModuleServiceProxy::Release()
{
    DRM_INFO_LOG("MediaDecryptModuleServiceProxy::Release() enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaDecryptModuleServiceProxy Release Write interface token failed");
        return IPC_PROXY_ERR;
    }

    int error = Remote()->SendRequest(DECRYPT_MODULE_RELEASE, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaDecryptModuleServiceProxy::Release failed, error: %{public}d", error);
        return error;
    }

    DRM_INFO_LOG("MediaDecryptModuleServiceProxy::Release() exit.");
    return reply.ReadInt32();
}

int32_t MediaDecryptModuleServiceProxy::DecryptData(bool secureDecodrtState,
    IMediaDecryptModuleService::CryptInfo &cryptInfo, uint64_t srcBuffer, uint64_t dstBuffer)
{
    DRM_INFO_LOG("MediaDecryptModuleServiceProxy::DecryptData enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaDecryptModuleServiceProxy DecryptData Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteBool(secureDecodrtState)) {
        DRM_ERR_LOG("MediaDecryptModuleServiceProxy DecryptData Write secureDecodrtState failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(cryptInfo.type)) {
        DRM_ERR_LOG("MediaDecryptModuleServiceProxy DecryptData Write cryptInfo.type failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(cryptInfo.keyId.size())) {
        DRM_ERR_LOG("MediaDecryptModuleServiceProxy DecryptData Write cryptInfo.keyId size failed");
        return IPC_PROXY_ERR;
    }

    for (auto keyId : cryptInfo.keyId) {
        if (!data.WriteUint8(keyId)) {
            DRM_ERR_LOG("MediaDecryptModuleServiceProxy DecryptData Write cryptInfo.keyId failed");
            return IPC_PROXY_ERR;
        }
    }

    if (!data.WriteInt32(cryptInfo.iv.size())) {
        DRM_ERR_LOG("MediaDecryptModuleServiceProxy DecryptData Write cryptInfo.iv size failed");
        return IPC_PROXY_ERR;
    }

    for (auto iv : cryptInfo.iv) {
        if (!data.WriteUint8(iv)) {
            DRM_ERR_LOG("MediaDecryptModuleServiceProxy DecryptData Write cryptInfo.iv failed");
            return IPC_PROXY_ERR;
        }
    }

    if (!data.WriteInt32(cryptInfo.pattern.encryptBlocks)) {
        DRM_ERR_LOG("MediaDecryptModuleServiceProxy DecryptData Write cryptInfo.pattern.encryptBlocks failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(cryptInfo.pattern.skipBlocks)) {
        DRM_ERR_LOG("MediaDecryptModuleServiceProxy DecryptData Write cryptInfo.pattern.skipBlocks failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(cryptInfo.subSample.size())) {
        DRM_ERR_LOG("MediaDecryptModuleServiceProxy DecryptData Write cryptInfo.subSample.size() failed");
        return IPC_PROXY_ERR;
    }

    for (size_t i = 0; i < cryptInfo.subSample.size(); i++) {
        if (!data.WriteInt32(cryptInfo.subSample[i].clearHeaderLen)) {
            DRM_ERR_LOG("MediaDecryptModuleServiceProxy DecryptData Write cryptInfo.subSample.clearHeaderLen failed");
            return IPC_PROXY_ERR;
        }
        if (!data.WriteInt32(cryptInfo.subSample[i].payLoadLen)) {
            DRM_ERR_LOG("MediaDecryptModuleServiceProxy DecryptData Write cryptInfo.subSample.payLoadLen failed");
            return IPC_PROXY_ERR;
        }
    }

    (void)data.WriteFileDescriptor(srcBuffer);
    (void)data.WriteFileDescriptor(dstBuffer);
    int error = Remote()->SendRequest(DECRYPT_MODULE_DECRYPT_DATA, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaDecryptModuleServiceProxy DecryptData failed, error: %{public}d", error);
        return error;
    }
    DRM_INFO_LOG("MediaDecryptModuleServiceProxy::DecryptData exit.");
    return reply.ReadInt32();
}

int32_t MediaDecryptModuleServiceProxy::RequireSecureDecoderModule(std::string &mimeType, bool *status)
{
    DRM_INFO_LOG("MediaDecryptModuleServiceProxy::RequireSecureDecoderModule enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaDecryptModuleServiceProxy RequireSecureDecoderModule Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteString(mimeType)) {
        DRM_ERR_LOG("MediaDecryptModuleServiceProxy RequireSecureDecoderModule Write response failed");
        return IPC_PROXY_ERR;
    }

    int error = Remote()->SendRequest(DECRYPT_MODULE_REQUIRE_SECURE_DECODER, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaDecryptModuleServiceProxy RequireSecureDecoderModule failed, error: %{public}d", error);
        return error;
    }
    *status = reply.ReadBool();
    DRM_INFO_LOG("MediaDecryptModuleServiceProxy::RequireSecureDecoderModule exit.");
    return reply.ReadInt32();
}
} // DrmStandard
} // OHOS