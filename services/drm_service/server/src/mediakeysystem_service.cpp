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

#include "mediakeysystem_service.h"
#include "key_session_service.h"
#include <securec.h>
#include <unordered_set>

#include "access_token.h"
#include "drm_napi_utils.h"
#include "iservice_registry.h"
#include "drm_log.h"
#include "system_ability_definition.h"
#include "ipc_skeleton.h"

namespace OHOS{
namespace DrmStandard{

MediaKeySystemService::MediaKeySystemService()
{
    DRM_DEBUG_LOG("~MediaKeySystemService");
}

MediaKeySystemService::~MediaKeySystemService() {
    DRM_DEBUG_LOG("~MediaKeySystemService");
}

int32_t MediaKeySystemService::CloseMediaKeySystemServiceByCallback()
{
    DRM_INFO_LOG("MediaKeySystemService::CloseMediaKeySystemServiceByCallback enter.");
    DRM_INFO_LOG("MediaKeySystemService::CloseMediaKeySystemServiceByCallback exit.");
    return DRM_OK;
}

int32_t MediaKeySystemService::Release()
{
    DRM_INFO_LOG("MediaKeySystemService::Release enter.");
    DRM_INFO_LOG("MediaKeySystemService::Release exit.");
    return DRM_OK;
}

int32_t MediaKeySystemService::SetMediaKeySystemServiceOperatorsCallback(wptr<IMediaKeySystemServiceOperatorsCallback> callback)
{
    return DRM_OK;
}

int32_t MediaKeySystemService::GenerateKeySystemRequest(IMediaKeySystemService::RequestType type, std::vector<uint8_t> &request, std::string &defaultUrl) {
    DRM_INFO_LOG("MediaKeySystemService::GenerateKeySystemRequest enter, type:%{public}d.", type);
    DRM_INFO_LOG("MediaKeySystemService::GenerateKeySystemRequest exit.");
    return DRM_OK;
}

int32_t MediaKeySystemService::ProcessKeySystemResponse(IMediaKeySystemService::RequestType type, const std::vector<uint8_t> &response) {
    DRM_INFO_LOG("MediaKeySystemService::ProcessKeySystemResponse enter, type:%{public}d.", type);
    DRM_INFO_LOG("MediaKeySystemService::ProcessKeySystemResponse exit.");
    return DRM_OK;
}

int32_t MediaKeySystemService::SetConfiguration(IMediaKeySystemService::ConfigType type, std::string &propertyName, std::string &value) {
    DRM_INFO_LOG("MediaKeySystemService::SetConfiguration enter, configType:%{public}d, propertyName:%{public}s, value:%{public}s.", (int)type, propertyName.c_str(), value.c_str());
    DRM_INFO_LOG("MediaKeySystemService::SetConfiguration exit.");
    return DRM_OK;
}

int32_t MediaKeySystemService::GetConfiguration(IMediaKeySystemService::ConfigType type, std::string &propertyName, std::string &value) {
    DRM_INFO_LOG("MediaKeySystemService::GetConfiguration enter, configType:%{public}d, propertyName:%{public}s.", (int)type, propertyName.c_str());
    DRM_INFO_LOG("MediaKeySystemService::GetConfiguration exit, value:%{public}s.", value.c_str());
    return DRM_OK;
}

int32_t MediaKeySystemService::CreateKeySession(IKeySessionService::SecurityLevel securityLevel, sptr<IKeySessionService> &keySessionProxy)
{
    DRM_INFO_LOG("MediaKeySystemService::CreateKeySession enter, securityLevel:%{public}d.", securityLevel);
    DRM_INFO_LOG("MediaKeySystemService::CreateKeySession exit.");
    return DRM_OK;
}

int32_t MediaKeySystemService::CloseKeySessionService(sptr<KeySessionService> sessionService)
{
    DRM_INFO_LOG("MediaKeySystemService::CloseKeySessionService enter.");
    DRM_INFO_LOG("MediaKeySystemService::CloseKeySessionService exit.");
    return DRM_OK;
}

int32_t MediaKeySystemService::GetMetric(std::vector<IMediaKeySystemService::KeyValue> &infoMap)
{
    DRM_INFO_LOG("MediaKeySystemService::GetMetric enter");
    DRM_INFO_LOG("MediaKeySystemService::GetMetric exit");
    return DRM_OK;
}

int32_t MediaKeySystemService::GetSecurityLevel(IKeySessionService::SecurityLevel *securityLevel) {
    DRM_INFO_LOG("MediaKeySystemService::GetSecurityLevel enter.");
    DRM_INFO_LOG("MediaKeySystemService::GetSecurityLevel exit.");
    return DRM_OK;
}

} // DrmStandard
} // OHOS