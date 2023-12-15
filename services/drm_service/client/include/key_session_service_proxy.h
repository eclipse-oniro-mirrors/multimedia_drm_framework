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

#ifndef OHOS_DRM_KEY_SESSION_PROXY_H
#define OHOS_DRM_KEY_SESSION_PROXY_H

#include "i_keysession_service.h"
#include "iremote_proxy.h"

namespace OHOS {
namespace DrmStandard {
class MediaKeySessionServiceProxy : public IRemoteProxy<IMediaKeySessionService> {
public:
    explicit MediaKeySessionServiceProxy(const sptr<IRemoteObject> &impl);
    virtual ~MediaKeySessionServiceProxy() = default;
    int32_t Release() override;

    int32_t GenerateLicenseRequest(IMediaKeySessionService::LicenseRequestInfo &licenseRequestInfo,
        IMediaKeySessionService::LicenseRequest &licenseRequest) override;
    int32_t ProcessLicenseResponse(std::vector<uint8_t> &licenseId, std::vector<uint8_t> &licenseResponse) override;
    int32_t GenerateOfflineReleaseRequest(std::vector<uint8_t> &licenseId,
        std::vector<uint8_t> &releaseRequest) override;
    int32_t ProcessOfflineReleaseResponse(std::vector<uint8_t> &licenseId,
        std::vector<uint8_t> &releaseReponse) override;
    int32_t CheckLicenseStatus(std::map<std::string, MediaKeySessionKeyStatus> &licenseStatus) override;
    int32_t RestoreOfflineLicense(std::vector<uint8_t> &licenseId) override;
    int32_t RemoveLicense() override;

    int32_t CreateMediaDecryptModule(sptr<IMediaDecryptModuleService> &decryptModule) override;
    int32_t GetSecurityLevel(IMediaKeySessionService::SecurityLevel *securityLevel) override;
    int32_t RequireSecureDecoderModule(std::string &mimeType, bool *status) override;

    int32_t SetCallback(sptr<IMediaKeySessionServiceCallback> &callback) override;

private:
    static inline BrokerDelegator<MediaKeySessionServiceProxy> delegator_;
};
} // DrmStandard
} // OHOS

#endif