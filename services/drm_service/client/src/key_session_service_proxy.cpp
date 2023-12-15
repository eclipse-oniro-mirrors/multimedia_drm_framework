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

#include "key_session_service_proxy.h"
#include "drm_log.h"
#include "remote_request_code.h"

namespace OHOS {
namespace DrmStandard {
MediaKeySessionServiceProxy::MediaKeySessionServiceProxy(const sptr<IRemoteObject> &impl)
    : IRemoteProxy<IMediaKeySessionService>(impl)
{
    DRM_INFO_LOG("MediaKeySessionServiceProxy::MediaKeySessionServiceProxy Initialized");
}

int32_t MediaKeySessionServiceProxy::CreateMediaDecryptModule(sptr<IMediaDecryptModuleService> &decryptModule)
{
    DRM_INFO_LOG("MediaKeySessionServiceProxy::CreateMediaDecryptModule enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy CreateMediaDecryptModule Write interface token failed");
        return IPC_PROXY_ERR;
    }

    int32_t error =
        MediaKeySessionServiceProxy::Remote()->SendRequest(CREATE_MEDIA_DECRYPT_MODULE, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy::CreateMediaDecryptModule failed, error: %{public}d", error);
        return error;
    }

    auto remoteObject = reply.ReadRemoteObject();
    if (remoteObject != nullptr) {
        decryptModule = iface_cast<IMediaDecryptModuleService>(remoteObject);
    } else {
        DRM_ERR_LOG("MediaKeySessionServiceProxy CreateMediaDecryptModule decryptModule is nullptr");
        error = IPC_PROXY_ERR;
    }
    DRM_INFO_LOG("MediaKeySessionServiceProxy::CreateMediaDecryptModule exit.");
    return error;
}

int32_t MediaKeySessionServiceProxy::GetSecurityLevel(IMediaKeySessionService::SecurityLevel *securityLevel)
{
    DRM_INFO_LOG("MediaKeySessionServiceProxy::GetSecurityLevel enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy GetSecurityLevel Write interface token failed");
        return IPC_PROXY_ERR;
    }

    int32_t error = Remote()->SendRequest(MEDIA_KEY_SESSION_GETSECURITYLEVEL, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy GetSecurityLevel failed, error: %{public}d", error);
        return error;
    }

    *securityLevel = (IMediaKeySessionService::SecurityLevel)reply.ReadInt32();

    DRM_INFO_LOG("MediaKeySessionServiceProxy::GetSecurityLevel exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySessionServiceProxy::Release()
{
    DRM_INFO_LOG("MediaKeySessionServiceProxy::Release enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy Release Write interface token failed");
        return IPC_PROXY_ERR;
    }
    int32_t error = MediaKeySessionServiceProxy::Remote()->SendRequest(KEY_SESSION_RELEASE, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy Release failed, error: %{public}d", error);
    }
    DRM_INFO_LOG("MediaKeySessionServiceProxy::Release exit.");
    return error;
}

int32_t MediaKeySessionServiceProxy::GenerateLicenseRequest(
    IMediaKeySessionService::LicenseRequestInfo &licenseRequestInfo,
    IMediaKeySessionService::LicenseRequest &licenseRequest)
{
    DRM_INFO_LOG("MediaKeySessionServiceProxy::GenerateLicenseRequest enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(MediaKeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy GenerateLicenseRequest Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(licenseRequestInfo.licenseType)) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy GenerateLicenseRequest Write licenseType failed");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteString(licenseRequestInfo.mimeType)) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy GenerateLicenseRequest Write mimeType failed");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteInt32(licenseRequestInfo.initData.size())) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy GenerateLicenseRequest Write initData.size failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteBuffer(licenseRequestInfo.initData.data(), licenseRequestInfo.initData.size())) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy GenerateLicenseRequest Write initData.size failed");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteInt32(licenseRequestInfo.optionalData.size())) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy GenerateLicenseRequest Write optionalData.size failed");
        return IPC_PROXY_ERR;
    }
    for (auto optionalData : licenseRequestInfo.optionalData) {
        data.WriteString(optionalData.first);
        data.WriteString(optionalData.second);
    }
    int32_t error = MediaKeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_GENERATE_LICENSE_REQUEST, data,
        reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy::GenerateLicenseRequest failed, error: %{public}d", error);
        return error;
    }

    licenseRequest.requestType = (IMediaKeySessionService::RequestType)reply.ReadInt32();
    uint32_t dataSize = reply.ReadInt32();
    for (int32_t i = 0; i < dataSize; i++) {
        licenseRequest.mData.push_back(reply.ReadUint8());
    }

    licenseRequest.mDefaultURL = reply.ReadString();
    DRM_INFO_LOG("MediaKeySessionServiceProxy::GenerateLicenseRequest exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySessionServiceProxy::ProcessLicenseResponse(std::vector<uint8_t> &licenseId,
    std::vector<uint8_t> &licenseResponse)
{
    DRM_INFO_LOG("MediaKeySessionServiceProxy::ProcessLicenseResponse enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy ProcessLicenseResponse Write interface token failed");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteInt32(licenseResponse.size())) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy ProcessLicenseResponse Write licenseResponse size failed");
        return IPC_PROXY_ERR;
    }
    for (auto response : licenseResponse) {
        if (!data.WriteUint8(response)) {
            DRM_ERR_LOG("MediaKeySessionServiceProxy ProcessLicenseResponse Write licenseResponse failed");
            return IPC_PROXY_ERR;
        }
    }
    int32_t error = MediaKeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_PROCESS_LICENSE_RESPONSE, data,
        reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy::ProcessLicenseResponse failed, error: %{public}d", error);
        return error;
    }
    uint32_t licenseIdSize = reply.ReadInt32();
    for (int32_t i = 0; i < licenseIdSize; i++) {
        licenseId.push_back(reply.ReadUint8());
    }
    DRM_INFO_LOG("MediaKeySessionServiceProxy::ProcessLicenseResponse exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySessionServiceProxy::GenerateOfflineReleaseRequest(std::vector<uint8_t> &licenseId,
    std::vector<uint8_t> &releaseRequest)
{
    DRM_INFO_LOG("MediaKeySessionServiceProxy::GenerateOfflineReleaseRequest enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy GenerateOfflineReleaseRequest Write interface token failed");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteInt32(licenseId.size())) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy GenerateOfflineReleaseRequest Write licenseId size failed");
        return IPC_PROXY_ERR;
    }
    for (auto id : licenseId) {
        if (!data.WriteUint8(id)) {
            DRM_ERR_LOG("MediaKeySessionServiceProxy GenerateOfflineReleaseRequest Write licenseId failed");
            return IPC_PROXY_ERR;
        }
    }

    int32_t error = MediaKeySessionServiceProxy::Remote()->SendRequest(
        MEDIA_KEY_SESSION_GENERATE_OFFLINE_RELEASE_REQUEST, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy::GenerateOfflineReleaseRequest failed, error: %{public}d", error);
        return error;
    }
    uint32_t requestSize = reply.ReadInt32();
    for (int32_t i = 0; i < requestSize; i++) {
        releaseRequest.push_back(reply.ReadUint8());
    }
    DRM_INFO_LOG("MediaKeySessionServiceProxy::GenerateOfflineReleaseRequest exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySessionServiceProxy::ProcessOfflineReleaseResponse(std::vector<uint8_t> &licenseId,
    std::vector<uint8_t> &releaseReponse)
{
    DRM_INFO_LOG("MediaKeySessionServiceProxy::ProcessOfflineReleaseResponse enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy ProcessOfflineReleaseResponse Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(licenseId.size())) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy ProcessOfflineReleaseResponse Write licenseId size failed");
        return IPC_PROXY_ERR;
    }
    for (auto id : licenseId) {
        if (!data.WriteUint8(id)) {
            DRM_ERR_LOG("MediaKeySessionServiceProxy ProcessOfflineReleaseResponse Write licenseId failed");
            return IPC_PROXY_ERR;
        }
    }
    if (!data.WriteInt32(releaseReponse.size())) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy ProcessOfflineReleaseResponse Write releaseReponse size failed");
        return IPC_PROXY_ERR;
    }
    for (auto response : releaseReponse) {
        if (!data.WriteUint8(response)) {
            DRM_ERR_LOG("MediaKeySessionServiceProxy ProcessOfflineReleaseResponse Write releaseReponse failed");
            return IPC_PROXY_ERR;
        }
    }
    int32_t error = MediaKeySessionServiceProxy::Remote()->SendRequest(
        MEDIA_KEY_SESSION_PROCESS_OFFLINE_RELEASE_RESPONSE, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy::ProcessOfflineReleaseResponse failed, error: %{public}d", error);
        return error;
    }

    DRM_INFO_LOG("MediaKeySessionServiceProxy::ProcessOfflineReleaseResponse exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySessionServiceProxy::CheckLicenseStatus(std::map<std::string, MediaKeySessionKeyStatus> &licenseStatus)
{
    DRM_INFO_LOG("MediaKeySessionServiceProxy::GenerateOfflineReleaseRequest enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy GenerateOfflineReleaseRequest Write interface token failed");
        return IPC_PROXY_ERR;
    }

    int32_t error = MediaKeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_GENERATE_CHECK_LICENSE_STATUS,
        data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy::GenerateOfflineReleaseRequest failed, error: %{public}d", error);
        return error;
    }
    int32_t licenseStatusMapSize = reply.ReadInt32();
    for (int32_t i = 0; i < licenseStatusMapSize; i++) {
        std::string name = reply.ReadString();
        MediaKeySessionKeyStatus status = (MediaKeySessionKeyStatus)reply.ReadInt32();
        licenseStatus.insert(std::make_pair(name, status));
    }
    DRM_INFO_LOG("MediaKeySessionServiceProxy::GenerateOfflineReleaseRequest exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySessionServiceProxy::RestoreOfflineLicense(std::vector<uint8_t> &licenseId)
{
    DRM_INFO_LOG("MediaKeySessionServiceProxy::RestoreOfflineLicense enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy RestoreOfflineLicense Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(licenseId.size())) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy RestoreOfflineLicense Write licenseId size failed");
        return IPC_PROXY_ERR;
    }
    for (auto id : licenseId) {
        if (!data.WriteUint8(id)) {
            DRM_ERR_LOG("MediaKeySessionServiceProxy RestoreOfflineLicense Write licenseId failed");
            return IPC_PROXY_ERR;
        }
    }

    int32_t error =
        MediaKeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_RESTORE_OFFLINEKEYS, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy::RestoreOfflineLicense failed, error: %{public}d", error);
        return error;
    }

    DRM_INFO_LOG("MediaKeySessionServiceProxy::RestoreOfflineLicense exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySessionServiceProxy::RemoveLicense()
{
    DRM_INFO_LOG("MediaKeySessionServiceProxy::RemoveLicense enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy RemoveLicense Write interface token failed");
        return IPC_PROXY_ERR;
    }

    int32_t error =
        MediaKeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_REMOVE_LICENSE, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy::RemoveLicense failed, error: %{public}d", error);
        return error;
    }

    DRM_INFO_LOG("MediaKeySessionServiceProxy::RemoveLicense exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySessionServiceProxy::RequireSecureDecoderModule(std::string &mimeType, bool *status)
{
    DRM_INFO_LOG("MediaKeySessionServiceProxy::RequireSecureDecoderModule enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy RequireSecureDecoderModule Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteString(mimeType)) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy RequireSecureDecoderModule Write response failed");
        return IPC_PROXY_ERR;
    }

    int32_t error = Remote()->SendRequest(MEDIA_KEY_SESSION_REQUIRE_SECURE_DECODER, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy RequireSecureDecoderModule failed, error: %{public}d", error);
        return error;
    }
    *status = reply.ReadBool();

    DRM_INFO_LOG("MediaKeySessionServiceProxy::RequireSecureDecoderModule exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySessionServiceProxy::SetCallback(sptr<IMediaKeySessionServiceCallback> &callback)
{
    DRM_INFO_LOG("MediaKeySessionServiceProxy::SetCallback enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (callback == nullptr) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy SetCallback callback is null");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy SetCallback Write interface token failed");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteRemoteObject(callback->AsObject())) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy SetCallback write CameraServiceCallback obj failed");
        return IPC_PROXY_ERR;
    }

    int32_t error = Remote()->SendRequest(MEDIA_KEY_SESSION_SET_CALLBACK, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySessionServiceProxy SetCallback failed, error: %{public}d", error);
    }
    return error;
}
} // DrmStandard
} // OHOS