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
#include <securec.h>
#include "drm_log.h"
#include "drm_error_code.h"
#include "native_drm_common.h"
#include "native_drm_base.h"
#include "native_drm_object.h"
#include "native_mediakeysystem.h"

using namespace OHOS::DrmStandard;

OH_DrmBool OH_MediaKeySystem_IsSupported(const char *uuid)
{
    DRM_INFO_LOG("OH_MediaKeySystemIsSupportedByUuid enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(uuid != nullptr, DRM_BOOL_FALSE, "OH_MediaKeySystem uuid is nullptr!");
    std::string Uuid(uuid);
    OH_DrmBool IsSurooprtted = DRM_BOOL_FALSE;
    DRM_CHECK_AND_RETURN_RET_LOG(Uuid.size() != 0, DRM_BOOL_FALSE, "OH_MediaKeySystem Uuid.size is nullptr!");
    OHOS::sptr<MediaKeySystemFactoryImpl> fatory = MediaKeySystemFactoryImpl::GetInstance();
    DRM_CHECK_AND_RETURN_RET_LOG(fatory != nullptr, DRM_BOOL_FALSE, "OH_MediaKeySystem fatory is nullptr!");
    bool isSurpportted = fatory->IsMediaKeySystemSupported(Uuid);
    if (isSurpportted) {
        IsSurooprtted = DRM_BOOL_TRUE;
    }
    DRM_INFO_LOG("OH_MediaKeySystemIsSupportedByUuid exit.");
    return IsSurooprtted;
}

OH_DrmBool OH_MediaKeySystem_IsSupported2(const char *uuid, const char *mimeType)
{
    DRM_INFO_LOG("OH_MediaKeySystem_IsSupported2 enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(uuid != nullptr, DRM_BOOL_FALSE, "OH_MediaKeySystem uuid is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mimeType != nullptr, DRM_BOOL_FALSE, "OH_MediaKeySystem uuid is nullptr!");
    OH_DrmBool IsSurpportted = DRM_BOOL_FALSE;
    std::string Uuid(uuid);
    DRM_CHECK_AND_RETURN_RET_LOG(Uuid.size() != 0, DRM_BOOL_FALSE, "OH_MediaKeySystem Uuid.size is nullptr!");
    std::string mimeTypePtr = std::string(mimeType);
    DRM_CHECK_AND_RETURN_RET_LOG(mimeTypePtr.size() != 0, DRM_BOOL_FALSE, "OH_MediaKeySystem level is nullptr!");

    OHOS::sptr<MediaKeySystemFactoryImpl> fatory = MediaKeySystemFactoryImpl::GetInstance();
    DRM_CHECK_AND_RETURN_RET_LOG(fatory != nullptr, DRM_BOOL_FALSE, "OH_MediaKeySystem fatory is nullptr!");
    bool isSurpportted = fatory->IsMediaKeySystemSupported(Uuid, mimeTypePtr);
    if (isSurpportted) {
        IsSurpportted = DRM_BOOL_TRUE;
    }
    DRM_INFO_LOG("OH_MediaKeySystem_IsSupported2 exit.");
    return IsSurpportted;
}

OH_DrmBool OH_MediaKeySystem_IsSupported3(const char *uuid, const char *mimeType,
    OH_DRM_ContentProtectionLevel cSecurityLevel)
{
    DRM_INFO_LOG("OH_MediaKeySystem_IsSupported3 enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(uuid != nullptr, DRM_BOOL_FALSE, "OH_MediaKeySystem uuid is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mimeType != nullptr, DRM_BOOL_FALSE, "OH_MediaKeySystem uuid is nullptr!");
    OH_DrmBool IsSurpportted = DRM_BOOL_FALSE;
    std::string uuidPtr(uuid);
    DRM_CHECK_AND_RETURN_RET_LOG(uuidPtr.size() != 0, DRM_BOOL_FALSE, "OH_MediaKeySystem Uuid.size is nullptr!");
    std::string mimeTypePtr = std::string(mimeType);
    DRM_CHECK_AND_RETURN_RET_LOG(mimeTypePtr.size() != 0, DRM_BOOL_FALSE, "OH_MediaKeySystem level is nullptr!");

    OHOS::sptr<MediaKeySystemFactoryImpl> fatory = MediaKeySystemFactoryImpl::GetInstance();
    DRM_CHECK_AND_RETURN_RET_LOG(fatory != nullptr, DRM_BOOL_FALSE, "OH_MediaKeySystem fatory is nullptr!");

    IMediaKeySessionService::SecurityLevel securityLevel = (IMediaKeySessionService::SecurityLevel)cSecurityLevel;
    if ((securityLevel < IMediaKeySessionService::SECURITY_LEVEL_UNKNOWN) ||
        (securityLevel >= IMediaKeySessionService::SECURITY_LEVEL_MAX)) {
        DRM_ERR_LOG("cSecurityLevel is invalid");
        return DRM_BOOL_FALSE;
    }
    bool isSurpportted = fatory->IsMediaKeySystemSupported(uuidPtr, mimeTypePtr, securityLevel);
    if (isSurpportted) {
        IsSurpportted = DRM_BOOL_TRUE;
    }
    DRM_INFO_LOG("OH_MediaKeySystem_IsSupported3 exit.");
    return IsSurpportted;
}

OH_DrmErrCode OH_MediaKeySystem_Create(const char *name, OH_MediaKeySystem **mediaKeySystem)
{
    DRM_CHECK_AND_RETURN_RET_LOG(name != nullptr, DRM_ERR_INVALID_VAL, "name is nullptr!");

    OHOS::sptr<MediaKeySystemFactoryImpl> factory = MediaKeySystemFactoryImpl::GetInstance();
    DRM_CHECK_AND_RETURN_RET_LOG(factory != nullptr, DRM_ERR_INVALID_VAL, "factory is nullptr!");

    std::string nameStr = name;
    OHOS::sptr<OHOS::DrmStandard::MediaKeySystemImpl> system = nullptr;
    factory->CreateMediaKeySystem(nameStr, &system);
    DRM_CHECK_AND_RETURN_RET_LOG(system != nullptr, DRM_ERR_INVALID_VAL, "system create by name failed!");

    struct MediaKeySystemObject *object = new (std::nothrow) MediaKeySystemObject(system);
    DRM_CHECK_AND_RETURN_RET_LOG(object != nullptr, DRM_ERR_INVALID_VAL, "MediaKeySystemObject create failed!");
    *mediaKeySystem = object;

    return DRM_ERR_OK;
}

OH_DrmErrCode OH_MediaKeySystem_SetConfigurationString(OH_MediaKeySystem *mediaKeySystem, const char *configName,
    const char *value)
{
    DRM_INFO_LOG("OH_SetConfigurationString enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(configName != nullptr, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString configName is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(value != nullptr, DRM_ERR_INVALID_VAL, "OH_SetConfigurationString value is nullptr!");

    int32_t result = DRM_ERR_ERROR;
    std::string name(configName);
    DRM_CHECK_AND_RETURN_RET_LOG(name.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString configName.size is not zero!");
    std::string valuePtr(value);
    DRM_CHECK_AND_RETURN_RET_LOG(valuePtr.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString value.size is not zero!");

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString systemObject is nullptr!");
    result = systemObject->systemImpl_->SetConfigurationString(name, valuePtr);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString mediaKeySystemImpl::SetConfigurationString faild!");

    DRM_INFO_LOG("OH_SetConfigurationString exit.");
    return DRM_ERR_OK;
}

OH_DrmErrCode OH_MediaKeySystem_GetConfigurationString(OH_MediaKeySystem *mediaKeySystem, const char *configName,
    char *value)
{
    DRM_INFO_LOG("OH_GetConfigurationString enter");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationString mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(configName != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationString configName is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(value != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationString valuePtr is nullptr!");
    memset_s(value, strlen(value), 0, strlen(value));
    std::string valuePtr;
    int32_t result = DRM_ERR_ERROR;
    std::string name = std::string(configName);
    DRM_CHECK_AND_RETURN_RET_LOG(name.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString configName.size is not zero!");

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationString systemObject is nullptr!");
    result = systemObject->systemImpl_->GetConfigurationString(name, valuePtr);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString mediaKeySystemImpl::GetConfigurationString faild!");
    int32_t ret = memcpy_s(value, valuePtr.size(), valuePtr.c_str(), valuePtr.size());
    if (ret != 0) {
        DRM_ERR_LOG("OH_GetConfigurationString memcpy_s faild!");
        return DRM_ERR_ERROR;
    }
    DRM_INFO_LOG("OH_GetConfigurationString exit");
    return DRM_ERR_OK;
}

OH_DrmErrCode OH_MediaKeySystem_SetConfigurationByteArray(OH_MediaKeySystem *mediaKeySystem, const char *configName,
    unsigned char *value, uint32_t valueLen)
{
    DRM_INFO_LOG("OH_SetConfigurationByteArray enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(configName != nullptr, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray configName is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(value != nullptr, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray value is nullptr!");

    int32_t result = DRM_ERR_ERROR;
    std::string name(configName);
    DRM_CHECK_AND_RETURN_RET_LOG(name.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray configName.size is not zero!");
    uint8_t *valueDataPtr = reinterpret_cast<uint8_t *>(value);
    DRM_CHECK_AND_RETURN_RET_LOG(valueDataPtr != nullptr, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray value is nullptr!");
    std::vector<uint8_t> valueptr(valueDataPtr, valueDataPtr + valueLen);
    DRM_CHECK_AND_RETURN_RET_LOG(valueptr.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray value.size is not zero!");

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray systemObject is nullptr!");
    result = systemObject->systemImpl_->SetConfigurationByteArray(name, valueptr);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray mediaKeySystemImpl::SetConfigurationByteArray faild!");
    DRM_INFO_LOG("OH_SetConfigurationByteArray exit.");
    return DRM_ERR_OK;
}

OH_DrmErrCode OH_MediaKeySystem_GetConfigurationByteArray(OH_MediaKeySystem *mediaKeySystem, const char *configName,
    unsigned char *value, uint32_t *valueLen)
{
    DRM_INFO_LOG("OH_GetConfigurationByteArray enter");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationByteArray mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(configName != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationByteArray configName is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(value != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationByteArray valuePtr is nullptr!");

    std::vector<uint8_t> valuePtr;
    int32_t result = DRM_ERR_ERROR;
    std::string name = std::string(configName);
    DRM_CHECK_AND_RETURN_RET_LOG(name.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationByteArray configName.size is not zero!");

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationString systemObject is nullptr!");
    result = systemObject->systemImpl_->GetConfigurationByteArray(name, valuePtr);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationByteArray mediaKeySystemImpl::GetConfigurationByteArray faild!");
    int32_t ret = memcpy_s(value, valuePtr.size(), valuePtr.data(), valuePtr.size());
    if (ret != 0) {
        DRM_ERR_LOG("OH_GetConfigurationByteArray memcpy_s faild!");
        return DRM_ERR_ERROR;
    }
    *valueLen = valuePtr.size();
    DRM_INFO_LOG("OH_GetConfigurationByteArray exit");
    return DRM_ERR_OK;
}

static OH_DRM_Statistics *vectorToClist(std::vector<IMediaKeySystemService::MetircKeyValue> &metrics)
{
    DRM_INFO_LOG("vectorToCArray start.");
    int32_t max = sizeof(uint32_t);
    int offset = metrics.size() * sizeof(OH_DRM_CharBufferPair);
    for (size_t i = 0; i < metrics.size(); i++) {
        max += (sizeof(OH_DRM_CharBufferPair) + metrics[i].value.size() + metrics[i].name.size());
    }
    OH_DRM_Statistics *cArray = (OH_DRM_Statistics *)malloc(max);
    cArray->statisticsCount = metrics.size();
    OH_DRM_CharBufferPair *dest = &((cArray->info)[0]);
    for (size_t i = 0; i < metrics.size(); i++) {
        dest[i].name.bufferLen = metrics[i].name.size();
        dest[i].name.buffer = (char *)((uint8_t *)dest + offset);
        int32_t ret = memcpy_s(dest[i].name.buffer, metrics[i].name.size(), metrics[i].name.c_str(), metrics[i].name.size());
        offset += metrics[i].name.size();
        dest[i].value.bufferLen = metrics[i].value.size();
        dest[i].value.buffer = (char *)((uint8_t *)dest + offset);
        int32_t ret = memcpy_s(dest[i].value.buffer, metrics[i].value.size(), metrics[i].value.c_str(), metrics[i].value.size());
        offset += metrics[i].value.size();
    }
    DRM_INFO_LOG("vectorToCArray exit.");
    return cArray;
}

OH_DrmErrCode OH_MediaKeySystem_GetStatistics(OH_MediaKeySystem *mediaKeySystem, OH_DRM_Statistics **statistics)
{
    DRM_INFO_LOG("OH_MediaKeySystem_GetStatistics enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetStatistics mediaKeySystem is nullptr!");
    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetStatistics systemObject is nullptr!");
    std::vector<IMediaKeySystemService::MetircKeyValue> metrics;
    int32_t result = systemObject->systemImpl_->GetMetrics(metrics);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetStatistics systemObject is nullptr!");
    *(statistics) = vectorToClist(metrics);
    DRM_CHECK_AND_RETURN_RET_LOG(*statistics != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetStatistics *OH_DRM_Statistics is nullptr!");
    DRM_INFO_LOG("OH_MediaKeySystem_GetStatistics exit.");
    return DRM_ERR_OK;
}

OH_SecurityLevel OH_MediaKeySystem_GetMaxContentProtectionLevel(OH_MediaKeySystem *mediaKeySystem)
{
    DRM_INFO_LOG("GetMaxSecurityLevel enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, SECURITY_LEVEL_UNKNOWN,
        "OH_GetMaxSecurityLevel mediaKeySystem is nullptr!");
    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);

    IMediaKeySessionService::SecurityLevel level = IMediaKeySessionService::SECURITY_LEVEL_UNKNOWN;
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, SECURITY_LEVEL_UNKNOWN,
        "OH_GetMaxSecurityLevel systemObject is nullptr!");
    systemObject->systemImpl_->GetMaxSecurityLevel(&level);
    if (level < IMediaKeySessionService::SECURITY_LEVEL_UNKNOWN ||
        level > IMediaKeySessionService::SECURITY_LEVEL_MAX) {
        DRM_ERR_LOG("mediaKeySystemImpl::GetMaxSecurityLevel faild!");
        return SECURITY_LEVEL_UNKNOWN;
    }
    OH_SecurityLevel SecurityLevel = static_cast<OH_SecurityLevel>(level);
    DRM_INFO_LOG("OH_GetMaxSecurityLevel exit");
    return SecurityLevel;
}

OH_DrmErrCode OH_MediaKeySystem_GenerateKeySystemRequest(OH_MediaKeySystem *mediaKeySystem, unsigned char **request,
    int32_t *requestLen, char **defaultUrl, int32_t *defaultUrlLen)
{
    DRM_INFO_LOG("OH_MediaKeySystem_GenerateKeySystemRequest enter");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GenerateKeySystemRequest mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(requestLen != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GenerateKeySystemRequest mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(defaultUrlLen != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GenerateKeySystemRequest mediaKeySystem is nullptr!");
    std::vector<uint8_t> requestData;
    std::string defaultUrlData;

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GenerateKeySystemRequest systemObject is nullptr!");
    int32_t result = systemObject->systemImpl_->GenerateKeySystemRequest(requestData, defaultUrlData);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GenerateKeySystemRequest mediaKeySystem is nullptr!");
    *request = (unsigned char *)malloc(requestData.size());
    int32_t ret = memcpy_s(*request, requestData.size(), requestData.size(), requestData.data(), requestData.size());
    *requestLen = requestData.size();
    *defaultUrl = (char *)malloc(defaultUrlData.size());
    int32_t ret = memcpy_s(*defaultUrl, defaultUrlData.size(), defaultUrlData.size(), defaultUrlData.data(), defaultUrlData.size());
    *defaultUrlLen = defaultUrlData.size();
    return DRM_ERR_OK;
}

OH_DrmErrCode OH_MediaKeySystem_ProcessKeySystemResponse(OH_MediaKeySystem *mediaKeySystem, uint8_t *reponseData,
    size_t reponseDataLen)
{
    DRM_INFO_LOG("OH_ProcessKeySystemResponse enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_ProcessKeySystemResponse mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(reponseData != nullptr, DRM_ERR_INVALID_VAL,
        "OH_ProcessKeySystemResponse reponseData is nullptr!");

    int32_t result = DRM_ERR_ERROR;
    DRM_CHECK_AND_RETURN_RET_LOG(reponseData != nullptr, DRM_ERR_INVALID_VAL,
        "OH_ProcessKeySystemResponse reponseData is nullptr!");
    std::vector<uint8_t> keySystemResponse(reponseData, reponseData + reponseDataLen);
    DRM_CHECK_AND_RETURN_RET_LOG(keySystemResponse.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_ProcessKeySystemResponse keySystemResponse.size is zero!");

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_ProcessKeySystemResponse systemObject is nullptr!");
    result = systemObject->systemImpl_->ProcessKeySystemResponse(keySystemResponse);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_ProcessKeySystemResponse systemObject is nullptr!");
    DRM_INFO_LOG("OH_ProcessKeySystemResponse exit.");
    return DRM_ERR_OK;
}

OH_DRM_CertificateStatus OH_MediaKeySystem_GetCertificateStatus(OH_MediaKeySystem *mediaKeySystem)
{
    DRM_INFO_LOG("OH_GetCertificateStatus enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, CERT_STATUS_GET_FAILED,
        "OH_ProcessKeySystemResponse mediaKeySystem is nullptr!");
    IMediaKeySystemService::CertificateStatus certStatus;
    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, CERT_STATUS_GET_FAILED,
        "OH_GetCertificateStatus systemObject is nullptr!");
    int32_t result = systemObject->systemImpl_->GetCertificateStatus(&certStatus);
    if (result != DRM_ERR_OK) {
        DRM_ERR_LOG("OH_MediaKeySystem_GetCertificateStatus faild!");
        return CERT_STATUS_GET_FAILED;
    }
    OH_DRM_CertificateStatus CertStatus = (OH_DRM_CertificateStatus)((int32_t)(certStatus));
    if (CertStatus < CERT_STATUS_PROVISIONED || CertStatus > CERT_STATUS_GET_FAILED) {
        DRM_ERR_LOG("OH_MediaKeySystem_GetCertificateStatus faild!");
        return CERT_STATUS_GET_FAILED;
    }
    DRM_INFO_LOG("OH_MediaKeySystem_GetCertificateStatus exit.");
    return CertStatus;
}

OH_DrmErrCode OH_MediaKeySystem_SetMediaKeySystemCallback(OH_MediaKeySystem *mediaKeySystem,
    OH_MediaKeySystemCallback *callback)
{
    return DRM_ERR_OK;
}

OH_DrmErrCode OH_MediaKeySystem_CreateMediaKeySession(OH_MediaKeySystem *mediaKeySystem,
    OH_DRM_ContentProtectionLevel *level, OH_MediaKeySession **mediaKeySession)
{
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL, "mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(level != nullptr, DRM_ERR_INVALID_VAL, "level is nullptr!");

    struct MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL, "systemObject is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemImpl_ != nullptr, DRM_ERR_INVALID_VAL, "systemImpl_ is nullptr!");

    OH_DrmErrCode retCode = DRM_ERR_OK;
    int32_t secure = static_cast<int32_t>(*level);
    IMediaKeySessionService::SecurityLevel secureLevel = static_cast<IMediaKeySessionService::SecurityLevel>(secure);
    OHOS::sptr<MediaKeySessionImpl> keySessionImpl = nullptr;
    int32_t ret = systemObject->systemImpl_->CreateMediaKeySession(secureLevel, &keySessionImpl);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_ERR_OK, DRM_ERR_UNKNOWN, "session create return failed!");
    DRM_CHECK_AND_RETURN_RET_LOG(keySessionImpl != nullptr, DRM_ERR_UNKNOWN, "session create failed!");

    struct MediaKeySessionObject *object = new (std::nothrow) MediaKeySessionObject(keySessionImpl);
    DRM_CHECK_AND_RETURN_RET_LOG(object != nullptr, DRM_ERR_NO_MEMORY, "MediaKeySessionObject create failed!");
    *mediaKeySession = static_cast<OH_MediaKeySession *>(object);
    return retCode;
}

static OH_DRM_MediakeyIdArray *vectorToC2DArray(std::vector<std::vector<uint8_t>> licenseIds)
{
    DRM_INFO_LOG("vectorToC2DArray enter.");
    int32_t max = sizeof(uint32_t);
    int offset = licenseIds.size() * sizeof(OH_DRM_Uint8Buffer);
    for (size_t i = 0; i < licenseIds.size(); i++) {
        max += (sizeof(OH_DRM_MediakeyIdArray) + licenseIds[i].size() + licenseIds[i].size());
    }
    OH_DRM_MediakeyIdArray *cArray = (OH_DRM_MediakeyIdArray *)malloc(max);
    cArray->mediaKeyIdCount = licenseIds.size();
    OH_DRM_Uint8Buffer *dest = &((cArray->mediaKeyIds)[0]);
    for (size_t i = 0; i < licenseIds.size(); i++) {
        dest[i].bufferLen = licenseIds[i].size();
        dest[i].buffer = ((uint8_t *)dest + offset);
        int32_t ret = memcpy_s(dest[i].buffer, licenseIds[i].size(), licenseIds[i].data(), licenseIds[i].size());
        offset += licenseIds[i].size();
    }
    DRM_INFO_LOG("vectorToC2DArray exit.");
    return cArray;
}

OH_DrmErrCode OH_MediaKeySystem_GetOfflineMediaKeyIds(OH_MediaKeySystem *mediaKeySystem,
    OH_DRM_MediakeyIdArray **mediaKeyIds)
{
    DRM_INFO_LOG("OH_MediaKeySystem_GetOfflineMediaKeyIds enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetOfflineMediaKeyIds mediaKeySystem is nullptr!");
    std::vector<std::vector<uint8_t>> licenseIds;

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetCertificateStatus systemObject is nullptr!");
    int32_t result = systemObject->systemImpl_->GetOfflineLicenseIds(licenseIds);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetOfflineMediaKeyIds faild!");
    *mediaKeyIds = vectorToC2DArray(licenseIds);
    DRM_INFO_LOG("OH_MediaKeySystem_GetOfflineMediaKeyIds exit.");
    return DRM_ERR_OK;
}

OH_DrmErrCode OH_MediaKeySystem_GetOfflineMediaKeyStatus(OH_MediaKeySystem *mediaKeySystem,
    OH_DRM_Uint8Buffer *mediaKeyId, OH_DRM_OfflineMediaKeyStatus *status)
{
    return DRM_ERR_OK;
}

OH_OfflineLicenseStatus OH_GetOfflineLicenseStatus(OH_MediaKeySystem *mediaKeySystem, uint8_t *licenseId,
    size_t licenseIdLen)
{
    DRM_INFO_LOG("OH_GetOfflineLicenseStatus enter");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, OFFLINELICENSE_STATUS_UNKNOWN,
        "OH_GetOfflineLicenseStatus mediaKeySystem is nullptr!");
    int32_t result = OFFLINELICENSE_STATUS_UNKNOWN;

    std::vector<uint8_t> licenseIdVec(licenseId, licenseId + licenseIdLen);
    IMediaKeySessionService::OfflineLicenseStatus offlineLicenseStatus =
        IMediaKeySessionService::OFFLINELICENSESTATUS_UNKNOWN;

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, OFFLINELICENSE_STATUS_UNKNOWN,
        "OH_GetCertificateStatus systemObject is nullptr!");
    result = systemObject->systemImpl_->GetOfflineLicenseStatus(licenseIdVec, offlineLicenseStatus);
    if (result != DRM_ERR_OK) {
        DRM_ERR_LOG("OH_GetOfflineLicenseStatus faild!");
        return OFFLINELICENSE_STATUS_UNKNOWN;
    }
    OH_OfflineLicenseStatus CofflineLicenseStatus = (OH_OfflineLicenseStatus)((int32_t)(offlineLicenseStatus));
    if (CofflineLicenseStatus < OFFLINELICENSE_STATUS_UNKNOWN ||
        CofflineLicenseStatus > OFFLINELICENSE_STATUS_INACTIVE) {
        DRM_ERR_LOG("OH_GetOfflineLicenseStatus faild!");
        return OFFLINELICENSE_STATUS_UNKNOWN;
    }
    DRM_INFO_LOG("OH_GetOfflineLicenseStatus exit");
    return CofflineLicenseStatus;
}


OH_DrmErrCode OH_RemoveOfflineLicense(OH_MediaKeySystem *mediaKeySystem, uint8_t *licenseId, size_t licenseIdLen)
{
    DRM_INFO_LOG("OH_RemoveOfflineLicense enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetOfflineLicenseStatus mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(licenseId != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetOfflineLicenseStatus licenseIdptr is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(licenseIdLen != 0, DRM_ERR_INVALID_VAL,
        "OH_GetOfflineLicenseStatus licenseIdLen is zero!");
    int32_t result = DRM_ERR_ERROR;

    std::vector<uint8_t> licenseIdVec(licenseId, licenseId + licenseIdLen);
    DRM_CHECK_AND_RETURN_RET_LOG(licenseIdVec.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_RemoveOfflineLicense configName.size is not zero!");

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetCertificateStatus systemObject is nullptr!");
    result = systemObject->systemImpl_->RemoveOfflineLicense(licenseIdVec);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_RemoveOfflineLicense mediaKeySystemImpl::RemoveOfflineLicense faild!");
    DRM_INFO_LOG("OH_RemoveOfflineLicense exit.");
    return DRM_ERR_OK;
}

OH_DrmErrCode OH_MediaKeySystem_Destroy(OH_MediaKeySystem *mediaKeySystem)
{
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL, "mediaKeySystem is nullptr!");

    struct MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL, "systemObject is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemImpl_ != nullptr, DRM_ERR_INVALID_VAL, "systemImpl_ is nullptr!");

    int32_t ret = systemObject->systemImpl_->Release();
    OH_DrmErrCode retCode = DRM_ERR_OK;
    if (ret != DRM_ERR_OK) {
        DRM_ERR_LOG("call media key system release failed!");
        retCode = DRM_ERR_UNKNOWN;
    }
    delete mediaKeySystem;
    mediaKeySystem = nullptr;
    return retCode;
}
