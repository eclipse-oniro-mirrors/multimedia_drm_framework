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
#ifndef OHOS_DRM_NATIVE_MEDIA_KEY_SESSION_H
#define OHOS_DRM_NATIVE_MEDIA_KEY_SESSION_H

#include <stdint.h>
#include <stdio.h>
#include "native_drm_common.h"
#include "native_drm_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OH_MediaKeySession OH_MediaKeySession;
/**
 * @brief Generate license request.
 * @syscap SystemCapability.Multimedia.Drm.Core
 * @param mediaKeySession Media key session instance.
 * if the function returns DRM_ERR_OK.
 * @param info License request info.
 * @param request Out parameter. License request.
 * @returns OH_DrmErrCode refers to OH_DrmErrCode.
 * @since 9
 * @version 1.0
 */
OH_DrmErrCode OH_MediaKeySession_GenerateLicenseRequest(OH_MediaKeySession *keySession,
    OH_DRM_MediaKeyRequestInfo *info, OH_DRM_LicenseRequest *request);

/**
 * @brief Process license request.
 * @syscap SystemCapability.Multimedia.Drm.Core
 * @param mediaKeySession Media key session instance.
 * @param response License resposne.
 * @param responseLen The length of license resposne.
 * @param licenseId Specifies which license corresponded.
 * @param licenseIdLen The length of license id.
 * @returns OH_DrmErrCode refers to OH_DrmErrCode.
 * @since 9
 * @version 1.0
 */
OH_DrmErrCode OH_MediaKeySession_ProcessLicenseResponse(OH_MediaKeySession *keySession, unsigned char *response,
    uint32_t &responseLen, unsigned char *licenseId, uint32_t licenseIdLen);

/**
 * @brief Check license status.
 * @syscap SystemCapability.Multimedia.Drm.Core
 * @param mediaKeySession Media key session instance.
 * @param response License resposne.
 * @param responseLen The length of license resposne.
 * @param licenseId Specifies which license corresponded.
 * @param licenseIdLen The length of license id.
 * @returns OH_DrmErrCode refers to OH_DrmErrCode.
 * @since 9
 * @version 1.0
 */
OH_DrmErrCode OH_MediaKeySession_CheckLicenseStatus(OH_MediaKeySession *keySession, OH_DRM_MediaKeyStatus *status,
    uint32_t *licenseStatusCount);

#ifdef __cplusplus
}
#endif

#endif // OHOS_DRM_NATIVE_MEDIA_KEY_SESSION_H