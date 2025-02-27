/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
#ifndef OHOS_DRM_DRM_ERROR_CODE_NAPI_H
#define OHOS_DRM_DRM_ERROR_CODE_NAPI_H

#include "drm_error_code.h"

namespace OHOS {
namespace DrmStandard {

enum DrmNapiErrCode : int32_t {
    /**
     * the operation completed successfully.
     */
    DRM_NAPI_ERR_OK = 0,
    /**
     * invalid argument.
     */
    DRM_NAPI_ERR_INVALID_VAL = 401,
    /**
     * DRM_NAPI_ERR_BASE.
     */
    DRM_NAPI_ERR_BASE = 24700100,
    /**
     * all unknown errors.
     */
    DRM_NAPI_ERR_UNKNOWN = DRM_NAPI_ERR_BASE + 1,
    /**
     * Meet max MediaKeySystem num limit.
     */
    DRM_NAPI_ERR_MAX_SYSTEM_NUM_REACHED = DRM_NAPI_ERR_BASE + 3,
    /**
     * Meet max MediaKeySession num limit.
     */
    DRM_NAPI_ERR_MAX_SESSION_NUM_REACHED = DRM_NAPI_ERR_BASE + 4,
    /**
     * device not provisioned.
     */
    DRM_NAPI_ERR_NOT_PROVISIONED = DRM_NAPI_ERR_BASE + 5,
    /**
     * no device cert.
     */
    DRM_NAPI_ERR_NO_DEVICE_CERT = DRM_NAPI_ERR_BASE + 6,
    /**
     * drm certificates are revoked.
     */
    DRM_NAPI_ERR_DRM_CERTIFICATES_REVOKED = DRM_NAPI_ERR_BASE + 7,
    /**
     * time error.
     */
    DRM_NAPI_ERR_DEVICE_TIME_ERROR = DRM_NAPI_ERR_BASE + 8,
    /**
     * service denied when processing key response.
     */
    DRM_NAPI_ERR_SERVICE_DENIED = DRM_NAPI_ERR_BASE + 9,
    /**
     * no license when decrypting media samples.
     */
    DRM_NAPI_ERR_NO_LICENSE = DRM_NAPI_ERR_BASE + 10,
    /**
     * no key when decrypting media samples.
     */
    DRM_NAPI_ERR_NO_KEY = DRM_NAPI_ERR_BASE + 11,
    /**
     * service fatal error, for example, service died.
     */
    DRM_NAPI_ERR_SERVICE_FATAL = 24700201,

//缺少memory error
};

__attribute__((visibility("default"))) DrmNapiErrCode DrmInnerErrToNapiErr(DrmInnerErrCode errCode);

__attribute__((visibility("default"))) DrmNapiErrCode DrmInnerErrToNapiErrAPI12(DrmInnerErrCode errCode);

} // namespace DrmStandard
} // namespace OHOS
#endif // OHOS_DRM_DRM_ERROR_CODE_NAPI_H