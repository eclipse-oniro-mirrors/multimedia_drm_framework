/*
 * Copyright (c) 2023  Huawei Device Co., Ltd.
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

#ifndef OHOS_DRM_LOG_H
#define OHOS_DRM_LOG_H

#include <stdio.h>
#include <cinttypes>
#include "hilog/log.h"
#include "hisysevent.h"


#define MAX_STRING_SIZE 256

#define __DRM_FILE_NAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)

#define POINTER_MASK 0x00FFFFFF
#define FAKE_POINTER(addr) (POINTER_MASK & reinterpret_cast<uintptr_t>(addr))

#define DECORATOR_HILOG(op, fmt, args...)                                                                            \
    do {                                                                                                             \
        op(LOG_CORE, "{%{public}s()-%{public}s:%{public}d}" fmt, __FUNCTION__, __DRM_FILE_NAME__, __LINE__, ##args); \
    } while (0)

#define DRM_DEBUG_LOG(fmt, ...) DECORATOR_HILOG(HILOG_DEBUG, fmt, ##__VA_ARGS__)
#define DRM_ERR_LOG(fmt, ...) DECORATOR_HILOG(HILOG_ERROR, fmt, ##__VA_ARGS__)
#define DRM_WARNING_LOG(fmt, ...) DECORATOR_HILOG(HILOG_WARN, fmt, ##__VA_ARGS__)
#define DRM_INFO_LOG(fmt, ...) DECORATOR_HILOG(HILOG_INFO, fmt, ##__VA_ARGS__)
#define DRM_FATAL_LOG(fmt, ...) DECORATOR_HILOG(HILOG_FATAL, fmt, ##__VA_ARGS__)

#ifndef DRM_CHECK_AND_RETURN_RET_LOG
#define DRM_CHECK_AND_RETURN_RET_LOG(cond, ret, fmt, ...) \
    do {                                                  \
        if (!(cond)) {                                    \
            DRM_ERR_LOG(fmt, ##__VA_ARGS__);              \
            return ret;                                   \
        }                                                 \
    } while (0)
#endif

#ifndef DRM_CHECK_AND_RETURN_LOG
#define DRM_CHECK_AND_RETURN_LOG(cond, fmt, ...) \
    do {                                         \
        if (!(cond)) {                           \
            DRM_ERR_LOG(fmt, ##__VA_ARGS__);     \
            return;                              \
        }                                        \
    } while (0)
#endif

#define POINTER_MASK 0x00FFFFFF

#endif // OHOS_DRM_LOG_H