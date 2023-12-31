/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef DRM_NAPI_UTILS_H
#define DRM_NAPI_UTILS_H

#include <vector>
#include "drm_error_code.h"
#include "drm_log.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"

#define DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar)           \
    do {                                                               \
        void *data;                                                    \
        napi_get_cb_info(env, info, &(argc), argv, &(thisVar), &data); \
    } while (0)

#define DRM_NAPI_GET_JS_OBJ_WITH_ZERO_ARGS(env, info, status, thisVar)             \
    do {                                                                           \
        void *data;                                                                \
        status = napi_get_cb_info(env, info, nullptr, nullptr, &(thisVar), &data); \
    } while (0)

#define DRM_NAPI_GET_JS_ASYNC_CB_REF(env, arg, count, cbRef)  \
    do {                                                      \
        napi_valuetype valueType = napi_undefined;            \
        napi_typeof(env, arg, &valueType);                    \
        if (valueType == napi_function) {                     \
            napi_create_reference(env, arg, count, &(cbRef)); \
        } else {                                              \
            NAPI_ASSERT(env, false, "type mismatch");         \
        }                                                     \
    } while (0)

#define DRM_NAPI_ASSERT_NULLPTR_CHECK(env, result) \
    do {                                           \
        if ((result) == nullptr) {                 \
            napi_get_undefined(env, &(result));    \
            return result;                         \
        }                                          \
    } while (0)

#define DRM_NAPI_CREATE_PROMISE(env, callbackRef, deferred, result) \
    do {                                                            \
        if ((callbackRef) == nullptr) {                             \
            napi_create_promise(env, &(deferred), &(result));       \
        }                                                           \
    } while (0)

#define DRM_NAPI_CREATE_RESOURCE_NAME(env, resource, resourceName)                 \
    do {                                                                           \
        napi_create_string_utf8(env, resourceName, NAPI_AUTO_LENGTH, &(resource)); \
    } while (0)

#define DRM_NAPI_CHECK_NULL_PTR_RETURN_UNDEFINED(env, ptr, ret, message) \
    do {                                                                 \
        if ((ptr) == nullptr) {                                          \
            HiLog::Error(LABEL, message);                                \
            napi_get_undefined(env, &(ret));                             \
            return ret;                                                  \
        }                                                                \
    } while (0)

#define DRM_NAPI_CHECK_NULL_PTR_RETURN_VOID(ptr, message) \
    do {                                                  \
        if ((ptr) == nullptr) {                           \
            HiLog::Error(LABEL, message);                 \
            return;                                       \
        }                                                 \
    } while (0)

#define DRM_NAPI_ASSERT_EQUAL(condition, errMsg) \
    do {                                         \
        if (!(condition)) {                      \
            HiLog::Error(LABEL, errMsg);         \
            return;                              \
        }                                        \
    } while (0)

#define DRM_NAPI_CHECK_AND_BREAK_LOG(cond, fmt, ...) \
    do {                                             \
        if (!(cond)) {                               \
            DRM_ERR_LOG(fmt, ##__VA_ARGS__);         \
            break;                                   \
        }                                            \
    } while (0)

#define DRM_NAPI_CHECK_AND_RETURN_LOG(cond, fmt, ...) \
    do {                                              \
        if (!(cond)) {                                \
            DRM_ERR_LOG(fmt, ##__VA_ARGS__);          \
            return;                                   \
        }                                             \
    } while (0)

namespace OHOS {
namespace DrmStandard {
/* Constants for array index */
const int32_t PARAM0 = 0;
const int32_t PARAM1 = 1;
const int32_t PARAM2 = 2;
const int32_t PARAM3 = 3;

/* Constants for array size */
const int32_t ARGS_ZERO = 0;
const int32_t ARGS_ONE = 1;
const int32_t ARGS_TWO = 2;
const int32_t ARGS_THREE = 3;
const int32_t ARGS_FOUR = 4;


struct AsyncContext {
    napi_env env;
    napi_async_work work;
    napi_deferred deferred;
    napi_ref callbackRef;
    bool status;
    int32_t taskId;
    int32_t errorCode;
    std::string errorMsg;
    std::string funcName;
    bool isInvalidArgument;
};
} // namespace DrmStandard
} // namespace OHOS
#endif /* DRM_NAPI_UTILS_H_ */
