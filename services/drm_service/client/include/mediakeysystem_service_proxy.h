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

#ifndef OHOS_DRM_MEDIA_KEY_SYSTEM_SERVICE_PROXY_H
#define OHOS_DRM_MEDIA_KEY_SYSTEM_SERVICE_PROXY_H

#include <hilog/log.h>
#include "iremote_proxy.h"
#include "i_mediakeysystem_service.h"

namespace OHOS {
namespace DrmStandard {
class MediaKeySystemServiceProxy : public IRemoteProxy<IMediaKeySystemService> {
public:
    explicit MediaKeySystemServiceProxy(const sptr<IRemoteObject> &impl);
    virtual ~MediaKeySystemServiceProxy() = default;
    int32_t Release() override;
    int32_t GenerateKeySystemRequest(IMediaKeySystemService::RequestType type, std::vector<uint8_t> &request,
        std::string &defaultUrl) override;
    int32_t ProcessKeySystemResponse(IMediaKeySystemService::RequestType type,
        const std::vector<uint8_t> &response) override;
    int32_t SetConfiguration(IMediaKeySystemService::ConfigType type, std::string &propertyName,
        std::string &value) override;
    int32_t GetConfiguration(IMediaKeySystemService::ConfigType configType, std::string &propertyName,
        std::string &value) override;
    int32_t CreateKeySession(IKeySessionService::SecurityLevel securityLevel,
        sptr<IKeySessionService> &keySessionProxy) override;
    int32_t GetMetric(std::vector<IMediaKeySystemService::KeyValue> &infoMap) override;
    int32_t GetSecurityLevel(IKeySessionService::SecurityLevel *securityLevel) override;
private:
    static inline BrokerDelegator<MediaKeySystemServiceProxy> delegator_;
};
} // DrmStandard
} // OHOS

#endif // OHOS_DRM_MEDIA_KEY_SYSTEM_SERVICE_PROXY_H
