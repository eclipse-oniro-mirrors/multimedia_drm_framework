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

#include "key_session_service_proxy.h"
#include "drm_log.h"
#include "remote_request_code.h"

namespace OHOS {
namespace DrmStandard {
KeySessionServiceProxy::KeySessionServiceProxy(const sptr<IRemoteObject> &impl)
    : IRemoteProxy<IKeySessionService>(impl)
{
    DRM_INFO_LOG("KeySessionServiceProxy Initialized.");
}

int32_t KeySessionServiceProxy::CreateMediaDecryptModule(sptr<IMediaDecryptModuleService> &decryptModule)
{
    DRM_INFO_LOG("KeySessionServiceProxy::CreateMediaDecryptModule enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    
    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("KeySessionServiceProxy CreateMediaDecryptModule Write interface token failed");
        return IPC_PROXY_ERR;
    }

    int error = KeySessionServiceProxy::Remote()->SendRequest(CREATE_MEDIA_DECRYPT_MODULE, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("KeySessionServiceProxy::CreateMediaDecryptModule failed, error: %{public}d", error);
        return error;
    }

    auto remoteObject = reply.ReadRemoteObject();
    if (remoteObject != nullptr) {
        decryptModule = iface_cast<IMediaDecryptModuleService>(remoteObject);
    } else {
        DRM_ERR_LOG("KeySessionServiceProxy CreateMediaDecryptModule decryptModule is nullptr");
        error = IPC_PROXY_ERR;
    }
    DRM_INFO_LOG("KeySessionServiceProxy::CreateMediaDecryptModule exit.");
    return error;
}


int32_t KeySessionServiceProxy::Release()
{
    DRM_INFO_LOG("KeySessionServiceProxy::Release enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("KeySessionServiceProxy Release Write interface token failed");
        return IPC_PROXY_ERR;
    }
    int error = KeySessionServiceProxy::Remote()->SendRequest(KEY_SESSION_RELEASE, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("KeySessionServiceProxy Release failed, error: %{public}d", error);
    }
    DRM_INFO_LOG("KeySessionServiceProxy::Release exit.");
    return error;
}

int32_t KeySessionServiceProxy::GenerateLicenseRequest(IKeySessionService::DrmInfo &drmInfo,
    IKeySessionService::LicenseInfo &licenseInfo)
{
    DRM_INFO_LOG("KeySessionServiceProxy::GenerateLicenseRequest enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(KeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("KeySessionServiceProxy GenerateLicenseRequest Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(drmInfo.keyType)) {
        DRM_ERR_LOG("KeySessionServiceProxy GenerateLicenseRequest Write keyType failed");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteString(drmInfo.mimeType)) {
        DRM_ERR_LOG("KeySessionServiceProxy GenerateLicenseRequest Write mimeType failed");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteInt32(drmInfo.indexInfo.size())) {
        DRM_ERR_LOG("KeySessionServiceProxy GenerateLicenseRequest Write indexInfo.size failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteBuffer(drmInfo.indexInfo.data(), drmInfo.indexInfo.size())) {
        DRM_ERR_LOG("KeySessionServiceProxy GenerateLicenseRequest Write indexInfo.size failed");
        return IPC_PROXY_ERR;
    }
    int error = KeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_GENERATE_LICENSE_REQUEST, data,
        reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("KeySessionServiceProxy::GenerateLicenseRequest failed, error: %{public}d", error);
        return error;
    }

    licenseInfo.requestType = (IKeySessionService::RequestType)reply.ReadInt32();
    uint32_t dataSize = reply.ReadInt32();
    for (int i = 0; i < dataSize; i++) {
        licenseInfo.mData.push_back(reply.ReadUint8());
    }

    licenseInfo.mDefaultURL = reply.ReadString();
    DRM_INFO_LOG("KeySessionServiceProxy::GenerateLicenseRequest exit.");
    return reply.ReadInt32();
}

int32_t KeySessionServiceProxy::ProcessLicenseResponse(std::vector<uint8_t> &keyId,
    std::vector<uint8_t> &licenseResponse)
{
    DRM_INFO_LOG("KeySessionServiceProxy::ProcessLicenseResponse enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(KeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("KeySessionServiceProxy ProcessLicenseResponse Write interface token failed");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteInt32(licenseResponse.size())) {
        DRM_ERR_LOG("KeySessionServiceProxy ProcessLicenseResponse Write licenseResponse size failed");
        return IPC_PROXY_ERR;
    }
    for (auto response : licenseResponse) {
        if (!data.WriteUint8(response)) {
            DRM_ERR_LOG("KeySessionServiceProxy ProcessLicenseResponse Write licenseResponse failed");
            return IPC_PROXY_ERR;
        }
    }
    int error = KeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_PROCESS_LICENSE_RESPONSE, data,
        reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("KeySessionServiceProxy::ProcessLicenseResponse failed, error: %{public}d", error);
        return error;
    }
    uint32_t keyIdSize = reply.ReadInt32();
    for (int i = 0; i < keyIdSize; i++) {
        keyId.push_back(reply.ReadUint8());
    }
    DRM_INFO_LOG("KeySessionServiceProxy::ProcessLicenseResponse exit.");
    return reply.ReadInt32();
}

int32_t KeySessionServiceProxy::GenerateOfflineReleaseRequest(std::vector<uint8_t> &keyId,
    std::vector<uint8_t> &releaseRequest)
{
    DRM_INFO_LOG("KeySessionServiceProxy::GenerateOfflineReleaseRequest enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(KeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("KeySessionServiceProxy GenerateOfflineReleaseRequest Write interface token failed");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteInt32(keyId.size())) {
        DRM_ERR_LOG("KeySessionServiceProxy GenerateOfflineReleaseRequest Write keyId size failed");
        return IPC_PROXY_ERR;
    }
    for (auto id : keyId) {
        if (!data.WriteUint8(id)) {
            DRM_ERR_LOG("KeySessionServiceProxy GenerateOfflineReleaseRequest Write keyId failed");
            return IPC_PROXY_ERR;
        }
    }

    int error = KeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_GENERATE_OFFLINE_RELEASE_REQUEST,
        data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("KeySessionServiceProxy::GenerateOfflineReleaseRequest failed, error: %{public}d", error);
        return error;
    }
    uint32_t requestSize = reply.ReadInt32();
    for (int i = 0; i < requestSize; i++) {
        releaseRequest.push_back(reply.ReadUint8());
    }
    DRM_INFO_LOG("KeySessionServiceProxy::GenerateOfflineReleaseRequest exit.");
    return reply.ReadInt32();
}

int32_t KeySessionServiceProxy::ProcessOfflineReleaseResponse(std::vector<uint8_t> &keyId,
    std::vector<uint8_t> &releaseReponse)
{
    DRM_INFO_LOG("KeySessionServiceProxy::ProcessOfflineReleaseResponse enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(KeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("KeySessionServiceProxy ProcessOfflineReleaseResponse Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(keyId.size())) {
        DRM_ERR_LOG("KeySessionServiceProxy ProcessOfflineReleaseResponse Write keyId size failed");
        return IPC_PROXY_ERR;
    }
    for (auto id : keyId) {
        if (!data.WriteUint8(id)) {
            DRM_ERR_LOG("KeySessionServiceProxy ProcessOfflineReleaseResponse Write keyId failed");
            return IPC_PROXY_ERR;
        }
    }
    if (!data.WriteInt32(releaseReponse.size())) {
        DRM_ERR_LOG("KeySessionServiceProxy ProcessOfflineReleaseResponse Write releaseReponse size failed");
        return IPC_PROXY_ERR;
    }
    for (auto response : releaseReponse) {
        if (!data.WriteUint8(response)) {
            DRM_ERR_LOG("KeySessionServiceProxy ProcessOfflineReleaseResponse Write releaseReponse failed");
            return IPC_PROXY_ERR;
        }
    }
    int error = KeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_PROCESS_OFFLINE_RELEASE_RESPONSE,
        data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("KeySessionServiceProxy::ProcessOfflineReleaseResponse failed, error: %{public}d", error);
        return error;
    }

    DRM_INFO_LOG("KeySessionServiceProxy::ProcessOfflineReleaseResponse exit.");
    return reply.ReadInt32();
}

int32_t KeySessionServiceProxy::CheckLicenseStatus(std::vector<IKeySessionService::KeyValue> &infoMap)
{
    DRM_INFO_LOG("KeySessionServiceProxy::GenerateOfflineReleaseRequest enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(KeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("KeySessionServiceProxy GenerateOfflineReleaseRequest Write interface token failed");
        return IPC_PROXY_ERR;
    }

    int error = KeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_GENERATE_CHECK_LICENSE_STATUS,
        data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("KeySessionServiceProxy::GenerateOfflineReleaseRequest failed, error: %{public}d", error);
        return error;
    }
    int infoMapSize = reply.ReadInt32();
    for (int i = 0; i < infoMapSize; i++) {
        KeyValue keyValue;
        keyValue.name = reply.ReadString();
        keyValue.value = reply.ReadString();
        infoMap.push_back(keyValue);
    }
    DRM_INFO_LOG("KeySessionServiceProxy::GenerateOfflineReleaseRequest exit.");
    return reply.ReadInt32();
}

int32_t KeySessionServiceProxy::RestoreOfflineKeys(std::vector<uint8_t> &keyId)
{
    DRM_INFO_LOG("KeySessionServiceProxy::RestoreOfflineKeys enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(KeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("KeySessionServiceProxy RestoreOfflineKeys Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(keyId.size())) {
        DRM_ERR_LOG("KeySessionServiceProxy RestoreOfflineKeys Write keyId size failed");
        return IPC_PROXY_ERR;
    }
    for (auto id : keyId) {
        if (!data.WriteUint8(id)) {
            DRM_ERR_LOG("KeySessionServiceProxy RestoreOfflineKeys Write keyId failed");
            return IPC_PROXY_ERR;
        }
    }

    int error = KeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_RESTORE_OFFLINEKEYS, data,
        reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("KeySessionServiceProxy::RestoreOfflineKeys failed, error: %{public}d", error);
        return error;
    }

    DRM_INFO_LOG("KeySessionServiceProxy::RestoreOfflineKeys exit.");
    return reply.ReadInt32();
}

int32_t KeySessionServiceProxy::RemoveOfflineKeys(std::vector<uint8_t> &keyId)
{
    DRM_INFO_LOG("KeySessionServiceProxy::RemoveOfflineKeys enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(KeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("KeySessionServiceProxy RemoveOfflineKeys Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(keyId.size())) {
        DRM_ERR_LOG("KeySessionServiceProxy RestoreOfflineKeys Write keyId size failed");
        return IPC_PROXY_ERR;
    }
    for (auto id : keyId) {
        if (!data.WriteUint8(id)) {
            DRM_ERR_LOG("KeySessionServiceProxy RestoreOfflineKeys Write keyId failed");
            return IPC_PROXY_ERR;
        }
    }

    int error = KeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_REMOVE_OFFLINEKEYS, data,
        reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("KeySessionServiceProxy::RemoveOfflineKeys failed, error: %{public}d", error);
        return error;
    }

    DRM_INFO_LOG("KeySessionServiceProxy::RemoveOfflineKeys exit.");
    return reply.ReadInt32();
}

int32_t KeySessionServiceProxy::GetOfflineKeyIds(std::vector<std::vector<uint8_t>> &keyIds)
{
    DRM_INFO_LOG("KeySessionServiceProxy::GetOfflineKeyIds enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(KeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("KeySessionServiceProxy GetOfflineKeyIds Write interface token failed");
        return IPC_PROXY_ERR;
    }
    int32_t error = KeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_GET_OFFLINEKEYIDS,
        data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("KeySessionServiceProxy::GetOfflineKeyIds failed, error: %{public}d", error);
        return error;
    }
    int32_t keyIdsSize = reply.ReadInt32();
    for (int i = 0; i < keyIdsSize; i++) {
        int32_t keyIdSize = reply.ReadInt32();
        std::vector<uint8_t> keyId;
        const uint8_t* keyIdBuf = static_cast<const uint8_t*>(reply.ReadBuffer(keyIdSize));
        if (keyIdBuf == nullptr) {
            DRM_ERR_LOG("KeySessionServiceProxy::GetOfflineKeyIds ReadBuffer failed");
            return error;
        }
        keyId.assign(keyIdBuf, keyIdBuf + keyIdSize);
        keyIds.push_back(keyId);
    }
    DRM_INFO_LOG("KeySessionServiceProxy::GetOfflineKeyIds exit.");
    return reply.ReadInt32();
}

int32_t KeySessionServiceProxy::RemoveLicenses()
{
    DRM_INFO_LOG("KeySessionServiceProxy::RemoveLicenses enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(KeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("KeySessionServiceProxy RemoveLicenses Write interface token failed");
        return IPC_PROXY_ERR;
    }

    int error = KeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_REMOVE_LICENSE, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("KeySessionServiceProxy::RemoveLicenses failed, error: %{public}d", error);
        return error;
    }

    DRM_INFO_LOG("KeySessionServiceProxy::RemoveLicenses exit.");
    return reply.ReadInt32();
}

int32_t KeySessionServiceProxy::GetOfflineKeyState(std::vector<uint8_t> &keyId,
    IKeySessionService::OfflineKeyState &state)
{
    DRM_INFO_LOG("KeySessionServiceProxy::GetOfflineKeyState enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(KeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("KeySessionServiceProxy GetOfflineKeyState Write interface token failed");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteInt32(keyId.size())) {
        DRM_ERR_LOG("KeySessionServiceProxy GetOfflineKeyState Write keyId size failed");
        return IPC_PROXY_ERR;
    }
    for (auto id : keyId) {
        if (!data.WriteUint8(id)) {
            DRM_ERR_LOG("KeySessionServiceProxy GetOfflineKeyState Write keyId failed");
            return IPC_PROXY_ERR;
        }
    }

    int error = KeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_GET_OFFLINEKEY_STATE, data,
        reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("KeySessionServiceProxy::GetOfflineKeyState failed, error: %{public}d", error);
        return error;
    }
    state = (IKeySessionService::OfflineKeyState)reply.ReadInt32();
    DRM_INFO_LOG("KeySessionServiceProxy::GetOfflineKeyState exit.");
    return reply.ReadInt32();
}

int32_t KeySessionServiceProxy::SetKeySessionServiceCallback(sptr<IKeySessionServiceCallback> &callback)
{
    DRM_INFO_LOG("KeySessionServiceProxy::SetKeySessionServiceCallback enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (callback == nullptr) {
        DRM_ERR_LOG("KeySessionServiceProxy SetCallback callback is null");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("KeySessionServiceProxy SetCallback Write interface token failed");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteRemoteObject(callback->AsObject())) {
        DRM_ERR_LOG("KeySessionServiceProxy SetCallback write CameraServiceCallback obj failed");
        return IPC_PROXY_ERR;
    }

    int error = Remote()->SendRequest(MEDIA_KEY_SESSION_SET_CALLBACK, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("KeySessionServiceProxy SetCallback failed, error: %{public}d", error);
    }
    return error;
}
} // DrmStandard
} // OHOS