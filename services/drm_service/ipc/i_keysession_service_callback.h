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

#ifndef OHOS_DRM_IKEY_SESSION_SERVICE_CALLBACK_H
#define OHOS_DRM_IKEY_SESSION_SERVICE_CALLBACK_H

#include <map>
#include "iremote_broker.h"
#include "drm_types.h"

namespace OHOS {
namespace DrmStandard {
class IMediaKeySessionServiceCallback : public IRemoteBroker {
public:
    virtual int32_t SendEvent(DrmEventType event, int32_t extra, const std::vector<uint8_t> &data) = 0;
    virtual int32_t SendEventKeyChanged(std::map<std::vector<uint8_t>, MediaKeySessionKeyStatus> statusTable,
        bool hasNewGoodLicense) = 0;

    DECLARE_INTERFACE_DESCRIPTOR(u"IMediaKeySessionServiceCallback");
};
} // namespace DrmStandard
} // namespace OHOS

#endif