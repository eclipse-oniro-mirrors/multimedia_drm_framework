/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#ifndef DRM_HELPER_H
#define DRM_HELPER_H

#include "datashare_helper.h"

namespace OHOS {
namespace DrmStandard {
class DrmHelper {
public:
    static std::string GetDeviceType();
    static std::string GetSettingDataValue(const std::string &tableType, const std::string &key);
    static std::shared_ptr<DataShare::DataShareHelper> CreateDataShareHelperProxy(
        int32_t userId, std::string tableType = "");
    static int32_t GetCurrentUserId();
};
}  // namespace DrmStandard
}  // namespace OHOS

#endif  // DRM_HELPER_H