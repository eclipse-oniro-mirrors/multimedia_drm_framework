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

#include "mediakeysystem_service_stub.h"
#include "key_session_service.h"
#include "drm_host_manager.h"
#include "drm_log.h"
#include "drm_error_code.h"
#include "iremote_stub.h"
#include "system_ability.h"

#include "safe_map.h"
#include <iostream>
#include <refbase.h>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include <set>
#include <unordered_set>

namespace OHOS {
namespace DrmStandard {
class IMediaKeySystemServiceOperatorsCallback;
class MediaKeySystemService : public MediaKeySystemServiceStub, public DrmHostManager::StatusCallback,
                              public IKeySessionServiceOperatorsCallback {
public:
    explicit MediaKeySystemService();
    ~MediaKeySystemService() override;
    int32_t Release() override;
    int32_t SetMediaKeySystemServiceOperatorsCallback(wptr<IMediaKeySystemServiceOperatorsCallback> callback);
    int32_t CloseMediaKeySystemServiceByCallback();
    
    int32_t GenerateKeySystemRequest(IMediaKeySystemService::RequestType type, std::vector<uint8_t> &request, std::string &defaultUrl) override;
    int32_t ProcessKeySystemResponse(IMediaKeySystemService::RequestType type, const std::vector<uint8_t> &response) override;
    int32_t SetConfiguration(IMediaKeySystemService::ConfigType type, std::string &propertyName, std::string &value) override;
    int32_t GetConfiguration(IMediaKeySystemService::ConfigType type, std::string &propertyName, std::string &value) override;       
    int32_t CreateKeySession(IKeySessionService::SecurityLevel securityLevel, sptr<IKeySessionService> &keySessionProxy) override;
    //IMediaKeySystemServiceOperatorsCallback
    int32_t CloseKeySessionService(sptr<KeySessionService> sessionService) override;
    int32_t GetMetric(std::vector<IMediaKeySystemService::KeyValue> &infoMap) override;
    int32_t GetSecurityLevel(IKeySessionService::SecurityLevel *securityLevel) override;

private:
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