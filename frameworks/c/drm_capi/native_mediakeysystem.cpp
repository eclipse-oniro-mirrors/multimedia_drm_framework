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

#include <mutex>
#include <shared_mutex>
#include <string>
#include <refbase.h>
#include <securec.h>
#include "drm_log.h"
#include "drm_error_code.h"
#include "native_drm_common.h"
#include "native_drm_base.h"
#include "native_drm_object.h"
#include "native_mediakeysystem.h"

using namespace OHOS::DrmStandard;

bool OH_MediaKeySystem_IsSupported(const char *uuid)
{
    DRM_INFO_LOG("OH_MediaKeySystemIsSupportedByUuid enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(uuid != nullptr, false, "MediaKeySystem uuid is nullptr!");
    std::string uuidPtr(uuid);
    bool isSurooprtted = false;
    DRM_CHECK_AND_RETURN_RET_LOG(uuidPtr.size() != 0, false, "MediaKeySystem uuidPtr.size is nullptr!");
    OHOS::sptr<MediaKeySystemFactoryImpl> fatory = MediaKeySystemFactoryImpl::GetInstance();
    isSurooprtted = fatory->IsMediaKeySystemSupported(uuidPtr);
    DRM_INFO_LOG("OH_MediaKeySystemIsSupportedByUuid exit.");
    return isSurooprtted;
}

bool OH_MediaKeySystem_IsSupported2(const char *uuid, const char *mimeType)
{
    DRM_INFO_LOG("OH_MediaKeySystem_IsSupported2 enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(((uuid != nullptr) && (mimeType != nullptr)), false,
        "OH_MediaKeySystem uuid is nullptr!");
    bool isSurooprtted = false;
    std::string uuidPtr(uuid);
    DRM_CHECK_AND_RETURN_RET_LOG(uuidPtr.size() != 0, false, "MediaKeySystem uuidPtr.size is nullptr!");
    std::string mimeTypePtr = std::string(mimeType);
    DRM_CHECK_AND_RETURN_RET_LOG(mimeTypePtr.size() != 0, false, "MediaKeySystem level is nullptr!");

    OHOS::sptr<MediaKeySystemFactoryImpl> fatory = MediaKeySystemFactoryImpl::GetInstance();
    isSurooprtted = fatory->IsMediaKeySystemSupported(uuidPtr, mimeTypePtr);
    DRM_INFO_LOG("OH_MediaKeySystem_IsSupported2 exit.");
    return isSurooprtted;
}

bool OH_MediaKeySystem_IsSupported3(const char *uuid, const char *mimeType,
    DRM_ContentProtectionLevel ContentProtectionLevel)
{
    DRM_INFO_LOG("OH_MediaKeySystem_IsSupported3 enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(((uuid != nullptr) && (mimeType != nullptr)), false,
        "OH_MediaKeySystem uuid is nullptr!");
    bool isSurooprtted = false;
    std::string uuidPtr(uuid);
    DRM_CHECK_AND_RETURN_RET_LOG(uuidPtr.size() != 0, false, "MediaKeySystem Uuid.size is nullptr!");
    std::string mimeTypePtr = std::string(mimeType);
    DRM_CHECK_AND_RETURN_RET_LOG(mimeTypePtr.size() != 0, false, "MediaKeySystem level is nullptr!");

    OHOS::sptr<MediaKeySystemFactoryImpl> fatory = MediaKeySystemFactoryImpl::GetInstance();

    IMediaKeySessionService::ContentProtectionLevel securityLevel =
        (IMediaKeySessionService::ContentProtectionLevel)ContentProtectionLevel;
    if ((securityLevel < IMediaKeySessionService::SECURITY_LEVEL_UNKNOWN) ||
        (securityLevel >= IMediaKeySessionService::SECURITY_LEVEL_MAX)) {
        DRM_ERR_LOG("ContentProtectionLevel is invalid");
        return false;
    }
    isSurooprtted = fatory->IsMediaKeySystemSupported(uuidPtr, mimeTypePtr, securityLevel);
    DRM_INFO_LOG("OH_MediaKeySystem_IsSupported3 exit.");
    return isSurooprtted;
}

Drm_ErrCode OH_MediaKeySystem_Create(const char *name, MediaKeySystem **mediaKeySystem)
{
    DRM_CHECK_AND_RETURN_RET_LOG(((name != nullptr) && (mediaKeySystem != nullptr)), DRM_ERR_INVALID_VAL,
        "name is nullptr!");
    OHOS::sptr<MediaKeySystemFactoryImpl> factory = MediaKeySystemFactoryImpl::GetInstance();
    DRM_CHECK_AND_RETURN_RET_LOG(factory != nullptr, DRM_ERR_INVALID_VAL, "factory is nullptr!");

    std::string nameStr = name;
    DRM_CHECK_AND_RETURN_RET_LOG(nameStr.size() != 0, DRM_ERR_INVALID_VAL, "nameStr.size() is zero");
    OHOS::sptr<OHOS::DrmStandard::MediaKeySystemImpl> system = nullptr;
    factory->CreateMediaKeySystem(nameStr, &system);
    DRM_CHECK_AND_RETURN_RET_LOG(system != nullptr, DRM_ERR_INVALID_VAL, "system create by name failed!");

    struct MediaKeySystemObject *object = new (std::nothrow) MediaKeySystemObject(system);
    DRM_CHECK_AND_RETURN_RET_LOG(object != nullptr, DRM_ERR_INVALID_VAL, "MediaKeySystemObject create failed!");

    object->systemCallback_ = new (std::nothrow) MediaKeySystemCallbackCapi();
    DRM_CHECK_AND_RETURN_RET_LOG(object->systemCallback_ != nullptr, DRM_ERR_NO_MEMORY,
        "MediaKeySystemObject create systemCallback failed!");
    int32_t ret = object->systemImpl_->SetCallback(object->systemCallback_);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_ERR_OK, DRM_ERR_UNKNOWN, "system set callback failed!");

    *mediaKeySystem = object;
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_SetConfigurationString(MediaKeySystem *mediaKeySystem, const char *configName,
    const char *value)
{
    DRM_INFO_LOG("OH_SetConfigurationString enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (configName != nullptr) && (value != nullptr)),
        DRM_ERR_INVALID_VAL, "OH_SetConfigurationString mediaKeySystem is nullptr!");

    int32_t result = DRM_ERR_OK;
    std::string name(configName);
    DRM_CHECK_AND_RETURN_RET_LOG(name.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString configName.size is not zero!");
    std::string valuePtr(value);
    DRM_CHECK_AND_RETURN_RET_LOG(valuePtr.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString value.size is not zero!");

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    result = systemObject->systemImpl_->SetConfigurationString(name, valuePtr);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString mediaKeySystemImpl::SetConfigurationString faild!");

    DRM_INFO_LOG("OH_SetConfigurationString exit.");
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_GetConfigurationString(MediaKeySystem *mediaKeySystem, const char *configName,
    char **value, int32_t *valueLen)
{
    DRM_INFO_LOG("OH_GetConfigurationString enter");
    DRM_CHECK_AND_RETURN_RET_LOG(
        ((mediaKeySystem != nullptr) && (configName != nullptr) && (value != nullptr) && (valueLen != nullptr)),
        DRM_ERR_INVALID_VAL, "OH_GetConfigurationString mediaKeySystem is nullptr!");
    std::string valuePtr;
    int32_t result = DRM_ERR_OK;
    std::string name = std::string(configName);
    DRM_CHECK_AND_RETURN_RET_LOG(name.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString configName.size is not zero!");

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    result = systemObject->systemImpl_->GetConfigurationString(name, valuePtr);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString mediaKeySystemImpl::GetConfigurationString faild!");
    *value = (char *)malloc(valuePtr.size());
    DRM_CHECK_AND_RETURN_RET_LOG(*value != nullptr, DRM_ERR_INVALID_VAL, "OH_SetConfigurationString malloc faild!");
    *valueLen = valuePtr.size();
    if (valuePtr.size() == 0) {
        DRM_DEBUG_LOG("valuePtr.data() is nullptr!");
    }
    int32_t ret = memcpy_s(*value, valuePtr.size(), valuePtr.c_str(), valuePtr.size());
    if (ret != 0) {
        DRM_ERR_LOG("OH_GetConfigurationString memcpy_s faild!");
        free(*value);
        *value = nullptr;
        return DRM_ERR_NO_MEMORY;
    }
    DRM_INFO_LOG("OH_GetConfigurationString exit");
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_SetConfigurationByteArray(MediaKeySystem *mediaKeySystem,
    const char *configName, DRM_Uint8Buffer *value)
{
    DRM_INFO_LOG("OH_SetConfigurationByteArray enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (configName != nullptr) && (value != nullptr)),
        DRM_ERR_INVALID_VAL, "OH_SetConfigurationByteArray mediaKeySystem is nullptr!");

    int32_t result = DRM_ERR_OK;
    std::string name(configName);
    DRM_CHECK_AND_RETURN_RET_LOG(name.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray configName.size is not zero!");
    uint8_t *valueDataPtr = reinterpret_cast<uint8_t *>(value->buffer);
    DRM_CHECK_AND_RETURN_RET_LOG(valueDataPtr != nullptr, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray value is nullptr!");
    std::vector<uint8_t> valueptr(valueDataPtr, valueDataPtr + value->bufferLen);
    DRM_CHECK_AND_RETURN_RET_LOG(valueptr.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray value.size is not zero!");

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    result = systemObject->systemImpl_->SetConfigurationByteArray(name, valueptr);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray mediaKeySystemImpl::SetConfigurationByteArray faild!");
    DRM_INFO_LOG("OH_SetConfigurationByteArray exit.");
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_GetConfigurationByteArray(MediaKeySystem *mediaKeySystem, const char *configName,
    unsigned char **value, int32_t *valueLen)
{
    DRM_INFO_LOG("OH_GetConfigurationByteArray enter");
    DRM_CHECK_AND_RETURN_RET_LOG(
        ((mediaKeySystem != nullptr) && (configName != nullptr) && (value != nullptr) && (valueLen != nullptr)),
        DRM_ERR_INVALID_VAL, "OH_GetConfigurationByteArray mediaKeySystem is nullptr!");

    std::vector<uint8_t> valuePtr;
    int32_t result = DRM_ERR_OK;
    std::string name = std::string(configName);
    DRM_CHECK_AND_RETURN_RET_LOG(name.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationByteArray configName.size is not zero!");

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    result = systemObject->systemImpl_->GetConfigurationByteArray(name, valuePtr);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationByteArray mediaKeySystemImpl::GetConfigurationByteArray faild!");
    *valueLen = valuePtr.size();
    if (valuePtr.size() == 0) {
        DRM_DEBUG_LOG("valuePtr.data() is nullptr!");
        return DRM_ERR_OK;
    }
    *value = (unsigned char *)malloc(valuePtr.size());
    DRM_CHECK_AND_RETURN_RET_LOG(*value != nullptr, DRM_ERR_INVALID_VAL, "OH_GetConfigurationByteArray malloc faild!");
    int32_t ret = memcpy_s(*value, valuePtr.size(), valuePtr.data(), valuePtr.size());
    if (ret != 0) {
        DRM_ERR_LOG("OH_GetConfigurationByteArray memcpy_s faild!");
        return DRM_ERR_NO_MEMORY;
    }
    DRM_INFO_LOG("OH_GetConfigurationByteArray exit");
    return DRM_ERR_OK;
}

static DRM_Statistics *vectorToClist(std::vector<IMediaKeySystemService::MetircKeyValue> &metrics)
{
    DRM_INFO_LOG("vectorToCArray start.");
    int32_t max = sizeof(uint32_t);
    int offset = metrics.size() * sizeof(DRM_CharBufferPair);
    for (size_t i = 0; i < metrics.size(); i++) {
        max += (sizeof(DRM_CharBufferPair) + metrics[i].value.size() + metrics[i].name.size());
    }
    DRM_Statistics *cArray = (DRM_Statistics *)malloc(max);
    DRM_CHECK_AND_RETURN_RET_LOG(cArray != nullptr, nullptr, "malloc faild!");
    cArray->statisticsCount = metrics.size();
    DRM_CharBufferPair *dest = &((cArray->info)[0]);
    for (size_t i = 0; i < metrics.size(); i++) {
        dest[i].name.bufferLen = metrics[i].name.size();
        dest[i].name.buffer = (char *)((uint8_t *)dest + offset);
        int32_t ret =
            memcpy_s(dest[i].name.buffer, metrics[i].name.size(), metrics[i].name.c_str(), metrics[i].name.size());
        if (ret != 0) {
            DRM_ERR_LOG(" memcpy_s faild!");
            return nullptr;
        }
        offset += metrics[i].name.size();
        dest[i].value.bufferLen = metrics[i].value.size();
        dest[i].value.buffer = (char *)((uint8_t *)dest + offset);
        ret =
            memcpy_s(dest[i].value.buffer, metrics[i].value.size(), metrics[i].value.c_str(), metrics[i].value.size());
        if (ret != 0) {
            DRM_ERR_LOG(" memcpy_s faild!");
            return nullptr;
        }
        offset += metrics[i].value.size();
    }
    DRM_INFO_LOG("vectorToCArray exit.");
    return cArray;
}

Drm_ErrCode OH_MediaKeySystem_GetStatistics(MediaKeySystem *mediaKeySystem, DRM_Statistics **statistics)
{
    DRM_INFO_LOG("OH_MediaKeySystem_GetStatistics enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (statistics != nullptr)), DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetStatistics mediaKeySystem is nullptr!");
    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    std::vector<IMediaKeySystemService::MetircKeyValue> metrics;
    int32_t result = systemObject->systemImpl_->GetStatistics(metrics);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetStatistics systemObject is nullptr!");
    *(statistics) = vectorToClist(metrics);
    DRM_CHECK_AND_RETURN_RET_LOG(*statistics != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetStatistics *DRM_Statistics is nullptr!");
    DRM_INFO_LOG("OH_MediaKeySystem_GetStatistics exit.");
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_GetMaxContentProtectionLevel(MediaKeySystem *mediaKeySystem,
    DRM_ContentProtectionLevel *contentProtectionLevel)
{
    DRM_INFO_LOG("GetMaxContentProtectionLevel enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (contentProtectionLevel != nullptr)),
        DRM_ERR_INVALID_VAL, "OH_GetMaxContentProtectionLevel mediaKeySystem is nullptr!");
    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    int32_t result = DRM_ERR_OK;
    IMediaKeySessionService::ContentProtectionLevel level = IMediaKeySessionService::SECURITY_LEVEL_UNKNOWN;
    result = systemObject->systemImpl_->GetMaxContentProtectionLevel(&level);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL, "OH_GetMaxContentProtectionLevel  fail!");
    if (level < IMediaKeySessionService::SECURITY_LEVEL_UNKNOWN ||
        level > IMediaKeySessionService::SECURITY_LEVEL_MAX) {
        DRM_ERR_LOG("mediaKeySystemImpl::GetMaxContentProtectionLevel faild!");
        return DRM_ERR_INVALID_VAL;
    }
    *contentProtectionLevel = static_cast<DRM_ContentProtectionLevel>(level);
    DRM_INFO_LOG("OH_GetMaxContentProtectionLevel exit");
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_GenerateKeySystemRequest(MediaKeySystem *mediaKeySystem, unsigned char **request,
    int32_t *requestLen, char **defaultUrl, int32_t *defaultUrlLen)
{
    DRM_INFO_LOG("OH_MediaKeySystem_GenerateKeySystemRequest enter");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (request != nullptr) && (requestLen != nullptr) &&
        (defaultUrl != nullptr) && (defaultUrlLen != nullptr)),
        DRM_ERR_INVALID_VAL, "OH_MediaKeySystem_GenerateKeySystemRequest mediaKeySystem is nullptr!");
    std::vector<uint8_t> requestData;
    std::string defaultUrlData;

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    int32_t result = systemObject->systemImpl_->GenerateKeySystemRequest(requestData, defaultUrlData);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GenerateKeySystemRequest mediaKeySystem is nullptr!");
    *request = (unsigned char *)malloc(requestData.size());
    DRM_CHECK_AND_RETURN_RET_LOG(*request != nullptr, DRM_ERR_INVALID_VAL, "malloc faild!");
    int32_t ret = memcpy_s(*request, requestData.size(), requestData.data(), requestData.size());
    if (ret != 0) {
        DRM_DEBUG_LOG(" requestData.data() is nullptr!");
    }
    *requestLen = requestData.size();
    *defaultUrl = (char *)malloc(defaultUrlData.size());
    DRM_CHECK_AND_RETURN_RET_LOG(*defaultUrl != nullptr, DRM_ERR_INVALID_VAL, "malloc faild!");
    ret = memcpy_s(*defaultUrl, defaultUrlData.size(), defaultUrlData.data(), defaultUrlData.size());
    if (ret != 0) {
        DRM_DEBUG_LOG(" defaultUrlData.data() is nullptr!");
    }
    *defaultUrlLen = defaultUrlData.size();
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_ProcessKeySystemResponse(MediaKeySystem *mediaKeySystem, uint8_t *reponseData,
    size_t reponseDataLen)
{
    DRM_INFO_LOG("OH_ProcessKeySystemResponse enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (reponseData != nullptr) && (reponseDataLen != 0)),
        DRM_ERR_INVALID_VAL, "OH_ProcessKeySystemResponse mediaKeySystem is nullptr!");
    int32_t result = DRM_ERR_OK;
    std::vector<uint8_t> keySystemResponse(reponseData, reponseData + reponseDataLen);
    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    result = systemObject->systemImpl_->ProcessKeySystemResponse(keySystemResponse);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_ProcessKeySystemResponse systemObject is nullptr!");
    DRM_INFO_LOG("OH_ProcessKeySystemResponse exit.");
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_GetCertificateStatus(MediaKeySystem *mediaKeySystem, DRM_CertificateStatus *certStatus)
{
    DRM_INFO_LOG("OH_GetCertificateStatus enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (certStatus != nullptr)), DRM_ERR_INVALID_VAL,
        "OH_ProcessKeySystemResponse mediaKeySystem is nullptr!");
    IMediaKeySystemService::CertificateStatus CertStatus;
    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    int32_t result = systemObject->systemImpl_->GetCertificateStatus(&CertStatus);
    if (result != DRM_ERR_OK) {
        *certStatus = CERT_STATUS_UNAVAILABLE;
        DRM_ERR_LOG("OH_MediaKeySystem_GetCertificateStatus faild!");
        return DRM_ERR_INVALID_VAL;
    }
    *certStatus = (DRM_CertificateStatus)((int32_t)(CertStatus));
    if (*certStatus < CERT_STATUS_PROVISIONED || *certStatus > CERT_STATUS_UNAVAILABLE) {
        DRM_ERR_LOG("OH_MediaKeySystem_GetCertificateStatus faild!");
        *certStatus = CERT_STATUS_UNAVAILABLE;
        return DRM_ERR_INVALID_VAL;
    }
    DRM_INFO_LOG("OH_MediaKeySystem_GetCertificateStatus exit.");
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_SetMediaKeySystemCallback(MediaKeySystem *mediaKeySystem,
    MediaKeySystem_Callback callback)
{
    DRM_INFO_LOG("OH_MediaKeySystem_SetMediaKeySystemCallback enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (callback != nullptr)), DRM_ERR_INVALID_VAL,
        "mediaKeySystem is nullptr!");
    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    systemObject->systemCallback_->SetCallbackReference(callback);
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_CreateMediaKeySession(MediaKeySystem *mediaKeySystem, DRM_ContentProtectionLevel *level,
    MediaKeySession **mediaKeySession)
{
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (level != nullptr) && (mediaKeySession != nullptr) &&
        (*level > CONTENT_PROTECTION_LEVEL_UNKNOWN) && (*level <= CONTENT_PROTECTION_LEVEL_MAX)),
        DRM_ERR_INVALID_VAL, "mediaKeySystem is nullptr!");
    struct MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);

    Drm_ErrCode retCode = DRM_ERR_OK;
    int32_t secure = static_cast<int32_t>(*level);
    IMediaKeySessionService::ContentProtectionLevel secureLevel =
        static_cast<IMediaKeySessionService::ContentProtectionLevel>(secure);
    OHOS::sptr<MediaKeySessionImpl> keySessionImpl = nullptr;
    int32_t ret = systemObject->systemImpl_->CreateMediaKeySession(secureLevel, &keySessionImpl);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_ERR_OK, DRM_ERR_UNKNOWN, "session create return failed!");
    DRM_CHECK_AND_RETURN_RET_LOG(keySessionImpl != nullptr, DRM_ERR_UNKNOWN, "session create failed!");

    struct MediaKeySessionObject *sessionObject = new (std::nothrow) MediaKeySessionObject(keySessionImpl);
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject != nullptr, DRM_ERR_NO_MEMORY, "MediaKeySessionObject create failed!");

    sessionObject->sessionCallback_ = new (std::nothrow) MediaKeySessionCallbackCapi();
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject->sessionCallback_ != nullptr, DRM_ERR_NO_MEMORY,
        "MediaKeySessionObject create sessionCallback failed!");
    ret = sessionObject->sessionImpl_->SetCallback(sessionObject->sessionCallback_);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_ERR_OK, DRM_ERR_UNKNOWN, "session set callback failed!");

    *mediaKeySession = static_cast<MediaKeySession *>(sessionObject);
    return retCode;
}

static DRM_MediakeyIdArray *vectorToC2DArray(std::vector<std::vector<uint8_t>> licenseIds)
{
    int32_t max = sizeof(uint32_t);
    int offset = licenseIds.size() * sizeof(DRM_Uint8Buffer);
    for (size_t i = 0; i < licenseIds.size(); i++) {
        max += (sizeof(DRM_MediakeyIdArray) + licenseIds[i].size() + licenseIds[i].size());
    }
    DRM_MediakeyIdArray *cArray = (DRM_MediakeyIdArray *)malloc(max);
    DRM_CHECK_AND_RETURN_RET_LOG(cArray != nullptr, nullptr, "malloc failed!");
    cArray->mediaKeyIdCount = licenseIds.size();
    DRM_Uint8Buffer *dest = &((cArray->mediaKeyIds)[0]);
    for (size_t i = 0; i < licenseIds.size(); i++) {
        dest[i].bufferLen = licenseIds[i].size();
        dest[i].buffer = ((uint8_t *)dest + offset);
        int32_t ret = memcpy_s(dest[i].buffer, licenseIds[i].size(), licenseIds[i].data(), licenseIds[i].size());
        if (ret != 0) {
            DRM_ERR_LOG(" memcpy_s faild!");
            return nullptr;
        }
        offset += licenseIds[i].size();
    }
    DRM_INFO_LOG("vectorToC2DArray exit.");
    return cArray;
}

Drm_ErrCode OH_MediaKeySystem_GetOfflineMediaKeyIds(MediaKeySystem *mediaKeySystem, DRM_MediakeyIdArray **mediaKeyIds)
{
    DRM_INFO_LOG("OH_MediaKeySystem_GetOfflineMediaKeyIds enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (mediaKeyIds != nullptr)), DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetOfflineMediaKeyIds mediaKeySystem is nullptr!");
    std::vector<std::vector<uint8_t>> licenseIds;

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    int32_t result = systemObject->systemImpl_->GetOfflineMediaKeyIds(licenseIds);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetOfflineMediaKeyIds faild!");
    if (licenseIds.size() == 0) {
        DRM_DEBUG_LOG("licenseIds.data() is nullptr!");
        return DRM_ERR_OK;
    }
    *mediaKeyIds = vectorToC2DArray(licenseIds);
    DRM_INFO_LOG("OH_MediaKeySystem_GetOfflineMediaKeyIds exit.");
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_GetOfflineMediaKeyStatus(MediaKeySystem *mediaKeySystem, DRM_Uint8Buffer *mediaKeyId,
    DRM_OfflineMediaKeyStatus *status)
{
    DRM_INFO_LOG("OH_MediaKeySystem_GetOfflineMediaKeyStatus enter");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (mediaKeyId != nullptr) && (mediaKeyId->buffer) &&
        (mediaKeyId->bufferLen != 0) && (status != nullptr)),
        DRM_ERR_INVALID_VAL, "OH_MediaKeySystem_GetOfflineMediaKeyStatus mediaKeySystem is nullptr!");
    int32_t result = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;

    std::vector<uint8_t> licenseIdVec(mediaKeyId->buffer, mediaKeyId->buffer + mediaKeyId->bufferLen);
    IMediaKeySessionService::OfflineMediaKeyStatus offlineMediaKeyStatus =
        IMediaKeySessionService::OFFLINELICENSESTATUS_UNKNOWN;

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    result = systemObject->systemImpl_->GetOfflineMediaKeyStatus(licenseIdVec, offlineMediaKeyStatus);
    if (result != DRM_ERR_OK) {
        DRM_ERR_LOG("OH_MediaKeySystem_GetOfflineMediaKeyStatus faild!");
        return DRM_ERR_INVALID_VAL;
    }
    DRM_OfflineMediaKeyStatus CofflineMediaKeyStatus = (DRM_OfflineMediaKeyStatus)((int32_t)(offlineMediaKeyStatus));
    if (CofflineMediaKeyStatus < OFFLINE_MEDIA_KEY_STATUS_UNKNOWN ||
        CofflineMediaKeyStatus > OFFLINE_MEDIA_KEY_STATUS_INACTIVE) {
        DRM_ERR_LOG("OH_MediaKeySystem_GetOfflineMediaKeyStatus faild!");
        return DRM_ERR_INVALID_VAL;
    }
    *status = CofflineMediaKeyStatus;
    DRM_INFO_LOG("OH_MediaKeySystem_GetOfflineMediaKeyStatus exit");
    return DRM_ERR_OK;
}


Drm_ErrCode OH_MediaKeySystem_ClearOfflineMediaKeys(MediaKeySystem *mediaKeySystem, DRM_Uint8Buffer *mediaKeyId)
{
    DRM_INFO_LOG("OH_RemoveOfflineLMediaKey enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (mediaKeyId != nullptr) &&
        (mediaKeyId->buffer != nullptr) && (mediaKeyId->bufferLen != 0)),
        DRM_ERR_INVALID_VAL, "OH_GetOfflineMediaKeyStatus mediaKeySystem is nullptr!");
    int32_t result = DRM_ERR_OK;
    std::vector<uint8_t> licenseIdVec(mediaKeyId->buffer, mediaKeyId->buffer + mediaKeyId->bufferLen);
    DRM_CHECK_AND_RETURN_RET_LOG(licenseIdVec.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_RemoveOfflineLMediaKey configName.size is not zero!");
    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    result = systemObject->systemImpl_->ClearOfflineMediaKeys(licenseIdVec);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_RemoveOfflineLMediaKey mediaKeySystemImpl::ClearOfflineMediaKeys faild!");
    DRM_INFO_LOG("OH_RemoveOfflineLMediaKey exit.");
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_Destroy(MediaKeySystem *mediaKeySystem)
{
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL, "mediaKeySystem is nullptr!");

    struct MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);

    int32_t ret = systemObject->systemImpl_->Release();
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_ERR_OK, DRM_ERR_INVALID_VAL, "call media key system release failed!");
    delete mediaKeySystem;
    return DRM_ERR_OK;
}