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

/**
 * @addtogroup Drm
 * @{
 *
 * @brief Provides APIs of Drm.
 * @since 11
 * @version 1.0
 */

/**
 * @file native_mediakeysession.h
 * @brief Defines the Drm MediaKeySession APIs. Provide following function:
 * generate media key request, process media key response, event listening,
 * get content protection level, check media key status, remove media key etc..
 * @library libnative_drm.z.so
 * @Syscap SystemCapability.Multimedia.Drm.Core
 * @since 11
 * @version 1.0
 */

#ifndef OHOS_DRM_NATIVE_MEDIA_KEY_SESSION_H
#define OHOS_DRM_NATIVE_MEDIA_KEY_SESSION_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "native_drm_err.h"
#include "native_drm_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Call back will be invoked when event triggers.
 * @param eventType Event type.
 * @param eventInfo Event info gotten from media key system.
 * @return OH_DrmErrCode.
 * @since 11
 * @version 1.0
 */
typedef OH_DrmErrCode (*OH_MediaKeySessionEventCallback)(OH_DRM_ListenerType eventType,
    OH_DRM_Uint8CharBufferPair *eventInfo);

/**
 * @brief Call back will be invoked when key changes.
 * @param keysInfo Key info gotten from media key system.
 * @return OH_DrmErrCode.
 * @since 11
 * @version 1.0
 */
typedef OH_DrmErrCode (*OH_MediaKeySessionKeyChangeCallback)(OH_DRM_KeysInfo *keysInfo, bool hasNewGoodKeys);

/**
 * @brief OH_MediaKeySessionCallback struct, used to listen event like key expired and key change etc..
 * @since 11
 * @version 1.0
 */
typedef struct OH_MediaKeySessionCallback {
    /* *
     * Normal event callback like key expired etc..
     */
    OH_MediaKeySessionEventCallback eventCallback;
    /* *
     * Key change callback for keys change event.
     */
    OH_MediaKeySessionKeyChangeCallback keyChangeCallback;
} OH_MediaKeySessionCallback;

/**
 * @brief Generate media key request.
 * @param mediaKeySession Media key session instance.
 * if the function return DRM_ERR_OK.
 * @param info Media key request info.
 * @param mediaKeyRequest Media key request.
 * @return OH_DrmErrCode.
 * @since 11
 * @version 1.0
 */
OH_DrmErrCode OH_MediaKeySession_GenerateMediaKeyRequest(OH_MediaKeySession *mediaKeySession,
    OH_DRM_MediaKeyRequestInfo *info, OH_DRM_MediaKeyRequest **mediaKeyRequest);

/**
 * @brief Process media key response.
 * @param mediaKeySession Media key session instance.
 * @param response Media Key resposne.
 * @param mediaKeyId Media key identifier.
 * @return OH_DrmErrCode.
 * @since 11
 * @version 1.0
 */
OH_DrmErrCode OH_MediaKeySession_ProcessMediaKeyResponse(OH_MediaKeySession *keySession, OH_DRM_Uint8Buffer *response,
    unsigned char **mediaKeyId, int32_t *mediaKeyIdLen);

/**
 * @brief Check media key status.
 * @param mediaKeySession Media key session instance.
 * @param mediaKeyDescription Media key status description.
 * @return OH_DrmErrCode.
 * @since 11
 * @version 1.0
 */
OH_DrmErrCode OH_MediaKeySession_CheckMediaKeyStatus(OH_MediaKeySession *mediaKeySessoin,
    OH_DRM_MediaKeyDescription **mediaKeyDescription);

/**
 * @brief Clear media keys of the current session .
 * @param mediaKeySession Media key session instance.
 * @return OH_DrmErrCode.
 * @since 11
 * @version 1.0
 */
OH_DrmErrCode OH_MediaKeySession_ClearMediaKeys(OH_MediaKeySession *mediaKeySessoin);
/**
 * @brief Generate offline media key release request.
 * @param mediaKeySession Media key session instance.
 * @param mediaKeyId Media key identifier.
 * @param releaseRequest Media Key release request.
 * @return OH_DrmErrCode.
 * @since 11
 * @version 1.0
 */
OH_DrmErrCode OH_MediaKeySession_GenerateOfflineReleaseRequest(OH_MediaKeySession *mediaKeySessoin,
    OH_DRM_Uint8Buffer *mediaKeyId, unsigned char **releaseRequest, int32_t *releaseRequestLen);

/**
 * @brief Process offline media key release response.
 * @param mediaKeySession Media key session instance.
 * @param mediaKeyId Media key identifier.
 * @param releaseReponse Media Key resposne.
 * @return OH_DrmErrCode.
 * @since 11
 * @version 1.0
 */
OH_DrmErrCode OH_MediaKeySession_ProcessOfflineReleaseResponse(OH_MediaKeySession *mediaKeySessoin,
    OH_DRM_Uint8Buffer *mediaKeyId, OH_DRM_Uint8Buffer *releaseReponse);

/**
 * @brief Restore offline media keys by ID.
 * @param mediaKeySession Media key session instance.
 * @param mediaKeyId Media key identifier.
 * @return OH_DrmErrCode.
 * @since 11
 * @version 1.0
 */
OH_DrmErrCode OH_MediaKeySession_RestoreOfflineMediaKeys(OH_MediaKeySession *mediaKeySessoin,
    OH_DRM_Uint8Buffer *mediaKeyId);

/**
 * @brief Get content protection level of the session.
 * @param mediaKeySession Media key session instance.
 * @param contentProtectionLevel Content protection level.
 * @return OH_DrmErrCode.
 * @since 11
 * @version 1.0
 */
OH_DrmErrCode OH_MediaKeySession_GetContentProtectionLevel(OH_MediaKeySession *mediaKeySessoin,
    OH_DRM_ContentProtectionLevel *contentProtectionLevel);

/**
 * @brief Whether the encrypted content require a secure decoder or not.
 * @param mediaKeySession Media key session instance.
 * @param mimeType The media type.
 * @param status Whether secure decoder is required.
 * @return OH_DrmErrCode.
 * @since 11
 * @version 1.0
 */
OH_DrmErrCode OH_MediaKeySession_RequireSecureDecoderModule(OH_MediaKeySession *mediaKeySessoin, const char *mimeType,
    bool *status);

/**
 * @brief Set media key session event callback.
 * @param mediaKeySession Media key session instance.
 * @param callback Callback to be set to the media key session.
 * @return OH_DrmErrCode.
 * @since 11
 * @version 1.0
 */
OH_DrmErrCode OH_MediaKeySession_SetMediaKeySessionCallback(OH_MediaKeySession *mediaKeySessoin,
    OH_MediaKeySessionCallback *callback);

/**
 * @brief Release the resource before the session gonna be unused.
 * @param mediaKeySession Media key session instance.
 * @return OH_DrmErrCode.
 * @since 11
 * @version 1.0
 */
OH_DrmErrCode OH_MediaKeySession_Destroy(OH_MediaKeySession *mediaKeySessoin);

#ifdef __cplusplus
}
#endif

#endif // OHOS_DRM_NATIVE_MEDIA_KEY_SYSTEM_H