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

#ifndef DRM_ERROR_CODE_H
#define DRM_ERROR_CODE_H

namespace OHOS {
namespace DrmStandard {
/**
 * @brief drm remote request code for IPC.
 *
 * @since 1.0
 * @version 1.0
 */
enum DrmErrorCode {
    DRM_ERROR = -1,
    DRM_OK = 0,
    DRM_ALLOC_ERROR,
    DRM_INVALID_ARG,
    DRM_UNSUPPORTED,
    DRM_INVALID_SESSION_CFG,
    DRM_INVALID_STATE,
    DRM_UNKNOWN_ERROR,
    DRM_OPERATION_NOT_ALLOWED,
    DRM_HOST_ERROR,
    DRM_SERVICE_ERROR,
};
} // namespace DrmStandard
} // namespace OHOS
#endif // DRM_ERROR_CODE_H