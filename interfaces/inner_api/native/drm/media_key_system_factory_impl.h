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

#ifndef OHOS_DRM_MEDIA_KEY_SYSTEMP_FACTORY_IMPL_H
#define OHOS_DRM_MEDIA_KEY_SYSTEMP_FACTORY_IMPL_H

#include "media_key_system_impl.h"
#include "system_ability_definition.h"
#include "ipc_skeleton.h"
#include "iservice_registry.h"
#include "nocopyable.h"
#include "i_mediakeysystemfactory_service.h"
#include "drm_death_recipient.h"
#include "drm_log.h"

namespace OHOS {
namespace DrmStandard {
class MediaKeySystemFactoryImpl : public RefBase {
public:
    MediaKeySystemFactoryImpl();
    ~MediaKeySystemFactoryImpl();
    void Init();
    static sptr<MediaKeySystemFactoryImpl> &GetInstance();
    bool IsMediaKeySystemSupported(std::string &uuid);
    bool IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType);
    bool IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType,
        IMediaKeySessionService::ContentProtectionLevel securityLevel);
    int32_t CreateMediaKeySystem(std::string &uuid, sptr<MediaKeySystemImpl> *mediaKeySystemImpl);
    int32_t keySystemNumber = 0;

private:
    void MediaKeySystemServerDied(pid_t pid);
    std::mutex mutex_;
    static sptr<MediaKeySystemFactoryImpl> mediaKeySystemFactoryImpl_;
    sptr<OHOS::DrmStandard::IMediaKeySystemFactoryService> serviceProxy_;
    sptr<DrmDeathRecipient> deathRecipient_;
};
} // DrmStandard
} // OHOS
#endif // OHOS_DRM_MEDIA_KEY_SYSTEMP_FACTORY_IMPL_H