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

#include "mediakeysystemfactory_service_stub.h"
#include "xcollie/xcollie.h"
#include "xcollie/xcollie_define.h"
#include "drm_error_code.h"
#include "drm_log.h"
#include "drm_napi_utils.h"
#include "ipc_skeleton.h"

namespace OHOS {
namespace DrmStandard {
MediaKeySystemFactoryServiceStub::MediaKeySystemFactoryServiceStub()
{
    deathRecipientMap_.clear();
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " Instances create", (POINTER_MASK & reinterpret_cast<uintptr_t>(this)));
}

MediaKeySystemFactoryServiceStub::~MediaKeySystemFactoryServiceStub()
{
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " Instances destroy", (POINTER_MASK & reinterpret_cast<uintptr_t>(this)));
}

int MediaKeySystemFactoryServiceStub::OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    if (data.ReadInterfaceToken() != GetDescriptor()) {
        DRM_DEBUG_LOG("MediaKeySystemFactoryServiceStub: ReadInterfaceToken failed");
        return -1;
    }

    switch (code) {
        case MEDIA_KEY_SYSTEM_FACTORY_IS_MEDIA_KEY_SYSTEM_SURPPORTED: {
            DRM_INFO_LOG("MEDIA_KEY_SYSTEM_FACTORY_IS_MEDIA_KEY_SYSTEM_SURPPORTED enter.");
            int paramNum = data.ReadInt32();
            bool isSurpported = false;
            if (paramNum > ARGS_THREE || paramNum < 0) {
                DRM_ERR_LOG("MediaKeySystemFactoryServiceStub paramNum is invalid");
                return IPC_STUB_WRITE_PARCEL_ERR;
            }
            DRM_ERR_LOG("MediaKeySystemFactoryServiceStub ");

            // get uuid
            std::string uuid = data.ReadString();
            // search by uuid
            if (paramNum == ARGS_ONE) {
                DRM_ERR_LOG("MediaKeySystemFactoryServiceStub ");
                int32_t ret = IsMediaKeySystemSupported(uuid, &isSurpported);
                DRM_ERR_LOG("MediaKeySystemFactoryServiceStub ");
                if (!reply.WriteBool(isSurpported)) {
                    DRM_ERR_LOG("MediaKeySystemFactoryServiceStub Write isSurpported failed");
                    return IPC_STUB_WRITE_PARCEL_ERR;
                }
                DRM_ERR_LOG("MediaKeySystemFactoryServiceStub ");
                return ret;
            }

            // get mimeType
            std::string mimeType = data.ReadString();
            // search by uuid and mimeType
            if (paramNum == ARGS_TWO) {
                int32_t ret = IsMediaKeySystemSupported(uuid, mimeType, &isSurpported);
                if (!reply.WriteBool(isSurpported)) {
                    DRM_ERR_LOG("MediaKeySystemFactoryServiceStub Write isSurpported failed");
                    return IPC_STUB_WRITE_PARCEL_ERR;
                }
                return ret;
            }

            // get securityLevel
            int32_t securityLevel = data.ReadInt32();
            // search by uuid, mineType and securityLevel
            if (paramNum == ARGS_THREE) {
                int32_t ret = IsMediaKeySystemSupported(uuid, mimeType, securityLevel, &isSurpported);
                if (!reply.WriteBool(isSurpported)) {
                    DRM_ERR_LOG("MediaKeySystemFactoryServiceStub Write isSurpported failed");
                    return IPC_STUB_WRITE_PARCEL_ERR;
                }
                return ret;
            }
            DRM_INFO_LOG("MEDIA_KEY_SYSTEM_FACTORY_IS_MEDIA_KEY_SYSTEM_SURPPORTED exit.");
            return -1;
        }
        case MEDIA_KEY_SYSTEM_FACTORY_CREATE_MEDIA_KEYSYSTEM: {
            DRM_INFO_LOG("MediaKeySystemFactoryServiceStub MEDIA_KEY_SYSTEM_FACTORY_CREATE_MEDIA_KEYSYSTEM enter.");
            std::string uuid = data.ReadString();
            int errCode = CreateMediaKeySystem(uuid);
            if (errCode != ERR_NONE) {
                DRM_ERR_LOG("MediaKeySystemFactoryServiceStub CreateMediaKeySystem failed : %{public}d", errCode);
                return errCode;
            }
            DRM_INFO_LOG("MediaKeySystemFactoryServiceStub MEDIA_KEY_SYSTEM_FACTORY_CREATE_MEDIA_KEYSYSTEM exit.");
            return errCode;
        }
    }
    return DRM_OK;
}
} // namespace DrmStandard
} // namespace OHOS