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
#include "key_session_service_callback_stub.h"
#include "remote_request_code.h"
#include "drm_log.h"
#include "drm_error_code.h"

namespace OHOS {
namespace DrmStandard {
int32_t MediaKeySessionServiceCallbackStub::OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    int32_t errCode = -1;
    if (data.ReadInterfaceToken() != GetDescriptor()) {
        return errCode;
    }
    switch (code) {
        case MEDIA_KEY_SESSION_SERVICE_CALLBACK_SEND_EVENT:
            errCode = MediaKeySessionServiceCallbackStub::HandleSendEvent(data);
            break;
        case MEDIA_KEY_SESSION_SERVICE_CALLBACK_SEND_EVENT_KEY_CHANGED:
            errCode = MediaKeySessionServiceCallbackStub::HandleSendEventKeyChanged(data);
            break;
        default:
            DRM_ERR_LOG("MediaKeySessionServiceCallbackStub request code %{public}u not handled", code);
            errCode = IPCObjectStub::OnRemoteRequest(code, data, reply, option);
            break;
    }
    return errCode;
}

int32_t MediaKeySessionServiceCallbackStub::HandleSendEvent(MessageParcel &data)
{
    DRM_INFO_LOG("MediaKeySessionServiceCallbackStub HandleSendEvent enter.");
    int32_t event = data.ReadInt32();
    uint32_t extra = data.ReadUint32();
    uint32_t dataSize = data.ReadUint32();
    std::vector<uint8_t> customizedData;
    for (uint32_t i = 0; i < dataSize; i++) {
        customizedData.emplace_back(data.ReadUint8());
    }
    DrmEventType eventType = static_cast<DrmEventType>(event);
    return SendEvent(eventType, extra, customizedData);
}

int32_t MediaKeySessionServiceCallbackStub::HandleSendEventKeyChanged(MessageParcel &data)
{
    DRM_INFO_LOG("MediaKeySessionServiceCallbackStub HandleSendEventKeyChanged enter.");
    std::map<std::vector<uint8_t>, MediaKeySessionKeyStatus> statusTable;
    uint32_t mapSize = data.ReadUint32();
    for (uint32_t index = 0; index < mapSize; index++) {
        std::vector<uint8_t> item;
        uint32_t idSize = data.ReadUint32();
        for (uint32_t i = 0; i < idSize; i++) {
            item.emplace_back(data.ReadUint8());
        }
        MediaKeySessionKeyStatus licenseStatus = static_cast<MediaKeySessionKeyStatus>(data.ReadInt32());
        statusTable[item] = licenseStatus;
    }

    bool hasNewGoodLicense = data.ReadBool();
    return SendEventKeyChanged(statusTable, hasNewGoodLicense);
}
} // namespace DrmStandard
} // namespace OHOS