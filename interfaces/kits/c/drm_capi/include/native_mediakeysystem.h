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

typedef enum {
    SECURITY_LEVEL_UNKNOWN = 0,
    SECURITY_LEVEL_SW_CRYPTO = 1,
    SECURITY_LEVEL_SW_DECODE = 2,
    SECURITY_LEVEL_HW_CRYPTO = 3,
    SECURITY_LEVEL_HW_DECODE = 4,
    SECURITY_LEVEL_HW_ALL = 5,
    SECURITY_LEVEL_MAX = 6,
} OH_SecurityLevel;

typedef enum {

    CERT_STATUS_PROVISIONED = 0,
    CERT_STATUS_NOT_PROVISIONED,
    CERT_STATUS_EXPIRED,
    CERT_STATUS_INVALID,
    CERT_STATUS_GET_FAILED,
} OH_CertificateStatus;

typedef enum {
    OFFLINELICENSE_STATUS_UNKNOWN = 0,
    OFFLINELICENSE_STATUS_USABLE = 1,
    OFFLINELICENSE_STATUS_INACTIVE = 2,
} OH_OfflineLicenseStatus;

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
 * @brief Judge whether a system that specifies UUID, mimetype and security level is supported.
 * @syscap System Capability Multimedia Drm Core
 * @param uuid Used to point a Digital Right Managements solution.
 * @returns Whether these conditions will be met.
 * @since 9
 * @version 1.0
 */
OH_DrmBool OH_MediaKeySystemIsSupportedByUuid(const char *uuid);
OH_DrmBool OH_MediaKeySystemIsSupportedV2(const char *uuid, const char *mimeTypePtr);
OH_DrmBool OH_MediaKeySystemIsSupportedV3(const char *uuidPtr, const char *mimeTypePtr, int32_t cSecurityLevel);

/**
 * @brief Set the specified configuration.
 * @syscap SystemCapability.Multimedia.Drm.Core
 * @param configName Used to specify the config name.
 * @param value The value to be set.
 * @since 9
 * @version 1.0
 */
OH_DrmErrCode OH_SetConfigurationString(OH_MediaKeySystem *mediaKeySystem, const char *configName,
    const char *valuePtr);

/**
 * @brief Get the specified configuration.
 * @syscap SystemCapability.Multimedia.Drm.Core
 * @param configName Used to specify the config name.
 * @param valuePtr Used to obtain the value of the specified configuration.
 * @returns The result.
 * @since 8
 * @version 1.0
 */
OH_DrmErrCode OH_GetConfigurationString(OH_MediaKeySystem *mediaKeySystem, const char *configName, char *valuePtr);

/**
 * @briefGet the specified configuration.
 * @syscap SystemCapability.Multimedia.Drm.Core
 * @param mediaKeySystem Media key system instance which will create media key session.
 * @param configName Used to specify the config name.
 * @param valuePtr Used to obtain the value of the specified configuration.
 * @returns The result.
 * @since 9
 * @version 1.0
 */
OH_DrmErrCode OH_GetConfigurationByteArray(OH_MediaKeySystem *mediaKeySystem, const char *configName,
    unsigned char *valuePtr, uint32_t *valueLen);

/**
 * @brief Set the specified configuration.
 * @syscap SystemCapability.Multimedia.Drm.Core
 * @param configName Used to specify the config name.
 * @param value The value to be set.
 * @returns OH_DrmErrCode refers to OH_DrmErrCode
 * @since 9
 * @version 1.0
 */
OH_DrmErrCode OH_SetConfigurationByteArray(OH_MediaKeySystem *mediaKeySystem, const char *configName,
    unsigned char *valuePtr, uint32_t valueLen);

/**
 * @brief Get security level.
 * @syscap SystemCapability.Multimedia.Drm.Core
 * @param mediaKeySystem Media key system instance which will create media key session.
 * @returns return SecurityLevel.
 * @since 9
 * @version 1.0
 */
OH_SecurityLevel OH_GetMaxSecurityLevel(OH_MediaKeySystem *mediaKeySystem);

/**
 * @brief Process the response corresponding the key system request obtained by the application.
 * @syscap SystemCapability.Multimedia.Drm.Core
 * @param mediaKeySystem Media key system instance which will create media key session.
 * @param reponseData Response corresponding to the request.
 * @param reponseDataLen Response corresponding to the request.
 * @returns OH_DrmErrCode refers to OH_DrmErrCode
 * @since 9
 * @version 1.0
 */
OH_DrmErrCode OH_ProcessKeySystemResponse(OH_MediaKeySystem *mediaKeySystem, uint8_t *reponseData,
    size_t reponseDataLen);

/**
 * @brief Get certificate status of DRM system.
 * @syscap SystemCapability.Multimedia.Drm.Core
 * @param mediaKeySystem Media key system instance which will create media key session.
 * @returns Certificate Status.
 * if the function returns DRM_ERR
 * @since 9
 * @version 1.0
 */
OH_CertificateStatus OH_GetCertificateStatus(OH_MediaKeySystem *mediaKeySystem);

/**
 * Get offline license status corresponding to the licenseId.
 * @syscap SystemCapability.Multimedia.Drm.Core
 * @param mediaKeySystem Media key system instance which will create media key session.
 * @param licenseId The licenseId specifies license.
 * @param licenseIdLen The LicenseId specifies the length of the license
 * @returns OfflineKeyState.
 * @since 9
 * @version 1.0
 */
OH_OfflineLicenseStatus OH_GetOfflineLicenseStatus(OH_MediaKeySystem *mediaKeySystem, uint8_t *licenseId,
    size_t licenseIdLen);

/**
 * Remove license corresponding to the licenseId.
 * @since 8
 * @syscap SystemCapability.Multimedia.Drm.Core
 * @param mediaKeySystem Media key system instance which will create media key session.
 * @param licenseId The licenseId specifies which license should be remove.
 * @param licenseIdLen The length of the license
 * @returns OH_DrmErrCode refers to OH_DrmErrCode
 * if the function returns DRM_ERR_OK.
 * @since 9
 * @version 1.0
 */
OH_DrmErrCode OH_RemoveOfflineLicense(OH_MediaKeySystem *mediaKeySystem, uint8_t *licenseId, size_t licenseIdLen);

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
 * @param OH_SecurityLevel Specifies the security level.
 * @param mediaKeySession Out parameter. Media key session instance has been created
 * @returns OH_DrmErrCode refers to OH_DrmErrCode
 * if the function returns DRM_ERR_OK.
 * @since 9
 * @version 1.0
 */
OH_MediaKeySession *OH_MediaKeySystem_CreateMediaKeySession(OH_MediaKeySystem *mediaKeySystem, OH_SecurityLevel *level);

#ifdef __cplusplus
}
#endif

#endif // OHOS_DRM_NATIVE_MEDIA_KEY_SYSTEM_H