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
#include "drm_log.h"
#include "drm_error_code.h"
#include "drm_napi_utils.h"
#include "key_session_callback_napi.h"

namespace OHOS {
namespace DrmStandard {
MediaKeySessionCallbackNapi::MediaKeySessionCallbackNapi() {}

MediaKeySessionCallbackNapi::~MediaKeySessionCallbackNapi() {}

void MediaKeySessionCallbackNapi::SetCallbackReference(const std::string eventType, sptr<CallBackPair> callbackPair)
{
    DRM_INFO_LOG("MediaKeySessionCallbackNapi SetCallbackReference");
    std::lock_guard<std::mutex> lock(mutex_);
    callbackMap_[eventType] = callbackPair;
}

void MediaKeySessionCallbackNapi::ClearCallbackReference(const std::string eventType)
{
    DRM_INFO_LOG("MediaKeySessionCallbackNapi ClearCallbackReference");
    std::lock_guard<std::mutex> lock(mutex_);
    if (callbackMap_.find(eventType) != callbackMap_.end()) {
        callbackMap_.erase(eventType);
        DRM_INFO_LOG("MediaKeySessionCallbackNapi ClearCallbackReference %{public}s", eventType.c_str());
    } else {
        DRM_WARNING_LOG("ClearCallbackReference failed, no this event: %{public}s", eventType.c_str());
    }
}

void MediaKeySessionCallbackNapi::SendEvent(const std::string event, uint32_t extra, const std::vector<uint8_t> data)
{
    DRM_INFO_LOG("MediaKeySessionCallbackNapi SendEvent %{public}s", event.c_str());
    DRM_NAPI_CHECK_AND_RETURN_LOG(!event.empty(), "Service event code error");

    napi_value result;
    napi_value jsCallback = nullptr;
    napi_value retVal;
    napi_status state;

    DRM_NAPI_CHECK_AND_RETURN_LOG(callbackMap_.find(event) != callbackMap_.end(), "Not register this callback yet");
    sptr<CallBackPair> item = callbackMap_[event];

    DRM_NAPI_CHECK_AND_RETURN_LOG(item != nullptr, "sptr callbackPair is nullptr");
    napi_env env = item->GetEnv();
    napi_ref callbackRef = item->GetCallback();

    napi_get_undefined(env, &result);
    napi_create_object(env, &result);
    napi_value extraValue;
    napi_create_uint32(env, extra, &extraValue);
    napi_value array = nullptr;
    state = napi_create_array_with_length(env, data.size(), &array);
    for (uint32_t i = 0; i < data.size(); i++) {
        napi_value number = nullptr;
        (void)napi_create_uint32(env, data[i], &number);
        (void)napi_set_element(env, array, i, number);
    }
    napi_value args[ARGS_TWO] = {extraValue, array};
    napi_get_reference_value(env, callbackRef, &jsCallback);
    state = napi_call_function(env, nullptr, jsCallback, ARGS_TWO, args, &retVal);
    DRM_NAPI_CHECK_AND_RETURN_LOG(state == napi_ok,
        "%{public}s failed to napi_call_function", event.c_str());
}

void MediaKeySessionCallbackNapi::SendEventKeyChanged(
    std::map<std::vector<uint8_t>, MediaKeySessionKeyStatus> statusTable, bool hasNewGoodLicense)
{
    DRM_INFO_LOG("MediaKeySessionCallbackNapi SendEventKeyChanged");
    napi_value result;
    napi_value jsCallback = nullptr;
    napi_value retVal;
    napi_status state;

    DRM_NAPI_CHECK_AND_RETURN_LOG(callbackMap_.find("keyChanged") != callbackMap_.end(),
        "Not register this callback yet");
    sptr<CallBackPair> item = callbackMap_["keyChanged"];

    DRM_NAPI_CHECK_AND_RETURN_LOG(item != nullptr, "sptr callbackPair is nullptr");
    napi_env env = item->GetEnv();
    napi_ref callbackRef = item->GetCallback();
    napi_get_undefined(env, &result);
    napi_create_object(env, &result);

    uint32_t index = 0;
    napi_value map;
    napi_create_array_with_length(env, statusTable.size(), &map);
    for (auto item : statusTable) {
        napi_value jsObject;
        napi_value jsKeyId;
        napi_value jsKeyStatus;
        napi_create_object(env, &jsObject);
        state = napi_create_array_with_length(env, item.first.size(), &jsKeyId);
        for (uint32_t i = 0; i < item.first.size(); i++) {
            napi_value number = nullptr;
            (void)napi_create_uint32(env, item.first[i], &number);
            (void)napi_set_element(env, jsKeyId, i, number);
        }
        napi_set_named_property(env, jsObject, "keyId", jsKeyId);
        napi_create_uint32(env, item.second, &jsKeyStatus);
        napi_set_named_property(env, jsObject, "keyStatus", jsKeyStatus);
        napi_set_element(env, map, index, jsObject);
        index++;
    }

    napi_value hasNewGoodLicenseValue;
    napi_get_boolean(env, hasNewGoodLicense, &hasNewGoodLicenseValue);

    napi_value args[ARGS_TWO] = {map, hasNewGoodLicenseValue};
    napi_get_reference_value(env, callbackRef, &jsCallback);
    state = napi_call_function(env, nullptr, jsCallback, ARGS_TWO, args, &retVal);
    DRM_NAPI_CHECK_AND_RETURN_LOG(state == napi_ok,
        "failed to napi_call_function keyChanged");
}
}
}