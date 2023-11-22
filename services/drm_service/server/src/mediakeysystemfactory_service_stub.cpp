/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <codecvt>
#include "ipc_skeleton.h"
#include "xcollie/xcollie.h"
#include "xcollie/xcollie_define.h"
#include "drm_error_code.h"
#include "drm_log.h"
#include "drm_napi_utils.h"
#include "mediakeysystemfactory_service_stub.h"

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

static int ProcessMediaKeySystemSupportedRequest(MediaKeySystemFactoryServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option)
{
    int paramNum = data.ReadInt32();
    bool isSurpported = false;

    DRM_CHECK_AND_RETURN_RET_LOG((paramNum <= ARGS_NUM_THREE) && (paramNum >= ARGS_NUM_ONE), IPC_STUB_WRITE_PARCEL_ERR,
        "MediaKeySystemFactoryServiceStub paramNum is invalid");
    std::string uuid = data.ReadString();
    if (paramNum == ARGS_NUM_ONE) {
        int32_t ret = stub->IsMediaKeySystemSupported(uuid, &isSurpported);
        DRM_CHECK_AND_RETURN_RET_LOG(reply.WriteBool(isSurpported), IPC_STUB_WRITE_PARCEL_ERR,
            "MediaKeySystemFactoryServiceStub Write isSurpported failed");
        return ret;
    }
    std::string mimeType = data.ReadString();
    if (paramNum == ARGS_NUM_TWO) {
        int32_t ret = stub->IsMediaKeySystemSupported(uuid, mimeType, &isSurpported);
        DRM_CHECK_AND_RETURN_RET_LOG(reply.WriteBool(isSurpported), IPC_STUB_WRITE_PARCEL_ERR,
            "MediaKeySystemFactoryServiceStub Write isSurpported failed");
        return ret;
    }

    int32_t securityLevel = data.ReadInt32();
    if (paramNum == ARGS_NUM_THREE) {
        int32_t ret = stub->IsMediaKeySystemSupported(uuid, mimeType, securityLevel, &isSurpported);
        DRM_CHECK_AND_RETURN_RET_LOG(reply.WriteBool(isSurpported), IPC_STUB_WRITE_PARCEL_ERR,
            "MediaKeySystemFactoryServiceStub Write isSurpported failed");
        return ret;
    }
    return DRM_OK;
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
            DRM_INFO_LOG("MediaKeySystemFactoryServiceStub IS_MEDIA_KEY_SYSTEM_SURPPORTED enter.");
            int ret = ProcessMediaKeySystemSupportedRequest(this, data, reply, option);
            DRM_INFO_LOG("MediaKeySystemFactoryServiceStub IS_MEDIA_KEY_SYSTEM_SURPPORTED exit.");
            return ret;
        }
        case MEDIA_KEY_SYSTEM_FACTORY_CREATE_MEDIA_KEYSYSTEM: {
            DRM_INFO_LOG("MediaKeySystemFactoryServiceStub CREATE_MEDIA_KEYSYSTEM enter.");
            sptr<IMediaKeySystemService> mediaKeysystemProxy = nullptr;
            std::string uuid = data.ReadString();
            int errCode = CreateMediaKeySystem(uuid, mediaKeysystemProxy);
            if (errCode != ERR_NONE) {
                DRM_ERR_LOG("MediaKeySystemFactoryServiceStub CreateMediaKeySystem failed : %{public}d", errCode);
                return errCode;
            }
            if (!reply.WriteRemoteObject(mediaKeysystemProxy->AsObject())) {
                DRM_ERR_LOG("MediaKeySystemFactoryServiceStub CreateMediaKeySystem Write MediaKeySession obj failed");
                return IPC_STUB_WRITE_PARCEL_ERR;
            }
            DRM_INFO_LOG("MediaKeySystemFactoryServiceStub CREATE_MEDIA_KEYSYSTEM exit.");
            return errCode;
        }
    }
    return DRM_OK;
}
} // namespace DrmStandard
} // namespace OHOS