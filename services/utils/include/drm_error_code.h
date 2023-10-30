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

#ifndef DRM_ERROR_CODE_H
#define DRM_ERROR_CODE_H

namespace OHOS{
namespace DrmStandard{
/**
 * @brief drm remote request code for IPC.
 * 
 * @since 1.0
 * @version 1.0
*/
enum DrmErrorCode{
    DRM_ERROR = -1,
    DRM_SUCCESS = 0,
    NO_SYSTEMP_APP_PERMISSION = 202,
    INVALID_ARGUMENT = 7400101,
    OPERATION_NOT_ALLOWED = 7400102,
    SESSION_NOT_CONFIG = 7400103,
    SESSION_NOT_RUNNING = 7400104,
    SESSION_CONFIG_LOCKED = 7400105,
    DEVICE_SETTING_LOCKED = 7400106,
    DEVICE_DISABLED = 7400108,
    SERVICE_FATAL_ERROR = 7400201
};
} // namespace DrmStandard
} // namespace OHOS
#endif // DRM_ERROR_CODE_H