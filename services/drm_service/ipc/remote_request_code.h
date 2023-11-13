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

#ifndef OHOS_DRM_REMOTE_REQUEST_CODE_H
#define OHOS_DRM_REMOTE_REQUEST_CODE_H

namespace OHOS {
namespace DrmStandard {
enum MediaKeySystemFactoryServiceRequestCode {
    MEDIA_KEY_SYSTEM_FACTORY_IS_MEDIA_KEY_SYSTEM_SURPPORTED = 0,
    MEDIA_KEY_SYSTEM_FACTORY_CREATE_MEDIA_KEYSYSTEM = 1,
};

enum MediaKeySystemServiceRequestCode {
    MEDIA_KEY_SYSTEM_CREATE_KEY_SESSION = 0,
    MEDIA_KEY_SYSTEM_DESTROY_KEY_SESSION = 1,
    MEDIA_KEY_SYSTEM_GENERATE_KEYSYSTEM_REQUEST = 2,
    MEDIA_KEY_SYSTEM_GENERATE_KEYSYSTEM_RESPONSE = 3,
    MEDIA_KEY_SYSTEM_SETCONFIGURATION = 4,
    MEDIA_KEY_SYSTEM_GETCONFIGURATION = 5,
    MEDIA_KEY_SYSTEM_GETMETRIC = 6,
    MEDIA_KEY_SYSTEM_RELEASE = 7,
    MEDIA_KEY_SYSTEM_GETSECURITYLEVEL = 8,
};

enum KeySessionServiceRequestCode {
    CREATE_MEDIA_DECRYPT_MODULE = 0,
    RELEASE_MEDIA_DECRYPT_MODULE = 1,
    KEY_SESSION_RELEASE = 2,
    MEDIA_KEY_SESSION_GENERATE_LICENSE_REQUEST = 3,
    MEDIA_KEY_SESSION_PROCESS_LICENSE_RESPONSE = 4,
    MEDIA_KEY_SESSION_GENERATE_OFFLINE_RELEASE_REQUEST = 5,
    MEDIA_KEY_SESSION_PROCESS_OFFLINE_RELEASE_RESPONSE = 6,
    MEDIA_KEY_SESSION_GENERATE_CHECK_LICENSE_STATUS = 7,
    MEDIA_KEY_SESSION_RESTORE_OFFLINEKEYS = 8,
    MEDIA_KEY_SESSION_REMOVE_OFFLINEKEYS = 9,
    MEDIA_KEY_SESSION_GET_OFFLINEKEYIDS = 10,
    MEDIA_KEY_SESSION_REMOVE_LICENSE = 11,
    MEDIA_KEY_SESSION_GET_OFFLINEKEY_STATE = 12,
    MEDIA_KEY_SESSION_SET_CALLBACK = 13,
};

enum MediaDecryptModuleServiceRequestCode {
    DECRYPT_MODULE_DECRYPT_DATA = 0,
    DECRYPT_MODULE_REQUIRE_SECURE_DECODER = 1,
    DECRYPT_MODULE_RELEASE = 2,
};

enum KeySessionServiceCallbackRequestCode {
    KEY_SESSION_SERVICE_CALLBACK_ON_KEY_EXPIRED = 0,
    KEY_SESSION_SERVICE_CALLBACK_ON_KEY_SESSION_RECLAIMED,
};
} // namespace DrmStandard
} // namespace OHOS

#endif // OHOS_DRM_REMOTE_REQUEST_CODE_H
