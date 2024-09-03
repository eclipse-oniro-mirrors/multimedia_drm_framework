/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
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

#ifndef DRM_NATIVE_ERR_CONVERTOR_H
#define DRM_NATIVE_ERR_CONVERTOR_H

#include <iostream>
#include "drm_error_code.h"
#include "native_drm_err.h"

namespace OHOS {
namespace DrmStandard {

__attribute__((visibility("default"))) Drm_ErrCode DrmInnerErrToNdkErr(DrmInnerErrCode errCode);

} // namespace DrmStandard
} // namespace OHOS
#endif // DRM_NATIVE_ERR_CONVERTOR_H