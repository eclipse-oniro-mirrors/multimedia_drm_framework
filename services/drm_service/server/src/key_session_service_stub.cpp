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

static int32_t ProcessCreateMediaDecryptModule(MediaKeySessionServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option);

static int32_t ProcessRleaseKeySession(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessLicenseRequest(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessLicenseResponse(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessOfflineReleaseRequest(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessOfflineReleaseResponse(MediaKeySessionServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option);

static int32_t ProcessCheckLicenseStatus(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessRestoreOfflineLicense(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessRemoveLicense(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessSetCallback(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessRequireSecureDecoder(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessGetSecurityLevel(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static struct ProcessRemoteRequestFuncArray g_mediaKeySessionServiceStubRequestProcessFunc[] = {
    {CREATE_MEDIA_DECRYPT_MODULE, ProcessCreateMediaDecryptModule},
    {KEY_SESSION_RELEASE, ProcessRleaseKeySession},
    {MEDIA_KEY_SESSION_GENERATE_LICENSE_REQUEST, ProcessLicenseRequest},
    {MEDIA_KEY_SESSION_PROCESS_LICENSE_RESPONSE, ProcessLicenseResponse},
    {MEDIA_KEY_SESSION_GENERATE_OFFLINE_RELEASE_REQUEST, ProcessOfflineReleaseRequest},
    {MEDIA_KEY_SESSION_PROCESS_OFFLINE_RELEASE_RESPONSE, ProcessOfflineReleaseResponse},
    {MEDIA_KEY_SESSION_GENERATE_CHECK_LICENSE_STATUS, ProcessCheckLicenseStatus},
    {MEDIA_KEY_SESSION_RESTORE_OFFLINEKEYS, ProcessRestoreOfflineLicense},
    {MEDIA_KEY_SESSION_REMOVE_LICENSE, ProcessRemoveLicense},
    {MEDIA_KEY_SESSION_SET_CALLBACK, ProcessSetCallback},
    {MEDIA_KEY_SESSION_REQUIRE_SECURE_DECODER, ProcessRequireSecureDecoder},
    {MEDIA_KEY_SESSION_GETSECURITYLEVEL, ProcessGetSecurityLevel},
};

static int32_t ProcessCreateMediaDecryptModule(MediaKeySessionServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub GET_MEDIA_DECRYPT_MODULE enter.");
    sptr<IMediaDecryptModuleService> decryptModuleServiceProxy = nullptr;
    int32_t errCode = stub->CreateMediaDecryptModule(decryptModuleServiceProxy);
    if (errCode != ERR_NONE) {
        DRM_ERR_LOG("MediaKeySessionServiceStub CreateMediaDecryptModule failed : %{public}d", errCode);
        return errCode;
    }
    if (!reply.WriteRemoteObject(decryptModuleServiceProxy->AsObject())) {
        DRM_ERR_LOG("MediaKeySessionServiceStub Write MediaDecryptModule obj failed");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    DRM_INFO_LOG("MediaKeySessionServiceStub GET_MEDIA_DECRYPT_MODULE exit.");
    return DRM_OK;
}

static int32_t ProcessRleaseKeySession(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub KEY_SESSION_RELEASE enter.");
    int32_t errCode = stub->Release();
    DRM_INFO_LOG("MediaKeySessionServiceStub KEY_SESSION_RELEASE exit.");
    return errCode;
}

static int32_t ProcessLicenseRequest(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_GENERATE_LICENSE_REQUEST enter.");
    IMediaKeySessionService::LicenseRequestInfo licenseRequestInfo;
    IMediaKeySessionService::LicenseRequest licenseRequest;
    int32_t optionalDataMapSize = data.ReadInt32();
    for (int32_t i = 0; i < optionalDataMapSize; i++) {
        std::string name = data.ReadString();
        std::string value = data.ReadString();
        licenseRequestInfo.optionalData.insert(std::make_pair(name, value));
    }
    licenseRequestInfo.licenseType = (OHOS::DrmStandard::IMediaKeySessionService::LicenseType)data.ReadInt32();
    licenseRequestInfo.mimeType = data.ReadString();
    uint32_t initDataSize = data.ReadInt32();
    DRM_CHECK_AND_RETURN_RET_LOG(initDataSize < DATA_MAX_LEN, DRM_MEMORY_ERROR, "The size of initData is too large.");
    if (initDataSize != 0) {
        const uint8_t *initDataBuf = static_cast<const uint8_t *>(data.ReadBuffer(initDataSize));
        if (initDataBuf == nullptr) {
            DRM_ERR_LOG("MediaKeySessionServiceStub ReadBuffer failed");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
        licenseRequestInfo.initData.assign(initDataBuf, initDataBuf + initDataSize);
    }
    int32_t ret = stub->GenerateLicenseRequest(licenseRequestInfo, licenseRequest);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_OK, ret, "GenerateLicenseRequest faild, errCode:%{public}d", ret);
    if (!reply.WriteString(licenseRequest.mDefaultURL)) {
        DRM_ERR_LOG("MediaKeySessionServiceStub Write licenseRequest.mDefaultURL GenerateLicenseRequest failed");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    
    if (!reply.WriteInt32(licenseRequest.requestType)) {
        DRM_ERR_LOG("MediaKeySessionServiceStub Write requestType GenerateLicenseRequest failed");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    if (!reply.WriteInt32(licenseRequest.mData.size())) {
        DRM_ERR_LOG("MediaKeySessionServiceStub Write licenseRequestmDatasize GenerateLicenseRequest failed");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    DRM_CHECK_AND_RETURN_RET_LOG(licenseRequest.mData.size() < REQUEST_MAX_LEN, DRM_MEMORY_ERROR,
        "The size of mData is too large.");
    if (licenseRequest.mData.size() != 0) {
        if (!reply.WriteBuffer(licenseRequest.mData.data(), licenseRequest.mData.size())) {
            DRM_ERR_LOG("MediaKeySessionServiceStub GenerateLicenseRequest write mData failed");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
    }
    
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_GENERATE_LICENSE_REQUEST exit.");
    return ret;
}

static int32_t ProcessLicenseResponse(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SYSTEM_PROCESS_LICENSE_RESPONSE enter.");
    std::vector<uint8_t> response;
    std::vector<uint8_t> licenseId;
    uint32_t responseSize = data.ReadInt32();
    DRM_CHECK_AND_RETURN_RET_LOG(responseSize < RESPONSE_MAX_LEN, DRM_MEMORY_ERROR,
        "The size of response is too large.");
    if (responseSize != 0) {
        const uint8_t *responseBuf = static_cast<const uint8_t *>(data.ReadBuffer(responseSize));
        if (responseBuf == nullptr) {
            DRM_ERR_LOG("MediaKeySystemServiceProxy::ProcessLicenseResponse read response failed");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
        response.assign(responseBuf, responseBuf + responseSize);
    }
    int32_t ret = stub->ProcessLicenseResponse(licenseId, response);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_OK, ret, "ProcessLicenseResponse faild, ret:%{public}d", ret);
    if (!reply.WriteInt32(licenseId.size())) {
        DRM_ERR_LOG("MediaKeySessionServiceStub GenerateLicenseRequest Write licenseId.size failed");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    DRM_CHECK_AND_RETURN_RET_LOG(licenseId.size() < LICENSEID_MAX_LEN, DRM_MEMORY_ERROR,
        "The size of licenseId is too large.");
    if (licenseId.size() != 0) {
        if (!reply.WriteBuffer(licenseId.data(), licenseId.size())) {
            DRM_ERR_LOG("MediaKeySessionServiceStub GenerateLicenseRequest write licenseId failed");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
    }

    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SYSTEM_PROCESS_LICENSE_RESPONSE exit.");
    return ret;
}

static int32_t ProcessOfflineReleaseRequest(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_GENERATE_OFFLINE_RELEASE_REQUEST enter.");
    std::vector<uint8_t> licenseId;
    uint32_t licenseIdSize = data.ReadInt32();
    DRM_CHECK_AND_RETURN_RET_LOG(licenseIdSize < LICENSEID_MAX_LEN, DRM_MEMORY_ERROR,
        "The size of licenseId is too large.");
    if (licenseIdSize != 0) {
        const uint8_t *licenseIdBuf = static_cast<const uint8_t *>(data.ReadBuffer(licenseIdSize));
        if (licenseIdBuf == nullptr) {
            DRM_ERR_LOG("MediaKeySessionServiceStub::ProcessOfflineReleaseRequest read licenseId failed");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
        licenseId.assign(licenseIdBuf, licenseIdBuf + licenseIdSize);
    }
    std::vector<uint8_t> releaseRequest;
    int32_t ret = stub->GenerateOfflineReleaseRequest(licenseId, releaseRequest);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_OK, ret, "GenerateOfflineReleaseRequest faild, errCode:%{public}d", ret);
    if (!reply.WriteInt32(releaseRequest.size())) {
        DRM_ERR_LOG("MediaKeySessionServiceStub Write releaseRequest.size failed");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    DRM_CHECK_AND_RETURN_RET_LOG(releaseRequest.size() < REQUEST_MAX_LEN, DRM_MEMORY_ERROR,
        "The size of releaseRequest is too large.");
    if (releaseRequest.size() != 0) {
        if (!reply.WriteBuffer(releaseRequest.data(), releaseRequest.size())) {
            DRM_ERR_LOG("MediaKeySessionServiceStub ProcessOfflineReleaseRequest Write licenseId failed");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
    }
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_GENERATE_OFFLINE_RELEASE_REQUEST exit.");
    return ret;
}

static int32_t ProcessOfflineReleaseResponse(MediaKeySessionServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_PROCESS_OFFLINE_RELEASE_RESPONSE enter.");
    uint32_t licenseIdSize = data.ReadInt32();
    std::vector<uint8_t> licenseId;
    DRM_CHECK_AND_RETURN_RET_LOG(licenseIdSize < LICENSEID_MAX_LEN, DRM_MEMORY_ERROR,
        "The size of licenseId is too large.");
    const uint8_t *licenseIdBuf = static_cast<const uint8_t *>(data.ReadBuffer(licenseIdSize));
    if (licenseIdBuf == nullptr) {
        DRM_ERR_LOG("MediaKeySessionServiceStub::ProcessOfflineReleaseResponse read licenseId failed");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    licenseId.assign(licenseIdBuf, licenseIdBuf + licenseIdSize);
    std::vector<uint8_t> response;
    uint32_t responseSize = data.ReadInt32();
    DRM_CHECK_AND_RETURN_RET_LOG(responseSize < RESPONSE_MAX_LEN, DRM_MEMORY_ERROR,
        "The size of response is too large.");
    if (responseSize != 0) {
        const uint8_t *responseBuf = static_cast<const uint8_t *>(data.ReadBuffer(responseSize));
        if (responseBuf == nullptr) {
            DRM_ERR_LOG("MediaKeySessionServiceStub::ProcessOfflineReleaseResponse read response failed");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
        response.assign(responseBuf, responseBuf + responseSize);
    }
    int32_t ret = stub->ProcessOfflineReleaseResponse(licenseId, response);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_OK, ret, "ProcessOfflineReleaseResponse faild, errCode:%{public}d", ret);
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_PROCESS_OFFLINE_RELEASE_RESPONSE exit.");
    return ret;
}

static int32_t ProcessCheckLicenseStatus(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_GENERATE_CHECK_LICENSE_STATUS enter.");
    std::map<std::string, MediaKeySessionKeyStatus> licenseStatus;
    int32_t ret = stub->CheckLicenseStatus(licenseStatus);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_OK, ret, "CheckLicenseStatus faild, errCode:%{public}d", ret);

    reply.WriteInt32(licenseStatus.size());
    for (auto licenseStatu : licenseStatus) {
        reply.WriteString(licenseStatu.first);
        reply.WriteInt32(licenseStatu.second);
    }
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_GENERATE_CHECK_LICENSE_STATUS exit.");
    return ret;
}

static int32_t ProcessRestoreOfflineLicense(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_RESTORE_OFFLINEKEYS enter.");
    std::vector<uint8_t> licenseId;
    uint32_t licenseIdSize = data.ReadInt32();
    DRM_CHECK_AND_RETURN_RET_LOG(licenseIdSize < LICENSEID_MAX_LEN, DRM_MEMORY_ERROR,
        "The size of licenseId is too large.");
    if (licenseIdSize != 0) {
        const uint8_t *licenseIdBuf = static_cast<const uint8_t *>(data.ReadBuffer(licenseIdSize));
        if (licenseIdBuf == nullptr) {
            DRM_ERR_LOG("MediaKeySessionServiceStub::ProcessRestoreOfflineLicense read licenseId failed");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
        licenseId.assign(licenseIdBuf, licenseIdBuf + licenseIdSize);
    }
    int32_t ret = stub->RestoreOfflineLicense(licenseId);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_OK, ret, "RestoreOfflineLicense faild, errCode:%{public}d", ret);
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_RESTORE_OFFLINEKEYS exit.");
    return ret;
}

static int32_t ProcessRemoveLicense(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_REMOVE_LICENSE enter.");
    int32_t ret = stub->RemoveLicense();
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_OK, ret, "RemoveLicense faild, errCode:%{public}d", ret);
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_REMOVE_LICENSE exit.");
    return ret;
}

static int32_t ProcessSetCallback(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_SET_CALLBACK enter.");
    auto remoteObject = data.ReadRemoteObject();
    if (remoteObject == nullptr) {
        DRM_ERR_LOG("MediaKeySessionServiceStub SetCallback CameraServiceCallback is null");
        return IPC_STUB_INVALID_DATA_ERR;
    }
    auto callback = iface_cast<IMediaKeySessionServiceCallback>(remoteObject);
    if (callback == nullptr) {
        DRM_ERR_LOG("MediaKeySessionServiceStub SetCallback cast nullptr");
        return IPC_STUB_INVALID_DATA_ERR;
    }
    int32_t errCode = stub->SetCallback(callback);
    DRM_CHECK_AND_RETURN_RET_LOG(errCode == DRM_OK, errCode, "SetCallback faild, errCode:%{public}d", errCode);
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_SET_CALLBACK exit.");
    return errCode;
}

static int32_t ProcessRequireSecureDecoder(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_REQUIRE_SECURE_DECODER enter.");
    std::string mimeType = data.ReadString();
    bool status;
    int32_t ret = stub->RequireSecureDecoderModule(mimeType, &status);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_OK, ret, "ProcessRequireSecureDecoder faild, errCode:%{public}d", ret);
    reply.WriteBool(status);
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_REQUIRE_SECURE_DECODER exit.");
    return ret;
}

static int32_t ProcessGetSecurityLevel(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_GETSECURITYLEVEL enter.");
    IMediaKeySessionService::SecurityLevel securityLevel = IMediaKeySessionService::SECURITY_LEVEL_UNKNOWN;
    int32_t ret = stub->GetSecurityLevel(&securityLevel);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_OK, ret, "ProcessGetSecurityLevel faild, errCode:%{public}d", ret);
    if (!reply.WriteInt32(securityLevel)) {
        DRM_ERR_LOG("MediaKeySessionServiceStub Write GetSecurityLevel failed");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_GETSECURITYLEVEL exit.");
    return ret;
}

int32_t MediaKeySessionServiceStub::OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub::OnRemoteRequest enter.");
    int32_t errCode = -1;
    DRM_DEBUG_LOG("OnRemoteRequest, cmd = %{public}u", code);
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " is keySessionServiceStub", FAKE_POINTER(this));

    DRM_CHECK_AND_RETURN_RET_LOG(data.ReadInterfaceToken() == GetDescriptor(), errCode,
        "MediaKeySessionServiceStub: ReadInterfaceToken failed");

    DRM_CHECK_AND_RETURN_RET_LOG((code >= CREATE_MEDIA_DECRYPT_MODULE) && (code <= MEDIA_KEY_SESSION_GETSECURITYLEVEL),
        IPCObjectStub::OnRemoteRequest(code, data, reply, option),
        "code not match, need check MediaKeySessionServiceStub");
    return g_mediaKeySessionServiceStubRequestProcessFunc[code].processFunc(this, data, reply, option);
}
} // DrmStandard
} // OHOS