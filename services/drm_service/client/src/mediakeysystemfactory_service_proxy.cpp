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

#include "remote_request_code.h"
#include "drm_log.h"
#include "drm_error_code.h"
#include "mediakeysystemfactory_service_proxy.h"

namespace OHOS {
namespace DrmStandard {
MediaKeySystemFactoryServiceProxy::MediaKeySystemFactoryServiceProxy(const sptr<IRemoteObject> &impl)
    : IRemoteProxy<IMediaKeySystemFactoryService>(impl)
{
    DRM_DEBUG_LOG("MediaKeySystemFactoryServiceProxy Initialized.");
}

int32_t MediaKeySystemFactoryServiceProxy::SetListenerObject(const sptr<IRemoteObject> &object)
{
    DRM_INFO_LOG("MediaKeySystemFactoryServiceProxy SetListenerObject.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    data.WriteInterfaceToken(GetDescriptor());
    (void)data.WriteRemoteObject(object);
    int error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_FACTORY_SET_LISTENER_OBJ, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("Set listener obj failed, error: %{public}d", error);
        return IPC_PROXY_ERR;
    }
    DRM_INFO_LOG("MediaKeySystemFactoryServiceProxy SetListenerObject exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySystemFactoryServiceProxy::IsMediaKeySystemSupported(std::string &uuid, bool *isSurpported)
{
    DRM_INFO_LOG("MediaKeySystemFactoryServiceProxy::IsMediaKeySystemSupported one param called, enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG(
            "MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported one param Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(ARGS_NUM_ONE)) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported one param Write paramNum failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteString(uuid)) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported Write uuid failed");
        return IPC_PROXY_ERR;
    }

    int32_t error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_FACTORY_IS_MEDIA_KEY_SYSTEM_SURPPORTED, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy::IsMediaKeySystemSupported failed, error: %{public}d", error);
        return error;
    }

    *isSurpported = reply.ReadBool();
    DRM_INFO_LOG("MediaKeySystemFactoryServiceProxy::IsMediaKeySystemSupported one param called, exit.");

    return error;
}

int32_t MediaKeySystemFactoryServiceProxy::IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType,
    bool *isSurpported)
{
    DRM_INFO_LOG("MediaKeySystemFactoryServiceProxy::IsMediaKeySystemSupported two param called, enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG(
            "MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported two params Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(ARGS_NUM_TWO)) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported two params Write paramNum failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteString(uuid)) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported two params Write uuid failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteString(mimeType)) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported two params Write mimeType failed");
        return IPC_PROXY_ERR;
    }

    int32_t error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_FACTORY_IS_MEDIA_KEY_SYSTEM_SURPPORTED, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy::IsMediaKeySystemSupported failed, error: %{public}d", error);
        return error;
    }

    *isSurpported = reply.ReadBool();
    DRM_INFO_LOG("MediaKeySystemFactoryServiceProxy::IsMediaKeySystemSupported two param called, exit.");
    return error;
}

int32_t MediaKeySystemFactoryServiceProxy::IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType,
    int32_t securityLevel, bool *isSurpported)
{
    DRM_INFO_LOG("MediaKeySystemFactoryServiceProxy::IsMediaKeySystemSupported three param called, enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG(
            "MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported three params Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(ARGS_NUM_THREE)) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported three params Write paramNum failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteString(uuid)) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported three params Write uuid failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteString(mimeType)) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported three params Write mimeType failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(securityLevel)) {
        DRM_ERR_LOG(
            "MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported three params Write securityLevel failed");
        return IPC_PROXY_ERR;
    }

    int32_t error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_FACTORY_IS_MEDIA_KEY_SYSTEM_SURPPORTED, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy::IsMediaKeySystemSupported failed, error: %{public}d", error);
        return error;
    }

    *isSurpported = reply.ReadBool();
    DRM_INFO_LOG("MediaKeySystemFactoryServiceProxy::IsMediaKeySystemSupported three param called, exit.");

    return error;
}

int32_t MediaKeySystemFactoryServiceProxy::CreateMediaKeySystem(std::string &uuid,
    sptr<IMediaKeySystemService> &mediaKeySystemProxy)
{
    DRM_INFO_LOG("MediaKeySystemFactoryServiceProxy::CreateMediaKeySystem enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy CreateMediaKeySystem Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteString(uuid)) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy CreateMediaKeySystem Write format failed");
        return IPC_PROXY_ERR;
    }

    int32_t error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_FACTORY_CREATE_MEDIA_KEYSYSTEM, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy::CreateMediaKeySystem failed, error: %{public}d", error);
        return error;
    }

    auto remoteObject = reply.ReadRemoteObject();
    if (remoteObject != nullptr) {
        mediaKeySystemProxy = iface_cast<IMediaKeySystemService>(remoteObject);
    } else {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy::CreateMediaKeySystem mediaKeySystemProxy is nullptr");
        error = IPC_PROXY_ERR;
    }
    DRM_INFO_LOG("MediaKeySystemFactoryServiceProxy::CreateMediaKeySystem exit.");
    return error;
}
} // DrmStandard
} // OHOS