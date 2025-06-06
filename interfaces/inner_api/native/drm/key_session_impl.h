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
#ifndef OHOS_DRM_KEY_SESSION_IMPL_H_
#define OHOS_DRM_KEY_SESSION_IMPL_H_

#include <thread>
#include <queue>
#include <cstring>
#include <map>
#include <unordered_map>
#include "nocopyable.h"
#include "ipc_skeleton.h"
#include "i_keysession_service.h"
#include "i_keysession_service_callback.h"
#include "drm_listener_stub.h"
#include "key_session_service_callback_stub.h"
#include "drm_death_recipient.h"

namespace OHOS {
namespace DrmStandard {
namespace MediaKeySessionEvent {
const std::string EVENT_STR_KEY_NEEDED = "keyRequired";
const std::string EVENT_STR_KEY_EXPIRED = "keyExpired";
const std::string EVENT_STR_EXPIRATION_UPDATED = "expirationUpdate";
const std::string EVENT_STR_KEY_CHANGED = "keysChange";
const std::string EVENT_STR_VENDOR_DEFINED = "vendorDefined";
}

class MediaKeySessionImplCallback : public RefBase {
public:
    MediaKeySessionImplCallback() = default;
    virtual ~MediaKeySessionImplCallback() = default;
    virtual void SendEvent(const std::string &event, int32_t extra, const std::vector<uint8_t> &data) = 0;
    virtual void SendEventKeyChanged(std::map<std::vector<uint8_t>, MediaKeySessionKeyStatus> statusTable,
        bool hasNewGoodLicense) = 0;
};

class MediaKeySessionServiceCallback;

class MediaKeySessionImpl : public RefBase {
public:
    explicit MediaKeySessionImpl(sptr<IMediaKeySessionService> &keySession);
    ~MediaKeySessionImpl();
    int32_t Release();
    int32_t Init();

    int32_t GenerateMediaKeyRequest(IMediaKeySessionService::MediaKeyRequestInfo &licenseRequestInfo,
        IMediaKeySessionService::MediaKeyRequest &licenseRequest);
    int32_t ProcessMediaKeyResponse(std::vector<uint8_t> &licenseId, std::vector<uint8_t> &licenseResponse);
    int32_t GenerateOfflineReleaseRequest(std::vector<uint8_t> &licenseId, std::vector<uint8_t> &releaseRequest);
    int32_t ProcessOfflineReleaseResponse(std::vector<uint8_t> &licenseId, std::vector<uint8_t> &releaseReponse);
    int32_t CheckMediaKeyStatus(std::map<std::string, std::string> &licenseStatus);
    int32_t RestoreOfflineMediaKeys(std::vector<uint8_t> &licenseId);

    int32_t ClearMediaKeys();

    int32_t GetContentProtectionLevel(IMediaKeySessionService::ContentProtectionLevel *securityLevel);

    sptr<IMediaKeySessionService> GetMediaKeySessionServiceProxy();
    sptr<MediaKeySessionImplCallback> GetApplicationCallback();
    int32_t SetCallback(const sptr<MediaKeySessionImplCallback> &callback);

    int32_t RequireSecureDecoderModule(std::string &mimeType, bool *status);

private:
    void MediaKeySessionServerDied(pid_t pid);
    sptr<MediaKeySessionImplCallback> keySessionApplicationCallback_;
    sptr<MediaKeySessionServiceCallback> keySessionServiceCallback_;
    sptr<OHOS::DrmStandard::IMediaKeySessionService> keySessionServiceProxy_;
    std::recursive_mutex mutex_;
    sptr<DrmDeathRecipient> deathRecipient_ = nullptr;
};

struct KeySessionEventMessage {
    enum MessageType { EventKeyChanged = 0, EventKey };
    MessageType messageType;
    DrmEventType event;
    int32_t extra;
    std::vector<uint8_t> data;
    std::map<std::vector<uint8_t>, MediaKeySessionKeyStatus> statusTable;
    bool hasNewGoodLicense;
};

class MediaKeySessionServiceCallback : public MediaKeySessionServiceCallbackStub {
public:
    MediaKeySessionServiceCallback() : keySessionImpl_(nullptr)
    {
        InitEventMap();
    };
    explicit MediaKeySessionServiceCallback(MediaKeySessionImpl *keySessionImpl)
        : keySessionImpl_(keySessionImpl)
    {
        InitEventMap();
    }

    ~MediaKeySessionServiceCallback()
    {
        Release();
    }

    void InitEventMap();
    std::string GetEventName(DrmEventType event);
    int32_t SendEvent(DrmEventType event, int32_t extra, const std::vector<uint8_t> &data) override;
    int32_t SendEventKeyChanged(std::map<std::vector<uint8_t>, MediaKeySessionKeyStatus> statusTable,
        bool hasNewGoodLicense) override;
    void Release();
    void Init();
private:
    int32_t SendEventHandler(DrmEventType event, int32_t extra, const std::vector<uint8_t> &data);
    int32_t SendEventKeyChangedHandler(
        const std::map<std::vector<uint8_t>, MediaKeySessionKeyStatus> &statusTable, bool hasNewGoodLicense);
    std::recursive_mutex mutex_;
    MediaKeySessionImpl *keySessionImpl_;
    std::unordered_map<int32_t, std::string> eventMap_;
    bool serviceThreadRunning = false;
    void ProcessEventMessage();
    std::thread eventQueueThread;
    std::queue<KeySessionEventMessage> eventQueue;
    std::mutex queueMutex;
    std::condition_variable cv;
};
} // DrmStandard
} // OHOS


#endif