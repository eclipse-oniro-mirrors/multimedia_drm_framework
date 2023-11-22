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

#ifndef OHOS_DRM_I_MEDIA_KEY_SYSTEM_SERVICE_H
#define OHOS_DRM_I_MEDIA_KEY_SYSTEM_SERVICE_H

#include "i_keysession_service.h"
#include "ipc_types.h"
#include "iremote_broker.h"
#include "iremote_proxy.h"
#include "iremote_stub.h"

namespace OHOS {
namespace DrmStandard {
class IMediaKeySystemService : public IRemoteBroker {
public:
    enum HdcpLevel {
        HDCP_UNKNOWN,
        HDCP_NONE,
        HDCP_V1,
        HDCP_V2,
        HDCP_V2_1,
        HDCP_V2_2,
        HDCP_V2_3,
        HDCP_NO_OUTPUT = 0x7fff,
    };

    enum AdcpLevel {
        ADCP_UNKNOWN = 0,
        ADCP_V1_L1,
        ADCP_V1_L2,
        ADCP_V1_L3,
        ADCP_NO_OUTPUT = 0x7fff,
    };

    enum CertificateStatus {
        CERT_STATUS_PROVISIONED = 0,
        CERT_STATUS_NOT_PROVISIONED = 1,
        CERT_STATUS_EXPIRED = 3,
        CERT_STATUS_INVALID = 4,
        CERT_STATUS_GET_FAILED = 5,
    };

    struct MetircKeyValue {
        std::string name;
        std::string value;
    };

    virtual ~IMediaKeySystemService() = default;
    virtual int32_t Release() = 0;
    virtual int32_t GenerateKeySystemRequest(std::vector<uint8_t> &request, std::string &defaultUrl) = 0;
    virtual int32_t ProcessKeySystemResponse(const std::vector<uint8_t> &response) = 0;
    virtual int32_t SetConfigurationString(std::string &configName, std::string &value) = 0;
    virtual int32_t GetConfigurationString(std::string &configName, std::string &value) = 0;
    virtual int32_t SetConfigurationByteArray(std::string &configName, std::vector<uint8_t> &value) = 0;
    virtual int32_t GetConfigurationByteArray(std::string &configName, std::vector<uint8_t> &value) = 0;
    virtual int32_t CreateMediaKeySession(IMediaKeySessionService::SecurityLevel securityLevel,
        sptr<IMediaKeySessionService> &keySessionProxy) = 0;
    virtual int32_t GetMetrics(std::vector<IMediaKeySystemService::MetircKeyValue> &metrics) = 0;
    virtual int32_t GetMaxSecurityLevel(IMediaKeySessionService::SecurityLevel *securityLevel) = 0;
    virtual int32_t GetCertificateStatus(IMediaKeySystemService::CertificateStatus *certStatus) = 0;
    virtual int32_t GetOfflineLicenseIds(std::vector<std::vector<uint8_t>> &licenseIds) = 0;
    virtual int32_t GetOfflineLicenseStatus(std::vector<uint8_t> &licenseId,
        IMediaKeySessionService::OfflineLicenseStatus &status) = 0;
    virtual int32_t RemoveOfflineLicense(std::vector<uint8_t> &licenseId) = 0;

    DECLARE_INTERFACE_DESCRIPTOR(u"IMediaKeySystemSystemService");
};
} // DrmStandard
} // OHOS
#endif // OHOS_DRM_I_MEDIA_KEY_SYSTEM_SERVICE_H
