/*
 * Copyright (c) 2023  Huawei Device Co., Ltd.
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

#include "media_key_system_factory_impl.h"
#include "i_mediakeysystem_service.h"

namespace OHOS {
namespace DrmStandard {
sptr<MediaKeySystemFactoryImpl> MediaKeySystemFactoryImpl::mediaKeySystemFactoryImpl_;
MediaKeySystemFactoryImpl::MediaKeySystemFactoryImpl()
{
    DRM_DEBUG_LOG("MediaKeySystemFactoryImpl:0x%{public}06" PRIXPTR "MediaKeySystemFactoryImpl Instances create",
        FAKE_POINTER(this));
    Init();
}

MediaKeySystemFactoryImpl::~MediaKeySystemFactoryImpl()
{
    DRM_INFO_LOG("MediaKeySystemFactoryImpl::~MediaKeySystemFactoryImpl enter.");
    serviceProxy_ = nullptr;
    DRM_INFO_LOG("MediaKeySystemFactoryImpl::~MediaKeySystemFactoryImpl exit.");
    deathRecipient_ = nullptr;
}

sptr<MediaKeySystemFactoryImpl> &MediaKeySystemFactoryImpl::GetInstance()
{
    DRM_INFO_LOG("MediaKeySystemFactoryImpl::GetInstance enter.");
    if (MediaKeySystemFactoryImpl::mediaKeySystemFactoryImpl_ == nullptr) {
        DRM_DEBUG_LOG("Initializing MediaKeySystemFactoryImpl for first time!");
        MediaKeySystemFactoryImpl::mediaKeySystemFactoryImpl_ = new (std::nothrow) MediaKeySystemFactoryImpl();
        if (MediaKeySystemFactoryImpl::mediaKeySystemFactoryImpl_ == nullptr) {
            DRM_ERR_LOG("MediaKeySystemFactoryImpl::GetInstance failed to new MediaKeySystemFactoryImpl");
        }
    }
    DRM_INFO_LOG("MediaKeySystemFactoryImpl::GetInstance exit.");
    return MediaKeySystemFactoryImpl::mediaKeySystemFactoryImpl_;
}

void MediaKeySystemFactoryImpl::Init()
{
    DRM_INFO_LOG("MediaKeySystemFactoryImpl::Init enter.");
    sptr<IRemoteObject> object = nullptr;

    auto samgr = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (samgr == nullptr) {
        DRM_ERR_LOG("Failed to get System ability manager");
        return;
    }
    object = samgr->GetSystemAbility(MEDIA_KEY_SYSTEM_SERVICE_ID);
    if (object == nullptr) {
        DRM_ERR_LOG("MediaKeySystemFactoryImpl::GetSystemAbility() is failed");
        return;
    }
    serviceProxy_ = iface_cast<IMediaKeySystemFactoryService>(object);
    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemFactoryImpl::init serviceProxy_ is null.");
        return;
    }
    pid_t pid = 0;
    deathRecipient_ = new (std::nothrow) DrmDeathRecipient(pid);

    deathRecipient_->SetNotifyCb(
        std::bind(&MediaKeySystemFactoryImpl::MediaKeySystemServerDied, this, std::placeholders::_1));
    bool result = object->AddDeathRecipient(deathRecipient_);
    if (!result) {
        DRM_ERR_LOG("failed to add deathRecipient");
        return;
    }
    DRM_INFO_LOG("MediaKeySystemFactoryImpl::Init exit.");
}

void MediaKeySystemFactoryImpl::MediaKeySystemServerDied(pid_t pid)
{
    DRM_ERR_LOG("MediaKeySystemServer has died, pid:%{public}d!", pid);
    if (serviceProxy_ != nullptr) {
        (void)serviceProxy_->AsObject()->RemoveDeathRecipient(deathRecipient_);
        serviceProxy_ = nullptr;
    }
    deathRecipient_ = nullptr;
}

bool MediaKeySystemFactoryImpl::IsMediaKeySystemSupported(std::string &uuid)
{
    DRM_INFO_LOG("MediaKeySystemFactoryImpl::IsMediaKeySystemSupported enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;
    bool isSurpported = false;
    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemFactoryImpl::IsMediaKeySystemSupported serviceProxy_ is null");
        return isSurpported;
    }
    retCode = serviceProxy_->IsMediaKeySystemSupported(uuid, &isSurpported);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemFactoryImpl::IsMediaKeySystemSupported failed, retCode: %{public}d", retCode);
    }
    DRM_INFO_LOG("MediaKeySystemFactoryImpl::IsMediaKeySystemSupported exit.");

    return isSurpported;
}

bool MediaKeySystemFactoryImpl::IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType)
{
    DRM_INFO_LOG("MediaKeySystemFactoryImpl::IsMediaKeySystemSupported enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;
    bool isSurpported = false;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemFactoryImpl::IsMediaKeySystemSupported serviceProxy_ is null");
        return isSurpported;
    }
    retCode = serviceProxy_->IsMediaKeySystemSupported(uuid, mimeType, &isSurpported);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemFactoryImpl::IsMediaKeySystemSupported failed, retCode: %{public}d", retCode);
    }
    DRM_INFO_LOG("MediaKeySystemFactoryImpl::IsMediaKeySystemSupported exit.");

    return isSurpported;
}

bool MediaKeySystemFactoryImpl::IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType,
    IMediaKeySessionService::SecurityLevel securityLevel)
{
    DRM_INFO_LOG("MediaKeySystemFactoryImpl::IsMediaKeySystemSupported enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;
    bool isSurpported = false;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemFactoryImpl::IsMediaKeySystemSupported serviceProxy_ is null");
        return isSurpported;
    }
    retCode = serviceProxy_->IsMediaKeySystemSupported(uuid, mimeType, securityLevel, &isSurpported);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemFactoryImpl::IsMediaKeySystemSupported failed, retCode: %{public}d", retCode);
    }
    DRM_INFO_LOG("MediaKeySystemFactoryImpl::IsMediaKeySystemSupported exit.");

    return isSurpported;
}

int32_t MediaKeySystemFactoryImpl::CreateMediaKeySystem(std::string &uuid, sptr<MediaKeySystemImpl> *mediaKeySystemImpl)
{
    DRM_INFO_LOG("MediaKeySystemFactoryImpl:: CreateMediaKeySystem enter.");
    sptr<IMediaKeySystemService> mediaKeySystemProxy = nullptr;
    sptr<MediaKeySystemImpl> localMediaKeySystemImpl = nullptr;
    int32_t retCode = DRM_OK;
    if (mediaKeySystemImpl == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl:: mediaKeySystemImpl is nullptr");
        return DRM_INVALID_ARG;
    }
    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemFactoryImpl:: serviceProxy_ == nullptr");
        return DRM_SERVICE_ERROR;
    }

    retCode = serviceProxy_->CreateMediaKeySystem(uuid, mediaKeySystemProxy);
    if (retCode == DRM_OK) {
        if (mediaKeySystemProxy != nullptr) {
            localMediaKeySystemImpl = new (std::nothrow) MediaKeySystemImpl(mediaKeySystemProxy);
            if (localMediaKeySystemImpl == nullptr) {
                DRM_ERR_LOG("Failed to new MediaKeySystemImpl");
                return DRM_ALLOC_ERROR;
            }
        } else {
            DRM_ERR_LOG("mediaKeySystemProxy is nullptr");
            return DRM_UNKNOWN_ERROR;
        }
    } else {
        DRM_ERR_LOG("Failed to get session object from mediakeysystem service!, %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    *mediaKeySystemImpl = localMediaKeySystemImpl;
    DRM_INFO_LOG("MediaKeySystemFactoryImpl:: CreateMediaKeySystem exit.");
    return DRM_OK;
}
} // namespace DrmStandard
} // namespace OHOS