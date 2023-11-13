/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include "mediakeysystem_service_proxy.h"
#include "remote_request_code.h"
#include "drm_log.h"
#include "drm_error_code.h"

namespace OHOS {
namespace DrmStandard {
MediaKeySystemServiceProxy::MediaKeySystemServiceProxy(const sptr<IRemoteObject> &impl)
    : IRemoteProxy<IMediaKeySystemService>(impl)
{
    DRM_DEBUG_LOG("MediaKeySystemServiceProxy Initialized.");
}

int32_t MediaKeySystemServiceProxy::Release()
{
    DRM_INFO_LOG("MediaKeySystemServiceProxy::Release enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy Release Write interface token failed");
        return IPC_PROXY_ERR;
    }
    int error = MediaKeySystemServiceProxy::Remote()->SendRequest(MEDIA_KEY_SYSTEM_RELEASE, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy Release failed, error: %{public}d", error);
        return error;
    }
    DRM_INFO_LOG("MediaKeySystemServiceProxy::Release exit.");
    return 0;
}

int32_t MediaKeySystemServiceProxy::GenerateKeySystemRequest(IMediaKeySystemService::RequestType type,
    std::vector<uint8_t> &request, std::string &defaultUrl)
{
    DRM_INFO_LOG("MediaKeySystemServiceProxy::GenerateKeySystemRequest enter. type:%{public}d.", type);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySystemServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GenerateKeySystemRequest two params Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32((int)type)) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GenerateKeySystemRequest two params Write RequestType failed");
        return IPC_PROXY_ERR;
    }

    uint32_t error = MediaKeySystemServiceProxy::Remote()->SendRequest(MEDIA_KEY_SYSTEM_GENERATE_KEYSYSTEM_REQUEST,
        data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy::GenerateKeySystemRequest failed, error: %{public}d", error);
        return error;
    }

    uint32_t requestSize = reply.ReadInt32();
    for (uint32_t i = 0; i < requestSize; i++) {
        request.push_back(reply.ReadUint8());
    }
    defaultUrl = reply.ReadString();

    DRM_INFO_LOG("MediaKeySystemServiceProxy::GenerateKeySystemRequest exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySystemServiceProxy::ProcessKeySystemResponse(IMediaKeySystemService::RequestType type,
    const std::vector<uint8_t> &response)
{
    DRM_INFO_LOG("MediaKeySystemServiceProxy::ProcessKeySystemResponse enter. type:%{public}d.", type);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy ProcessKeySystemResponse Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32((int)type)) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy ProcessKeySystemResponse Write RequestType failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(response.size())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy ProcessKeySystemResponse Write response size failed");
        return IPC_PROXY_ERR;
    }

    for (auto res : response) {
        if (!data.WriteUint8(res)) {
            DRM_ERR_LOG("MediaKeySystemServiceProxy ProcessKeySystemResponse Write response failed");
            return IPC_PROXY_ERR;
        }
    }

    int error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_GENERATE_KEYSYSTEM_RESPONSE, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy ProcessKeySystemResponse failed, error: %{public}d", error);
        return error;
    }

    DRM_INFO_LOG("MediaKeySystemServiceProxy ProcessKeySystemResponse exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySystemServiceProxy::SetConfiguration(IMediaKeySystemService::ConfigType type,
    std::string &propertyName, std::string &value)
{
    DRM_INFO_LOG("SetConfiguration enter, configType:%{public}d, propertyName:%{public}s, value:%{public}s.",
        (int)type, propertyName.c_str(), value.c_str());
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy SetConfiguration Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32((int32_t)type)) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy SetConfiguration Write RequestType failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteString(propertyName)) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy SetConfiguration Write response failed");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteString(value)) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy SetConfiguration Write response failed");
        return IPC_PROXY_ERR;
    }

    int error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_SETCONFIGURATION, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy SetConfiguration failed, error: %{public}d", error);
        return error;
    }

    DRM_INFO_LOG("SetConfiguration exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySystemServiceProxy::GetSecurityLevel(IKeySessionService::SecurityLevel *securityLevel)
{
    DRM_INFO_LOG("MediaKeySystemServiceProxy::GetSecurityLevel enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySystemServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetSecurityLevel Write interface token failed");
        return IPC_PROXY_ERR;
    }

    int error = MediaKeySystemServiceProxy::Remote()->SendRequest(MEDIA_KEY_SYSTEM_GETSECURITYLEVEL, data,
        reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetSecurityLevel failed, error: %{public}d", error);
        return error;
    }

    *securityLevel = (IKeySessionService::SecurityLevel)reply.ReadInt32();

    DRM_INFO_LOG("MediaKeySystemServiceProxy::GetSecurityLevel exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySystemServiceProxy::GetConfiguration(IMediaKeySystemService::ConfigType configType,
    std::string &propertyName, std::string &value)
{
    DRM_INFO_LOG("MediaKeySystemServiceProxy::GetConfiguration enter, configType:%{public}d, propertyName:%{public}s.",
        (int)configType, propertyName.c_str());
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetConfiguration Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32((int)configType)) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetConfiguration Write RequestType failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteString(propertyName)) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetConfiguration Write response failed");
        return IPC_PROXY_ERR;
    }

    int error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_GETCONFIGURATION, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetConfiguration failed, error: %{public}d", error);
        return error;
    }

    value = reply.ReadString();

    DRM_INFO_LOG("MediaKeySystemServiceProxy::GetConfiguration exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySystemServiceProxy::CreateKeySession(IKeySessionService::SecurityLevel securityLevel,
    sptr<IKeySessionService> &keySessionProxy)
{
    DRM_INFO_LOG("MediaKeySystemServiceProxy::CreateKeySession enter, securityLevel:%{public}d.", securityLevel);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy CreateKeySession Write interface token failed");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteInt32(securityLevel)) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy CreateKeySession Write format failed");
        return IPC_PROXY_ERR;
    }

    int error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_CREATE_KEY_SESSION, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy::CreateKeySession failed, error: %{public}d", error);
        return error;
    }

    auto remoteObject = reply.ReadRemoteObject();
    if (remoteObject != nullptr) {
        keySessionProxy = iface_cast<IKeySessionService>(remoteObject);
    } else {
        DRM_ERR_LOG("MediaKeySystemServiceProxy CreateKeySession keySessionProxy is nullptr");
        error = IPC_PROXY_ERR;
    }
    DRM_INFO_LOG("MediaKeySystemServiceProxy::CreateKeySession exit.");
    return error;
}

int32_t MediaKeySystemServiceProxy::GetMetric(std::vector<IMediaKeySystemService::KeyValue> &infoMap)
{
    DRM_INFO_LOG("MediaKeySystemServiceProxy::GetMetric enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetMetric two params Write interface token failed");
        return IPC_PROXY_ERR;
    }

    int error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_GETMETRIC, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy::GetMetric failed, error: %{public}d", error);
        return error;
    }
    int infoMapSize = reply.ReadInt32();
    for (int i = 0; i < infoMapSize; i++) {
        IMediaKeySystemService::KeyValue keyValue;
        keyValue.name = reply.ReadString();
        keyValue.value = reply.ReadString();
        infoMap.push_back(keyValue);
    }
    DRM_INFO_LOG("MediaKeySystemServiceProxy::GetMetric exit.");
    return reply.ReadInt32();
}
} // DrmStandard
} // OHOS