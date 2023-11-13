/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef OHOS_DRM_KEY_SESSION_PROXY_H
#define OHOS_DRM_KEY_SESSION_PROXY_H

#include "iremote_proxy.h"
#include "i_keysession_service.h"

namespace OHOS {
namespace DrmStandard {
class KeySessionServiceProxy : public IRemoteProxy<IKeySessionService> {
public:
    explicit KeySessionServiceProxy(const sptr<IRemoteObject> &impl);
    virtual ~KeySessionServiceProxy() = default;
    int32_t Release() override;
    int32_t CreateMediaDecryptModule(sptr<IMediaDecryptModuleService> &decryptModule) override;
    int32_t GenerateLicenseRequest(IKeySessionService::DrmInfo &drmInfo,
        IKeySessionService::LicenseInfo &licenseInfo) override;
    int32_t ProcessLicenseResponse(std::vector<uint8_t> &keyId, std::vector<uint8_t> &licenseResponse) override;
    int32_t GenerateOfflineReleaseRequest(std::vector<uint8_t> &keyId, std::vector<uint8_t> &releaseRequest) override;
    int32_t ProcessOfflineReleaseResponse(std::vector<uint8_t> &keyId, std::vector<uint8_t> &releaseReponse) override;
    int32_t CheckLicenseStatus(std::vector<KeyValue> &infoMap) override;
    int32_t RestoreOfflineKeys(std::vector<uint8_t> &keyId) override;
    int32_t RemoveOfflineKeys(std::vector<uint8_t> &keyId) override;
    int32_t GetOfflineKeyIds(std::vector<std::vector<uint8_t>> &keyIds) override;
    int32_t RemoveLicenses() override;
    int32_t GetOfflineKeyState(std::vector<uint8_t> &keyId, IKeySessionService::OfflineKeyState &state) override;
    int32_t SetKeySessionServiceCallback(sptr<IKeySessionServiceCallback> &callback) override;
private:
    static inline BrokerDelegator<KeySessionServiceProxy> delegator_;
};
} // DrmStandard
} // OHOS

#endif