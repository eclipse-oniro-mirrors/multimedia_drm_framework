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

#include <securec.h>
#include <unordered_set>
#include "system_ability_definition.h"
#include "ipc_skeleton.h"
#include "access_token.h"
#include "iservice_registry.h"
#include "drm_log.h"
#include "drm_napi_utils.h"
#include "mediakeysystem_service.h"
#include "mediakeysystemfactory_service.h"

namespace OHOS {
namespace DrmStandard {
REGISTER_SYSTEM_ABILITY_BY_ID(MediaKeySystemFactoryService, MEDIA_KEY_SYSTEM_SERVICE_ID, true)

MediaKeySystemFactoryService::MediaKeySystemFactoryService(int32_t systemAbilityId, bool runOnCreate)
    : SystemAbility(systemAbilityId, runOnCreate), drmHostManager_(nullptr)
{}

MediaKeySystemFactoryService::~MediaKeySystemFactoryService()
{
    DRM_INFO_LOG("~MediaKeySystemFactoryService");
}

void MediaKeySystemFactoryService::OnStart()
{
    if (drmHostManager_ == nullptr) {
        drmHostManager_ = new (std::nothrow) DrmHostManager(this);
        if (drmHostManager_ == nullptr) {
            DRM_ERR_LOG("MediaKeySystemFactoryService OnStart failed to create drmHostManager_obj");
            return;
        }
    }
    if (drmHostManager_->Init() != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemFactoryService OnStart failed to init drm host manager");
    }
    bool res = Publish(this);
    if (res) {
        DRM_INFO_LOG("MediaKeySystemFactoryService OnStart res=%{public}d", res);
    }
}

void MediaKeySystemFactoryService::OnDump()
{
    DRM_INFO_LOG("MediaKeySystemFactoryService::OnDump called");
}

void MediaKeySystemFactoryService::OnStop()
{
    DRM_INFO_LOG("MediaKeySystemFactoryService::OnStop called");

    if (drmHostManager_) {
        drmHostManager_->DeInit();
        delete drmHostManager_;
        drmHostManager_ = nullptr;
    }
}

int32_t MediaKeySystemFactoryService::CreateMediaKeySystem(std::string &uuid,
    sptr<IMediaKeySystemService> &mediaKeySystemProxy)
{
    DRM_INFO_LOG("MediaKeySystemFactoryService CreateMediaKeySystem enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    sptr<MediaKeySystemService> mediaKeySystemService = nullptr;
    sptr<IMediaKeySystem> hdiMediaKeySystem = nullptr;
    int32_t ret = drmHostManager_->CreateMediaKeySystem(uuid, hdiMediaKeySystem);
    if (hdiMediaKeySystem == nullptr) {
        DRM_ERR_LOG("MediaKeySystemFactoryService:: drmHostManager_ return hdiMediaKeySystem nullptr");
        return DRM_SERVICE_ERROR;
    }
    mediaKeySystemService = new (std::nothrow) MediaKeySystemService(hdiMediaKeySystem);
    if (mediaKeySystemService == nullptr) {
        DRM_ERR_LOG("MediaKeySystemFactoryService::CreateMediaKeySystem allocation failed");
        return DRM_ALLOC_ERROR;
    }
    mediaKeySystemService->SetMediaKeySystemServiceOperatorsCallback(this);
    int32_t pid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySystemFactoryService CreateMediaKeySystem GetCallingPID: %{public}d", pid);
    mediaKeySystemForPid_[pid].insert(mediaKeySystemService);
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " is Current mediaKeySystemService",
        FAKE_POINTER(mediaKeySystemService.GetRefPtr()));
    mediaKeySystemProxy = mediaKeySystemService;
    DRM_INFO_LOG("MediaKeySystemFactoryService CreateMediaKeySystem exit.");
    return ret;
}

int32_t MediaKeySystemFactoryService::CloseMediaKeySystemService(sptr<MediaKeySystemService> mediaKeySystemService)
{
    DRM_INFO_LOG("MediaKeySystemFactoryService CloseMediaKeySystemService enter.");
    int32_t errCode = DRM_OK;
    int32_t currentPid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySystemFactoryService GetCallingPID: %{public}d", currentPid);

    int32_t pid = currentPid;
    if (mediaKeySystemService != nullptr) {
        DRM_INFO_LOG("MediaKeySystemFactoryService call CloseMediaKeySystemService ");
        errCode = mediaKeySystemService->CloseMediaKeySystemServiceByCallback();
    }
    DRM_DEBUG_LOG("MediaKeySystemFactoryService mediaKeySystemForPid_ size:%{public}u",
        mediaKeySystemForPid_[pid].size());
    if (mediaKeySystemForPid_[pid].find(mediaKeySystemService) != mediaKeySystemForPid_[pid].end()) {
        DRM_DEBUG_LOG("MediaKeySystemFactoryService before sessionSet size:%{public}u",
            mediaKeySystemForPid_[pid].size());
        mediaKeySystemForPid_[pid].erase(mediaKeySystemService);
        DRM_DEBUG_LOG("MediaKeySystemFactoryService after sessionSet size:%{public}u",
            mediaKeySystemForPid_[pid].size());
    } else {
        DRM_ERR_LOG("MediaKeySystemFactoryService not find sessions for PID:%{public}d", pid);
    }
    DRM_DEBUG_LOG("MediaKeySystemFactoryService mediaKeySystemForPid_ size:%{public}u",
        mediaKeySystemForPid_[pid].size());

    mediaKeySystemService = nullptr;
    DRM_INFO_LOG("MediaKeySystemFactoryService CloseMediaKeySystemService exit.");
    return errCode;
}

int32_t MediaKeySystemFactoryService::IsMediaKeySystemSupported(std::string &uuid, bool *isSurpported)
{
    DRM_INFO_LOG("MediaKeySystemFactoryService::IsMediaKeySystemSupported one parameters enter");
    int32_t ret = drmHostManager_->IsMediaKeySystemSupported(uuid, isSurpported);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemFactoryService::IsMediaKeySystemSupported failed");
        return ret;
    }
    DRM_INFO_LOG("MediaKeySystemFactoryService::IsMediaKeySystemSupported one parameters exit");
    return ret;
}

int32_t MediaKeySystemFactoryService::IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType,
    bool *isSurpported)
{
    DRM_INFO_LOG("MediaKeySystemFactoryService::IsMediaKeySystemSupported two parameters enter");
    int32_t ret = drmHostManager_->IsMediaKeySystemSupported(uuid, mimeType, isSurpported);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemFactoryService::IsMediaKeySystemSupported failed");
        return ret;
    }
    DRM_INFO_LOG("MediaKeySystemFactoryService::IsMediaKeySystemSupported two parameters exit");
    return ret;
}

int32_t MediaKeySystemFactoryService::IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType,
    int32_t securityLevel, bool *isSurpported)
{
    DRM_INFO_LOG("MediaKeySystemFactoryService::IsMediaKeySystemSupported three parameters enter");
    int32_t ret = drmHostManager_->IsMediaKeySystemSupported(uuid, mimeType, securityLevel, isSurpported);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemFactoryService::IsMediaKeySystemSupported failed");
        return ret;
    }
    DRM_INFO_LOG("MediaKeySystemFactoryService enter IsMediaKeySystemSupported  three parameters exit");
    return ret;
}
} // DrmStandard
} // OHOS