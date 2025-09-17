/*
 * Copyright (C) 2025 Huawei Device Co., Ltd.
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
#ifndef OHOS_DRM_DRM_TAIHE_UTILS_H
#define OHOS_DRM_DRM_TAIHE_UTILS_H

#include <map>
#include <mutex>
#include <vector>
#include "drm_log.h"
#include "ohos.multimedia.drm.proj.hpp"
#include "ohos.multimedia.drm.impl.hpp"
#include "taihe/runtime.hpp"

namespace ANI {
namespace DrmStandard {
using namespace taihe;
using namespace ohos::multimedia::drm;
struct TaiheProvisionRequest {
    std::vector<uint8_t> data;
    std::string defaultURL;
};

struct DrmEventParame {
    int32_t extra;
    std::vector<uint8_t> data;
};

class DrmTaiheUtils {
public:
    static string ToTaiheString(const std::string &src);
    static void ThrowExceptionError(const std::string errMsg);
    template <typename EnumType>
    static bool GetEnumKeyByValue(int32_t value, typename EnumType::key_t &key);
};

#define DRM_TAIHE_CHECK_AND_RETURN_VOID_LOG(cond, fmt, ...) \
    do {                                              \
        if (!(cond)) {                                \
            DRM_ERR_LOG(fmt, ##__VA_ARGS__);          \
            return;                                   \
        }                                             \
    } while (0)

#define DRM_TAIHE_CHECK_AND_RETURN_LOG(cond, status, fmt, ...) \
    do {                                              \
        if (!(cond)) {                                \
            DRM_ERR_LOG(fmt, ##__VA_ARGS__);          \
            return status;                                   \
        }                                             \
    } while (0)

} // namespace DrmStandard
} // namespace ANI
#endif // OHOS_DRM_DRM_TAIHE_UTILS_H