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

#include <codecvt>
#include "ipc_skeleton.h"
#include "xcollie/xcollie.h"
#include "xcollie/xcollie_define.h"
#include "drm_error_code.h"
#include "drm_log.h"
#include "mediakeysystemfactory_service_stub.h"

namespace {
constexpr uint32_t MAX_LISTNER_NUM = 64;
}

namespace OHOS {
namespace DrmStandard {
MediaKeySystemFactoryServiceStub::MediaKeySystemFactoryServiceStub()
{
    deathRecipientMap_.clear();
}

MediaKeySystemFactoryServiceStub::~MediaKeySystemFactoryServiceStub()
{
}

void MediaKeySystemFactoryServiceStub::MediaKeySystemFactoryClientDied(pid_t pid)
{
    DRM_ERR_LOG("MediaKeySystemFactory client has died, pid:%{public}d", pid);
    if (clientListenerMap_.find(pid) != clientListenerMap_.end()) {
        if (clientListenerMap_[pid] != nullptr && clientListenerMap_[pid]->AsObject() != nullptr &&
            deathRecipientMap_.find(pid) != deathRecipientMap_.end() && deathRecipientMap_[pid] != nullptr) {
            (void)clientListenerMap_[pid]->AsObject()->RemoveDeathRecipient(deathRecipientMap_[pid]);
        }
        deathRecipientMap_.erase(pid);
        clientListenerMap_.erase(pid);
    }
    DistroyForClientDied(pid);
}

int32_t MediaKeySystemFactoryServiceStub::SetListenerObject(const sptr<IRemoteObject> &object)
{
    pid_t pid = IPCSkeleton::GetCallingPid();
    if (clientListenerMap_.find(pid) != clientListenerMap_.end()) {
        if (clientListenerMap_[pid] != nullptr && clientListenerMap_[pid]->AsObject() != nullptr &&
            deathRecipientMap_.find(pid) != deathRecipientMap_.end() && deathRecipientMap_[pid] != nullptr) {
            (void)clientListenerMap_[pid]->AsObject()->RemoveDeathRecipient(deathRecipientMap_[pid]);
        }
        deathRecipientMap_.erase(pid);
        clientListenerMap_.erase(pid);
    }
    DRM_CHECK_AND_RETURN_RET_LOG(clientListenerMap_.size() < MAX_LISTNER_NUM,
        DRM_OPERATION_NOT_ALLOWED, "the number of listeners exceeds MAX_LISTNER_NUM: 64");
    DRM_CHECK_AND_RETURN_RET_LOG(object != nullptr, DRM_MEMORY_ERROR, "set listener object is nullptr");
    sptr<IDrmListener> clientListener = iface_cast<IDrmListener>(object);
    DRM_CHECK_AND_RETURN_RET_LOG(
        clientListener != nullptr, DRM_MEMORY_ERROR, "failed to convert IDrmListener");
    sptr<DrmDeathRecipient> deathRecipient = new (std::nothrow) DrmDeathRecipient(pid);
    DRM_CHECK_AND_RETURN_RET_LOG(deathRecipient != nullptr, DRM_MEMORY_ERROR, "failed to new DrmDeathRecipient");
    deathRecipient->SetNotifyCb([this] (pid_t pid) {
        this->MediaKeySystemFactoryClientDied(pid);
    });
    if (clientListener->AsObject() != nullptr) {
        (void)clientListener->AsObject()->AddDeathRecipient(deathRecipient);
    }
    DRM_DEBUG_LOG("MediaKeySystem client pid:%{public}d", pid);
    deathRecipientMap_[pid] = deathRecipient;
    clientListenerMap_[pid] = clientListener;
    return DRM_OK;
}

static int32_t ProcessMediaKeySystemSupportedRequest(MediaKeySystemFactoryServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option)
{
    int32_t paramNum = data.ReadInt32();
    bool isSurpported = false;

    DRM_CHECK_AND_RETURN_RET_LOG((paramNum <= (int32_t)ARGS_NUM_THREE) && (paramNum >= (int32_t)ARGS_NUM_ONE),
        IPC_STUB_WRITE_PARCEL_ERR, "paramNum is invalid");
    std::string name = data.ReadString();
    if (paramNum == ARGS_NUM_ONE) {
        int32_t ret = stub->IsMediaKeySystemSupported(name, &isSurpported);
        DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_OK, ret, "IsMediaKeySystemSupported faild, errCode:%{public}d", ret);
        DRM_CHECK_AND_RETURN_RET_LOG(reply.WriteBool(isSurpported), IPC_STUB_WRITE_PARCEL_ERR,
            "Write isSurpported failed.");
        return ret;
    }
    std::string mimeType = data.ReadString();
    if (paramNum == ARGS_NUM_TWO) {
        int32_t ret = stub->IsMediaKeySystemSupported(name, mimeType, &isSurpported);
        DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_OK, ret, "IsMediaKeySystemSupported faild, errCode:%{public}d", ret);
        DRM_CHECK_AND_RETURN_RET_LOG(reply.WriteBool(isSurpported), IPC_STUB_WRITE_PARCEL_ERR,
            "Write isSurpported failed.");
        return ret;
    }

    int32_t securityLevel = data.ReadInt32();
    if (paramNum == ARGS_NUM_THREE) {
        int32_t ret = stub->IsMediaKeySystemSupported(name, mimeType, securityLevel, &isSurpported);
        DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_OK, ret, "IsMediaKeySystemSupported faild, errCode:%{public}d", ret);
        DRM_CHECK_AND_RETURN_RET_LOG(reply.WriteBool(isSurpported), IPC_STUB_WRITE_PARCEL_ERR,
            "Write isSurpported failed.");
        return ret;
    }
    return DRM_OK;
}

static int32_t ProcessSetListenerObject(MediaKeySystemFactoryServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option)
{
    DRM_INFO_LOG("ProcessSetListenerObject enter.");
    (void)reply;
    sptr<IRemoteObject> object = data.ReadRemoteObject();
    int32_t ret = stub->SetListenerObject(object);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_OK, ret,
        "ProcessSetListenerObject faild, errCode:%{public}d", ret);
    return ret;
}

static int32_t ProcessGetMediaKeySystems(MediaKeySystemFactoryServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option)
{
    DRM_INFO_LOG("ProcessGetMediaKeySystems enter.");
    std::map<std::string, std::string> mediaKeySystemNames;
    int32_t ret = stub->GetMediaKeySystems(mediaKeySystemNames);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_OK, ret, "GetMediaKeySystems faild, errCode:%{public}d", ret);
    reply.WriteInt32(mediaKeySystemNames.size());
    for (auto mediaKeySystemName : mediaKeySystemNames) {
        DRM_CHECK_AND_RETURN_RET_LOG(reply.WriteString(mediaKeySystemName.first), IPC_STUB_WRITE_PARCEL_ERR,
            "Write mediaKeySystem name failed.");
        DRM_CHECK_AND_RETURN_RET_LOG(reply.WriteString(mediaKeySystemName.second), IPC_STUB_WRITE_PARCEL_ERR,
            "Write mediaKeySystem uuid failed.");
    }
    return DRM_OK;
}

static int32_t ProcessGetMediaKeySystemUuid(MediaKeySystemFactoryServiceStub *stub, MessageParcel &data,
    MessageParcel &reply, MessageOption &option)
{
    DRM_INFO_LOG("ProcessGetMediaKeySystemUuid enter.");
    std::string name = data.ReadString();
    std::string uuid;
    int32_t ret = stub->GetMediaKeySystemUuid(name, uuid);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_OK, ret, "ProcessGetMediaKeySystemUuid faild, errCode:%{public}d", ret);
    if (!reply.WriteString(uuid)) {
        DRM_ERR_LOG("ProcessGetMediaKeySystemUuid write value failed.");
        return IPC_STUB_WRITE_PARCEL_ERR;
    }
    return DRM_OK;
}

int32_t MediaKeySystemFactoryServiceStub::OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    int32_t ret = DRM_OK;
    if (data.ReadInterfaceToken() != GetDescriptor()) {
        DRM_DEBUG_LOG("ReadInterfaceToken failed.");
        return -1;
    }

    switch (code) {
        case MEDIA_KEY_SYSTEM_FACTORY_IS_MEDIA_KEY_SYSTEM_SURPPORTED: {
            DRM_INFO_LOG("IS_MEDIA_KEY_SYSTEM_SURPPORTED enter.");
            return ProcessMediaKeySystemSupportedRequest(this, data, reply, option);
        }
        case MEDIA_KEY_SYSTEM_FACTORY_CREATE_MEDIA_KEYSYSTEM: {
            DRM_INFO_LOG("CREATE_MEDIA_KEYSYSTEM enter.");
            sptr<IMediaKeySystemService> mediaKeysystemProxy = nullptr;
            std::string name = data.ReadString();
            ret = CreateMediaKeySystem(name, mediaKeysystemProxy);
            if (ret != DRM_OK) {
                DRM_ERR_LOG("CreateMediaKeySystem failed, errCode: %{public}d", ret);
                return ret;
            }
            if (!reply.WriteRemoteObject(mediaKeysystemProxy->AsObject())) {
                DRM_ERR_LOG("CreateMediaKeySystem Write MediaKeySession object failed.");
                return IPC_STUB_WRITE_PARCEL_ERR;
            }
            return ret;
        }
        case MEDIA_KEY_SYSTEM_FACTORY_SET_LISTENER_OBJ: {
            DRM_INFO_LOG("MEDIA_KEY_SYSTEM_FACTORY_SET_LISTENER_OBJ enter.");
            return ProcessSetListenerObject(this, data, reply, option);
        }
        case MEDIA_KEY_SYSTEM_FACTORY_GET_MEDIA_KEYSYSTEM_NAME: {
            DRM_INFO_LOG("MEDIA_KEY_SYSTEM_FACTORY_GST_MEDIA_KEYSYSTEM_NAME enter.");
            return ProcessGetMediaKeySystems(this, data, reply, option);
        }
        case MEDIA_KEY_SYSTEM_FACTORY_GET_MEDIA_KEYSYSTEM_UUID: {
            DRM_INFO_LOG("MEDIA_KEY_SYSTEM_FACTORY_GET_MEDIA_KEYSYSTEM_UUID enter.");
            return ProcessGetMediaKeySystemUuid(this, data, reply, option);
        }
    }
    return ret;
}
} // namespace DrmStandard
} // namespace OHOS