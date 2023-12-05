/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
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

#include <mutex>
#include <shared_mutex>
#include <string>
#include <refbase.h>

#include "drm_log.h"
#include "drm_error_code.h"

#include "native_drm_common.h"
#include "native_drm_object.h"
#include "native_mediakeysystem.h"

using namespace OHOS::DrmStandard;

struct OH_MediaKeySystem *OH_MediaKeySystem_Create(const char *uuid)
{
    DRM_INFO_LOG("OH_MediaKeySystem_Create called");
    DRM_CHECK_AND_RETURN_RET_LOG(uuid != nullptr, nullptr, "OH_MediaKeySystem uuid is nullptr!");

    OHOS::sptr<MediaKeySystemFactoryImpl> fatory = MediaKeySystemFactoryImpl::GetInstance();
    DRM_CHECK_AND_RETURN_RET_LOG(fatory != nullptr, nullptr, "OH_MediaKeySystem fatory is nullptr!");

    std::string uuidStr(uuid);
    OHOS::sptr<OHOS::DrmStandard::MediaKeySystemImpl> system = nullptr;
    fatory->CreateMediaKeySystem(uuidStr, &system);
    DRM_CHECK_AND_RETURN_RET_LOG(system != nullptr, nullptr, "OH_MediaKeySystem system create by uuid failed!");

    struct MediaKeySystemObject *object = new (std::nothrow) MediaKeySystemObject(system);
    DRM_CHECK_AND_RETURN_RET_LOG(object != nullptr, nullptr, "OH_MediaKeySystem MediaKeySystemObject create failed!");

    return object;
}

OH_DrmErrCode OH_MediaKeySystem_Destroy(OH_MediaKeySystem *mediaKeySystem)
{
    DRM_INFO_LOG("OH_MediaKeySystem_Destroy called");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem mediaKeySystem is nullptr!");

    struct MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem systemObject is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem systemImpl_ is nullptr!");

    int32_t ret = systemObject->systemImpl_->Release();
    OH_DrmErrCode retCode = DRM_ERR_OK;
    if (ret != DRM_OK) {
        DRM_ERR_LOG("call media key system release failed!");
        retCode = DRM_ERR_UNKNOWN;
    }
    delete mediaKeySystem;
    mediaKeySystem = nullptr;
    return retCode;
}

OH_MediaKeySession *OH_MediaKeySystem_CreateMediaKeySession(OH_MediaKeySystem *mediaKeySystem,
    OH_DRM_ContentProtectionLevel *level)
{
    DRM_INFO_LOG("OH_MediaKeySystem_CreateMediaKeySession called");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, nullptr, "OH_MediaKeySystem mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(level != nullptr, nullptr, "OH_MediaKeySystem level is nullptr!");

    struct MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, nullptr, "OH_MediaKeySystem systemObject is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemImpl_ != nullptr, nullptr, "systemImpl_ is nullptr!");

    int32_t secure = static_cast<int32_t>(*level);
    DRM_CHECK_AND_RETURN_RET_LOG(secure <= CONTENT_PROTECTION_LEVEL_MAX && secure >= CONTENT_PROTECTION_LEVEL_UNKNOWN,
        nullptr, "OH_MediaKeySystem secure input is invalid");
    IMediaKeySessionService::SecurityLevel secureLevel = static_cast<IMediaKeySessionService::SecurityLevel>(secure);
    OHOS::sptr<MediaKeySessionImpl> keySessionImpl = nullptr;
    int32_t ret = systemObject->systemImpl_->CreateMediaKeySession(secureLevel, &keySessionImpl);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_OK, nullptr, "OH_MediaKeySystem create session impl return failed!");
    DRM_CHECK_AND_RETURN_RET_LOG(keySessionImpl != nullptr, nullptr, "OH_MediaKeySystem session impl create failed!");

    struct MediaKeySessionObject *object = new (std::nothrow) MediaKeySessionObject(keySessionImpl);
    DRM_CHECK_AND_RETURN_RET_LOG(object != nullptr, nullptr, "OH_MediaKeySystem MediaKeySessionObject create failed!");
    return object;
}