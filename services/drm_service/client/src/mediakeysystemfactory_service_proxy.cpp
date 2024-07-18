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
    int ret = Remote()->SendRequest(MEDIA_KEY_SYSTEM_FACTORY_SET_LISTENER_OBJ, data, reply, option);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("Set listener obj failed, error: %{public}d", ret);
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
            "MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported one param Write interface token failed.");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(ARGS_NUM_ONE)) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported one param Write paramNum failed.");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteString(uuid)) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported Write uuid failed.");
        return IPC_PROXY_ERR;
    }

    int32_t ret = Remote()->SendRequest(MEDIA_KEY_SYSTEM_FACTORY_IS_MEDIA_KEY_SYSTEM_SURPPORTED, data, reply, option);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy::IsMediaKeySystemSupported failed, error: %{public}d", ret);
        return ret;
    }

    *isSurpported = reply.ReadBool();
    DRM_INFO_LOG("MediaKeySystemFactoryServiceProxy::IsMediaKeySystemSupported one param called, exit.");

    return ret;
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
            "MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported two params Write interface token failed.");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(ARGS_NUM_TWO)) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported two params Write paramNum failed.");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteString(uuid)) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported two params Write uuid failed.");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteString(mimeType)) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported two params Write mimeType failed.");
        return IPC_PROXY_ERR;
    }

    int32_t ret = Remote()->SendRequest(MEDIA_KEY_SYSTEM_FACTORY_IS_MEDIA_KEY_SYSTEM_SURPPORTED, data, reply, option);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy::IsMediaKeySystemSupported failed, error: %{public}d", ret);
        return ret;
    }

    *isSurpported = reply.ReadBool();
    DRM_INFO_LOG("MediaKeySystemFactoryServiceProxy::IsMediaKeySystemSupported two param called, exit.");
    return ret;
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
            "MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported three params Write interface token failed.");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(ARGS_NUM_THREE)) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported three params Write paramNum failed.");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteString(uuid)) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported three params Write uuid failed.");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteString(mimeType)) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported three params Write mimeType failed.");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(securityLevel)) {
        DRM_ERR_LOG(
            "MediaKeySystemFactoryServiceProxy IsMediaKeySystemSupported three params Write securityLevel failed.");
        return IPC_PROXY_ERR;
    }

    int32_t ret = Remote()->SendRequest(MEDIA_KEY_SYSTEM_FACTORY_IS_MEDIA_KEY_SYSTEM_SURPPORTED, data, reply, option);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy::IsMediaKeySystemSupported failed, error: %{public}d", ret);
        return ret;
    }

    *isSurpported = reply.ReadBool();
    DRM_INFO_LOG("MediaKeySystemFactoryServiceProxy::IsMediaKeySystemSupported three param called, exit.");

    return ret;
}

int32_t MediaKeySystemFactoryServiceProxy::GetMediaKeySystems(std::map<std::string, std::string> &keySystemNames)
{
    DRM_INFO_LOG("MediaKeySystemFactoryServiceProxy::GetMediaKeySystems enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySystemFactoryServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy::GetMediaKeySystems Write interface token failed.");
        return IPC_PROXY_ERR;
    }
    int32_t ret =
        MediaKeySystemFactoryServiceProxy::Remote()->SendRequest(MEDIA_KEY_SYSTEM_FACTORY_GET_MEDIA_KEYSYSTEM_NAME,
        data, reply, option);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy::GetMediaKeySystems failed, ret: %{public}d", ret);
        return ret;
    }
    int32_t mediaKeySystemNameMapSize = reply.ReadInt32();
    for (int32_t i = 0; i < mediaKeySystemNameMapSize; i++) {
        std::string name = reply.ReadString();
        std::string uuid = reply.ReadString();
        keySystemNames.insert(std::make_pair(name, uuid));
    }
    DRM_INFO_LOG("MediaKeySessiMediaKeySystemFactoryServiceProxyonServiceProxy::GetMediaKeySystems exit.");
    return ret;
}

int32_t MediaKeySystemFactoryServiceProxy::GetMediaKeySystemUuid(std::string &name, std::string &uuid)
{
    DRM_INFO_LOG("MediaKeySystemFactoryServiceProxy::GetMediaKeySystemUuid enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySystemFactoryServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy::GetMediaKeySystemUuid Write interface token failed.");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteString(name)) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetConfiguration Write configName failed.");
        return IPC_PROXY_ERR;
    }
    int32_t ret =
        MediaKeySystemFactoryServiceProxy::Remote()->SendRequest(MEDIA_KEY_SYSTEM_FACTORY_GET_MEDIA_KEYSYSTEM_UUID,
        data, reply, option);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy::GetMediaKeySystemUuid failed, ret: %{public}d", ret);
        return ret;
    }
    uuid = reply.ReadString();
    DRM_INFO_LOG("MediaKeySessiMediaKeySystemFactoryServiceProxyonServiceProxy::GetMediaKeySystemUuid exit.");
    return ret;
}

int32_t MediaKeySystemFactoryServiceProxy::CreateMediaKeySystem(std::string &name,
    sptr<IMediaKeySystemService> &mediaKeySystemProxy)
{
    DRM_INFO_LOG("MediaKeySystemFactoryServiceProxy::CreateMediaKeySystem enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy CreateMediaKeySystem Write interface token failed.");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteString(name)) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy CreateMediaKeySystem Write format failed.");
        return IPC_PROXY_ERR;
    }

    int32_t ret = Remote()->SendRequest(MEDIA_KEY_SYSTEM_FACTORY_CREATE_MEDIA_KEYSYSTEM, data, reply, option);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy::CreateMediaKeySystem failed, error: %{public}d", ret);
        return ret;
    }

    auto remoteObject = reply.ReadRemoteObject();
    if (remoteObject != nullptr) {
        mediaKeySystemProxy = iface_cast<IMediaKeySystemService>(remoteObject);
    } else {
        DRM_ERR_LOG("MediaKeySystemFactoryServiceProxy::CreateMediaKeySystem mediaKeySystemProxy is nullptr");
        ret = IPC_PROXY_ERR;
    }
    DRM_INFO_LOG("MediaKeySystemFactoryServiceProxy::CreateMediaKeySystem exit.");
    return ret;
}
} // DrmStandard
} // OHOS