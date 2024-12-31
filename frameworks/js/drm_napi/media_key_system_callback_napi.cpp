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
#include "media_key_system_callback_napi.h"

namespace OHOS {
namespace DrmStandard {
MediaKeySystemCallbackNapi::MediaKeySystemCallbackNapi(napi_env env)
{
    env_ = env;
}

MediaKeySystemCallbackNapi::~MediaKeySystemCallbackNapi() {}

void MediaKeySystemCallbackNapi::SetCallbackReference(const std::string eventType,
    std::shared_ptr<AutoRef> callbackPair)
{
    DRM_INFO_LOG("MediaKeySystemCallbackNapi SetCallbackReference");
    std::lock_guard<std::mutex> lock(mutex_);
    callbackMap_[eventType] = callbackPair;
}

void MediaKeySystemCallbackNapi::ClearCallbackReference(const std::string eventType)
{
    DRM_INFO_LOG("MediaKeySystemCallbackNapi ClearCallbackReference");
    std::lock_guard<std::mutex> lock(mutex_);
    callbackMap_.erase(eventType);
}

void MediaKeySystemCallbackNapi::SendEvent(const std::string &event, int32_t extra, const std::vector<uint8_t> &data)
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::unique_ptr<MediaKeySystemJsCallback> cb = std::make_unique<MediaKeySystemJsCallback>();
    DRM_NAPI_CHECK_AND_RETURN_VOID_LOG(cb != nullptr, "No memory");
    cb->callback = callbackMap_[event];
    cb->callbackName = event;
    cb->eventParame.extra = extra;
    cb->eventParame.data.assign(data.begin(), data.end());
    return OnJsCallbackInterrupt(cb);
}

void MediaKeySystemCallbackNapi::OnJsCallbackInterrupt(std::unique_ptr<MediaKeySystemJsCallback> &jsCb)
{
    DRM_NAPI_CHECK_AND_RETURN_VOID_LOG(jsCb.get() != nullptr, "OnJsCallbackInterrupt: jsCb.get() is null");
    MediaKeySystemJsCallback *event = jsCb.get();
    auto task = [event]() {
        std::shared_ptr<MediaKeySystemJsCallback> context(
            static_cast<MediaKeySystemJsCallback*>(event),
            [](MediaKeySystemJsCallback* ptr) {
                delete ptr;
        });

        DRM_NAPI_CHECK_AND_RETURN_VOID_LOG(event != nullptr, "event is nullptr");
        std::string request = event->callbackName;

        DRM_NAPI_CHECK_AND_RETURN_VOID_LOG(event->callback != nullptr, "event is nullptr");
        napi_env env = event->callback->env_;
        napi_ref callback = event->callback->cb_;

        napi_handle_scope scope = nullptr;
        napi_open_handle_scope(env, &scope);
        DRM_NAPI_CHECK_AND_RETURN_VOID_LOG(scope != nullptr, "scope is nullptr");
        DRM_DEBUG_LOG("JsCallBack %{public}s, doWork", request.c_str());
        do {
            napi_value jsCallback = nullptr;
            napi_status nstatus = napi_get_reference_value(env, callback, &jsCallback);
            DRM_NAPI_CHECK_AND_CLOSE_RETURN_VOID_LOG(nstatus == napi_ok && jsCallback != nullptr,
                "%{public}s get reference value fail", request.c_str());

            napi_value args[ARGS_ONE] = { nullptr };
            nstatus = NapiParamUtils::SetDrmEventInfo(env, event->eventParame, args[PARAM0]);
            DRM_NAPI_CHECK_AND_CLOSE_RETURN_VOID_LOG(nstatus == napi_ok && args[PARAM0] != nullptr,
                "%{public}s fail to create keysystem callback", request.c_str());

            const size_t argCount = ARGS_ONE;
            napi_value result = nullptr;
            nstatus = napi_call_function(env, nullptr, jsCallback, argCount, args, &result);
            DRM_NAPI_CHECK_AND_CLOSE_RETURN_VOID_LOG(nstatus == napi_ok, "%{public}s fail to call Interrupt callback",
                request.c_str());
        } while (0);
        napi_close_handle_scope(env, scope);
    };
    if (napi_status::napi_ok != napi_send_event(env_, task, napi_eprio_immediate)) {
        DRM_ERR_LOG("OnJsCallbackInterrupt: Failed to SendEvent");
    } else {
        jsCb.release();
    }
}
} // namespace DrmStandard
} // namespace OHOS