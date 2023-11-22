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

#ifndef OHOS_DRM_MEDIA_KEY_SYSTEM_NAPI_H_
#define OHOS_DRM_MEDIA_KEY_SYSTEM_NAPI_H_

#include <cinttypes>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "hilog/log.h"
#include "drm_log.h"
#include "drm_napi_utils.h"
#include "key_session_impl.h"
#include "media_key_system_impl.h"
#include "media_key_system_factory_impl.h"

namespace OHOS {
namespace DrmStandard {
static const char MEDIA_KEY_SYSTEM_NAPI_CLASS_NAME[] = "MediaKeySystem";

class MediaKeySystemNapi {
public:
    static napi_value Init(napi_env env, napi_value exports);
    MediaKeySystemNapi();
    ~MediaKeySystemNapi();
    static napi_value IsMediaKeySystemSupported(napi_env env, napi_callback_info info);
    static napi_value CreateMediaKeySystemInstance(napi_env env, napi_callback_info info);
    static napi_value SetConfigurationString(napi_env env, napi_callback_info info);
    static napi_value GetConfigurationString(napi_env env, napi_callback_info info);
    static napi_value SetConfigurationByteArray(napi_env env, napi_callback_info info);
    static napi_value GetConfigurationByteArray(napi_env env, napi_callback_info info);
    static napi_value GetMaxSecurityLevel(napi_env env, napi_callback_info info);
    static napi_value GenerateKeySystemRequest(napi_env env, napi_callback_info info);
    static napi_value ProcessKeySystemResponse(napi_env env, napi_callback_info info);
    static napi_value CreateMediaKeySession(napi_env env, napi_callback_info info);
    static napi_value GetMetrics(napi_env env, napi_callback_info info);

    static napi_value GetOfflineLicenseIds(napi_env env, napi_callback_info info);
    static napi_value GetOfflineLicenseStatus(napi_env env, napi_callback_info info);
    static napi_value RemoveOfflineLicense(napi_env env, napi_callback_info info);

    static napi_value GetCertificateStatus(napi_env env, napi_callback_info info);
    static napi_value Destroy(napi_env env, napi_callback_info info);

private:
    static napi_value MediaKeySystemNapiConstructor(napi_env env, napi_callback_info info);
    static void MediaKeySystemNapiDestructor(napi_env env, void *nativeObject, void *finalize);

    static thread_local napi_ref sConstructor_;
    napi_env env_;
    napi_ref wrapper_;
    sptr<MediaKeySystemImpl> mediaKeySystemImpl_;
    static thread_local sptr<MediaKeySystemImpl> sMediaKeySystemImpl_;
};
} // namespace DrmStandard
} // namespace OHOS

#endif // OHOS_DRM_MEDIA_KEY_SYSTEM_NAPI_H_
