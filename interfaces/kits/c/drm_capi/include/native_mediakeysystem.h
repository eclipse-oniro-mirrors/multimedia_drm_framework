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
#ifndef OHOS_DRM_NATIVE_MEDIA_KEY_SYSTEM_H
#define OHOS_DRM_NATIVE_MEDIA_KEY_SYSTEM_H

#include <stdint.h>
#include <stdio.h>
#include "native_drm_err.h"
#include "native_drm_common.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct OH_MediaKeySystem OH_MediaKeySystem;
typedef struct OH_MediaKeySession OH_MediaKeySession;
/**
 * @brief Creates a media key system instance from the uuid.
 * @syscap SystemCapability.Multimedia.Drm.Core
 * @param uuid Secifies which drm system will be created.
 * @return Returns a Pointer to an OH_MediaKeySystem instance.
 * @since 9
 * @version 1.0
 */
OH_MediaKeySystem *OH_MediaKeySystem_Create(const char *uuid);

/**
 * @brief Destroy a media key system instance.
 * @syscap SystemCapability.Multimedia.Drm.Core
 * @param uuid Secifies which media key system instance will be destroyed.
 * @since 9
 * @version 1.0
 */
OH_DrmErrCode OH_MediaKeySystem_Destroy(OH_MediaKeySystem *mediaKeySystem);

/**
 * @brief Create a media key session instance.
 * @syscap SystemCapability.Multimedia.Drm.Core
 * @param mediaKeySystem Media key system instance which will create media key session.
 * @param OH_DRM_ContentProtectionLevel Specifies the security level.
 * @param mediaKeySession Out parameter. Media key session instance has been created
 * if the function returns DRM_ERR_OK.
 * @returns OH_DrmErrCode refers to OH_DrmErrCode
 * @since 9
 * @version 1.0
 */
OH_MediaKeySession *OH_MediaKeySystem_CreateMediaKeySession(OH_MediaKeySystem *mediaKeySystem,
    OH_DRM_ContentProtectionLevel *level);

#ifdef __cplusplus
}
#endif

#endif // OHOS_DRM_NATIVE_MEDIA_KEY_SYSTEM_H