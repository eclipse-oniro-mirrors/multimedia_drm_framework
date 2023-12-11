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

#include "key_session_service_callback_proxy.h"
#include "drm_log.h"
#include "remote_request_code.h"

namespace OHOS {
namespace DrmStandard {
MediaKeySessionServiceCallbackProxy::MediaKeySessionServiceCallbackProxy(const sptr<IRemoteObject> &impl)
    : IRemoteProxy<IMediaKeySessionServiceCallback>(impl) {};

int32_t MediaKeySessionServiceCallbackProxy::SendEvent(DrmEventType event, uint32_t extra,
    const std::vector<uint8_t> data)
{
    MessageParcel parcelData;
    MessageParcel reply;
    MessageOption option;
    DRM_INFO_LOG("KeySessionServiceCallbackProxy SendEvent called, event:%{public}d", event);
    if (!parcelData.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("KeySessionServiceCallbackProxy SendEvent Write interface token failed");
        return IPC_PROXY_ERR;
    }
    if (!parcelData.WriteInt32(event)) {
        DRM_ERR_LOG("KeySessionServiceCallbackProxy SendEvent Write event failed");
        return IPC_PROXY_ERR;
    }
    if (!parcelData.WriteUint32(extra)) {
        DRM_ERR_LOG("KeySessionServiceCallbackProxy SendEvent Write extra failed");
        return IPC_PROXY_ERR;
    }
    if (!parcelData.WriteUint32(data.size())) {
        DRM_ERR_LOG("KeySessionServiceCallbackProxy SendEvent Write data size failed");
        return IPC_PROXY_ERR;
    }
    for (auto item : data) {
        if (!parcelData.WriteUint8(item)) {
            DRM_ERR_LOG("KeySessionServiceCallbackProxy SendEvent Write data failed");
            return IPC_PROXY_ERR;
        }
    }
    int error = Remote()->SendRequest(MEDIA_KEY_SESSION_SERVICE_CALLBACK_SEND_EVENT,
        parcelData, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("KeySessionServiceCallbackProxy SendEvent failed, error: %{public}d", error);
    }
    return error;
}

int32_t MediaKeySessionServiceCallbackProxy::SendEventKeyChanged(std::map<std::vector<uint8_t>,
    MediaKeySessionKeyStatus> statusTable, bool hasNewGoodLicense)
{
    MessageParcel parcelData;
    MessageParcel reply;
    MessageOption option;
    DRM_INFO_LOG("SendEventKeyChanged");
    if (!parcelData.WriteInterfaceToken(GetDescriptor())) {
        DRM_ERR_LOG("SendEventKeyChanged Write interface token failed");
        return IPC_PROXY_ERR;
    }

    uint32_t mapSize = statusTable.size();
    if (!parcelData.WriteUint32(mapSize)) {
        DRM_ERR_LOG("SendEventKeyChanged Write mapSize failed");
        return IPC_PROXY_ERR;
    }
    for (auto item : statusTable) {
        uint32_t idSize = item.first.size();
        (void)parcelData.WriteUint32(idSize);
        for (uint32_t i = 0; i < idSize; i++) {
            (void)parcelData.WriteUint8(item.first[i]);
        }
        (void)parcelData.WriteInt32(static_cast<int32_t>(item.second));
    }

    if (!parcelData.WriteBool(hasNewGoodLicense)) {
        DRM_ERR_LOG("SendEventKeyChanged Write extra failed");
        return IPC_PROXY_ERR;
    }

    int error = Remote()->SendRequest(MEDIA_KEY_SESSION_SERVICE_CALLBACK_SEND_EVENT_KEY_CHANGED,
        parcelData, reply, option);
    if (error != ERR_NONE) {
        DRM_ERR_LOG("SendEventKeyChanged failed, error: %{public}d", error);
    }
    return error;
    return 0;
}
} // DrmStandard
} // namespace OHOS