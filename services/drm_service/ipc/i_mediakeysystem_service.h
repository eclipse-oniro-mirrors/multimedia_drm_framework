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
    enum RequestType {
        REQUEST_TYPE_UNKNOWN = 0,
        REQUEST_TYPE_INITIAL = 1,
        REQUEST_TYPE_RENEWAL = 2,
        REQUEST_TYPE_RELEASE = 3,
        REQUEST_TYPE_NONE = 4,
        REQUEST_TYPE_UPDATE = 5,
        REQUEST_TYPE_DOWNLOADCERT = 6,
    };

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

    enum ConfigType {
        CONFIGTYPE_DEVICEPROPERTY = 0,
        CONFIGTYPE_KEYSESSION = 1,
        CONFIGTYPE_OUTPUTPROTECTTYPE = 2,
    };

    struct KeyValue {
        std::string name;
        std::string value;
    };

    virtual ~IMediaKeySystemService() = default;
    virtual int32_t Release() = 0;
    virtual int32_t GenerateKeySystemRequest(RequestType type, std::vector<uint8_t> &request,
        std::string &defaultUrl) = 0;
    virtual int32_t ProcessKeySystemResponse(RequestType type, const std::vector<uint8_t> &response) = 0;
    virtual int32_t SetConfiguration(ConfigType type, std::string &propertyName, std::string &value) = 0;
    virtual int32_t GetConfiguration(ConfigType type, std::string &propertyName, std::string &value) = 0;
    virtual int32_t CreateKeySession(IKeySessionService::SecurityLevel securityLevel,
        sptr<IKeySessionService> &keySessionProxy) = 0;
    virtual int32_t GetMetric(std::vector<IMediaKeySystemService::KeyValue> &infoMap) = 0;
    virtual int32_t GetSecurityLevel(IKeySessionService::SecurityLevel *securityLevel) = 0;

    DECLARE_INTERFACE_DESCRIPTOR(u"IMediaKeySystemSystemService");
};
} // DrmStandard
} // OHOS
#endif // OHOS_DRM_I_MEDIA_KEY_SYSTEM_SERVICE_H
