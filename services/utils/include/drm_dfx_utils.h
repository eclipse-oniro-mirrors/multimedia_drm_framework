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

#ifndef DRM_DFX_UTILS_H
#define DRM_DFX_UTILS_H
#include <string>
#include <vector>

namespace OHOS {
namespace DrmStandard {
    struct StatisticsInfo {
        std::string pluginUuid;
        std::string pluginName;
        std::string vendorName;
        std::string versionName;
        std::string bundleName;
        uint8_t targetVersion;
    };
    std::string __attribute__((visibility("default"))) GetClientBundleName(int32_t uid);
    uint8_t __attribute__((visibility("default"))) GetClientBundleInfoTargetVersion(const std::string &bundleName,
        int32_t uid);
    std::string __attribute__((visibility("default"))) CastToHexString(std::vector<uint8_t> binaryData);
    uint32_t __attribute__((visibility("default"))) CalculateTimeDiff(std::chrono::system_clock::time_point timeBefore,
        std::chrono::system_clock::time_point timeAfter);
    const int minimumDigit = 2;
}  // namespace DrmStandard
}  // namespace OHOS

#endif  // DRM_DFX_UTILS_H