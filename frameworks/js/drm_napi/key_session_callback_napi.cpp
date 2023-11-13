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

#include "key_session_callback_napi.h"
#include "drm_log.h"
#include "drm_error_code.h"

namespace OHOS {
namespace DrmStandard {
KeySessionCallbackNapi::KeySessionCallbackNapi() {}

KeySessionCallbackNapi::~KeySessionCallbackNapi() {}

void KeySessionCallbackNapi::SetCallbackReference(const std::string eventType, sptr<CallBackPair> callbackPair)
{
    DRM_INFO_LOG("KeySessionCallbackNapi SetCallbackReference");
    std::lock_guard<std::mutex> lock(mutex_);
    callbackMap_[eventType] = callbackPair;
}

void KeySessionCallbackNapi::ClearCallbackReference(const std::string eventType)
{
    DRM_INFO_LOG("KeySessionCallbackNapi ClearCallbackReference");
    std::lock_guard<std::mutex> lock(mutex_);
    if (callbackMap_.find(eventType) != callbackMap_.end()) {
        callbackMap_.erase(eventType);
        DRM_INFO_LOG("KeySessionCallbackNapi ClearCallbackReference %{public}s", eventType.c_str());
    } else {
        DRM_WARNING_LOG("ClearCallbackReference failed, no this event: %{public}s", eventType.c_str());
    }
}

void KeySessionCallbackNapi::OnKeySessionKeyExpired(const std::string eventType, const KeyStatus status)
{
    DRM_INFO_LOG("KeySessionCallbackNapi OnKeySessionKeyExpired");
    napi_value result;
    napi_value jsCallback = nullptr;
    napi_value retVal;
    napi_status state;

    CHECK_AND_RETURN_LOG(callbackMap_.find(eventType) != callbackMap_.end(),
        "Napi OnKeySessionKeyExpired, not register this callback");
    sptr<CallBackPair> item = callbackMap_[eventType];

    CHECK_AND_RETURN_LOG(item != nullptr, "Napi OnKeySessionKeyExpired, sptr callbackPair is nullptr");
    napi_env env = item->GetEnv();
    napi_ref callbackRef = item->GetCallback();

    napi_get_undefined(env, &result);
    napi_create_object(env, &result);

    int32_t keyStatus = static_cast<int32_t>(status);
    napi_value args[1] = {nullptr};
    (void)napi_create_int32(env, keyStatus, &args[0]);
    DRM_INFO_LOG("Napi OnKeySessionKeyExpired, Status: %{public}d", keyStatus);
    
    napi_get_reference_value(env, callbackRef, &jsCallback);
    state = napi_call_function(env, nullptr, jsCallback, ARGS_ONE, args, &retVal);
    CHECK_AND_RETURN_LOG(state == napi_ok,
        "%{public}s failed to napi_call_function", eventType.c_str());
}

void KeySessionCallbackNapi::OnKeySessionReclaimed(const std::string eventType, const SessionStatus status)
{
    DRM_INFO_LOG("KeySessionCallbackNapi OnKeySessionReclaimed");
    napi_value result;
    napi_value jsCallback = nullptr;
    napi_value retVal;
    napi_status state;

    CHECK_AND_RETURN_LOG(callbackMap_.find(eventType) != callbackMap_.end(),
        "Napi OnKeySessionReclaimed, not register this callback");
    sptr<CallBackPair> item = callbackMap_[eventType];

    CHECK_AND_RETURN_LOG(item != nullptr, "Napi OnKeySessionReclaimed, sptr callbackPair is nullptr");
    napi_env env = item->GetEnv();
    napi_ref callbackRef = item->GetCallback();

    napi_get_undefined(env, &result);
    napi_create_object(env, &result);

    int32_t sessionStatus = static_cast<int32_t>(status);
    napi_value args[1] = {nullptr};
    (void)napi_create_int32(env, sessionStatus, &args[0]);
    DRM_INFO_LOG("Napi OnKeySessionReclaimed, Status: %{public}d", sessionStatus);
    
    napi_get_reference_value(env, callbackRef, &jsCallback);
    state = napi_call_function(env, nullptr, jsCallback, ARGS_ONE, args, &retVal);
    CHECK_AND_RETURN_LOG(state == napi_ok,
        "%{public}s failed to napi_call_function", eventType.c_str());
}
} // namespace DrmStandard
} // namespace OHOS