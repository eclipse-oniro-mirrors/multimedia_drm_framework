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

#include "mediakeysystemfactory_service.h"
#include <securec.h>
#include <unordered_set>
#include "access_token.h"
#include "ipc_skeleton.h"
#include "iservice_registry.h"
#include "mediakeysystem_service.h"
#include "drm_napi_utils.h"
#include "drm_log.h"
#include "system_ability_definition.h"

namespace OHOS {
namespace DrmStandard {
REGISTER_SYSTEM_ABILITY_BY_ID(MediaKeySystemFactoryService, MEDIA_KEY_SYSTEM_SERVICE_ID, true)

MediaKeySystemFactoryService::MediaKeySystemFactoryService(int32_t systemAbilityId, bool runOnCreate)
    : SystemAbility(systemAbilityId, runOnCreate) {}

MediaKeySystemFactoryService::~MediaKeySystemFactoryService()
{
    DRM_INFO_LOG("~MediaKeySystemFactoryService");
}

void MediaKeySystemFactoryService::OnStart()
{
    DRM_ERR_LOG("MediaKeySystemFactoryService");
}

void MediaKeySystemFactoryService::OnDump()
{
    DRM_INFO_LOG("MediaKeySystemFactoryService::OnDump called");
}

void MediaKeySystemFactoryService::OnStop()
{
    DRM_INFO_LOG("MediaKeySystemFactoryService::OnStop called");
}

int32_t MediaKeySystemFactoryService::CreateMediaKeySystem(std::string &uuid)
{
    DRM_INFO_LOG("MediaKeySystemFactoryService CreateMediaKeySystem enter.");
    DRM_INFO_LOG("MediaKeySystemFactoryService CreateMediaKeySystem exit.");
    return DRM_OK;
}

int32_t MediaKeySystemFactoryService::CloseMediaKeySystemService(sptr<MediaKeySystemService> mediaKeySystemService)
{
    DRM_INFO_LOG("MediaKeySystemFactoryService CloseMediaKeySystemService enter.");
    DRM_INFO_LOG("MediaKeySystemFactoryService CloseMediaKeySystemService exit.");
    return DRM_OK;
}

int32_t MediaKeySystemFactoryService::IsMediaKeySystemSupported(std::string &uuid, bool *isSurpported)
{
    DRM_INFO_LOG("MediaKeySystemFactoryService::IsMediaKeySystemSupported one parameters enter");
    DRM_INFO_LOG("MediaKeySystemFactoryService::IsMediaKeySystemSupported one parameters exit");
    return DRM_OK;
}

int32_t MediaKeySystemFactoryService::IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType,
    bool *isSurpported)
{
    DRM_INFO_LOG("MediaKeySystemFactoryService::IsMediaKeySystemSupported two parameters enter");
    DRM_INFO_LOG("MediaKeySystemFactoryService::IsMediaKeySystemSupported two parameters exit");
    return DRM_OK;
}

int32_t MediaKeySystemFactoryService::IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType,
    int32_t securityLevel, bool *isSurpported)
{
    DRM_INFO_LOG("MediaKeySystemFactoryService::IsMediaKeySystemSupported three parameters enter");
    DRM_INFO_LOG("MediaKeySystemFactoryService enter IsMediaKeySystemSupported  three parameters exit");
    return DRM_OK;
}

} // DrmStandard
} // OHOS