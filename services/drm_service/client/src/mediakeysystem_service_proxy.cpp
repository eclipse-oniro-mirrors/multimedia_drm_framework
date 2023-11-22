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
    int error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_RELEASE, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy Release failed, error: %{public}d", error);
        return error;
    }
    DRM_INFO_LOG("MediaKeySystemServiceProxy::Release exit.");
    return 0;
}

int32_t MediaKeySystemServiceProxy::GenerateKeySystemRequest(std::vector<uint8_t> &request, std::string &defaultUrl)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySystemServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GenerateKeySystemRequest Write interface token failed");
        return IPC_PROXY_ERR;
    }

    uint32_t error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_GENERATE_KEYSYSTEM_REQUEST, data, reply, option);
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

int32_t MediaKeySystemServiceProxy::ProcessKeySystemResponse(const std::vector<uint8_t> &response)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy ProcessKeySystemResponse Write interface token failed");
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

    int error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_PROCESS_KEYSYSTEM_RESPONSE, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy ProcessKeySystemResponse failed, error: %{public}d", error);
        return error;
    }

    DRM_INFO_LOG("MediaKeySystemServiceProxy::ProcessKeySystemResponse exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySystemServiceProxy::GetMaxSecurityLevel(IMediaKeySessionService::SecurityLevel *securityLevel)
{
    DRM_INFO_LOG("MediaKeySystemServiceProxy::GetMaxSecurityLevel enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySystemServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetMaxSecurityLevel Write interface token failed");
        return IPC_PROXY_ERR;
    }

    int error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_GETMAXSECURITYLEVEL, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetMaxSecurityLevel failed, error: %{public}d", error);
        return error;
    }

    *securityLevel = (IMediaKeySessionService::SecurityLevel)reply.ReadInt32();

    DRM_INFO_LOG("MediaKeySystemServiceProxy::GetMaxSecurityLevel exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySystemServiceProxy::GetCertificateStatus(IMediaKeySystemService::CertificateStatus *certStatus)
{
    DRM_INFO_LOG("MediaKeySystemServiceProxy::GetCertificateStatus enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySystemServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetCertificateStatus Write interface token failed");
        return IPC_PROXY_ERR;
    }

    int error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_GETCERTIFICATESTATUS, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetCertificateStatus failed, error: %{public}d", error);
        return error;
    }

    *certStatus = (IMediaKeySystemService::CertificateStatus)reply.ReadInt32();

    DRM_INFO_LOG("MediaKeySystemServiceProxy::GetCertificateStatus exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySystemServiceProxy::SetConfigurationString(std::string &configName, std::string &value)
{
    DRM_INFO_LOG("MediaKeySystemServiceProxy::SetConfiguration enter, configName:%{public}s, value:%{public}s.",
        configName.c_str(), value.c_str());
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy SetConfiguration Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteString(configName)) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy SetConfiguration Write configName failed");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteString(value)) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy SetConfiguration Write value failed");
        return IPC_PROXY_ERR;
    }

    int error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_SETCONFIGURATION_STRING, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy SetConfiguration failed, error: %{public}d", error);
        return error;
    }

    DRM_INFO_LOG("MediaKeySystemServiceProxy::SetConfiguration exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySystemServiceProxy::GetConfigurationString(std::string &configName, std::string &value)
{
    DRM_INFO_LOG("MediaKeySystemServiceProxy::GetConfiguration enter, configName:%{public}s.", configName.c_str());
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetConfiguration Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteString(configName)) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetConfiguration Write configName failed");
        return IPC_PROXY_ERR;
    }

    int error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_GETCONFIGURATION_STRING, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetConfiguration failed, error: %{public}d", error);
        return error;
    }

    value = reply.ReadString();

    DRM_INFO_LOG("MediaKeySystemServiceProxy::GetConfiguration exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySystemServiceProxy::SetConfigurationByteArray(std::string &configName, std::vector<uint8_t> &value)
{
    DRM_INFO_LOG("MediaKeySystemServiceProxy::SetConfiguration enter, configName:%{public}s.", configName.c_str());
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy SetConfiguration Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteString(configName)) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy SetConfiguration Write configName failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(value.size())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy SetConfiguration Write value.size size failed");
        return IPC_PROXY_ERR;
    }

    for (auto res : value) {
        if (!data.WriteUint8(res)) {
            DRM_ERR_LOG("MediaKeySystemServiceProxy SetConfiguration Write value failed");
            return IPC_PROXY_ERR;
        }
    }

    int error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_SETCONFIGURATION_BYTEARRAY, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy SetConfiguration failed, error: %{public}d", error);
        return error;
    }

    DRM_INFO_LOG("MediaKeySystemServiceProxy::SetConfiguration exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySystemServiceProxy::GetConfigurationByteArray(std::string &configName, std::vector<uint8_t> &value)
{
    DRM_INFO_LOG("MediaKeySystemServiceProxy::GetConfiguration enter, configName:%{public}s.", configName.c_str());
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetConfiguration Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteString(configName)) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetConfiguration Write configName failed");
        return IPC_PROXY_ERR;
    }

    int error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_GETCONFIGURATION_BYTEARRAY, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetConfiguration failed, error: %{public}d", error);
        return error;
    }

    uint32_t configSize = reply.ReadInt32();
    for (uint32_t i = 0; i < configSize; i++) {
        value.push_back(reply.ReadUint8());
    }

    DRM_INFO_LOG("MediaKeySystemServiceProxy::GetConfiguration exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySystemServiceProxy::CreateMediaKeySession(IMediaKeySessionService::SecurityLevel securityLevel,
    sptr<IMediaKeySessionService> &keySessionProxy)
{
    DRM_INFO_LOG("MediaKeySystemServiceProxy::CreateMediaKeySession enter, securityLevel:%{public}d.", securityLevel);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy CreateMediaKeySession Write interface token failed");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteInt32(securityLevel)) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy CreateMediaKeySession Write securityLevel failed");
        return IPC_PROXY_ERR;
    }

    int error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_CREATE_KEY_SESSION, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy::CreateMediaKeySession failed, error: %{public}d", error);
        return error;
    }

    auto remoteObject = reply.ReadRemoteObject();
    if (remoteObject != nullptr) {
        keySessionProxy = iface_cast<IMediaKeySessionService>(remoteObject);
    } else {
        DRM_ERR_LOG("MediaKeySystemServiceProxy CreateMediaKeySession keySessionProxy is nullptr");
        error = IPC_PROXY_ERR;
    }
    DRM_INFO_LOG("MediaKeySystemServiceProxy::CreateMediaKeySession exit.");
    return error;
}

int32_t MediaKeySystemServiceProxy::GetMetrics(std::vector<IMediaKeySystemService::MetircKeyValue> &metrics)
{
    DRM_INFO_LOG("MediaKeySystemServiceProxy::GetMetrics enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetMetrics  Write interface token failed");
        return IPC_PROXY_ERR;
    }

    int error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_GETMETRIC, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy::GetMetrics failed, error: %{public}d", error);
        return error;
    }
    int metricsSize = reply.ReadInt32();
    for (int i = 0; i < metricsSize; i++) {
        IMediaKeySystemService::MetircKeyValue keyValue;
        keyValue.name = reply.ReadString();
        keyValue.value = reply.ReadString();
        metrics.push_back(keyValue);
    }
    DRM_INFO_LOG("MediaKeySystemServiceProxy::GetMetrics exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySystemServiceProxy::GetOfflineLicenseIds(std::vector<std::vector<uint8_t>> &licenseIds)
{
    DRM_INFO_LOG("MediaKeySystemServiceProxy::GetOfflineLicenseIds enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySystemServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetOfflineLicenseIds Write interface token failed");
        return IPC_PROXY_ERR;
    }
    int32_t error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_GET_OFFLINELICENSEIDS, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy::GetOfflineLicenseIds failed, error: %{public}d", error);
        return error;
    }
    int32_t licenseIdsSize = reply.ReadInt32();
    for (int i = 0; i < licenseIdsSize; i++) {
        int32_t licenseIdSize = reply.ReadInt32();
        std::vector<uint8_t> licenseId;
        const uint8_t *licenseIdBuf = static_cast<const uint8_t *>(reply.ReadBuffer(licenseIdSize));
        if (licenseIdBuf == nullptr) {
            DRM_ERR_LOG("MediaKeySystemServiceProxy::GetOfflineLicenseIds ReadBuffer failed");
            return error;
        }
        licenseId.assign(licenseIdBuf, licenseIdBuf + licenseIdSize);
        licenseIds.push_back(licenseId);
    }
    DRM_INFO_LOG("MediaKeySystemServiceProxy::GetOfflineLicenseIds exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySystemServiceProxy::GetOfflineLicenseStatus(std::vector<uint8_t> &licenseId,
    IMediaKeySessionService::OfflineLicenseStatus &status)
{
    DRM_INFO_LOG("MediaKeySystemServiceProxy::GetOfflineLicenseStatus enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySystemServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetOfflineLicenseStatus Write interface token failed");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteInt32(licenseId.size())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy GetOfflineLicenseStatus Write licenseId size failed");
        return IPC_PROXY_ERR;
    }
    for (auto id : licenseId) {
        if (!data.WriteUint8(id)) {
            DRM_ERR_LOG("MediaKeySystemServiceProxy GetOfflineLicenseStatus Write licenseId failed");
            return IPC_PROXY_ERR;
        }
    }

    int error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_GET_OFFLINEKEY_STATUS, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy::GetOfflineLicenseStatus failed, error: %{public}d", error);
        return error;
    }
    status = (IMediaKeySessionService::OfflineLicenseStatus)reply.ReadInt32();
    DRM_INFO_LOG("MediaKeySystemServiceProxy::GetOfflineLicenseStatus exit.");
    return reply.ReadInt32();
}

int32_t MediaKeySystemServiceProxy::RemoveOfflineLicense(std::vector<uint8_t> &licenseId)
{
    DRM_INFO_LOG("MediaKeySystemServiceProxy::RemoveOfflineLicense enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySystemServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy RemoveOfflineLicense Write interface token failed");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(licenseId.size())) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy RestoreOfflineLicense Write licenseId size failed");
        return IPC_PROXY_ERR;
    }
    for (auto id : licenseId) {
        if (!data.WriteUint8(id)) {
            DRM_ERR_LOG("MediaKeySystemServiceProxy RestoreOfflineLicense Write licenseId failed");
            return IPC_PROXY_ERR;
        }
    }

    int error = Remote()->SendRequest(MEDIA_KEY_SYSTEM_REMOVE_OFFLINELICENSE, data, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySystemServiceProxy::RemoveOfflineLicense failed, error: %{public}d", error);
        return error;
    }

    DRM_INFO_LOG("MediaKeySystemServiceProxy::RemoveOfflineLicense exit.");
    return reply.ReadInt32();
}
} // DrmStandard
} // OHOS