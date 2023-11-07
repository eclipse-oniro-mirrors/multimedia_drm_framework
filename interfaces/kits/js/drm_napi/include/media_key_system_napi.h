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
#include "media_key_system_impl.h"
#include "media_key_system_factory_impl.h"
#include "key_session_impl.h"
#include "drm_log.h"
#include "drm_napi_utils.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "hilog/log.h"

namespace OHOS {
namespace DrmStandard {
static const char MEDIA_KEY_SYSTEM_NAPI_CLASS_NAME[] = "MediaKeySystem";
class MediaKeySystemNapi {
public:
    static napi_value Init(napi_env env, napi_value exports);
    MediaKeySystemNapi();
    ~MediaKeySystemNapi();
    napi_value Release(napi_env env, napi_callback_info info);
    static napi_value IsMediaKeySystemSupported(napi_env env, napi_callback_info info);
    static napi_value CreateMediaKeySystemInstance(napi_env env, napi_callback_info info);
    static napi_value SetConfiguration(napi_env env, napi_callback_info info);
    static napi_value GetConfiguration(napi_env env, napi_callback_info info);
    static napi_value GenerateKeySystemRequest(napi_env env, napi_callback_info info);
    static napi_value ProcessKeySystemResponse(napi_env env, napi_callback_info info);
    static napi_value CreateKeySession(napi_env env, napi_callback_info info);
    static napi_value GetMetric(napi_env env, napi_callback_info info);
    static napi_value GetSecurityLevel(napi_env env, napi_callback_info info);
private:
    static napi_value MediaKeySystemNapiConstructor(napi_env env, napi_callback_info info);
    static void MediaKeySystemNapiDestructor(napi_env env, void *nativeObject, void *finalize);

    static thread_local napi_ref sConstructor_;
    napi_env env_;
    napi_ref wrapper_;
    sptr<MediaKeySystemImpl> keySystem_;
    static thread_local sptr<MediaKeySystemImpl> sKeySystem_;
};
} // namespace DrmStandard
} // namespace OHOS

#endif // OHOS_DRM_MEDIA_KEY_SYSTEM_NAPI_H_
