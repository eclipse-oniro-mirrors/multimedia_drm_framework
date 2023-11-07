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

#ifndef OHOS_DRM_MEDIA_KEY_SYSTEMP_IMPL_H
#define OHOS_DRM_MEDIA_KEY_SYSTEMP_IMPL_H

#include "key_session_impl.h"
#include "system_ability_definition.h"
#include "ipc_skeleton.h"
#include "iservice_registry.h"
#include "nocopyable.h"
#include "i_mediakeysystem_service.h"
#include "drm_death_recipient.h"
#include "drm_log.h"
#include "drm_error_code.h"
#include "drm_napi_utils.h"

namespace OHOS{
namespace DrmStandard{
class MediaKeySystemImpl : public RefBase {
public:
    explicit MediaKeySystemImpl(sptr<IMediaKeySystemService> &mediaKeysystem);
    ~MediaKeySystemImpl();
    int32_t Release();
    int32_t GenerateKeySystemRequest(IMediaKeySystemService::RequestType type, std::vector<uint8_t> &request,
        std::string &defaultUrl);
    int32_t ProcessKeySystemResponse(IMediaKeySystemService::RequestType type, const std::vector<uint8_t> &response);
    int32_t SetConfiguration(IMediaKeySystemService::ConfigType type, std::string &propertyName, std::string &value);
    int32_t GetConfiguration(IMediaKeySystemService::ConfigType configType, std::string &propertyName,
        std::string &value);
    int32_t CreateKeySession(IKeySessionService::SecurityLevel securityLevel, sptr<KeySessionImpl> *keySessionImpl);
    int32_t GetMetric(std::vector<IMediaKeySystemService::KeyValue> &infoMap);
    int32_t GetSecurityLevel(IKeySessionService::SecurityLevel *securityLevel);
private:
    std::mutex mutex_;
    sptr<OHOS::DrmStandard::IMediaKeySystemService> serviceProxy_;
};
} // DrmStandard
} //OHOS
#endif // OHOS_DRM_MEDIA_KEY_SYSTEMP_IMPL_H