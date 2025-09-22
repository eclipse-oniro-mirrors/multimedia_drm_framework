/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
#include "media_key_system_callback_taihe.h"

namespace ANI {
namespace DrmStandard {
MediaKeySystemCallbackTaihe::MediaKeySystemCallbackTaihe()
{
    if (mainHandler_ == nullptr) {
        std::shared_ptr<OHOS::AppExecFwk::EventRunner> runner = OHOS::AppExecFwk::EventRunner::GetMainEventRunner();
        mainHandler_ = std::make_shared<OHOS::AppExecFwk::EventHandler>(runner);
    }
}

MediaKeySystemCallbackTaihe::~MediaKeySystemCallbackTaihe() {}

void MediaKeySystemCallbackTaihe::SetCallbackReference(const std::string eventType,
    std::shared_ptr<AutoRef> callbackPair)
{
    DRM_INFO_LOG("MediaKeySystemCallbackNapi SetCallbackReference");
    std::lock_guard<std::mutex> lock(mutex_);
    callbackMap_[eventType] = callbackPair;
}

void MediaKeySystemCallbackTaihe::ClearCallbackReference(const std::string eventType)
{
    DRM_INFO_LOG("MediaKeySystemCallbackNapi ClearCallbackReference");
    std::lock_guard<std::mutex> lock(mutex_);
    callbackMap_.erase(eventType);
}

void MediaKeySystemCallbackTaihe::SendEvent(const std::string &event, int32_t extra, const std::vector<uint8_t> &data)
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::unique_ptr<MediaKeySystemTaiheCallback> cb = std::make_unique<MediaKeySystemTaiheCallback>();
    DRM_CHECK_AND_RETURN_LOG(cb != nullptr, "No memory");
    cb->callback = callbackMap_[event];
    cb->callbackName = event;
    cb->eventParame.extra = extra;
    cb->eventParame.data.assign(data.begin(), data.end());
    return OnTaiheCallbackInterrupt(cb);
}

void MediaKeySystemCallbackTaihe::OnTaiheCallbackInterrupt(std::unique_ptr<MediaKeySystemTaiheCallback> &taiheCb)
{
    DRM_TAIHE_CHECK_AND_RETURN_VOID_LOG(taiheCb.get() != nullptr, "OnTaiheCallbackInterrupt: taiheCb is null");
    MediaKeySystemTaiheCallback *event = taiheCb.get();
    auto task = [event]() {
        std::shared_ptr<MediaKeySystemTaiheCallback> context(static_cast<MediaKeySystemTaiheCallback*>(event),
            [](MediaKeySystemTaiheCallback* ptr) {
            delete ptr;
        });
        DRM_TAIHE_CHECK_AND_RETURN_VOID_LOG(event != nullptr, "event is nullptr");
        do {
            std::shared_ptr<AutoRef> eventInfoRef = event->callback;
            DRM_TAIHE_CHECK_AND_RETURN_VOID_LOG(eventInfoRef != nullptr, "eventInfoRef is nullptr");
            DRM_TAIHE_CHECK_AND_RETURN_VOID_LOG(eventInfoRef->callbackRef_ != nullptr, "callbackRef_ is nullptr");
            std::shared_ptr<taihe::callback<void(EventInfo const&)>>
                cacheCallback = std::reinterpret_pointer_cast<taihe::callback<void
                    (EventInfo const&)>>(eventInfoRef->callbackRef_);
            std::vector<uint8_t> eventParameVec(event->eventParame.data.begin(), event->eventParame.data.end());
            ::taihe::array<uint8_t> infoArr(eventParameVec);
            EventInfo eventInfo{
                infoArr,
                taihe::string(std::to_string(event->eventParame.extra))
            };
            (*cacheCallback)(eventInfo);
        } while (0);
    };
    bool ret = mainHandler_->PostTask(task, "OnKeySystemRequired", 0,
        OHOS::AppExecFwk::EventQueue::Priority::IMMEDIATE, {});
    if (!ret) {
        DRM_ERR_LOG("Failed to PostTask!");
    }
    taiheCb.release();
}
} // namespace DrmStandard
} // namespace ANI