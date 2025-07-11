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
#include "key_session_callback_taihe.h"

namespace ANI {
namespace DrmStandard {

MediaKeySessionCallbackTaihe::MediaKeySessionCallbackTaihe()
{
    if (mainHandler_ == nullptr) {
        std::shared_ptr<OHOS::AppExecFwk::EventRunner> runner = OHOS::AppExecFwk::EventRunner::GetMainEventRunner();
        mainHandler_ = std::make_shared<OHOS::AppExecFwk::EventHandler>(runner);
    }
}

MediaKeySessionCallbackTaihe::~MediaKeySessionCallbackTaihe() {}

void MediaKeySessionCallbackTaihe::SetCallbackReference(const std::string eventType,
    std::shared_ptr<AutoRef> callbackPair)
{
    DRM_INFO_LOG("MediaKeySessionCallbackTaihe SetCallbackReference");
    std::lock_guard<std::mutex> lock(mutex_);
    callbackMap_[eventType] = callbackPair;
}

void MediaKeySessionCallbackTaihe::ClearCallbackReference(const std::string eventType)
{
    DRM_INFO_LOG("MediaKeySessionCallbackTaihe ClearCallbackReference");
    std::lock_guard<std::mutex> lock(mutex_);
    callbackMap_.erase(eventType);
}

void MediaKeySessionCallbackTaihe::SendEvent(const std::string &event, int32_t extra, const std::vector<uint8_t> &data)
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::unique_ptr<MediaKeySessionTaiheCallback> cb = std::make_unique<MediaKeySessionTaiheCallback>();
    DRM_CHECK_AND_RETURN_LOG(cb != nullptr, "No memory");
    cb->callback = callbackMap_[event];
    cb->callbackName = event;
    cb->eventParame.extra = extra;
    cb->eventParame.data.assign(data.begin(), data.end());
    return OnTaiheCallbackInterrupt(cb);
}

void MediaKeySessionCallbackTaihe::SendEventKeyChanged(
    std::map<std::vector<uint8_t>, OHOS::DrmStandard::MediaKeySessionKeyStatus> statusTable, bool hasNewGoodLicense)
{
    std::lock_guard<std::mutex> lock(mutex_);
    std::unique_ptr<MediaKeySessionTaiheCallback> cb = std::make_unique<MediaKeySessionTaiheCallback>();
    DRM_CHECK_AND_RETURN_LOG(cb != nullptr, "No memory");
    const std::string event = "keysChange";
    cb->callback = callbackMap_[event];
    cb->callbackName = event;
    cb->keysChangeParame.hasNewGoodLicense = hasNewGoodLicense;
    cb->keysChangeParame.statusTable = statusTable;
    return OnTaiheCallbackInterrupt(cb);
}

EventInfo MediaKeySessionCallbackTaihe::GetEventInfo(DrmEventParame &eventParame)
{
    std::vector<uint8_t> eventParameVec(eventParame.data.begin(), eventParame.data.end());
    array<uint8_t> infoArr(eventParameVec);
    EventInfo eventInfo{
        infoArr,
        string(std::to_string(eventParame.extra))
    };
    return eventInfo;
}

void MediaKeySessionCallbackTaihe::OnTaiheCallbackInterrupt(std::unique_ptr<MediaKeySessionTaiheCallback> &taiheCb)
{
    DRM_TAIHE_CHECK_AND_RETURN_VOID_LOG(taiheCb.get() != nullptr, "OnTaiheCallbackInterrupt: taiheCb is null");
    MediaKeySessionTaiheCallback *event = taiheCb.get();
    auto task = [event, this]() {
        std::shared_ptr<MediaKeySessionTaiheCallback> context(static_cast<MediaKeySessionTaiheCallback*>(event),
            [](MediaKeySessionTaiheCallback* ptr) {
            delete ptr;
        });
        DRM_TAIHE_CHECK_AND_RETURN_VOID_LOG(event != nullptr, "event is nullptr");
        do {
            std::shared_ptr<AutoRef> eventInfoRef = event->callback;
            DRM_TAIHE_CHECK_AND_RETURN_VOID_LOG(eventInfoRef != nullptr, "eventInfoRef is nullptr");
            DRM_TAIHE_CHECK_AND_RETURN_VOID_LOG(eventInfoRef->callbackRef_ != nullptr, "callbackRef_ is nullptr");
            if (event->callbackName == "keysChange") {
                std::shared_ptr<callback<void(array_view<KeysInfo>, bool)>>
                    cacheCallback = std::reinterpret_pointer_cast<callback<void(
                        array_view<KeysInfo>, bool)>>(eventInfoRef->callbackRef_);
                std::vector<KeysInfo> keysInfoVec;
                bool hasNewGoodLicense;
                SetDrmKeysChangeEventInfo(event->keysChangeParame, keysInfoVec, hasNewGoodLicense);

                array_view<KeysInfo> keysInfoView(keysInfoVec.data(), keysInfoVec.size());
                (*cacheCallback)(keysInfoView, hasNewGoodLicense);
            } else {
                std::shared_ptr<callback<void(EventInfo const&)>>
                    cacheCallback = std::reinterpret_pointer_cast<callback<void
                        (EventInfo const&)>>(eventInfoRef->callbackRef_);
                EventInfo eventInfo = GetEventInfo(event->eventParame);
                (*cacheCallback)(eventInfo);
            }
        } while (0);
    };
    bool ret = mainHandler_->PostTask(task, "OnEventInfo", 0, OHOS::AppExecFwk::EventQueue::Priority::IMMEDIATE, {});
    if (!ret) {
        DRM_ERR_LOG("Failed to PostTask!");
    }
    taiheCb.release();
}

void MediaKeySessionCallbackTaihe::SetDrmKeysChangeEventInfo(DrmKeysChangeEventParame &eventParame,
    std::vector<KeysInfo> &statusTableVec, bool &hasNewGoodLicense)
{
    SetValueMap(eventParame.statusTable, statusTableVec);
    hasNewGoodLicense = eventParame.hasNewGoodLicense;
}

void MediaKeySessionCallbackTaihe::SetValueMap(
    const std::map<std::vector<uint8_t>, OHOS::DrmStandard::MediaKeySessionKeyStatus> &statusTable,
    std::vector<KeysInfo> &statusTableVec)
{
    for (const auto &itemTmp : statusTable) {
        std::vector<uint8_t> keyId = itemTmp.first;
        array<uint8_t> infoArr(keyId);
        std::string value;
        switch (itemTmp.second) {
            case OHOS::DrmStandard::MediaKeySessionKeyStatus::MEDIA_KEY_SESSION_KEY_STATUS_USABLE:
                value = "USABLE";
                break;
            case OHOS::DrmStandard::MediaKeySessionKeyStatus::MEDIA_KEY_SESSION_KEY_STATUS_EXPIRED:
                value = "EXPIRED";
                break;
            case OHOS::DrmStandard::MediaKeySessionKeyStatus::MEDIA_KEY_SESSION_KEY_STATUS_OUTPUT_NOT_ALLOWED:
                value = "OUTPUT_NOT_ALLOWED";
                break;
            case OHOS::DrmStandard::MediaKeySessionKeyStatus::MEDIA_KEY_SESSION_KEY_STATUS_PENDING:
                value = "PENDING";
                break;
            case OHOS::DrmStandard::MediaKeySessionKeyStatus::MEDIA_KEY_SESSION_KEY_STATUS_INTERNAL_ERROR:
                value = "INTERNAL_ERROR";
                break;
            case OHOS::DrmStandard::MediaKeySessionKeyStatus::MEDIA_KEY_SESSION_KEY_STATUS_USABLE_IN_FUTURE:
                value = "USABLE_IN_FUTURE";
                break;
            default:
                value = "Fault Status";
                break;
        }
        KeysInfo info{
            infoArr,
            value
        };
        statusTableVec.push_back(info);
    }
}
} // namespace DrmStandard
} // namespace ANI