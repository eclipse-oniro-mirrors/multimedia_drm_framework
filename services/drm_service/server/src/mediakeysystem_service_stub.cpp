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

#include "mediakeysystem_service_stub.h"
#include "drm_error_code.h"
#include "drm_log.h"
#include "ipc_skeleton.h"
#include "xcollie/xcollie.h"
#include "xcollie/xcollie_define.h"

namespace OHOS {
namespace DrmStandard {
using ProcessRemoteRequestFunc = int32_t (*)(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

struct ProcessRemoteRequestFuncArray {
    OHOS::DrmStandard::MediaKeySystemServiceRequestCode requestCode;
    ProcessRemoteRequestFunc processFunc;
};

static int32_t ProcessCreatekeySession(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessKeySystemRequest(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessKeySystemResponse(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessSetConfigurationString(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessGetConfigurationString(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessSetConfigurationByteArray(MediaKeySystemServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option);

static int32_t ProcessGetConfigurationByteArray(MediaKeySystemServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option);

static int32_t ProcessGetMetircs(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessReleaseKeySystem(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessGetMaxContentProtectionLevel(MediaKeySystemServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option);

static int32_t ProcessGetCertificateStatus(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessGetOfflineMediaKeyIds(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessGetOfflineMediaKeyStatus(MediaKeySystemServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option);

static int32_t ProcessRemoveOfflineMediaKey(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static int32_t ProcessSetCallabck(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option);

static struct ProcessRemoteRequestFuncArray g_mediaKeySystemServiceStubRequestProcessFunc[] = {
    {MEDIA_KEY_SYSTEM_CREATE_KEY_SESSION, ProcessCreatekeySession},
    {MEDIA_KEY_SYSTEM_GENERATE_KEYSYSTEM_REQUEST, ProcessKeySystemRequest},
    {MEDIA_KEY_SYSTEM_PROCESS_KEYSYSTEM_RESPONSE, ProcessKeySystemResponse},
    {MEDIA_KEY_SYSTEM_SETCONFIGURATION_STRING, ProcessSetConfigurationString},
    {MEDIA_KEY_SYSTEM_GETCONFIGURATION_STRING, ProcessGetConfigurationString},
    {MEDIA_KEY_SYSTEM_SETCONFIGURATION_BYTEARRAY, ProcessSetConfigurationByteArray},
    {MEDIA_KEY_SYSTEM_GETCONFIGURATION_BYTEARRAY, ProcessGetConfigurationByteArray},
    {MEDIA_KEY_SYSTEM_GETMETRIC, ProcessGetMetircs},
    {MEDIA_KEY_SYSTEM_RELEASE, ProcessReleaseKeySystem},
    {MEDIA_KEY_SYSTEM_GETMAXSECURITYLEVEL, ProcessGetMaxContentProtectionLevel},
    {MEDIA_KEY_SYSTEM_GETCERTIFICATESTATUS, ProcessGetCertificateStatus},
    {MEDIA_KEY_SYSTEM_GET_OFFLINELICENSEIDS, ProcessGetOfflineMediaKeyIds},
    {MEDIA_KEY_SYSTEM_GET_OFFLINEKEY_STATUS, ProcessGetOfflineMediaKeyStatus},
    {MEDIA_KEY_SYSTEM_REMOVE_OFFLINELICENSE, ProcessRemoveOfflineMediaKey},
    {MEDIA_KEY_SYSTEM_SETCALLBACK, ProcessSetCallabck},
};

MediaKeySystemServiceStub::MediaKeySystemServiceStub()
{
    DRM_INFO_LOG("MediaKeySystemServiceStub enter.");
}

MediaKeySystemServiceStub::~MediaKeySystemServiceStub()
{
    DRM_INFO_LOG("~MediaKeySystemServiceStub enter.");
    deathRecipient_ = nullptr;
    clientListener_ = nullptr;
}

static int32_t ProcessCreatekeySession(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("ProcessCreatekeySession enter.");
    sptr<IMediaKeySessionService> keySessionServiceProxy = nullptr;
    const int32_t securityLevel = data.ReadInt32();
    int32_t ret = stub->CreateMediaKeySession((IMediaKeySessionService::ContentProtectionLevel)securityLevel,
        keySessionServiceProxy);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "CreateMediaKeySession faild, errCode:%{public}d",
        ret);

    if (!reply.WriteRemoteObject(keySessionServiceProxy->AsObject())) {
        DRM_ERR_LOG("CreateMediaKeySession Write MediaKeySession obj failed.");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    return ret;
}

static int32_t ProcessKeySystemRequest(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("ProcessKeySystemRequest enter.");
    std::vector<uint8_t> request;
    std::string defaultUrl;
    int32_t ret = stub->GenerateKeySystemRequest(request, defaultUrl);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "ProcessKeySystemRequest faild, errCode:%{public}d", ret);
    if (!reply.WriteString(defaultUrl)) {
        DRM_ERR_LOG("Write GenerateKeySystemRequest failed.");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    if (!reply.WriteInt32(request.size())) {
        DRM_ERR_LOG("Write request size failed.");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    DRM_CHECK_AND_RETURN_RET_LOG(request.size() < REQUEST_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of request is too large.");
    if (request.size() != 0) {
        if (!reply.WriteBuffer(request.data(), request.size())) {
            DRM_ERR_LOG("RestoreOfflineMediaKeys write request failed.");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
    }
    return ret;
}

static int32_t ProcessKeySystemResponse(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("ProcessKeySystemResponse enter.");
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
    int32_t ret = stub->ProcessKeySystemResponse(response);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "ProcessKeySystemResponse faild, errCode:%{public}d", ret);
    return ret;
}

static int32_t ProcessSetConfigurationString(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("ProcessSetConfigurationString enter.");
    std::string configName = data.ReadString();
    std::string value = data.ReadString();
    int32_t ret = stub->SetConfigurationString(configName, value);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "ProcessSetConfigurationString faild, errCode:%{public}d", ret);
    return ret;
}

static int32_t ProcessGetConfigurationString(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("ProcessGetConfigurationString enter.");
    std::string configName = data.ReadString();
    std::string value;
    int32_t ret = stub->GetConfigurationString(configName, value);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "ProcessGetConfigurationString faild, errCode:%{public}d", ret);
    if (!reply.WriteString(value)) {
        DRM_ERR_LOG("ProcessGetConfigurationString write value failed.");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    return ret;
}

static int32_t ProcessSetConfigurationByteArray(MediaKeySystemServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option)
{
    DRM_INFO_LOG("ProcessSetConfigurationByteArray enter.");
    std::string configName = data.ReadString();
    std::vector<uint8_t> value;
    int32_t valueSize = data.ReadInt32();
    DRM_CHECK_AND_RETURN_RET_LOG(valueSize < RESPONSE_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of configurate value is too large.");
    if (valueSize != 0) {
        const uint8_t *valueBuf = static_cast<const uint8_t *>(data.ReadUnpadBuffer(valueSize));
        if (valueBuf == nullptr) {
            DRM_ERR_LOG("ProcessSetConfigurationByteArray read value failed.");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
        value.assign(valueBuf, valueBuf + valueSize);
    }
    int32_t ret = stub->SetConfigurationByteArray(configName, value);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "ProcessSetConfigurationByteArray faild, errCode:%{public}d", ret);
    return ret;
}

static int32_t ProcessGetConfigurationByteArray(MediaKeySystemServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option)
{
    DRM_INFO_LOG("ProcessGetConfigurationByteArray enter.");
    std::string configName = data.ReadString();
    std::vector<uint8_t> value;
    int32_t ret = stub->GetConfigurationByteArray(configName, value);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "ProcessGetConfigurationByteArray faild, errCode:%{public}d", ret);
    if (!reply.WriteInt32(value.size())) {
        DRM_ERR_LOG("Write value size failed.");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    DRM_CHECK_AND_RETURN_RET_LOG(value.size() < RESPONSE_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of configurate value is too large.");
    if (value.size() != 0) {
        if (!reply.WriteBuffer(value.data(), value.size())) {
            DRM_ERR_LOG("ProcessGetConfigurationByteArray write value failed.");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
    }
    return ret;
}

static int32_t ProcessGetMetircs(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("ProcessGetMetircs enter.");
    std::vector<IMediaKeySystemService::MetircKeyValue> metrics;
    int32_t ret = stub->GetStatistics(metrics);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "ProcessGetMetircs faild, errCode:%{public}d", ret);
    reply.WriteInt32(metrics.size());
    for (auto info : metrics) {
        reply.WriteString(info.name);
        reply.WriteString(info.value);
    }
    return ret;
}

static int32_t ProcessReleaseKeySystem(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("ProcessReleaseKeySystem enter.");
    int32_t ret = stub->Release();
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "ProcessReleaseKeySystem faild, errCode:%{public}d", ret);
    return ret;
}

static int32_t ProcessGetMaxContentProtectionLevel(MediaKeySystemServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option)
{
    DRM_INFO_LOG("ProcessGetMaxContentProtectionLevel enter.");
    IMediaKeySessionService::ContentProtectionLevel securityLevel =
        IMediaKeySessionService::CONTENT_PROTECTION_LEVEL_UNKNOWN;
    int32_t ret = stub->GetMaxContentProtectionLevel(&securityLevel);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "ProcessGetMaxContentProtectionLevel faild, errCode:%{public}d",
        ret);
    if (!reply.WriteInt32(securityLevel)) {
        DRM_ERR_LOG("Write ProcessGetMaxContentProtectionLevel failed.");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    return ret;
}

static int32_t ProcessGetCertificateStatus(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("ProcessGetCertificateStatus enter.");
    IMediaKeySystemService::CertificateStatus certStatus = IMediaKeySystemService::CERT_STATUS_PROVISIONED;
    int32_t ret = stub->GetCertificateStatus(&certStatus);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "ProcessGetCertificateStatus faild, errCode:%{public}d", ret);
    if (!reply.WriteInt32(certStatus)) {
        DRM_ERR_LOG("ProcessGetCertificateStatus write certStatus failed.");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    return ret;
}

static int32_t ProcessGetOfflineMediaKeyIds(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("ProcessGetOfflineMediaKeyIds enter.");
    std::vector<std::vector<uint8_t>> licenseIds;
    int32_t ret = stub->GetOfflineMediaKeyIds(licenseIds);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "ProcessGetOfflineMediaKeyIds faild, errCode:%{public}d", ret);

    reply.WriteUint32(licenseIds.size());
    if (licenseIds.size() == 0 || licenseIds.size() > LICENSEID_MAX_LEN) {
        DRM_DEBUG_LOG("licenseIds size is error.");
        return ret;
    }
    for (uint32_t i = 0; i < licenseIds.size(); i++) {
        uint32_t licenseIdSize = licenseIds[i].size();
        reply.WriteUint32(licenseIdSize);
        if (licenseIdSize == 0 || licenseIdSize > LICENSEID_MAX_LEN) {
            continue;
        }
        if (!reply.WriteBuffer(licenseIds[i].data(), licenseIdSize)) {
            DRM_ERR_LOG("ProcessGetOfflineMediaKeyIds write licenseId failed.");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
    }
    return ret;
}

static int32_t ProcessGetOfflineMediaKeyStatus(MediaKeySystemServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option)
{
    DRM_INFO_LOG("ProcessGetOfflineMediaKeyStatus enter.");
    std::vector<uint8_t> licenseId;
    int32_t licenseIdSize = data.ReadInt32();
    DRM_CHECK_AND_RETURN_RET_LOG(licenseIdSize < LICENSEID_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of licenseId is too large.");
    if (licenseIdSize != 0) {
        const uint8_t *licenseIdBuf = static_cast<const uint8_t *>(data.ReadUnpadBuffer(licenseIdSize));
        if (licenseIdBuf == nullptr) {
            DRM_ERR_LOG(":ProcessGetOfflineMediaKeyStatus read licenseId failed.");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
        licenseId.assign(licenseIdBuf, licenseIdBuf + licenseIdSize);
    }
    IMediaKeySessionService::OfflineMediaKeyStatus status;
    int32_t ret = stub->GetOfflineMediaKeyStatus(licenseId, status);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "ProcessGetOfflineMediaKeyStatus faild, errCode:%{public}d", ret);
    if (!reply.WriteInt32((int32_t)status)) {
        DRM_ERR_LOG("Write state failed.");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    return ret;
}

static int32_t ProcessRemoveOfflineMediaKey(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("ProcessRemoveOfflineMediaKey enter.");
    std::vector<uint8_t> licenseId;
    int32_t licenseIdSize = data.ReadInt32();
    DRM_CHECK_AND_RETURN_RET_LOG(licenseIdSize < LICENSEID_MAX_LEN, DRM_INNER_ERR_MEMORY_ERROR,
        "The size of licenseId is too large.");
    if (licenseIdSize != 0) {
        const uint8_t *licenseIdBuf = static_cast<const uint8_t *>(data.ReadUnpadBuffer(licenseIdSize));
        if (licenseIdBuf == nullptr) {
            DRM_ERR_LOG("ProcessRemoveOfflineMediaKey read licenseId failed.");
            return IPC_STUB_WRITE_PARCEL_ERR;
        }
        licenseId.assign(licenseIdBuf, licenseIdBuf + licenseIdSize);
    }
    int32_t ret = stub->ClearOfflineMediaKeys(licenseId);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "ProcessRemoveOfflineMediaKey faild, errCode:%{public}d", ret);
    return ret;
}

void MediaKeySystemServiceStub::MediaKeySystemClientDied(pid_t pid)
{
    DRM_ERR_LOG("MediaKeySystemService client has died, pid:%{public}d", pid);
}

static int32_t ProcessSetCallabck(MediaKeySystemServiceStub *stub, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("ProcessSetCallabck enter.");
    auto remoteObject = data.ReadRemoteObject();
    if (remoteObject == nullptr) {
        DRM_ERR_LOG("ProcessSetCallabck remote is null");
        return IPC_STUB_INVALID_DATA_ERR;
    }
    auto callback = iface_cast<IMediaKeySystemServiceCallback>(remoteObject);
    if (callback == nullptr) {
        DRM_ERR_LOG("ProcessSetCallabck iface_cast nullptr");
        return IPC_STUB_INVALID_DATA_ERR;
    }
    int32_t ret = stub->SetCallback(callback);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, ret, "ProcessSetCallabck faild, errCode:%{public}d", ret);
    return ret;
}

int32_t MediaKeySystemServiceStub::OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_CHECK_AND_RETURN_RET_LOG(data.ReadInterfaceToken() == GetDescriptor(), -1,
        "ReadInterfaceToken failed.");
    DRM_INFO_LOG("OnRemoteRequest, cmd = %{public}u", code);

    DRM_CHECK_AND_RETURN_RET_LOG((code >= MEDIA_KEY_SYSTEM_CREATE_KEY_SESSION) &&
        (code <= MEDIA_KEY_SYSTEM_SETCALLBACK),
        IPCObjectStub::OnRemoteRequest(code, data, reply, option),
        "code not match, need check MediaKeySystemServiceStub");

    return g_mediaKeySystemServiceStubRequestProcessFunc[code].processFunc(this, data, reply, option);
}
} // namespace DrmStandard
} // namespace OHOS