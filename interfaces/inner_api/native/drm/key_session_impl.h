/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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
#ifndef OHOS_DRM_KEY_SESSION_IMPL_H_
#define OHOS_DRM_KEY_SESSION_IMPL_H_

#include "media_decrypt_module_impl.h"
#include "i_keysession_service.h"
#include "i_keysession_service_callback.h"
#include "key_session_service_callback_stub.h"
#include "drm_napi_utils.h"
#include "ipc_skeleton.h"
#include "iservice_registry.h"
#include "nocopyable.h"
#include <cstring>

namespace OHOS {
namespace DrmStandard {
class KeySessionImplCallback : public RefBase {
public:
    KeySessionImplCallback() = default;
    virtual ~KeySessionImplCallback() = default;
    virtual void OnKeySessionKeyExpired(const std::string eventType, const KeyStatus status) = 0;
    virtual void OnKeySessionReclaimed(const std::string eventType, const SessionStatus status) = 0;
};

class KeySessionImpl : public RefBase {
public:
    explicit KeySessionImpl(sptr<IKeySessionService> &keySession);
    ~KeySessionImpl();
    int32_t Release();
    int32_t Init();
    sptr<MediaDecryptModuleImpl> GetDecryptModule();
    int32_t GenerateLicenseRequest(IKeySessionService::DrmInfo &drmInfo,
        IKeySessionService::LicenseInfo &licenseInfo);
    int32_t ProcessLicenseResponse(std::vector<uint8_t> &keyId, std::vector<uint8_t> &licenseResponse);
    int32_t GenerateOfflineReleaseRequest(std::vector<uint8_t> &keyId, std::vector<uint8_t> &releaseRequest);
    int32_t ProcessOfflineReleaseResponse(std::vector<uint8_t> &keyId, std::vector<uint8_t> &releaseReponse);
    int32_t CheckLicenseStatus(std::vector<IKeySessionService::KeyValue> &infoMap);
    int32_t RestoreOfflineKeys(std::vector<uint8_t> &keyId);
    int32_t RemoveOfflineKeys(std::vector<uint8_t> &keyId);
    int32_t GetOfflineKeyIds(std::vector<std::vector<uint8_t>> &keyIds);
    int32_t RemoveLicenses();
    int32_t GetOfflineKeyState(std::vector<uint8_t> &keyId, IKeySessionService::OfflineKeyState &state);
    sptr<IKeySessionService> GetKeySessionServiceProxy();
    sptr<KeySessionImplCallback> GetKeySessionApplicationCallback();
    int32_t SetKeySessionCallback(const sptr<KeySessionImplCallback> &callback);
    int32_t SetKeySessionServiceCallback(sptr<IKeySessionServiceCallback> &callback);
private:
    sptr<KeySessionImplCallback> keySessionNapiCallback_;
    sptr<IKeySessionServiceCallback> keySessionServiceCallback_;
    sptr<OHOS::DrmStandard::IKeySessionService> keySessionServiceProxy_;
    sptr<MediaDecryptModuleImpl> mediaDecryptModuleImpl_;
    std::mutex mutex_;
};

class KeySessionStatusCallback : public KeySessionServiceCallbackStub {
public:
    KeySessionStatusCallback() : keySessionImpl_(nullptr) {};
    explicit KeySessionStatusCallback(const sptr<KeySessionImpl> &KeySessionImpl)
        : keySessionImpl_(KeySessionImpl) {}

    ~KeySessionStatusCallback() {
        keySessionImpl_ = nullptr;
    }

    int32_t OnKeySessionKeyExpired(const KeyStatus status) override;
    int32_t OnKeySessionReclaimed(const SessionStatus status) override;

private:
    sptr<KeySessionImpl> keySessionImpl_;
};

} // DrmStandard
} // OHOS


#endif