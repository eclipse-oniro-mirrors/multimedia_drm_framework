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

#include "key_session_service_callback_stub.h"
#include "remote_request_code.h"
#include "drm_log.h"
#include "drm_error_code.h"

namespace OHOS {
namespace DrmStandard {
int32_t KeySessionServiceCallbackStub::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    int32_t errCode = -1;

    if (data.ReadInterfaceToken() != GetDescriptor()) {
        return errCode;
    }
    switch (code) {
        case KEY_SESSION_SERVICE_CALLBACK_ON_KEY_EXPIRED:
            errCode = KeySessionServiceCallbackStub::HandleOnKeySessionKeyExpired(data);
            break;
        case KEY_SESSION_SERVICE_CALLBACK_ON_KEY_SESSION_RECLAIMED:
            errCode = KeySessionServiceCallbackStub::HandleOnKeySessionReclaimed(data);
            break;
        default:
            DRM_ERR_LOG("KeySessionServiceCallbackStub request code %{public}u not handled", code);
            errCode = IPCObjectStub::OnRemoteRequest(code, data, reply, option);
            break;
    }

    return errCode;
}

int32_t KeySessionServiceCallbackStub::HandleOnKeySessionKeyExpired(MessageParcel& data)
{
    int32_t status = data.ReadInt32();
    DRM_INFO_LOG("KeySessionServiceCallbackStub::HandleOnKeySessionKeyExpired called status = %{public}d", status);
    switch (status) {
        case KEY_STATUS_NORMAL:
            return OnKeySessionKeyExpired(KEY_STATUS_NORMAL);
        case KEY_STATUS_EXPIRED:
            return OnKeySessionKeyExpired(KEY_STATUS_EXPIRED);
        default:
            DRM_ERR_LOG("HandleOnKeySessionKeyExpired can't cast status");
            return DRM_ERROR;
    }
}

int32_t KeySessionServiceCallbackStub::HandleOnKeySessionReclaimed(MessageParcel& data)
{
    int32_t status = data.ReadInt32();
    DRM_INFO_LOG("KeySessionServiceCallbackStub::HandleOnKeySessionReclaimed called status = %{public}d", status);
    switch (status) {
        case SESSION_STATUS_NORMAL:
            return OnKeySessionReclaimed(SESSION_STATUS_NORMAL);
        case SESSION_STATUS_RECLAIMED:
            return OnKeySessionReclaimed(SESSION_STATUS_RECLAIMED);
        default:
            DRM_ERR_LOG("HandleOnKeySessionReclaimed can't cast status");
            return DRM_ERROR;
    }
}
} // namespace DrmStandard
} // namespace OHOS