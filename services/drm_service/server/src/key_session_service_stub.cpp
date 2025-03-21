/*
 * Copyright (c) 2023  Huawei Device Co., Ltd.
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

#include "key_session_service_stub.h"
#include "drm_error_code.h"
#include "drm_log.h"
#include "ipc_skeleton.h"
#include "xcollie/xcollie.h"
#include "xcollie/xcollie_define.h"

namespace OHOS {
namespace DrmStandard {
using ProcessRemoteRequestFunc = int32_t (*)(MediaKeySessionServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option);
struct ProcessRemoteRequestFuncArray {
    OHOS::DrmStandard::MediaKeySessionServiceRequestCode requestCode;
    ProcessRemoteRequestFunc processFunc;
};

static int32_t ProcessGetMediaDecryptModule(MediaKeySessionServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option);

static int32_t ProcessReleaseKeySession(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessMediaKeyRequest(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessMediaKeyResponse(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessOfflineReleaseRequest(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessOfflineReleaseResponse(MediaKeySessionServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option);

static int32_t ProcessCheckMediaKeyStatus(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessRestoreOfflineMediaKey(MediaKeySessionServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option);

static int32_t ProcessClearMediaKeys(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessSetCallback(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessRequireSecureDecoder(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessGetContentProtectionLevel(MediaKeySessionServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option);

static struct ProcessRemoteRequestFuncArray g_mediaKeySessionServiceStubRequestProcessFunc[] = {
    {GET_MEDIA_DECRYPT_MODULE, ProcessGetMediaDecryptModule},
    {KEY_SESSION_RELEASE, ProcessReleaseKeySession},
    {MEDIA_KEY_SESSION_GENERATE_LICENSE_REQUEST, ProcessMediaKeyRequest},
    {MEDIA_KEY_SESSION_PROCESS_LICENSE_RESPONSE, ProcessMediaKeyResponse},
    {MEDIA_KEY_SESSION_GENERATE_OFFLINE_RELEASE_REQUEST, ProcessOfflineReleaseRequest},
    {MEDIA_KEY_SESSION_PROCESS_OFFLINE_RELEASE_RESPONSE, ProcessOfflineReleaseResponse},
    {MEDIA_KEY_SESSION_GENERATE_CHECK_LICENSE_STATUS, ProcessCheckMediaKeyStatus},
    {MEDIA_KEY_SESSION_RESTORE_OFFLINEKEYS, ProcessRestoreOfflineMediaKey},
    {MEDIA_KEY_SESSION_REMOVE_LICENSE, ProcessClearMediaKeys},
    {MEDIA_KEY_SESSION_SET_CALLBACK, ProcessSetCallback},
    {MEDIA_KEY_SESSION_REQUIRE_SECURE_DECODER, ProcessRequireSecureDecoder},
    {MEDIA_KEY_SESSION_GETSECURITYLEVEL, ProcessGetContentProtectionLevel},
};

static int32_t ProcessGetMediaDecryptModule(MediaKeySessionServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option)
{
    DRM_INFO_LOG("ProcessGetMediaDecryptModule enter.");
    sptr<IMediaDecryptModuleService> decryptModuleServiceProxy = nullptr;
    int32_t ret = stub->GetMediaDecryptModule(decryptModuleServiceProxy);
    if (ret != 0) {
        DRM_ERR_LOG("GetMediaDecryptModule failed. errCode:%{public}d", ret);
        return ret;
    }
    if (!reply.WriteRemoteObject(decryptModuleServiceProxy->AsObject())) {
        DRM_ERR_LOG("Write MediaDecryptModule obj failed.");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    return 0;
}

static int32_t ProcessReleaseKeySession(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("ProcessReleaseKeySession enter.");
    int32_t ret = stub->Release();
    return ret;
}

static int32_t ProcessMediaKeyRequest(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("ProcessMediaKeyRequest enter.");
    IMediaKeySessionService::MediaKeyRequestInfo licenseRequestInfo;
    IMediaKeySessionService::MediaKeyRequest licenseRequest;
    int32_t optionalDataMapSize = data.ReadInt32();
    DRM_CHECK_AND_RETURN_RET_LOG(optionalDataMapSize < DATA_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of optionalData is too large.");
    for (int32_t i = 0; i < optionalDataMapSize; i++) {
        std::string name = data.ReadString();
        std::string value = data.ReadString();
        licenseRequestInfo.optionalData.insert(std::make_pair(name, value));
    }
    licenseRequestInfo.mediaKeyType = (OHOS::DrmStandard::IMediaKeySessionService::MediaKeyType)data.ReadInt32();
    licenseRequestInfo.mimeType = data.ReadString();
    int32_t initDataSize = data.ReadInt32();
    DRM_CHECK_AND_RETURN_RET_LOG(initDataSize < DATA_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of initData is too large.");
    if (initDataSize != 0) {
        const uint8_t *initDataBuf = static_cast<const uint8_t *>(data.ReadUnpadBuffer(initDataSize));
        if (initDataBuf == nullptr) {
            DRM_ERR_LOG("ReadUnpadBuffer failed.");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
        licenseRequestInfo.initData.assign(initDataBuf, initDataBuf + initDataSize);
    }
    int32_t ret = stub->GenerateMediaKeyRequest(licenseRequestInfo, licenseRequest);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "GenerateMediaKeyRequest faild, errCode:%{public}d", ret);
    if (!reply.WriteString(licenseRequest.mDefaultURL)) {
        DRM_ERR_LOG("Write licenseRequest.mDefaultURL GenerateMediaKeyRequest failed.");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }

    if (!reply.WriteInt32(licenseRequest.requestType)) {
        DRM_ERR_LOG("Write requestType GenerateMediaKeyRequest failed.");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    if (!reply.WriteInt32(licenseRequest.mData.size())) {
        DRM_ERR_LOG("Write licenseRequestmDatasize GenerateMediaKeyRequest failed.");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    DRM_CHECK_AND_RETURN_RET_LOG(licenseRequest.mData.size() < REQUEST_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of mData is too large.");
    if (licenseRequest.mData.size() != 0) {
        if (!reply.WriteBuffer(licenseRequest.mData.data(), licenseRequest.mData.size())) {
            DRM_ERR_LOG("GenerateMediaKeyRequest write mData failed.");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
    }
    return ret;
}

static int32_t ProcessMediaKeyResponse(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("ProcessMediaKeyResponse enter.");
    std::vector<uint8_t> response;
    std::vector<uint8_t> licenseId;
    int32_t responseSize = data.ReadInt32();
    DRM_CHECK_AND_RETURN_RET_LOG(responseSize < RESPONSE_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of response is too large.");
    if (responseSize != 0) {
        const uint8_t *responseBuf = static_cast<const uint8_t *>(data.ReadUnpadBuffer(responseSize));
        if (responseBuf == nullptr) {
            DRM_ERR_LOG("ProcessMediaKeyResponse read response failed.");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
        response.assign(responseBuf, responseBuf + responseSize);
    }
    int32_t ret = stub->ProcessMediaKeyResponse(licenseId, response);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "ProcessMediaKeyResponse faild, ret:%{public}d", ret);
    if (!reply.WriteInt32(licenseId.size())) {
        DRM_ERR_LOG("GenerateMediaKeyRequest Write licenseId.size failed.");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    DRM_CHECK_AND_RETURN_RET_LOG(licenseId.size() < LICENSEID_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of licenseId is too large.");
    if (licenseId.size() != 0) {
        if (!reply.WriteBuffer(licenseId.data(), licenseId.size())) {
            DRM_ERR_LOG("GenerateMediaKeyRequest write licenseId failed.");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
    }
    return ret;
}

static int32_t ProcessOfflineReleaseRequest(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("ProcessOfflineReleaseRequest enter.");
    std::vector<uint8_t> licenseId;
    int32_t licenseIdSize = data.ReadInt32();
    DRM_CHECK_AND_RETURN_RET_LOG(licenseIdSize < LICENSEID_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of licenseId is too large.");
    if (licenseIdSize != 0) {
        const uint8_t *licenseIdBuf = static_cast<const uint8_t *>(data.ReadUnpadBuffer(licenseIdSize));
        if (licenseIdBuf == nullptr) {
            DRM_ERR_LOG("ProcessOfflineReleaseRequest read licenseId failed.");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
        licenseId.assign(licenseIdBuf, licenseIdBuf + licenseIdSize);
    }
    std::vector<uint8_t> releaseRequest;
    int32_t ret = stub->GenerateOfflineReleaseRequest(licenseId, releaseRequest);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "GenerateOfflineReleaseRequest faild, errCode:%{public}d", ret);
    if (!reply.WriteInt32(releaseRequest.size())) {
        DRM_ERR_LOG("Write releaseRequest.size failed.");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    DRM_CHECK_AND_RETURN_RET_LOG(releaseRequest.size() < REQUEST_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of releaseRequest is too large.");
    if (releaseRequest.size() != 0) {
        if (!reply.WriteBuffer(releaseRequest.data(), releaseRequest.size())) {
            DRM_ERR_LOG("ProcessOfflineReleaseRequest Write licenseId failed.");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
    }
    return ret;
}

static int32_t ProcessOfflineReleaseResponse(MediaKeySessionServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option)
{
    DRM_INFO_LOG("ProcessOfflineReleaseResponse enter.");
    int32_t licenseIdSize = data.ReadInt32();
    std::vector<uint8_t> licenseId;
    DRM_CHECK_AND_RETURN_RET_LOG(licenseIdSize < LICENSEID_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of licenseId is too large.");
    const uint8_t *licenseIdBuf = static_cast<const uint8_t *>(data.ReadUnpadBuffer(licenseIdSize));
    if (licenseIdBuf == nullptr) {
        DRM_ERR_LOG("ProcessOfflineReleaseResponse read licenseId failed.");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    licenseId.assign(licenseIdBuf, licenseIdBuf + licenseIdSize);
    std::vector<uint8_t> response;
    int32_t responseSize = data.ReadInt32();
    DRM_CHECK_AND_RETURN_RET_LOG(responseSize < RESPONSE_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of response is too large.");
    if (responseSize != 0) {
        const uint8_t *responseBuf = static_cast<const uint8_t *>(data.ReadUnpadBuffer(responseSize));
        if (responseBuf == nullptr) {
            DRM_ERR_LOG("ProcessOfflineReleaseResponse read response failed.");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
        response.assign(responseBuf, responseBuf + responseSize);
    }
    int32_t ret = stub->ProcessOfflineReleaseResponse(licenseId, response);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "ProcessOfflineReleaseResponse faild, errCode:%{public}d", ret);
    return ret;
}

static int32_t ProcessCheckMediaKeyStatus(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("ProcessCheckMediaKeyStatus enter.");
    std::map<std::string, std::string> licenseStatus;
    int32_t ret = stub->CheckMediaKeyStatus(licenseStatus);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "CheckMediaKeyStatus faild, errCode:%{public}d", ret);

    reply.WriteInt32(licenseStatus.size());
    for (auto licenseStatu : licenseStatus) {
        reply.WriteString(licenseStatu.first);
        reply.WriteString(licenseStatu.second);
    }
    return ret;
}

static int32_t ProcessRestoreOfflineMediaKey(MediaKeySessionServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option)
{
    DRM_INFO_LOG("ProcessRestoreOfflineMediaKey enter.");
    std::vector<uint8_t> licenseId;
    int32_t licenseIdSize = data.ReadInt32();
    DRM_CHECK_AND_RETURN_RET_LOG(licenseIdSize < LICENSEID_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of licenseId is too large.");
    if (licenseIdSize != 0) {
        const uint8_t *licenseIdBuf = static_cast<const uint8_t *>(data.ReadUnpadBuffer(licenseIdSize));
        if (licenseIdBuf == nullptr) {
            DRM_ERR_LOG("ProcessRestoreOfflineMediaKey read licenseId failed.");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
        licenseId.assign(licenseIdBuf, licenseIdBuf + licenseIdSize);
    }
    int32_t ret = stub->RestoreOfflineMediaKeys(licenseId);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "RestoreOfflineMediaKeys faild, errCode:%{public}d", ret);
    return ret;
}

static int32_t ProcessClearMediaKeys(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("ProcessClearMediaKeys enter.");
    int32_t ret = stub->ClearMediaKeys();
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "ClearMediaKeys faild, errCode:%{public}d", ret);
    return ret;
}

static int32_t ProcessSetCallback(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("ProcessSetCallback enter.");
    auto remoteObject = data.ReadRemoteObject();
    if (remoteObject == nullptr) {
        DRM_ERR_LOG("ProcessSetCallback ReadRemoteObject is null");
        return IPC_STUB_INVALID_DATA_ERR;
    }
    auto callback = iface_cast<IMediaKeySessionServiceCallback>(remoteObject);
    if (callback == nullptr) {
        DRM_ERR_LOG("iface_cast cast nullptr");
        return IPC_STUB_INVALID_DATA_ERR;
    }
    int32_t ret = stub->SetCallback(callback);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "SetCallback faild, errCode:%{public}d", ret);
    return ret;
}

static int32_t ProcessRequireSecureDecoder(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("ProcessRequireSecureDecoder enter.");
    std::string mimeType = data.ReadString();
    bool status;
    int32_t ret = stub->RequireSecureDecoderModule(mimeType, &status);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "ProcessRequireSecureDecoder faild, errCode:%{public}d", ret);
    reply.WriteBool(status);
    return ret;
}

void MediaKeySessionServiceStub::MediaKeySessionClientDied(pid_t pid)
{
    DRM_ERR_LOG("MediaKeySession client has died, pid:%{public}d", pid);
}

static int32_t ProcessGetContentProtectionLevel(MediaKeySessionServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option)
{
    DRM_INFO_LOG("ProcessGetContentProtectionLevel enter.");
    IMediaKeySessionService::ContentProtectionLevel securityLevel =
        IMediaKeySessionService::CONTENT_PROTECTION_LEVEL_UNKNOWN;
    int32_t ret = stub->GetContentProtectionLevel(&securityLevel);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "ProcessGetContentProtectionLevel faild, errCode:%{public}d", ret);
    if (!reply.WriteInt32(securityLevel)) {
        DRM_ERR_LOG("Write GetContentProtectionLevel failed.");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    return ret;
}

int32_t MediaKeySessionServiceStub::OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("OnRemoteRequest enter.");
    int32_t ret = DRM_INNER_ERR_BASE;
    DRM_DEBUG_LOG("OnRemoteRequest, cmd = %{public}u", code);
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " is keySessionServiceStub", FAKE_POINTER(this));

    DRM_CHECK_AND_RETURN_RET_LOG(data.ReadInterfaceToken() == GetDescriptor(), ret,
        "ReadInterfaceToken failed.");

    DRM_CHECK_AND_RETURN_RET_LOG((code >= GET_MEDIA_DECRYPT_MODULE) && (code <= MEDIA_KEY_SESSION_GETSECURITYLEVEL),
        IPCObjectStub::OnRemoteRequest(code, data, reply, option),
        "code not match, need check MediaKeySessionServiceStub");
    return g_mediaKeySessionServiceStubRequestProcessFunc[code].processFunc(this, data, reply, option);
}
} // DrmStandard
} // OHOS