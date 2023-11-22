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

#ifndef OHOS_DRM_MEDIA_KEY_SYSTEM_SERVICE_H
#define OHOS_DRM_MEDIA_KEY_SYSTEM_SERVICE_H

#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>
#include <refbase.h>
#include "safe_map.h"
#include "mediakeysystem_service_stub.h"
#include "key_session_service.h"
#include "drm_host_manager.h"
#include "drm_log.h"
#include "drm_error_code.h"
#include "iremote_stub.h"
#include "system_ability.h"

#include "media_key_session_proxy.h"
#include "media_decrypt_module_proxy.h"
#include "media_key_system_factory_proxy.h"
#include "media_key_system_proxy.h"

namespace OHOS {
namespace DrmStandard {
using namespace OHOS::HDI::Drm::V1_0;
using namespace OHOS::HDI;
class IMediaKeySystemServiceOperatorsCallback;
class MediaKeySystemService : public MediaKeySystemServiceStub,
    public DrmHostManager::StatusCallback,
    public IMediaKeySessionServiceOperatorsCallback {
public:
    explicit MediaKeySystemService(sptr<OHOS::HDI::Drm::V1_0::IMediaKeySystem> hdiKeySystem);
    ~MediaKeySystemService() override;
    int32_t Release() override;
    int32_t SetMediaKeySystemServiceOperatorsCallback(wptr<IMediaKeySystemServiceOperatorsCallback> callback);
    int32_t CloseMediaKeySystemServiceByCallback();
    int32_t SetConfigurationString(std::string &configName, std::string &value) override;
    int32_t GetConfigurationString(std::string &configName, std::string &value) override;
    int32_t SetConfigurationByteArray(std::string &configName, std::vector<uint8_t> &value) override;
    int32_t GetConfigurationByteArray(std::string &configName, std::vector<uint8_t> &value) override;
    int32_t GetMaxSecurityLevel(IMediaKeySessionService::SecurityLevel *securityLevel) override;
    int32_t CreateMediaKeySession(IMediaKeySessionService::SecurityLevel securityLevel,
        sptr<IMediaKeySessionService> &keySessionProxy) override;
    int32_t GenerateKeySystemRequest(std::vector<uint8_t> &request, std::string &defaultUrl) override;
    int32_t ProcessKeySystemResponse(const std::vector<uint8_t> &response) override;
    int32_t GetCertificateStatus(IMediaKeySystemService::CertificateStatus *certStatus) override;

    int32_t GetOfflineLicenseIds(std::vector<std::vector<uint8_t>> &licenseIds) override;
    int32_t GetOfflineLicenseStatus(std::vector<uint8_t> &licenseId,
        IMediaKeySessionService::OfflineLicenseStatus &status) override;
    int32_t RemoveOfflineLicense(std::vector<uint8_t> &licenseId) override;

    int32_t CloseMediaKeySessionService(sptr<MediaKeySessionService> sessionService) override;
    int32_t GetMetrics(std::vector<IMediaKeySystemService::MetircKeyValue> &metrics) override;

private:
    std::mutex mutex_;
    sptr<DrmHostManager> drmHostManager_;
    wptr<IMediaKeySystemServiceOperatorsCallback> keySystemOperatoersCallback_;
    SafeMap<int32_t, std::set<sptr<MediaKeySessionService>>> sessionsForPid_;
    sptr<OHOS::HDI::Drm::V1_0::IMediaKeySystem> hdiKeySystem_;
};

class IMediaKeySystemServiceOperatorsCallback : public virtual RefBase {
public:
    IMediaKeySystemServiceOperatorsCallback() = default;
    virtual ~IMediaKeySystemServiceOperatorsCallback() = default;
    virtual int32_t CloseMediaKeySystemService(sptr<MediaKeySystemService> mediaKeySystemService) = 0;
};
} // DrmStandard
} // OHOS

#endif // OHOS_DRM_MEDIA_KEY_SYSTEM_SERVICE_H