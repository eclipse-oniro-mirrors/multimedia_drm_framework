/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include <unordered_map>
#include "native_err_convertor.h"

namespace OHOS {
namespace DrmStandard {

const std::unordered_map<DrmInnerErrCode, Drm_ErrCode> INNERCODE_TO_NDKCODE = {
    {DRM_INNER_ERR_OK,                                                         DRM_ERR_OK},
    {DRM_INNER_ERR_BASE,                                                DRM_CAPI_ERR_BASE},
    {DRM_INNER_ERR_UNKNOWN,                                               DRM_ERR_UNKNOWN},
    {DRM_INNER_ERR_OPERATION_NOT_PERMITTED,               DRM_ERR_OPERATION_NOT_PERMITTED},
    {DRM_INNER_ERR_MAX_SYSTEM_NUM_REACHED,                 DRM_ERR_MAX_SYSTEM_NUM_REACHED},
    {DRM_INNER_ERR_MAX_SESSION_NUM_REACHED,               DRM_ERR_MAX_SESSION_NUM_REACHED},
    {DRM_INNER_ERR_NO_MEMORY,                                           DRM_ERR_NO_MEMORY},
    {DRM_INNER_ERR_INVALID_VAL,                                       DRM_ERR_INVALID_VAL},
    {DRM_INNER_ERR_IO,                                                         DRM_ERR_IO},
    {DRM_INNER_ERR_TIMEOUT,                                               DRM_ERR_TIMEOUT},
    {DRM_INNER_ERR_SERVICE_DIED,                                     DRM_ERR_SERVICE_DIED},
    {DRM_INNER_ERR_INVALID_STATE,                                   DRM_ERR_INVALID_STATE},
    {DRM_INNER_ERR_UNSUPPORTED,                                       DRM_ERR_UNSUPPORTED},
    {DRM_INNER_ERR_INVALID_KEY_SESSION,                               DRM_ERR_INVALID_VAL},
    {DRM_INNER_ERR_INVALID_MEDIA_KEY_SYSTEM,                          DRM_ERR_INVALID_VAL},
    {DRM_INNER_ERR_KEY_REQUEST_NOT_ALLOWED,                           DRM_ERR_INVALID_VAL},
    {DRM_INNER_ERR_KEY_REQUEST_INVALID_MEDIA_KEY_TYPE,                DRM_ERR_INVALID_VAL},
    {DRM_INNER_ERR_KEY_REQUEST_INVALID_MIMETYPE,                      DRM_ERR_INVALID_VAL},
    {DRM_INNER_ERR_KEY_REQUEST_INVALID_INITDATA,                      DRM_ERR_INVALID_VAL},
    {DRM_INNER_ERR_KEY_REQUEST_INVALID_DRM_CERTIFICATES,              DRM_ERR_INVALID_VAL},
    {DRM_INNER_ERR_DECRYPT_SECURE_NOT_SUPPORT,                        DRM_ERR_UNSUPPORTED},
    {DRM_INNER_ERR_POLICY_CONTENT_PROTECTION_LEVEL_NOT_ALLOWED,       DRM_ERR_UNSUPPORTED},
    {DRM_INNER_ERR_MEMORY_ERROR,                                        DRM_ERR_NO_MEMORY},
    {DRM_INNER_ERR_SERVICE_FATAL_ERROR,                              DRM_ERR_SERVICE_DIED},
};

Drm_ErrCode DrmInnerErrToNdkErr(DrmInnerErrCode errCode)
{
    if (INNERCODE_TO_NDKCODE.find(errCode) != INNERCODE_TO_NDKCODE.end()) {
        return INNERCODE_TO_NDKCODE.at(errCode);
    }
    return DRM_ERR_UNKNOWN;
}

} // namespace DrmStandard
} // namespace OHOS