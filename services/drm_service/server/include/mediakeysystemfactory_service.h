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

#ifndef OHOS_DRM_MEDIA_KEY_SYSTEM_FACTORY_SERVICE_H
#define OHOS_DRM_MEDIA_KEY_SYSTEM_FACTORY_SERVICE_H

#include "mediakeysystemfactory_service_stub.h"
#include "mediakeysystem_service.h"
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
class MediaKeySystemFactoryService : public SystemAbility, public MediaKeySystemFactoryServiceStub,
    public DrmHostManager::StatusCallback,
                              public IMediaKeySystemServiceOperatorsCallback {
    DECLARE_SYSTEM_ABILITY(MediaKeySystemFactoryService);
public:
    DISALLOW_COPY_AND_MOVE(MediaKeySystemFactoryService);
    explicit MediaKeySystemFactoryService(int32_t systemAbilityId, bool runOnCreate = true);
    ~MediaKeySystemFactoryService() override;
    void OnDump() override;
    void OnStart() override;
    void OnStop() override;
    int32_t IsMediaKeySystemSupported(std::string &uuid, bool *isSurpported) override;
    int32_t IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType, bool *isSurpported) override;
    int32_t IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType, int32_t securityLevel,
        bool *isSurpported) override;
    int32_t CreateMediaKeySystem(std::string &uuid) override;
    int32_t CloseMediaKeySystemService(sptr<MediaKeySystemService> mediaKeySystemService) override;
private:
};
} // DrmStandard
} // OHOS

#endif // OHOS_DRM_MEDIA_KEY_SYSTEM_FACTORY_SERVICE_H