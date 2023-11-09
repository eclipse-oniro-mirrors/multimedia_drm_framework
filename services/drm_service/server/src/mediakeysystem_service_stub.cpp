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

#include "mediakeysystem_service_stub.h"
#include "drm_error_code.h"
#include "drm_log.h"
#include "drm_napi_utils.h"
#include "ipc_skeleton.h"
#include "xcollie/xcollie.h"
#include "xcollie/xcollie_define.h"

namespace OHOS {
namespace DrmStandard {
MediaKeySystemServiceStub::MediaKeySystemServiceStub()
{
    deathRecipientMap_.clear();
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " Instances create", (POINTER_MASK & reinterpret_cast<uintptr_t>(this)));
}

MediaKeySystemServiceStub::~MediaKeySystemServiceStub()
{
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " Instances destroy", (POINTER_MASK & reinterpret_cast<uintptr_t>(this)));
}

int MediaKeySystemServiceStub::OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    DRM_INFO_LOG("OnRemoteRequest, cmd = %{public}u", code);
    if (data.ReadInterfaceToken() != GetDescriptor()) {
        DRM_ERR_LOG("MediaKeySystemServiceStub ReadInterfaceToken failed");
        return -1;
    }
    switch (code) {
        case MEDIA_KEY_SYSTEM_CREATE_KEY_SESSION: {
            DRM_INFO_LOG("MediaKeySystemServiceStub MEDIA_KEY_SYSTEM_CREATE_KEY_SESSION enter.");
            sptr<IKeySessionService> keySessionServiceProxy = nullptr;
            const int32_t securityLevel = data.ReadInt32();
            int errCode = CreateKeySession((IKeySessionService::SecurityLevel)securityLevel, keySessionServiceProxy);
            if (errCode != ERR_NONE) {
                DRM_ERR_LOG("MediaKeySystemServiceStub CreateKeySession failed : %{public}d", errCode);
                return errCode;
            }
            if (!reply.WriteRemoteObject(keySessionServiceProxy->AsObject())) {
                DRM_ERR_LOG("MediaKeySystemServiceStub CreateKeySession Write KeySession obj failed");
                return IPC_STUB_WRITE_PARCEL_ERR;
            }
            DRM_INFO_LOG("MediaKeySystemServiceStub MEDIA_KEY_SYSTEM_CREATE_KEY_SESSION exit.");
            return errCode;
        }
        case MEDIA_KEY_SYSTEM_GENERATE_KEYSYSTEM_REQUEST: {
            DRM_INFO_LOG("MediaKeySystemServiceStub MEDIA_KEY_SYSTEM_GENERATE_KEYSYSTEM_REQUEST enter.");
            std::vector<uint8_t> request;
            std::string defaultUrl;
            int type = data.ReadInt32();
            int32_t ret = GenerateKeySystemRequest(IMediaKeySystemService::RequestType(type), request, defaultUrl);
            if (!reply.WriteInt32(request.size())) {
                DRM_ERR_LOG("KeySessionServiceStub Write request size failed");
                return IPC_STUB_WRITE_PARCEL_ERR;
            }
            for (auto req : request) {
                if (!reply.WriteUint8(req)) {
                    DRM_ERR_LOG("MediaKeySystemServiceStub Write request failed");
                    return IPC_STUB_WRITE_PARCEL_ERR;
                }
            }
            if (!reply.WriteString(defaultUrl)) {
                DRM_ERR_LOG("MediaKeySystemServiceStub Write GenerateKeySystemRequest failed");
                return IPC_STUB_WRITE_PARCEL_ERR;
            }
            DRM_INFO_LOG("MediaKeySystemServiceStub MEDIA_KEY_SYSTEM_GENERATE_KEYSYSTEM_REQUEST exit.");
            return ret;
        }
        case MEDIA_KEY_SYSTEM_GENERATE_KEYSYSTEM_RESPONSE: {
            DRM_INFO_LOG("MediaKeySystemServiceStub MEDIA_KEY_SYSTEM_GENERATE_KEYSYSTEM_RESPONSE enter.");
            int type = data.ReadInt32();
            std::vector<uint8_t> response;
            uint32_t responseSize = data.ReadInt32();
            for (int i = 0; i < responseSize; i++) {
                response.push_back(data.ReadUint8());
            }
            int32_t ret = ProcessKeySystemResponse(IMediaKeySystemService::RequestType(type), response);
            DRM_INFO_LOG("MediaKeySystemServiceStub MEDIA_KEY_SYSTEM_GENERATE_KEYSYSTEM_RESPONSE exit.");
            return ret;
        }
        case MEDIA_KEY_SYSTEM_GETSECURITYLEVEL: {
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SYSTEM_GETSECURITYLEVEL enter.");
            IKeySessionService::SecurityLevel securityLevel = IKeySessionService::SECURITY_LEVEL_UNKNOWN;
            int32_t ret = GetSecurityLevel(&securityLevel);
            if (!reply.WriteInt32(securityLevel)) {
                DRM_ERR_LOG("KeySessionServiceStub Write GetSecurityLevel failed");
                return IPC_STUB_WRITE_PARCEL_ERR;
            }
            DRM_INFO_LOG("KeySessionServiceStub MEDIA_KEY_SYSTEM_GETSECURITYLEVEL exit.");
            return ret;
        }
        case MEDIA_KEY_SYSTEM_SETCONFIGURATION: {
            DRM_INFO_LOG("MediaKeySystemServiceStub MEDIA_KEY_SYSTEM_SETCONFIGURATION enter.");
            int type = data.ReadInt32();
            std::string propertyName = data.ReadString();
            std::string value = data.ReadString();
            int32_t ret = SetConfiguration(IMediaKeySystemService::ConfigType(type), propertyName, value);
            DRM_INFO_LOG("bMediaKeySystemServiceStub MEDIA_KEY_SYSTEM_SETCONFIGURATION exit.");
            return ret;
        }
        case MEDIA_KEY_SYSTEM_GETCONFIGURATION: {
            DRM_INFO_LOG("bMediaKeySystemServiceStub MEDIA_KEY_SYSTEM_GETCONFIGURATION enter.");
            int type = data.ReadInt32();
            std::string propertyName = data.ReadString();
            std::string value;
            int32_t ret = GetConfiguration(IMediaKeySystemService::ConfigType(type), propertyName, value);
            if (!reply.WriteString(value)) {
                DRM_ERR_LOG("bMediaKeySystemServiceStub Write GenerateKeySystemRequest failed");
                return IPC_STUB_WRITE_PARCEL_ERR;
            }
            DRM_INFO_LOG("bMediaKeySystemServiceStub MEDIA_KEY_SYSTEM_GETCONFIGURATION exit.");
            return ret;
        }
        case MEDIA_KEY_SYSTEM_GETMETRIC: {
            DRM_INFO_LOG("MediaKeySystemServiceStub MEDIA_KEY_SYSTEM_GETMETRIC enter.");
            std::vector<IMediaKeySystemService::KeyValue> infoMap;
            int32_t ret = GetMetric(infoMap);
            reply.WriteInt32(infoMap.size());
            for (auto info : infoMap) {
                reply.WriteString(info.name);
                reply.WriteString(info.value);
            }
            DRM_INFO_LOG("MediaKeySystemServiceStub MEDIA_KEY_SYSTEM_GETMETRIC exit.");
            return ret;
        }
        case MEDIA_KEY_SYSTEM_RELEASE: {
            DRM_INFO_LOG("MediaKeySystemServiceStub KEY_SESSION_RELEASE enter.");
            int32_t ret = Release();
            DRM_INFO_LOG("MediaKeySystemServiceStub KEY_SESSION_RELEASE exit.");
            return ret;
        }
        default: {
            DRM_ERR_LOG("default case, need check MediaKeySystemServiceStub");
            return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
        }
    }
}
} // namespace DrmStandard
} // namespace OHOS