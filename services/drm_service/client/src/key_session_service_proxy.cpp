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
#include "drm_error_code.h"

namespace OHOS {
namespace DrmStandard {
MediaKeySessionServiceProxy::MediaKeySessionServiceProxy(const sptr<IRemoteObject> &impl)
    : IRemoteProxy<IMediaKeySessionService>(impl)
{
    DRM_INFO_LOG("MediaKeySessionServiceProxy Initialized");
}

int32_t MediaKeySessionServiceProxy::GetMediaDecryptModule(sptr<IMediaDecryptModuleService> &decryptModule)
{
    DRM_INFO_LOG("GetMediaDecryptModule enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("GetMediaDecryptModule Write interface token failed.");
        return IPC_PROXY_ERR;
    }

    int32_t ret = MediaKeySessionServiceProxy::Remote()->SendRequest(GET_MEDIA_DECRYPT_MODULE, data, reply, option);
    if (ret != 0) {
        DRM_ERR_LOG("GetMediaDecryptModule failed, ret: %{public}d", ret);
        return ret;
    }

    auto remoteObject = reply.ReadRemoteObject();
    if (remoteObject != nullptr) {
        decryptModule = iface_cast<IMediaDecryptModuleService>(remoteObject);
    } else {
        DRM_ERR_LOG("GetMediaDecryptModule decryptModule is nullptr");
        ret = IPC_PROXY_ERR;
    }
    return ret;
}

int32_t MediaKeySessionServiceProxy::GetContentProtectionLevel(
    IMediaKeySessionService::ContentProtectionLevel *securityLevel)
{
    DRM_INFO_LOG("GetContentProtectionLevel enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("GetContentProtectionLevel Write interface token failed.");
        return IPC_PROXY_ERR;
    }

    int32_t ret = Remote()->SendRequest(MEDIA_KEY_SESSION_GETSECURITYLEVEL, data, reply, option);
    if (ret != 0) {
        DRM_ERR_LOG("GetContentProtectionLevel failed, ret: %{public}d", ret);
        return ret;
    }

    *securityLevel = (IMediaKeySessionService::ContentProtectionLevel)reply.ReadInt32();

    return ret;
}

int32_t MediaKeySessionServiceProxy::Release()
{
    DRM_INFO_LOG("Release enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("Release Write interface token failed.");
        return IPC_PROXY_ERR;
    }
    int32_t ret = MediaKeySessionServiceProxy::Remote()->SendRequest(KEY_SESSION_RELEASE, data, reply, option);
    if (ret != 0) {
        DRM_ERR_LOG("Release failed, ret: %{public}d", ret);
    }
    return ret;
}

int32_t MediaKeySessionServiceProxy::GenerateMediaKeyRequest(
    IMediaKeySessionService::MediaKeyRequestInfo &licenseRequestInfo,
    IMediaKeySessionService::MediaKeyRequest &licenseRequest)
{
    DRM_INFO_LOG("GenerateMediaKeyRequest enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(MediaKeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("GenerateMediaKeyRequest Write interface token failed.");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(licenseRequestInfo.optionalData.size())) {
        DRM_ERR_LOG("GenerateMediaKeyRequest Write optionalData.size failed.");
        return IPC_PROXY_ERR;
    }
    DRM_CHECK_AND_RETURN_RET_LOG(licenseRequestInfo.optionalData.size() < DATA_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of optionalData is too large.");
    for (auto optionalData : licenseRequestInfo.optionalData) {
        data.WriteString(optionalData.first);
        data.WriteString(optionalData.second);
    }
    if (!data.WriteInt32(licenseRequestInfo.mediaKeyType)) {
        DRM_ERR_LOG("GenerateMediaKeyRequest Write licenseType failed.");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteString(licenseRequestInfo.mimeType)) {
        DRM_ERR_LOG("GenerateMediaKeyRequest Write mimeType failed.");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteInt32(licenseRequestInfo.initData.size())) {
        DRM_ERR_LOG("GenerateMediaKeyRequest Write initData.size failed.");
        return IPC_PROXY_ERR;
    }
    DRM_CHECK_AND_RETURN_RET_LOG(licenseRequestInfo.initData.size() < DATA_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of initData is too large.");
    if (licenseRequestInfo.initData.size() != 0) {
        if (!data.WriteBuffer(licenseRequestInfo.initData.data(), licenseRequestInfo.initData.size())) {
            DRM_ERR_LOG("GenerateMediaKeyRequest Write initData.size failed.");
            return IPC_PROXY_ERR;
        }
    }
    int32_t ret = MediaKeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_GENERATE_LICENSE_REQUEST, data,
        reply, option);
    if (ret != 0) {
        DRM_ERR_LOG("GenerateMediaKeyRequest failed, errcode: %{public}d", ret);
        return ret;
    }
    licenseRequest.mDefaultURL = reply.ReadString();
    licenseRequest.requestType = (IMediaKeySessionService::RequestType)reply.ReadInt32();
    int32_t dataSize = reply.ReadInt32();
    DRM_CHECK_AND_RETURN_RET_LOG(dataSize < DATA_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of initData is too large.");
    if (dataSize != 0) {
        const uint8_t *mDataBuf = static_cast<const uint8_t *>(reply.ReadUnpadBuffer(dataSize));
        if (mDataBuf == nullptr) {
            DRM_ERR_LOG("read licenseRequest.mData failed.");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
        licenseRequest.mData.assign(mDataBuf, mDataBuf + dataSize);
    }
    return ret;
}

int32_t MediaKeySessionServiceProxy::ProcessMediaKeyResponse(std::vector<uint8_t> &licenseId,
    std::vector<uint8_t> &licenseResponse)
{
    DRM_INFO_LOG("ProcessMediaKeyResponse enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("ProcessMediaKeyResponse Write interface token failed.");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteInt32(licenseResponse.size())) {
        DRM_ERR_LOG("ProcessMediaKeyResponse Write licenseResponse size failed.");
        return IPC_PROXY_ERR;
    }
    DRM_CHECK_AND_RETURN_RET_LOG(licenseResponse.size() < RESPONSE_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of response is too large.");
    if (licenseResponse.size() != 0) {
        if (!data.WriteBuffer(licenseResponse.data(), licenseResponse.size())) {
            DRM_ERR_LOG("ProcessMediaKeyResponse write licenseResponse failed.");
            return IPC_PROXY_ERR;
        }
    }
    int32_t ret = MediaKeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_PROCESS_LICENSE_RESPONSE, data,
        reply, option);
    if (ret != 0) {
        DRM_ERR_LOG("ProcessMediaKeyResponse failed, ret: %{public}d", ret);
        return ret;
    }
    int32_t licenseIdSize = reply.ReadInt32();
    DRM_CHECK_AND_RETURN_RET_LOG(licenseIdSize < LICENSEID_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of licenseId is too large.");
    if (licenseIdSize != 0) {
        const uint8_t *licenseIdBuf = static_cast<const uint8_t *>(reply.ReadUnpadBuffer(licenseIdSize));
        if (licenseIdBuf == nullptr) {
            DRM_ERR_LOG("ProcessMediaKeyResponse read licenseId failed.");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
        licenseId.assign(licenseIdBuf, licenseIdBuf + licenseIdSize);
    }
    return ret;
}

int32_t MediaKeySessionServiceProxy::GenerateOfflineReleaseRequest(std::vector<uint8_t> &licenseId,
    std::vector<uint8_t> &releaseRequest)
{
    DRM_INFO_LOG("GenerateOfflineReleaseRequest enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("GenerateOfflineReleaseRequest Write interface token failed.");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteInt32(licenseId.size())) {
        DRM_ERR_LOG("GenerateOfflineReleaseRequest Write licenseId size failed.");
        return IPC_PROXY_ERR;
    }
    DRM_CHECK_AND_RETURN_RET_LOG(licenseId.size() < LICENSEID_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of licenseId is too large.");
    if (licenseId.size() != 0) {
        if (!data.WriteBuffer(licenseId.data(), licenseId.size())) {
            DRM_ERR_LOG("ProcessMediaKeyResponse write licenseId failed.");
            return IPC_PROXY_ERR;
        }
    }

    int32_t ret = MediaKeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_GENERATE_OFFLINE_RELEASE_REQUEST,
        data, reply, option);
    if (ret != 0) {
        DRM_ERR_LOG("GenerateOfflineReleaseRequest failed, ret: %{public}d", ret);
        return ret;
    }
    int32_t requestSize = reply.ReadInt32();
    DRM_CHECK_AND_RETURN_RET_LOG(requestSize < REQUEST_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of request is too large.");
    if (requestSize != 0) {
        const uint8_t *requestBuf = static_cast<const uint8_t *>(reply.ReadBuffer(requestSize));
        if (requestBuf == nullptr) {
            DRM_ERR_LOG("ProcessMediaKeyResponse read licenseId failed.");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
        releaseRequest.assign(requestBuf, requestBuf + requestSize);
    }
    return ret;
}

int32_t MediaKeySessionServiceProxy::ProcessOfflineReleaseResponse(std::vector<uint8_t> &licenseId,
    std::vector<uint8_t> &releaseReponse)
{
    DRM_INFO_LOG("ProcessOfflineReleaseResponse enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("ProcessOfflineReleaseResponse Write interface token failed.");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(licenseId.size())) {
        DRM_ERR_LOG("ProcessOfflineReleaseResponse Write licenseId size failed.");
        return IPC_PROXY_ERR;
    }
    DRM_CHECK_AND_RETURN_RET_LOG(licenseId.size() < LICENSEID_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of licenseId is too large.");
    if (licenseId.size() != 0) {
        if (!data.WriteBuffer(licenseId.data(), licenseId.size())) {
            DRM_ERR_LOG("ProcessOfflineReleaseResponse write licenseId failed.");
            return IPC_PROXY_ERR;
        }
    }
    if (!data.WriteInt32(releaseReponse.size())) {
        DRM_ERR_LOG("ProcessOfflineReleaseResponse Write releaseReponse size failed.");
        return IPC_PROXY_ERR;
    }
    DRM_CHECK_AND_RETURN_RET_LOG(releaseReponse.size() < RESPONSE_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of response is too large.");
    if (releaseReponse.size() != 0) {
        if (!data.WriteBuffer(releaseReponse.data(), releaseReponse.size())) {
            DRM_ERR_LOG("ProcessOfflineReleaseResponse write releaseReponse failed.");
            return IPC_PROXY_ERR;
        }
    }
    int32_t ret = MediaKeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_PROCESS_OFFLINE_RELEASE_RESPONSE,
        data, reply, option);
    if (ret != 0) {
        DRM_ERR_LOG("ProcessOfflineReleaseResponse failed, errcode: %{public}d", ret);
        return ret;
    }
    return ret;
}

int32_t MediaKeySessionServiceProxy::CheckMediaKeyStatus(std::map<std::string, std::string> &licenseStatus)
{
    DRM_INFO_LOG("GenerateOfflineReleaseRequest enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("GenerateOfflineReleaseRequest Write interface token failed.");
        return IPC_PROXY_ERR;
    }

    int32_t ret = MediaKeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_GENERATE_CHECK_LICENSE_STATUS,
        data, reply, option);
    if (ret != 0) {
        DRM_ERR_LOG("GenerateOfflineReleaseRequest failed, errcode: %{public}d", ret);
        return ret;
    }
    int32_t licenseStatusMapSize = reply.ReadInt32();
    DRM_CHECK_AND_RETURN_RET_LOG(licenseStatusMapSize <= MAX_MEDIA_KEY_STATUS_NUMBER, DRM_INNER_ERR_MEMORY_ERROR,
        "The number of license status is too large.");
    for (int32_t i = 0; i < licenseStatusMapSize; i++) {
        std::string name = reply.ReadString();
        std::string status = reply.ReadString();
        licenseStatus.insert(std::make_pair(name, status));
    }
    return ret;
}

int32_t MediaKeySessionServiceProxy::RestoreOfflineMediaKeys(std::vector<uint8_t> &licenseId)
{
    DRM_INFO_LOG("RestoreOfflineMediaKeys enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("RestoreOfflineMediaKeys Write interface token failed.");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInt32(licenseId.size())) {
        DRM_ERR_LOG("RestoreOfflineMediaKeys Write licenseId size failed.");
        return IPC_PROXY_ERR;
    }
    DRM_CHECK_AND_RETURN_RET_LOG(licenseId.size() < LICENSEID_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of licenseId is too large.");
    if (licenseId.size() != 0) {
        if (!data.WriteBuffer(licenseId.data(), licenseId.size())) {
            DRM_ERR_LOG("RestoreOfflineMediaKeys write licenseId failed.");
            return IPC_PROXY_ERR;
        }
    }

    int32_t ret =
        MediaKeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_RESTORE_OFFLINEKEYS, data, reply, option);
    if (ret != 0) {
        DRM_ERR_LOG("RestoreOfflineMediaKeys failed, errcode: %{public}d", ret);
        return ret;
    }
    return ret;
}

int32_t MediaKeySessionServiceProxy::ClearMediaKeys()
{
    DRM_INFO_LOG("ClearMediaKeys enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(MediaKeySessionServiceProxy::GetDescriptor())) {
        DRM_ERR_LOG("ClearMediaKeys Write interface token failed.");
        return IPC_PROXY_ERR;
    }

    int32_t ret =
        MediaKeySessionServiceProxy::Remote()->SendRequest(MEDIA_KEY_SESSION_REMOVE_LICENSE, data, reply, option);
    if (ret != 0) {
        DRM_ERR_LOG("ClearMediaKeys failed, errcode: %{public}d", ret);
        return ret;
    }
    return ret;
}

int32_t MediaKeySessionServiceProxy::RequireSecureDecoderModule(std::string &mimeType, bool *status)
{
    DRM_INFO_LOG("RequireSecureDecoderModule enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("RequireSecureDecoderModule Write interface token failed.");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteString(mimeType)) {
        DRM_ERR_LOG("RequireSecureDecoderModule Write response failed.");
        return IPC_PROXY_ERR;
    }

    int32_t ret = Remote()->SendRequest(MEDIA_KEY_SESSION_REQUIRE_SECURE_DECODER, data, reply, option);
    if (ret != 0) {
        DRM_ERR_LOG("RequireSecureDecoderModule failed, errcode: %{public}d", ret);
        return ret;
    }
    *status = reply.ReadBool();
    return ret;
}

int32_t MediaKeySessionServiceProxy::SetCallback(const sptr<IMediaKeySessionServiceCallback> &callback)
{
    DRM_INFO_LOG("SetCallback enter.");
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;

    if (callback == nullptr) {
        DRM_ERR_LOG("SetCallback callback is null");
        return IPC_PROXY_ERR;
    }

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("SetCallback Write interface token failed.");
        return IPC_PROXY_ERR;
    }
    if (!data.WriteRemoteObject(callback->AsObject())) {
        DRM_ERR_LOG("SetCallback write CameraServiceCallback obj failed.");
        return IPC_PROXY_ERR;
    }

    int32_t ret = Remote()->SendRequest(MEDIA_KEY_SESSION_SET_CALLBACK, data, reply, option);
    if (ret != 0) {
        DRM_ERR_LOG("SetCallback failed, errcode: %{public}d", ret);
    }
    return ret;
}
} // DrmStandard
} // OHOS