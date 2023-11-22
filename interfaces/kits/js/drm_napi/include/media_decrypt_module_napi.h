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
#ifndef OHOS_DRM_DECYPT_MODULE_H_
#define OHOS_DRM_DECYPT_MODULE_H_

#include "media_decrypt_module_impl.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "drm_log.h"
#include "drm_napi_utils.h"
#include "drm_error_code.h"

namespace OHOS {
namespace DrmStandard {
static const char DECYPT_MODULE_NAPI_CLASS_NAME[] = "decryptModule";
class MediaDecryptModuleNapi {
public:
    MediaDecryptModuleNapi();
    ~MediaDecryptModuleNapi();
    static napi_value Init(napi_env env, napi_value exports);
    static napi_value GetDecryptModule(napi_env env, sptr<MediaDecryptModuleImpl> decryptModuleImpl);
    static napi_value Release(napi_env env, napi_callback_info info);
    static napi_value DecryptMediaData(napi_env env, napi_callback_info info);

private:
    static napi_value MediaDecryptModuleNapiConstructor(napi_env env, napi_callback_info info);
    static void MediaDecryptModuleNapiDestructor(napi_env env, void *nativeObject, void *finalize);

    napi_env env_;
    napi_ref wrapper_;
    static thread_local napi_ref sConstructor_;
    static thread_local sptr<MediaDecryptModuleImpl> sMediaDecryptModuleImpl_;
    sptr<MediaDecryptModuleImpl> mediaDecryptModuleImpl_;
};
} // DrmStandard
} // OHOS


#endif // OHOS_DRM_DECYPT_MODULE_H_