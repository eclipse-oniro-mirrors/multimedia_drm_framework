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

#ifndef NATIVE_DRM_OBJECT_H
#define NATIVE_DRM_OBJECT_H

#include <stdint.h>
#include <stdio.h>
#include "native_drm_err.h"
#include "native_drm_base.h"
#include "native_drm_common.h"

#include "key_session_impl.h"
#include "media_key_system_impl.h"
#include "media_key_system_factory_impl.h"

namespace OHOS {
namespace DrmStandard {
class MediaKeySystemCallbackCapi;
class MediaKeySessionCallbackCapi;

struct MediaKeySystemObject : public OH_MediaKeySystem {
    explicit MediaKeySystemObject(const OHOS::sptr<OHOS::DrmStandard::MediaKeySystemImpl> &impl) : systemImpl_(impl) {}
    ~MediaKeySystemObject() = default;

    const OHOS::sptr<OHOS::DrmStandard::MediaKeySystemImpl> systemImpl_ = nullptr;
    OHOS::sptr<OHOS::DrmStandard::MediaKeySystemCallbackCapi> systemCallback_ = nullptr;
};

struct MediaKeySessionObject : public OH_MediaKeySession {
    explicit MediaKeySessionObject(const OHOS::sptr<OHOS::DrmStandard::MediaKeySessionImpl> &impl) : sessionImpl_(impl)
    {}
    ~MediaKeySessionObject() = default;

    const OHOS::sptr<OHOS::DrmStandard::MediaKeySessionImpl> sessionImpl_ = nullptr;
    OHOS::sptr<MediaKeySessionCallbackCapi> sessionCallback_ = nullptr;
};

class MediaKeySystemCallbackCapi : public MediaKeySystemImplCallback {
public:
    MediaKeySystemCallbackCapi()
    {
        DRM_INFO_LOG("MediaKeySystemCallbackCapi.");
        InitEventMap();
    }

    void InitEventMap()
    {
        eventMap_.insert({ static_cast<std::string>(MediaKeySystemEvent::EVENT_STR_PROVISION_REQUIRED),
            LISTENER_PROVISION_REQUIRED });
        eventMap_.insert(
            { static_cast<std::string>(MediaKeySystemEvent::EVENT_STR_SESSION_LOST), LISTENER_SESSION_LOST });
    }

    virtual ~MediaKeySystemCallbackCapi()
    {
        DRM_INFO_LOG("~MediaKeySystemCallbackCapi.");
    }

    void SetCallbackReference(OH_MediaKeySystemCallback callback)
    {
        DRM_INFO_LOG("MediaKeySystemCallbackCapi SetCallbackReference.");
        std::lock_guard<std::mutex> lock(mutex_);
        callback_ = callback;
    }

    void ClearCallbackReference()
    {
        DRM_INFO_LOG("MediaKeySystemCallbackCapi ClearCallbackReference.");
        std::lock_guard<std::mutex> lock(mutex_);
        callback_ = nullptr;
    }

    void SendEvent(const std::string event, uint32_t extra, const std::vector<uint8_t> data) override
    {
        DRM_INFO_LOG("MediaKeySystemCallbackCapi SendEvent.");
        std::lock_guard<std::mutex> lock(mutex_);
        DRM_CHECK_AND_RETURN_LOG(callback_ != nullptr, "Callback is nullptr");
        if (eventMap_.find(event) == eventMap_.end()) {
            DRM_ERR_LOG("MediaKeySystemCallbackCapi SendEvent failed, not find this event type.");
            return;
        }
        OH_DRM_Uint8CharBufferPair extraData;
        extraData.uintData.bufferLen = data.size();
        extraData.uintData.buffer = const_cast<uint8_t *>(data.data());
        callback_(eventMap_[event], &extraData);
    }

private:
    OH_MediaKeySystemCallback callback_ = nullptr;
    std::mutex mutex_;
    std::unordered_map<std::string, OH_DRM_ListenerType> eventMap_;
};

class MediaKeySessionCallbackCapi : public MediaKeySessionImplCallback {
public:
    MediaKeySessionCallbackCapi()
    {
        DRM_INFO_LOG("MediaKeySessionCallbackCapi.");
        InitEventMap();
    }

    void InitEventMap()
    {
        eventMap_.insert({ static_cast<std::string>(MediaKeySessionEvent::EVENT_STR_EXPIRATION_UPDATED),
            LISTENER_EXPIRATION_UPDATE });
        eventMap_.insert(
            { static_cast<std::string>(MediaKeySessionEvent::EVENT_STR_KEY_EXPIRED), LISTENER_KEY_EXPIRED });
        eventMap_.insert(
            { static_cast<std::string>(MediaKeySessionEvent::EVENT_STR_KEY_NEEDED), LISTENER_KEY_REQUIRED });
        eventMap_.insert(
            { static_cast<std::string>(MediaKeySessionEvent::EVENT_STR_VENDOR_DEFINED), LISTENER_VENDOR_DEFINED });
    }

    virtual ~MediaKeySessionCallbackCapi()
    {
        DRM_INFO_LOG("~MediaKeySessionCallbackCapi.");
    }

    void SetCallbackReference(OH_MediaKeySessionCallback callback)
    {
        DRM_INFO_LOG("MediaKeySessionCallbackCapi SetCallbackReference.");
        std::lock_guard<std::mutex> lock(mutex_);
        callback_ = callback;
    }

    void ClearCallbackReference()
    {
        DRM_INFO_LOG("MediaKeySessionCallbackCapi ClearCallbackReference.");
        std::lock_guard<std::mutex> lock(mutex_);
        callback_ = {};
    }

    void SendEvent(const std::string event, uint32_t extra, const std::vector<uint8_t> data) override
    {
        DRM_INFO_LOG("MediaKeySessionCallbackCapi SendEvent.");
        std::lock_guard<std::mutex> lock(mutex_);
        DRM_CHECK_AND_RETURN_LOG(callback_.eventCallback != nullptr, "Callback is nullptr");
        if (eventMap_.find(event) == eventMap_.end()) {
            DRM_ERR_LOG("MediaKeySystemCallbackCapi SendEvent failed, not find this event type.");
            return;
        }
        OH_DRM_Uint8CharBufferPair extraData;
        extraData.uintData.bufferLen = data.size();
        extraData.uintData.buffer = const_cast<uint8_t *>(data.data());
        callback_.eventCallback(eventMap_[event], &extraData);
    }

    void SendEventKeyChanged(std::map<std::vector<uint8_t>, MediaKeySessionKeyStatus> statusTable,
        bool hasNewGoodLicense) override
    {
        DRM_INFO_LOG("MediaKeySessionCallbackCapi SendEventKeyChanged.");
        OH_DRM_KeysInfo info;
        info.keysCount = statusTable.size();
        info.keysInfo = (OH_DRM_Uint8CharBufferPair *)malloc(sizeof(OH_DRM_Uint8CharBufferPair) * info.keysCount);
        if (info.keysInfo == nullptr) {
            DRM_ERR_LOG("MediaKeySystemCallbackCapi SendEvent failed, malloc failed.");
            return;
        }
        uint32_t index = 0;
        for (auto item : statusTable) {
            uint32_t bufferSize = item.first.size();
            int32_t ret = memcpy_s(info.keysInfo[index].uintData.buffer, bufferSize, item.first.data(), bufferSize);
            if (ret != 0) {
                DRM_ERR_LOG("MediaKeySystemCallbackCapi SendEvent failed, memcpy failed.");
                free(info.keysInfo);
                info.keysInfo = nullptr;
                return;
            }
            info.keysInfo[index].uintData.bufferLen = item.first.size();
            *info.keysInfo[index].charData.buffer = static_cast<char>(item.second);
            info.keysInfo[index].charData.bufferLen = sizeof(char);
            index++;
        }
        std::lock_guard<std::mutex> lock(mutex_);
        DRM_CHECK_AND_RETURN_LOG(callback_.keyChangeCallback != nullptr, "keyChangeCallback is nullptr");
        callback_.keyChangeCallback(&info, hasNewGoodLicense);
        free(info.keysInfo);
        info.keysInfo = nullptr;
    }

private:
    struct OH_MediaKeySessionCallback callback_ = {};
    std::mutex mutex_;
    std::unordered_map<std::string, OH_DRM_ListenerType> eventMap_;
};
} // DrmStandard
} // OHOS

#endif // NATIVE_DRM_OBJECT_H