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

#include <cstring>
#include "nocopyable.h"
#include "ipc_skeleton.h"
#include "iservice_registry.h"
#include "media_decrypt_module_impl.h"
#include "i_keysession_service.h"
#include "i_keysession_service_callback.h"
#include "key_session_service_callback_stub.h"

namespace OHOS {
namespace DrmStandard {
class MediaKeySessionImplCallback : public RefBase {
public:
    MediaKeySessionImplCallback() = default;
    virtual ~MediaKeySessionImplCallback() = default;
    virtual void OnMediaKeySessionKeyExpired(const std::string eventType, const KeyStatus status) = 0;
    virtual void OnMediaKeySessionReclaimed(const std::string eventType, const SessionStatus status) = 0;
};

class MediaKeySessionImpl : public RefBase {
public:
    explicit MediaKeySessionImpl(sptr<IMediaKeySessionService> &keySession);
    ~MediaKeySessionImpl();
    int32_t Release();
    int32_t Init();

    int32_t GenerateLicenseRequest(IMediaKeySessionService::LicenseRequestInfo &licenseRequestInfo,
        IMediaKeySessionService::LicenseRequest &licenseRequest);
    int32_t ProcessLicenseResponse(std::vector<uint8_t> &licenseId, std::vector<uint8_t> &licenseResponse);
    int32_t GenerateOfflineReleaseRequest(std::vector<uint8_t> &licenseId, std::vector<uint8_t> &releaseRequest);
    int32_t ProcessOfflineReleaseResponse(std::vector<uint8_t> &licenseId, std::vector<uint8_t> &releaseReponse);
    int32_t CheckLicenseStatus(std::map<std::string,
        IMediaKeySessionService::MediaKeySessionKeyStatus>& licenseStatus);
    int32_t RestoreOfflineLicense(std::vector<uint8_t> &licenseId);

    int32_t RemoveLicense();

    sptr<MediaDecryptModuleImpl> GetDecryptModule();
    int32_t GetSecurityLevel(IMediaKeySessionService::SecurityLevel *securityLevel);

    sptr<IMediaKeySessionService> GetMediaKeySessionServiceProxy();
    sptr<MediaKeySessionImplCallback> GetMediaKeySessionApplicationCallback();
    int32_t SetMediaKeySessionCallback(const sptr<MediaKeySessionImplCallback> &callback);
    int32_t SetMediaKeySessionServiceCallback(sptr<IMediaKeySessionServiceCallback> &callback);

    int32_t RequireSecureDecoderModule(std::string &mimeType, bool *status);

private:
    sptr<MediaKeySessionImplCallback> keySessionNapiCallback_;
    sptr<IMediaKeySessionServiceCallback> keySessionServiceCallback_;
    sptr<OHOS::DrmStandard::IMediaKeySessionService> keySessionServiceProxy_;
    sptr<MediaDecryptModuleImpl> mediaDecryptModuleImpl_;
    std::mutex mutex_;
};

class MediaKeySessionStatusCallback : public MediaKeySessionServiceCallbackStub {
public:
    MediaKeySessionStatusCallback() : keySessionImpl_(nullptr) {};
    explicit MediaKeySessionStatusCallback(const sptr<MediaKeySessionImpl> &MediaKeySessionImpl)
        : keySessionImpl_(MediaKeySessionImpl)
    {}

    ~MediaKeySessionStatusCallback()
    {
        keySessionImpl_ = nullptr;
    }

    int32_t OnMediaKeySessionKeyExpired(const KeyStatus status) override;
    int32_t OnMediaKeySessionReclaimed(const SessionStatus status) override;

private:
    sptr<MediaKeySessionImpl> keySessionImpl_;
};
} // DrmStandard
} // OHOS


#endif