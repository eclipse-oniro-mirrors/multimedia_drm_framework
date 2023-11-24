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
#include "drm_napi_utils.h"
#include "ipc_skeleton.h"
#include "xcollie/xcollie.h"
#include "xcollie/xcollie_define.h"

namespace OHOS {
namespace DrmStandard {
using ProcessRemoteRequestFunc = int (*)(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);
struct ProcessRemoteRequestFuncArray {
    OHOS::DrmStandard::MediaKeySessionServiceRequestCode requestCode;
    ProcessRemoteRequestFunc processFunc;
};

static int ProcessCreateMediaDecryptModule(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int ProcessRleaseKeySession(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int ProcessLicenseRequest(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int ProcessLicenseResponse(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int ProcessOfflineReleaseRequest(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int ProcessOfflineReleaseResponse(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int ProcessCheckLicenseStatus(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int ProcessRestoreOfflineLicense(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int ProcessRemoveLicense(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int ProcessSetCallback(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int ProcessRequireSecureDecoder(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int ProcessGetSecurityLevel(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
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

static int ProcessCreateMediaDecryptModule(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub GET_MEDIA_DECRYPT_MODULE enter.");
    sptr<IMediaDecryptModuleService> decryptModuleServiceProxy = nullptr;
    int errCode = stub->CreateMediaDecryptModule(decryptModuleServiceProxy);
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

static int ProcessRleaseKeySession(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub KEY_SESSION_RELEASE enter.");
    int errCode = stub->Release();
    DRM_INFO_LOG("MediaKeySessionServiceStub KEY_SESSION_RELEASE exit.");
    return errCode;
}

static int ProcessLicenseRequest(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_GENERATE_LICENSE_REQUEST enter.");
    IMediaKeySessionService::LicenseRequestInfo licenseRequestInfo;
    IMediaKeySessionService::LicenseRequest licenseRequest;
    licenseRequestInfo.licenseType = (OHOS::DrmStandard::IMediaKeySessionService::LicenseType)data.ReadInt32();
    licenseRequestInfo.mimeType = data.ReadString();
    uint32_t initDataSize = data.ReadInt32();
    const uint8_t *initDataBuf = static_cast<const uint8_t *>(data.ReadBuffer(initDataSize));
    if (initDataBuf == nullptr) {
        DRM_ERR_LOG("MediaKeySessionServiceStub ReadBuffer failed");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    licenseRequestInfo.initData.assign(initDataBuf, initDataBuf + initDataSize);
    int32_t ret = stub->GenerateLicenseRequest(licenseRequestInfo, licenseRequest);
    if (!reply.WriteInt32(licenseRequest.requestType)) {
        DRM_ERR_LOG("MediaKeySessionServiceStub Write requestType GenerateLicenseRequest failed");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    if (!reply.WriteInt32(licenseRequest.mData.size())) {
        DRM_ERR_LOG("MediaKeySessionServiceStub Write licenseRequestmDatasize GenerateLicenseRequest failed");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }

    for (auto data : licenseRequest.mData) {
        if (!reply.WriteUint8(data)) {
            DRM_ERR_LOG("MediaKeySessionServiceStub Write data GenerateLicenseRequest failed");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
    }
    if (!reply.WriteString(licenseRequest.mDefaultURL)) {
        DRM_ERR_LOG("MediaKeySessionServiceStub Write licenseRequest.mDefaultURL GenerateLicenseRequest failed");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_GENERATE_LICENSE_REQUEST exit.");
    return ret;
}

static int ProcessLicenseResponse(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SYSTEM_PROCESS_LICENSE_RESPONSE enter.");
    std::vector<uint8_t> response;
    std::vector<uint8_t> licenseId;
    uint32_t responseSize = data.ReadInt32();
    for (int i = 0; i < responseSize; i++) {
        response.push_back(data.ReadUint8());
    }
    int32_t ret = stub->ProcessLicenseResponse(licenseId, response);
    if (!reply.WriteInt32(licenseId.size())) {
        DRM_ERR_LOG("MediaKeySessionServiceStub Write licenseId.size GenerateLicenseRequest failed");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    for (auto id : licenseId) {
        if (!reply.WriteUint8(id)) {
            DRM_ERR_LOG("MediaKeySessionServiceStub Write id GenerateLicenseRequest failed");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
    }

    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SYSTEM_PROCESS_LICENSE_RESPONSE exit.");
    return ret;
}

static int ProcessOfflineReleaseRequest(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_GENERATE_OFFLINE_RELEASE_REQUEST enter.");
    std::vector<uint8_t> licenseId;
    uint32_t licenseIdSize = data.ReadInt32();
    for (int i = 0; i < licenseIdSize; i++) {
        licenseId.push_back(data.ReadUint8());
    }
    std::vector<uint8_t> releaseRequest;
    int32_t ret = stub->GenerateOfflineReleaseRequest(licenseId, releaseRequest);
    if (!reply.WriteInt32(releaseRequest.size())) {
        DRM_ERR_LOG("MediaKeySessionServiceStub Write releaseRequest.size GenerateLicenseRequest failed");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }

    for (auto request : releaseRequest) {
        if (!reply.WriteUint8(request)) {
            DRM_ERR_LOG("MediaKeySessionServiceStub Write request GenerateLicenseRequest failed");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
    }
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_GENERATE_OFFLINE_RELEASE_REQUEST exit.");
    return ret;
}

static int ProcessOfflineReleaseResponse(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_PROCESS_OFFLINE_RELEASE_RESPONSE enter.");
    std::vector<uint8_t> licenseId;
    uint32_t licenseIdSize = data.ReadInt32();
    for (int i = 0; i < licenseIdSize; i++) {
        licenseId.push_back(data.ReadUint8());
    }
    std::vector<uint8_t> response;
    uint32_t responseSize = data.ReadInt32();
    for (int i = 0; i < responseSize; i++) {
        response.push_back(data.ReadUint8());
    }
    int32_t ret = stub->ProcessOfflineReleaseResponse(licenseId, response);
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_PROCESS_OFFLINE_RELEASE_RESPONSE exit.");
    return ret;
}

static int ProcessCheckLicenseStatus(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_GENERATE_CHECK_LICENSE_STATUS enter.");
    std::map<std::string, IMediaKeySessionService::MediaKeySessionKeyStatus> licenseStatus;
    int32_t ret = stub->CheckLicenseStatus(licenseStatus);
    if (ret != 0) {
        DRM_ERR_LOG("CheckLicenseStatus faild.");
        return ret;
    }
    reply.WriteInt32(licenseStatus.size());
    for (auto licenseStatu : licenseStatus) {
        reply.WriteString(licenseStatu.first);
        reply.WriteInt32(licenseStatu.second);
    }
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_GENERATE_CHECK_LICENSE_STATUS exit.");
    return ret;
}

static int ProcessRestoreOfflineLicense(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_RESTORE_OFFLINEKEYS enter.");
    std::vector<uint8_t> licenseId;
    uint32_t licenseIdSize = data.ReadInt32();
    for (int i = 0; i < licenseIdSize; i++) {
        licenseId.push_back(data.ReadUint8());
    }
    int32_t ret = stub->RestoreOfflineLicense(licenseId);
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_RESTORE_OFFLINEKEYS exit.");
    return ret;
}

static int ProcessRemoveLicense(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_REMOVE_LICENSE enter.");
    int32_t ret = stub->RemoveLicense();
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_REMOVE_LICENSE exit.");
    return ret;
}

static int ProcessSetCallback(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
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
    int errCode = stub->SetMediaKeySessionServiceCallback(callback);
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_SET_CALLBACK exit.");
    return errCode;
}

static int ProcessRequireSecureDecoder(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_REQUIRE_SECURE_DECODER enter.");
    std::string mimeType = data.ReadString();
    bool status;
    int32_t ret = stub->RequireSecureDecoderModule(mimeType, &status);
    if (ret != 0) {
        DRM_ERR_LOG("RequireSecureDecoderModule faild.");
        return ret;
    }
    reply.WriteBool(status);
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_REQUIRE_SECURE_DECODER exit.");
    return ret;
}

static int ProcessGetSecurityLevel(MediaKeySessionServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_GETSECURITYLEVEL enter.");
    IMediaKeySessionService::SecurityLevel securityLevel = IMediaKeySessionService::SECURITY_LEVEL_UNKNOWN;
    int32_t ret = stub->GetSecurityLevel(&securityLevel);
    if (!reply.WriteInt32(securityLevel)) {
        DRM_ERR_LOG("MediaKeySessionServiceStub Write GetSecurityLevel failed");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    DRM_INFO_LOG("MediaKeySessionServiceStub MEDIA_KEY_SESSION_GETSECURITYLEVEL exit.");
    return ret;
}

int MediaKeySessionServiceStub::OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("MediaKeySessionServiceStub::OnRemoteRequest enter.");
    int errCode = -1;
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