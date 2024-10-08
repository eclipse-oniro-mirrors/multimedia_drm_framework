/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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
#ifndef OHOS_DRM_MEDIA_KEY_SYSTEM_SERVICE_CALLBACK_STUB_H
#define OHOS_DRM_MEDIA_KEY_SYSTEM_SERVICE_CALLBACK_STUB_H

#include "i_mediakeysystem_service_callback.h"
#include "iremote_stub.h"
#include "remote_request_code.h"

namespace OHOS {
namespace DrmStandard {
class MediaKeySystemServiceCallbackStub : public IRemoteStub<IMediaKeySystemServiceCallback> {
public:
    MediaKeySystemServiceCallbackStub() = default;
    ~MediaKeySystemServiceCallbackStub() = default;
    virtual int32_t OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply,
        MessageOption &option) override;

private:
    int32_t HandleSendEvent(MessageParcel &data);
};
} // namespace DrmStandard
} // namespace OHOS

#endif // OHOS_DRM_MEDIA_KEY_SYSTEM_SERVICE_CALLBACK_STUB_H
