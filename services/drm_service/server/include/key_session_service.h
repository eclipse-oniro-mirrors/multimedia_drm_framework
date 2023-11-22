/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

#ifndef OHOS_DRM_KEY_SESSION_SERVICE_H
#define OHOS_DRM_KEY_SESSION_SERVICE_H

#include <iostream>
#include <refbase.h>
#include "drm_log.h"
#include "drm_napi_utils.h"
#include "drm_host_manager.h"
#include "key_session_service_stub.h"

#include "media_key_session_proxy.h"
#include "media_decrypt_module_proxy.h"
#include "media_key_system_factory_proxy.h"
#include "media_key_system_proxy.h"
#include "media_decrypt_module_service.h"

namespace OHOS {
namespace DrmStandard {
using namespace OHOS::HDI::Drm::V1_0;
using namespace OHOS::HDI;

class IMediaKeySessionServiceOperatorsCallback;

class MediaKeySessionService : public MediaKeySessionServiceStub {
public:
    explicit MediaKeySessionService(sptr<OHOS::HDI::Drm::V1_0::IMediaKeySession> hdiMediaKeySession);
    ~MediaKeySessionService();
    int32_t Release() override;
    int32_t CreateMediaDecryptModule(sptr<IMediaDecryptModuleService> &decryptModule) override;
    int32_t SetMediaKeySessionServiceOperatorsCallback(wptr<IMediaKeySessionServiceOperatorsCallback> callback);
    int32_t CloseMediaKeySessionServiceByCallback();
    int32_t GenerateLicenseRequest(IMediaKeySessionService::LicenseRequestInfo &licenseRequestInfo,
        IMediaKeySessionService::LicenseRequest &licenseRequest) override;
    int32_t ProcessLicenseResponse(std::vector<uint8_t> &licenseId, std::vector<uint8_t> &licenseResponse) override;
    int32_t GenerateOfflineReleaseRequest(std::vector<uint8_t> &licenseId,
        std::vector<uint8_t> &releaseRequest) override;
    int32_t ProcessOfflineReleaseResponse(std::vector<uint8_t> &licenseId,
        std::vector<uint8_t> &releaseResponse) override;
    int32_t CheckLicenseStatus(std::vector<LicenseStatus> &licenseStatus) override;
    int32_t RestoreOfflineLicense(std::vector<uint8_t> &licenseId) override;

    int32_t RemoveLicense() override;

    int32_t GetSecurityLevel(IMediaKeySessionService::SecurityLevel *securityLevel) override;
    int32_t RequireSecureDecoderModule(std::string &mimeType, bool *status) override;

    int32_t SetMediaKeySessionServiceCallback(sptr<IMediaKeySessionServiceCallback> &callback) override;
    void OnMediaKeySessionKeyExpiredStatus(const KeyStatus status);
    void OnMediaKeySessionReclaimed(const SessionStatus status);

private:
    std::mutex sessionMutex_;
    sptr<IMediaKeySessionServiceCallback> callback_;
    wptr<IMediaKeySessionServiceOperatorsCallback> sessionOperatorsCallback_;
    sptr<OHOS::HDI::Drm::V1_0::IMediaKeySession> hdiMediaKeySession_;
};

class IMediaKeySessionServiceOperatorsCallback : public virtual RefBase {
public:
    IMediaKeySessionServiceOperatorsCallback() = default;
    virtual ~IMediaKeySessionServiceOperatorsCallback() = default;
    virtual int32_t CloseMediaKeySessionService(sptr<MediaKeySessionService> sessionService) = 0;
};
} // DrmStandard
} // OHOS

#endif // OHOS_DRM_KEY_SESSION_SERVICE_H