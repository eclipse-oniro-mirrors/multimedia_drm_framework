/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

#include "key_session_service_stub.h"
#include "drm_error_code.h"
#include "drm_log.h"
#include "drm_napi_utils.h"
#include "ipc_skeleton.h"
#include "xcollie/xcollie.h"
#include "xcollie/xcollie_define.h"

namespace OHOS {
namespace DrmStandard {
int KeySessionServiceStub::OnRemoteRequest(uint32_t code, MessageParcel &data,
    MessageParcel &reply, MessageOption &option)
{
    DRM_INFO_LOG("KeySessionServiceStub::OnRemoteRequest enter.");
    int errCode = -1;
    DRM_DEBUG_LOG("OnRemoteRequest, cmd = %{public}u", code);
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " is keySessionServiceStub", FAKE_POINTER(this));

    if (data.ReadInterfaceToken() != GetDescriptor()) {
        DRM_ERR_LOG("KeySessionServiceStub: ReadInterfaceToken failed");
        return errCode;   
    }

    switch(code) {
        case CREATE_MEDIA_DECRYPT_MODULE: {
            DRM_INFO_LOG("KeySessionServiceStub GET_MEDIA_DECRYPT_MODULE enter.");
            sptr<IMediaDecryptModuleService> decryptModuleServiceProxy = nullptr;
            errCode = CreateMediaDecryptModule(decryptModuleServiceProxy);
            if (errCode != ERR_NONE) {
                DRM_ERR_LOG("KeySessionServiceStub CreateMediaDecryptModule failed : %{public}d", errCode);
                return errCode;
            }
            if (!reply.WriteRemoteObject(decryptModuleServiceProxy->AsObject())) {
                DRM_ERR_LOG("KeySessionServiceStub Write MediaDecryptModule obj failed");
                return IPC_STUB_WRITE_PARCEL_ERR;
            }
            DRM_INFO_LOG("KeySessionServiceStub GET_MEDIA_DECRYPT_MODULE exit.");
            return DRM_OK;
        }
        case MEDIA_KEY_SESSION_GENERATE_LICENSE_REQUEST: {
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SESSION_GENERATE_LICENSE_REQUEST enter.");
            IKeySessionService::DrmInfo drmInfo;
            IKeySessionService::LicenseInfo licenseInfo;
            drmInfo.keyType = (OHOS::DrmStandard::IKeySessionService::KeyType)data.ReadInt32();
            drmInfo.mimeType = data.ReadString();
            uint32_t indexInfoSize = data.ReadInt32();
            const uint8_t* indexInfoBuf = static_cast<const uint8_t*>(data.ReadBuffer(indexInfoSize));
            if (indexInfoBuf == nullptr) {
                DRM_ERR_LOG("KeySessionServiceStub ReadBuffer failed");
                return IPC_STUB_WRITE_PARCEL_ERR;
            }
            drmInfo.indexInfo.assign(indexInfoBuf, indexInfoBuf + indexInfoSize);
            int32_t ret = GenerateLicenseRequest(drmInfo, licenseInfo);
            if (!reply.WriteInt32(licenseInfo.requestType)) {
                DRM_ERR_LOG("KeySessionServiceStub Write GenerateLicenseRequest failed");
                return IPC_STUB_WRITE_PARCEL_ERR;
            }
            if (!reply.WriteInt32(licenseInfo.mData.size())) {
                DRM_ERR_LOG("KeySessionServiceStub Write GenerateLicenseRequest failed");
                return IPC_STUB_WRITE_PARCEL_ERR;
            }
            for (auto data : licenseInfo.mData) {
                if (!reply.WriteUint8(data)) {
                    DRM_ERR_LOG("KeySessionServiceStub Write GenerateLicenseRequest failed");
                    return IPC_STUB_WRITE_PARCEL_ERR;
                }
            }
            if (!reply.WriteString(licenseInfo.mDefaultURL)) {
                DRM_ERR_LOG("KeySessionServiceStub Write GenerateLicenseRequest failed");
                return IPC_STUB_WRITE_PARCEL_ERR;
            }
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SESSION_GENERATE_LICENSE_REQUEST exit.");
            return ret;
        }
        case MEDIA_KEY_SESSION_PROCESS_LICENSE_RESPONSE: {
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SYSTEM_PROCESS_LICENSE_RESPONSE enter.");
            std::vector<uint8_t> response;
            std::vector<uint8_t> keyId;
            uint32_t responseSize = data.ReadInt32();
            for (int i = 0; i < responseSize; i++) {
                response.push_back(data.ReadUint8());
            }
            int32_t ret = ProcessLicenseResponse(keyId, response);
            if (!reply.WriteInt32(keyId.size())) {
                DRM_ERR_LOG("KeySessionServiceStub Write GenerateLicenseRequest failed");
                return IPC_STUB_WRITE_PARCEL_ERR;
            }
            for (auto id : keyId) {
                if (!reply.WriteUint8(id)) {
                    DRM_ERR_LOG("KeySessionServiceStub Write GenerateLicenseRequest failed");
                    return IPC_STUB_WRITE_PARCEL_ERR;
                }
            }
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SYSTEM_PROCESS_LICENSE_RESPONSE exit.");
            return ret;
        }
        case MEDIA_KEY_SESSION_GENERATE_OFFLINE_RELEASE_REQUEST: {
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SESSION_GENERATE_OFFLINE_RELEASE_REQUEST enter.");
            std::vector<uint8_t> keyId;
            uint32_t keyIdSize = data.ReadInt32();
            for (int i = 0; i < keyIdSize; i++) {
                keyId.push_back(data.ReadUint8());
            }
            std::vector<uint8_t> releaseRequest;
            int32_t ret = GenerateOfflineReleaseRequest(keyId, releaseRequest);
            if (!reply.WriteInt32(releaseRequest.size())) {
                DRM_ERR_LOG("KeySessionServiceStub Write GenerateLicenseRequest failed");
                return IPC_STUB_WRITE_PARCEL_ERR;
            }

            for (auto request : releaseRequest) {
                if (!reply.WriteUint8(request)) {
                    DRM_ERR_LOG("KeySessionServiceStub Write GenerateLicenseRequest failed");
                    return IPC_STUB_WRITE_PARCEL_ERR;
                }
            }
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SESSION_GENERATE_OFFLINE_RELEASE_REQUEST exit.");
            return ret;
        }
        case MEDIA_KEY_SESSION_PROCESS_OFFLINE_RELEASE_RESPONSE: {
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SESSION_PROCESS_OFFLINE_RELEASE_RESPONSE enter.");
            std::vector<uint8_t> keyId;
            uint32_t keyIdSize = data.ReadInt32();
            for (int i = 0; i < keyIdSize; i++) {
                keyId.push_back(data.ReadUint8());
            }
            std::vector<uint8_t> response;
            uint32_t responseSize = data.ReadInt32();
            for (int i = 0; i < responseSize; i++) {
                response.push_back(data.ReadUint8());
            }
            int32_t ret = ProcessOfflineReleaseResponse(keyId, response);
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SESSION_PROCESS_OFFLINE_RELEASE_RESPONSE exit.");
            return ret;
        }
        case MEDIA_KEY_SESSION_GENERATE_CHECK_LICENSE_STATUS: {
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SESSION_GENERATE_CHECK_LICENSE_STATUS enter.");
            std::vector<IKeySessionService::KeyValue> infoMap;
            int32_t ret = CheckLicenseStatus(infoMap);
            if (ret != 0) {
                 DRM_ERR_LOG("CheckLicenseStatus faild.");
                 return ret;
            }
            reply.WriteInt32(infoMap.size());
            for (auto info : infoMap) {
                reply.WriteString(info.name);
                reply.WriteString(info.value);
            }
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SESSION_GENERATE_CHECK_LICENSE_STATUS exit.");
            return ret;
        }
        case MEDIA_KEY_SESSION_RESTORE_OFFLINEKEYS: {
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SESSION_RESTORE_OFFLINEKEYS enter.");
            std::vector<uint8_t> keyId;
            uint32_t keyIdSize = data.ReadInt32();
            for (int i = 0; i < keyIdSize; i++) {
                keyId.push_back(data.ReadUint8());
            }
            int32_t ret = RestoreOfflineKeys(keyId);
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SESSION_RESTORE_OFFLINEKEYS exit.");
            return ret;
        }
        case MEDIA_KEY_SESSION_REMOVE_OFFLINEKEYS: {
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SESSION_REMOVE_OFFLINEKEYS enter.");
            std::vector<uint8_t> keyId;
            uint32_t keyIdSize = data.ReadInt32();
            for (int i = 0; i < keyIdSize; i++) {
                keyId.push_back(data.ReadUint8());
            }
            int32_t ret = RemoveOfflineKeys(keyId);
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SESSION_REMOVE_OFFLINEKEYS exit.");
            return ret;
        }
        case MEDIA_KEY_SESSION_GET_OFFLINEKEYIDS: {
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SESSION_GET_OFFLINEKEYIDS enter.");
            std::vector<std::vector<uint8_t>> keyIds;
            int32_t ret = GetOfflineKeyIds(keyIds);
            reply.WriteInt32(keyIds.size());
            for (auto keyId : keyIds) {
                int32_t keyIdSize = keyId.size();
                reply.WriteInt32(keyIdSize);
                if (!reply.WriteBuffer(keyId.data(), keyId.size())) {
                    DRM_ERR_LOG("KeySessionServiceProxy GenerateLicenseRequest Write keyIds failed");
                    return IPC_STUB_WRITE_PARCEL_ERR;
               }
            }
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SESSION_GET_OFFLINEKEYIDS exit.");
            return ret;
        }
        case MEDIA_KEY_SESSION_REMOVE_LICENSE: {
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SESSION_REMOVE_LICENSE enter.");
            int32_t ret = RemoveLicenses();
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SESSION_REMOVE_LICENSE exit.");
            return ret;
        }
        case MEDIA_KEY_SESSION_GET_OFFLINEKEY_STATE: {
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SESSION_GET_OFFLINEKEY_STATE enter.");
            std::vector<uint8_t> keyId;
            uint32_t keyIdSize = data.ReadInt32();
            for (int i = 0; i < keyIdSize; i++) {
                keyId.push_back(data.ReadUint8());
            }
            IKeySessionService::OfflineKeyState state;
            int32_t ret = GetOfflineKeyState(keyId, state);
            if (ret != DRM_OK) {
               DRM_ERR_LOG("MEDIA_KEY_SESSION_GET_OFFLINEKEY_STATE failed");
               return ret;
            }
            if (!reply.WriteInt32((int32_t)state)) {
                DRM_ERR_LOG("KeySessionServiceStub Write state failed");
                return IPC_STUB_WRITE_PARCEL_ERR;
            }
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SESSION_GET_OFFLINEKEY_STATE exit.");
            return ret;
        }
        case RELEASE_MEDIA_DECRYPT_MODULE: {
            return errCode;
        }
        case KEY_SESSION_RELEASE: {
            DRM_INFO_LOG("KeySessionServiceStub KEY_SESSION_RELEASE enter.");
            errCode = Release();
            DRM_INFO_LOG("KeySessionServiceStub KEY_SESSION_RELEASE exit.");
            return errCode;
        }
        case MEDIA_KEY_SESSION_SET_CALLBACK: {
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SESSION_SET_CALLBACK enter.");
            auto remoteObject = data.ReadRemoteObject();
            if (remoteObject == nullptr) {
                DRM_ERR_LOG("KeySessionServiceStub SetCallback CameraServiceCallback is null");
                return IPC_STUB_INVALID_DATA_ERR;
            }
            auto callback = iface_cast<IKeySessionServiceCallback>(remoteObject);
            if (callback == nullptr) {
                DRM_ERR_LOG("KeySessionServiceStub SetCallback cast nullptr");
                return IPC_STUB_INVALID_DATA_ERR;
            }
            errCode = SetKeySessionServiceCallback(callback);
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SESSION_SET_CALLBACK exit.");
            return errCode;
        }
        default: {
            DRM_INFO_LOG("KeySessionServiceStub request code %{public}u not handled", code);
            return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
        }
    }
    return errCode;
}
} // DrmStandard
} // OHOS