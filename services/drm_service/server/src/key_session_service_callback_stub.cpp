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
        case KEY_SESSION_SERVICE_CALLBACK_ON_KEY_EXPIRED:
            errCode = MediaKeySessionServiceCallbackStub::HandleOnMediaKeySessionKeyExpired(data);
            break;
        case KEY_SESSION_SERVICE_CALLBACK_ON_KEY_SESSION_RECLAIMED:
            errCode = MediaKeySessionServiceCallbackStub::HandleOnMediaKeySessionReclaimed(data);
            break;
        default:
            DRM_ERR_LOG("MediaKeySessionServiceCallbackStub request code %{public}u not handled", code);
            errCode = IPCObjectStub::OnRemoteRequest(code, data, reply, option);
            break;
    }

    return errCode;
}

int32_t MediaKeySessionServiceCallbackStub::HandleOnMediaKeySessionKeyExpired(MessageParcel &data)
{
    int32_t status = data.ReadInt32();
    DRM_INFO_LOG("MediaKeySessionServiceCallbackStub::HandleOnMediaKeySessionKeyExpired called status = %{public}d", status);
    switch (status)
    {
        case KEY_STATUS_NORMAL:
            return OnMediaKeySessionKeyExpired(KEY_STATUS_NORMAL);
        case KEY_STATUS_EXPIRED:
            return OnMediaKeySessionKeyExpired(KEY_STATUS_EXPIRED);
        default:
            DRM_ERR_LOG("HandleOnMediaKeySessionKeyExpired can't cast status");
            return DRM_ERROR;
    }
}

int32_t MediaKeySessionServiceCallbackStub::HandleOnMediaKeySessionReclaimed(MessageParcel &data)
{
    int32_t status = data.ReadInt32();
    DRM_INFO_LOG("MediaKeySessionServiceCallbackStub::HandleOnMediaKeySessionReclaimed called status = %{public}d", status);
    switch (status)
    {
        case SESSION_STATUS_NORMAL:
            return OnMediaKeySessionReclaimed(SESSION_STATUS_NORMAL);
        case SESSION_STATUS_RECLAIMED:
            return OnMediaKeySessionReclaimed(SESSION_STATUS_RECLAIMED);
        default:
            DRM_ERR_LOG("HandleOnMediaKeySessionReclaimed can't cast status");
            return DRM_ERROR;
    }
}
} // namespace DrmStandard
} // namespace OHOS