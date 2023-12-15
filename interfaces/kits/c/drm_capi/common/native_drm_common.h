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
 * @file native_drm_common.h
 *
 * @brief Defines the Drm common struct.
 * @library libnative_drm.z.so
 * @Syscap SystemCapability.Multimedia.Drm.Core
 * @since 11
 * @version 1.0
 */

#ifndef NATIVE_DRM_COMMON_H
#define NATIVE_DRM_COMMON_H

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Enumerates event types of listener.
 * @brief Content potection level.
 * @since 11
 * @version 1.0
 */
typedef enum OH_DRM_ListenerType {
    /* *
     * DRM event base.
     */
    LISTENER_DRM_EVENT = 200,
    /* *
     * Provision required event.
     */
    LISTENER_PROVISION_REQUIRED = 201,
    /* *
     * Media key required event.
     */
    LISTENER_KEY_REQUIRED = 202,
    /* *
     * Media key expired event.
     */
    LISTENER_KEY_EXPIRED = 203,
    /* *
     * Vendor defined event.
     */
    LISTENER_VENDOR_DEFINED = 204,
    /* *
     * Expiration update event.
     */
    LISTENER_EXPIRATION_UPDATE = 206,
} OH_DRM_ListenerType;

/**
 * @brief Content potection level.
 * @since 11
 * @version 1.0
 */
typedef enum OH_DRM_ContentProtectionLevel {
    /* *
     * Content potection level unknown.
     */
    CONTENT_PROTECTION_LEVEL_UNKNOWN = 0,
    /* *
     * Content potection level software crypto.
     */
    CONTENT_PROTECTION_LEVEL_SW_CRYPTO,
    /* *
     * Content potection level hardware crypto.
     */
    CONTENT_PROTECTION_LEVEL_HW_CRYPTO,
    /* *
     * Content potection level enhanced hardware crypto.
     */
    CONTENT_PROTECTION_LEVEL_ENHANCED_HW_CRYPTO,
    /* *
     * Content potection level max stub.
     */
    CONTENT_PROTECTION_LEVEL_MAX,
} OH_DRM_ContentProtectionLevel;

/**
 * @brief Media key type.
 * @since 11
 * @version 1.0
 */
typedef enum OH_DRM_MediaKeyType {
    /* *
     * Media key type offline.
     */
    MEDIA_KEY_TYPE_OFFLINE = 0,
    /* *
     * Media key type online
     */
    MEDIA_KEY_TYPE_ONLINE,
} OH_DRM_MediaKeyType;

/**
 * @brief Media key request type.
 * @since 11
 * @version 1.0
 */
typedef enum OH_DRM_MediaKeyRequestType {
    /* *
     * Media key request type unknown.
     */
    MEDIA_KEY_REQUEST_TYPE_UNKNOWN = 0,
    /* *
     * Media key request type initial.
     */
    MEDIA_KEY_REQUEST_TYPE_INITIAL,
    /* *
     * Media key request type renewal.
     */
    MEDIA_KEY_REQUEST_TYPE_RENEWAL,
    /* *
     * Media key request type release.
     */
    MEDIA_KEY_REQUEST_TYPE_RELEASE,
    /* *
     * Media key request type none.
     */
    MEDIA_KEY_REQUEST_TYPE_NONE,
    /* *
     * Media key request type update.
     */
    MEDIA_KEY_REQUEST_TYPE_UPDATE,
} OH_DRM_MediaKeyRequestType;

/**
 * @brief Offline media key status.
 * @since 11
 * @version 1.0
 */
typedef enum OH_DRM_OfflineMediaKeyStatus {
    /* *
     * Offline media key status unknown.
     */
    OFFLINE_MEDIA_KEY_STATUS_UNKNOWN = 0,
    /* *
     * Offline media key status usable.
     */
    OFFLINE_MEDIA_KEY_STATUS_USABLE,
    /* *
     * Offline media key status inactive.
     */
    OFFLINE_MEDIA_KEY_STATUS_INACTIVE,
} OH_DRM_OfflineMediaKeyStatus;


typedef enum {
    OFFLINELICENSE_STATUS_UNKNOWN = 0,
    OFFLINELICENSE_STATUS_USABLE = 1,
    OFFLINELICENSE_STATUS_INACTIVE = 2,
} OH_OfflineLicenseStatus;

/**
 * @brief Media key status.
 * @since 11
 * @version 1.0
 */
typedef enum OH_DRM_MediaKeyStatus {
    /* *
     * Media key is usable.
     */
    MEDIA_KEY_STATUS_USABLE = 0,
    /* *
     * Media key expires.
     */
    MEDIA_KEY_STATUS_EXPIRED,
    /* *
     * Output not allowed with the media key.
     */
    MEDIA_KEY_STATUS_OUTPUT_NOT_ALLOWED,
    /* *
     * Media key is pending.
     */
    MEDIA_KEY_STATUS_PENDING,
    /* *
     * Media key is in internal error.
     */
    MEDIA_KEY_STATUS_INTERNAL_ERROR,
    /* *
     * Media key will be usable in future.
     */
    MEDIA_KEY_STATUS_USABLE_IN_FUTURE,
} OH_DRM_MediaKeyStatus;

/**
 * @brief Unsigned char buffer.
 * @since 11
 * @version 1.0
 */
typedef struct OH_DRM_Uint8Buffer {
    /* *
     * Unsigned char buffer addr.
     */
    unsigned char *buffer;
    /* *
     * Unsigned char buffer len.
     */
    uint32_t bufferLen;
} OH_DRM_Uint8Buffer;

/**
 * @brief Char buffer.
 * @since 11
 * @version 1.0
 */
typedef struct OH_DRM_CharBuffer {
    /* *
     * Char buffer addr.
     */
    char *buffer;
    /* *
     * Char buffer len.
     */
    uint32_t bufferLen;
} OH_DRM_CharBuffer;

/**
 * @brief Char-char buffer pair.
 * @since 11
 * @version 1.0
 */
typedef struct OH_DRM_CharBufferPair {
    /* Name buffer in chars. */
    OH_DRM_CharBuffer name;
    /* Value buffer in chars. */
    OH_DRM_CharBuffer value;
} OH_DRM_CharBufferPair;

typedef struct OH_DRM_EnumBufferPair {
    /* Name buffer in chars. */
    OH_DRM_CharBuffer name;
    /* Value buffer in chars. */
    int32_t value;
} OH_DRM_EnumBufferPair;

/**
 * @brief Unsignedchar-char buffer.
 * @since 11
 * @version 1.0
 */
typedef struct OH_DRM_Uint8CharBufferPair {
    /* Key buffer in Uint8Array. */
    OH_DRM_Uint8Buffer key;
    /* Value buffer in chars. */
    OH_DRM_CharBuffer value;
} OH_DRM_Uint8CharBufferPair;

/**
 * @brief Media key request info.
 * @since 11
 * @version 1.0
 */
typedef struct OH_DRM_MediaKeyRequest {
    /* *
     * Media key request type.
     */
    OH_DRM_MediaKeyRequestType type;
    /* *
     * Media key request data sent to media key server.
     */
    OH_DRM_Uint8Buffer data;
    /* *
     * Media key server URL.
     */
    OH_DRM_CharBuffer defaultURL;
} OH_DRM_MediaKeyRequest;

/**
 * @brief Media key request info.
 * @since 11
 * @version 1.0
 */
typedef struct OH_DRM_MediaKeyRequestInfo {
    /* *
     * Offline or online media key type.
     */
    OH_DRM_MediaKeyType type;
    /* *
     * Initial data format as PSSH after base64 encoding.
     */
    OH_DRM_Uint8Buffer data;
    /* *
     * Media content mime type.
     */
    OH_DRM_CharBuffer mimeType;
    /* *
     * OptionsData count.
     */
    uint32_t optionsCount;
    /* *
     * Options data the application set to drm framework.
     */
    OH_DRM_CharBufferPair *optionsData;
} OH_DRM_MediaKeyRequestInfo;

/**
 * @brief Statistics of OH_MediaKeySystem.
 * @since 11
 * @version 1.0
 */
typedef struct OH_DRM_Statistics {
    /* Statistics count. */
    uint32_t statisticsCount;
    /* Statistics info. */
    OH_DRM_CharBufferPair info[0];
} OH_DRM_Statistics;


typedef enum CertificateStatus {
    CERT_STATUS_PROVISIONED = 0,
    CERT_STATUS_NOT_PROVISIONED = 1,
    CERT_STATUS_EXPIRED = 3,
    CERT_STATUS_INVALID = 4,
    CERT_STATUS_GET_FAILED = 5,
} OH_DRM_CertificateStatus;

/**
 * @brief MediaKeyIds array.
 * @since 11
 * @version 1.0
 */
typedef struct OH_DRM_MediakeyIdArray {
    /* MediaKeyId count. */
    uint32_t mediaKeyIdCount;
    /* MediaKeyIds. */
    OH_DRM_Uint8Buffer mediaKeyIds[0];
} OH_DRM_MediakeyIdArray;

/**
 * @brief Media key info.
 * @since 11
 * @version 1.0
 */
typedef struct OH_DRM_KeysInfo {
    /* Keys count. */
    uint32_t keysCount;
    /* Keys info. */
    OH_DRM_Uint8CharBufferPair *keysInfo;
} OH_DRM_KeysInfo;

/**
 * @brief MediaKeydescription
 * @since 11
 * @version 1.0
 */
typedef struct OH_DRM_MediaKeyDescription {
    /* MediaKeycount. */
    uint32_t mediaKeyCount;
    /* MediaKeyinfo. */
    OH_DRM_EnumBufferPair description[0];
} OH_DRM_MediaKeyDescription;

/**
 * @brief Drm system uuid.
 * @since 11
 * @version 1.0
 */
#define OH_DRM_UUID_LEN 16

/**
 * @brief PSSH info by uuid.
 * @since 11
 * @version 1.0
 */
typedef struct OH_DRM_PsshInfo {
    /* *
     * Uuid.
     */
    char uuid[OH_DRM_UUID_LEN];
    /* *
     * Unsigned char PSSH len.
     */
    uint32_t dataLen;
    /* *
     * Unsigned char PSSH data.
     */
    unsigned char *data;
} OH_DRM_PsshInfo;

/**
 * @brief DrmInfo used for player to get DRM info from media source.
 * @since 11
 * @version 1.0
 */
typedef struct OH_DRM_DrmInfo {
    /* PSSH count. */
    uint32_t psshCount;
    /* PSSH info. */
    OH_DRM_PsshInfo *psshInfo;
} OH_DRM_DrmInfo;

/**
 * @brief Drm info call back.
 * @param drmInfo Drm info gotten from multimedia source.
 * @return void.
 * @since 11
 * @version 1.0
 */
typedef void (*OH_DRM_DrmInfoCallback)(OH_DRM_DrmInfo *drmInfo);

/**
 * @brief Media key system struct.
 * @since 11
 * @version 1.0
 */
typedef struct OH_MediaKeySystem OH_MediaKeySystem;

/**
 * @brief Media key session struct.
 * @since 11
 * @version 1.0
 */
typedef struct OH_MediaKeySession OH_MediaKeySession;

#ifdef __cplusplus
}
#endif

#endif // NATIVE_DRM_COMMON_H