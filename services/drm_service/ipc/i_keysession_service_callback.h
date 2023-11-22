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

#ifndef OHOS_DRM_IKEY_SESSION_SERVICE_CALLBACK_H_
#define OHOS_DRM_IKEY_SESSION_SERVICE_CALLBACK_H_

#include <map>
#include "iremote_broker.h"

namespace OHOS {
namespace DrmStandard {
enum KeyStatus {
    KEY_STATUS_NORMAL = 0,
    KEY_STATUS_EXPIRED,
};

enum SessionStatus {
    SESSION_STATUS_NORMAL = 0,
    SESSION_STATUS_RECLAIMED,
};

enum EventType : int32_t {
    EVENTTYPE_PROVISIONREQUIRED = 1,
    EVENTTYPE_KEYNEEDED = 2,
    EVENTTYPE_KEYEXPIRED = 3,
    EVENTTYPE_VENDOR_DEFINED = 4,
    EVENTTYPE_KEYSESSION_RECLAIMED = 5,
    EVENTTYPE_EXPIRATIONUPDATE = 6,
    EVENTTYPE_KEYCHANGE = 7,
    EVENTTYPE_KEYSESSION_LOST = 8,
};

enum MediaKeySessionLicenseStatus : int32_t {
    MEDIA_KEY_SESSION_LICENSE_STATUS_USABLE = 0,
    MEDIA_KEY_SESSION_LICENSE_STATUS_EXPIRED = 1,
    MEDIA_KEY_SESSION_LICENSE_STATUS_OUTPUT_NOT_ALLOWED = 2,
    MEDIA_KEY_SESSION_LICENSE_STATUS_PENDING = 3,
    MEDIA_KEY_SESSION_LICENSE_STATUS_INTERNAL_ERROR = 4,
    MEDIA_KEY_SESSION_LICENSE_STATUS_USABLE_IN_FUTURE = 5,
};

class IMediaKeySessionServiceCallback : public IRemoteBroker {
public:
    virtual int32_t OnMediaKeySessionKeyExpired(const KeyStatus status) = 0;
    virtual int32_t OnMediaKeySessionReclaimed(const SessionStatus status) = 0;

    DECLARE_INTERFACE_DESCRIPTOR(u"IMediaKeySessionServiceCallback");
};
} // namespace DrmStandard
} // namespace OHOS

#endif