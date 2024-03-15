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

#ifndef OHOS_DRM_REMOTE_REQUEST_CODE_H
#define OHOS_DRM_REMOTE_REQUEST_CODE_H

namespace OHOS {
namespace DrmStandard {
enum MediaKeySystemFactoryServiceRequestCode {
    MEDIA_KEY_SYSTEM_FACTORY_IS_MEDIA_KEY_SYSTEM_SURPPORTED = 0,
    MEDIA_KEY_SYSTEM_FACTORY_CREATE_MEDIA_KEYSYSTEM = 1,
    MEDIA_KEY_SYSTEM_FACTORY_SET_LISTENER_OBJ = 2,
};

enum MediaKeySystemServiceRequestCode {
    MEDIA_KEY_SYSTEM_CREATE_KEY_SESSION = 0,
    MEDIA_KEY_SYSTEM_GENERATE_KEYSYSTEM_REQUEST,
    MEDIA_KEY_SYSTEM_PROCESS_KEYSYSTEM_RESPONSE,
    MEDIA_KEY_SYSTEM_SETCONFIGURATION_STRING,
    MEDIA_KEY_SYSTEM_GETCONFIGURATION_STRING,
    MEDIA_KEY_SYSTEM_SETCONFIGURATION_BYTEARRAY,
    MEDIA_KEY_SYSTEM_GETCONFIGURATION_BYTEARRAY,
    MEDIA_KEY_SYSTEM_GETMETRIC,
    MEDIA_KEY_SYSTEM_RELEASE,
    MEDIA_KEY_SYSTEM_GETMAXSECURITYLEVEL,
    MEDIA_KEY_SYSTEM_GETCERTIFICATESTATUS,
    MEDIA_KEY_SYSTEM_GET_OFFLINELICENSEIDS,
    MEDIA_KEY_SYSTEM_GET_OFFLINEKEY_STATUS,
    MEDIA_KEY_SYSTEM_REMOVE_OFFLINELICENSE,
    MEDIA_KEY_SYSTEM_SET_LISTENER_OBJ,
    MEDIA_KEY_SYSTEM_SETCALLBACK,
};

enum MediaKeySessionServiceRequestCode {
    CREATE_MEDIA_DECRYPT_MODULE = 0,
    KEY_SESSION_RELEASE,
    MEDIA_KEY_SESSION_GENERATE_LICENSE_REQUEST,
    MEDIA_KEY_SESSION_PROCESS_LICENSE_RESPONSE,
    MEDIA_KEY_SESSION_GENERATE_OFFLINE_RELEASE_REQUEST,
    MEDIA_KEY_SESSION_PROCESS_OFFLINE_RELEASE_RESPONSE,
    MEDIA_KEY_SESSION_GENERATE_CHECK_LICENSE_STATUS,
    MEDIA_KEY_SESSION_RESTORE_OFFLINEKEYS,
    MEDIA_KEY_SESSION_REMOVE_LICENSE,
    MEDIA_KEY_SESSION_SET_CALLBACK,
    MEDIA_KEY_SESSION_REQUIRE_SECURE_DECODER,
    MEDIA_KEY_SESSION_SET_LISTENER_OBJ,
    MEDIA_KEY_SESSION_GETSECURITYLEVEL,
};

enum MediaDecryptModuleServiceRequestCode {
    DECRYPT_MODULE_DECRYPT_DATA = 0,
    DECRYPT_MODULE_SET_LISTENER_OBJ = 1,
    DECRYPT_MODULE_RELEASE = 2,
};

enum MediaKeySessionServiceCallbackRequestCode {
    MEDIA_KEY_SESSION_SERVICE_CALLBACK_SEND_EVENT = 0,
    MEDIA_KEY_SESSION_SERVICE_CALLBACK_SEND_EVENT_KEY_CHANGED,
};

enum MediaKeySystemServiceCallbackRequestCode {
    MEDIA_KEY_SYSTEM_SERVICE_CALLBACK_SEND_EVENT = 0,
};
} // namespace DrmStandard
} // namespace OHOS

#endif // OHOS_DRM_REMOTE_REQUEST_CODE_H
