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

bool OH_MediaKeySystem_IsSupported(const char *uuid)
{
    DRM_INFO_LOG("OH_MediaKeySystemIsSupportedByUuid enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(uuid != nullptr, false, "OH_MediaKeySystem uuid is nullptr!");
    std::string uuidPtr(uuid);
    bool isSurooprtted = false;
    DRM_CHECK_AND_RETURN_RET_LOG(uuidPtr.size() != 0, false, "OH_MediaKeySystem uuidPtr.size is nullptr!");
    OHOS::sptr<MediaKeySystemFactoryImpl> fatory = MediaKeySystemFactoryImpl::GetInstance();
    DRM_CHECK_AND_RETURN_RET_LOG(fatory != nullptr, false, "OH_MediaKeySystem fatory is nullptr!");
    isSurooprtted = fatory->IsMediaKeySystemSupported(uuidPtr);
    DRM_INFO_LOG("OH_MediaKeySystemIsSupportedByUuid exit.");
    return isSurooprtted;
}

bool OH_MediaKeySystem_IsSupported2(const char *uuid, const char *mimeType)
{
    DRM_INFO_LOG("OH_MediaKeySystem_IsSupported2 enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(uuid != nullptr, false, "OH_MediaKeySystem uuid is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mimeType != nullptr, false, "OH_MediaKeySystem uuid is nullptr!");
    bool isSurooprtted = false;
    std::string uuidPtr(uuid);
    DRM_CHECK_AND_RETURN_RET_LOG(uuidPtr.size() != 0, false, "OH_MediaKeySystem uuidPtr.size is nullptr!");
    std::string mimeTypePtr = std::string(mimeType);
    DRM_CHECK_AND_RETURN_RET_LOG(mimeTypePtr.size() != 0, false, "OH_MediaKeySystem level is nullptr!");

    OHOS::sptr<MediaKeySystemFactoryImpl> fatory = MediaKeySystemFactoryImpl::GetInstance();
    DRM_CHECK_AND_RETURN_RET_LOG(fatory != nullptr, false, "OH_MediaKeySystem fatory is nullptr!");
    isSurooprtted = fatory->IsMediaKeySystemSupported(uuidPtr, mimeTypePtr);
    DRM_INFO_LOG("OH_MediaKeySystem_IsSupported2 exit.");
    return isSurooprtted;
}

bool OH_MediaKeySystem_IsSupported3(const char *uuid, const char *mimeType, OH_DRM_ContentProtectionLevel SecurityLevel)
{
    DRM_INFO_LOG("OH_MediaKeySystem_IsSupported3 enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(uuid != nullptr, false, "OH_MediaKeySystem uuid is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mimeType != nullptr, false, "OH_MediaKeySystem uuid is nullptr!");
    bool isSurooprtted = false;
    std::string uuidPtr(uuid);
    DRM_CHECK_AND_RETURN_RET_LOG(uuidPtr.size() != 0, false, "OH_MediaKeySystem Uuid.size is nullptr!");
    std::string mimeTypePtr = std::string(mimeType);
    DRM_CHECK_AND_RETURN_RET_LOG(mimeTypePtr.size() != 0, false, "OH_MediaKeySystem level is nullptr!");

    OHOS::sptr<MediaKeySystemFactoryImpl> fatory = MediaKeySystemFactoryImpl::GetInstance();
    DRM_CHECK_AND_RETURN_RET_LOG(fatory != nullptr, false, "OH_MediaKeySystem fatory is nullptr!");

    IMediaKeySessionService::SecurityLevel securityLevel = (IMediaKeySessionService::SecurityLevel)SecurityLevel;
    if ((securityLevel < IMediaKeySessionService::SECURITY_LEVEL_UNKNOWN) ||
        (securityLevel >= IMediaKeySessionService::SECURITY_LEVEL_MAX)) {
        DRM_ERR_LOG("cSecurityLevel is invalid");
        return false;
    }
    isSurooprtted = fatory->IsMediaKeySystemSupported(uuidPtr, mimeTypePtr, securityLevel);
    DRM_INFO_LOG("OH_MediaKeySystem_IsSupported3 exit.");
    return isSurooprtted;
}

OH_DrmErrCode OH_MediaKeySystem_Create(const char *name, OH_MediaKeySystem **mediaKeySystem)
{
    DRM_CHECK_AND_RETURN_RET_LOG(name != nullptr, DRM_ERR_INVALID_VAL, "name is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL, "mediaKeySystem is nullptr!");
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
    DRM_CHECK_AND_RETURN_RET_LOG(object->systemImpl_ != nullptr, DRM_ERR_OPERATION_NOT_PERMITTED,
        "MediaKeySystemObject's systemImpl is nullptr!");
    int32_t ret = object->systemImpl_->SetCallback(object->systemCallback_);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_ERR_OK, DRM_ERR_UNKNOWN, "system set callback failed!");

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
    char **value, int32_t *valueLen)
{
    DRM_INFO_LOG("OH_GetConfigurationString enter");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationString mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(configName != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationString configName is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(value != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationString configName is nullptr!");
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
    *value = (char *)malloc(valuePtr.size());
    DRM_CHECK_AND_RETURN_RET_LOG(*value != nullptr, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString malloc faild!");
    *valueLen = valuePtr.size();
    if (valuePtr.size() == 0) {
        DRM_DEBUG_LOG("valuePtr.data() is nullptr!");
    }
    int32_t ret = memcpy_s(*value, valuePtr.size(), valuePtr.c_str(), valuePtr.size());
    if (ret != 0) {
        DRM_ERR_LOG("OH_GetConfigurationString memcpy_s faild!");
        free(*value);
        *value = NULL;
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
    unsigned char **value, uint32_t *valueLen)
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
    *valueLen = valuePtr.size();
    if (valuePtr.size() == 0) {
        DRM_DEBUG_LOG("valuePtr.data() is nullptr!");
        return DRM_ERR_OK;
    }
    *value = (unsigned char *)malloc(valuePtr.size());
    DRM_CHECK_AND_RETURN_RET_LOG(*value != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationByteArray malloc faild!");
    int32_t ret = memcpy_s(*value, valuePtr.size(), valuePtr.data(), valuePtr.size());
    if (ret != 0) {
        DRM_ERR_LOG("OH_GetConfigurationByteArray memcpy_s faild!");
        return DRM_ERR_ERROR;
    }
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
    DRM_CHECK_AND_RETURN_RET_LOG(cArray != nullptr, nullptr, "malloc faild!");
    cArray->statisticsCount = metrics.size();
    OH_DRM_CharBufferPair *dest = &((cArray->info)[0]);
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

OH_DrmErrCode OH_MediaKeySystem_GetStatistics(OH_MediaKeySystem *mediaKeySystem, OH_DRM_Statistics **statistics)
{
    DRM_INFO_LOG("OH_MediaKeySystem_GetStatistics enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetStatistics mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(statistics != nullptr, DRM_ERR_INVALID_VAL,
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

OH_DrmErrCode OH_MediaKeySystem_GetMaxContentProtectionLevel(OH_MediaKeySystem *mediaKeySystem,
    OH_DRM_ContentProtectionLevel *contentProtectionLevel)
{
    DRM_INFO_LOG("GetMaxSecurityLevel enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetMaxSecurityLevel mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(contentProtectionLevel != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetMaxSecurityLevel contentProtectionLevel is nullptr!");
    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);

    IMediaKeySessionService::SecurityLevel level = IMediaKeySessionService::SECURITY_LEVEL_UNKNOWN;
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetMaxSecurityLevel systemObject is nullptr!");
    systemObject->systemImpl_->GetMaxSecurityLevel(&level);
    if (level < IMediaKeySessionService::SECURITY_LEVEL_UNKNOWN ||
        level > IMediaKeySessionService::SECURITY_LEVEL_MAX) {
        DRM_ERR_LOG("mediaKeySystemImpl::GetMaxSecurityLevel faild!");
        return DRM_ERR_INVALID_VAL;
    }
    *contentProtectionLevel = static_cast<OH_DRM_ContentProtectionLevel>(level);
    DRM_INFO_LOG("OH_GetMaxSecurityLevel exit");
    return DRM_ERR_OK;
}

OH_DrmErrCode OH_MediaKeySystem_GenerateKeySystemRequest(OH_MediaKeySystem *mediaKeySystem, unsigned char **request,
    int32_t *requestLen, char **defaultUrl, int32_t *defaultUrlLen)
{
    DRM_INFO_LOG("OH_MediaKeySystem_GenerateKeySystemRequest enter");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GenerateKeySystemRequest mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(request != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GenerateKeySystemRequest request is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(requestLen != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GenerateKeySystemRequest mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(defaultUrl != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GenerateKeySystemRequest defaultUrl is nullptr!");
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

OH_DrmErrCode OH_MediaKeySystem_ProcessKeySystemResponse(OH_MediaKeySystem *mediaKeySystem, uint8_t *reponseData,
    size_t reponseDataLen)
{
    DRM_INFO_LOG("OH_ProcessKeySystemResponse enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_ProcessKeySystemResponse mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(reponseData != nullptr, DRM_ERR_INVALID_VAL,
        "OH_ProcessKeySystemResponse reponseData is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(reponseDataLen != 0, DRM_ERR_INVALID_VAL,
        "OH_ProcessKeySystemResponse reponseDataLen is zero!");
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

OH_DrmErrCode OH_MediaKeySystem_GetCertificateStatus(OH_MediaKeySystem *mediaKeySystem,
    OH_DRM_CertificateStatus *certStatus)
{
    DRM_INFO_LOG("OH_GetCertificateStatus enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_ProcessKeySystemResponse mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(certStatus != nullptr, DRM_ERR_INVALID_VAL,
        "OH_ProcessKeySystemResponse certStatus is nullptr!");
    IMediaKeySystemService::CertificateStatus CertStatus;
    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetCertificateStatus systemObject is nullptr!");
    int32_t result = systemObject->systemImpl_->GetCertificateStatus(&CertStatus);
    if (result != DRM_ERR_OK) {
        *certStatus = CERT_STATUS_GET_FAILED;
        DRM_ERR_LOG("OH_MediaKeySystem_GetCertificateStatus faild!");
        return DRM_ERR_INVALID_VAL;
    }
    *certStatus = (OH_DRM_CertificateStatus)((int32_t)(CertStatus));
    if (*certStatus < CERT_STATUS_PROVISIONED || *certStatus > CERT_STATUS_GET_FAILED) {
        DRM_ERR_LOG("OH_MediaKeySystem_GetCertificateStatus faild!");
        *certStatus = CERT_STATUS_GET_FAILED;
        return DRM_ERR_INVALID_VAL;
    }
    DRM_INFO_LOG("OH_MediaKeySystem_GetCertificateStatus exit.");
    return DRM_ERR_OK;
}

OH_DrmErrCode OH_MediaKeySystem_SetMediaKeySystemCallback(OH_MediaKeySystem *mediaKeySystem,
    OH_MediaKeySystemCallback callback)
{
    DRM_INFO_LOG("OH_MediaKeySystem_SetMediaKeySystemCallback enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL, "mediaKeySystem is nullptr!");
    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL, "systemObject is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemCallback_ != nullptr, DRM_ERR_INVALID_VAL,
        "systemCallback is nullptr!");
    systemObject->systemCallback_->SetCallbackReference(callback);
    return DRM_ERR_OK;
}

OH_DrmErrCode OH_MediaKeySystem_CreateMediaKeySession(OH_MediaKeySystem *mediaKeySystem,
    OH_DRM_ContentProtectionLevel *level, OH_MediaKeySession **mediaKeySession)
{
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL, "mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(level != nullptr, DRM_ERR_INVALID_VAL, "level is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySession != nullptr, DRM_ERR_INVALID_VAL, "level is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(*level > CONTENT_PROTECTION_LEVEL_UNKNOWN, DRM_ERR_INVALID_VAL, "level is 1 - 5!");
    DRM_CHECK_AND_RETURN_RET_LOG(*level <= CONTENT_PROTECTION_LEVEL_MAX, DRM_ERR_INVALID_VAL, "level is 1 - 5!");
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

    struct MediaKeySessionObject *sessionObject = new (std::nothrow) MediaKeySessionObject(keySessionImpl);
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject != nullptr, DRM_ERR_NO_MEMORY, "MediaKeySessionObject create failed!");

    sessionObject->sessionCallback_ = new (std::nothrow) MediaKeySessionCallbackCapi();
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject->sessionCallback_ != nullptr, DRM_ERR_NO_MEMORY,
        "MediaKeySessionObject create sessionCallback failed!");
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject->sessionImpl_ != nullptr, DRM_ERR_OPERATION_NOT_PERMITTED,
        "MediaKeySessionObject's sessionImpl is nullptr!");
    ret = sessionObject->sessionImpl_->SetCallback(sessionObject->sessionCallback_);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_ERR_OK, DRM_ERR_UNKNOWN, "session set callback failed!");

    *mediaKeySession = static_cast<OH_MediaKeySession *>(sessionObject);
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
    DRM_CHECK_AND_RETURN_RET_LOG(cArray != nullptr, nullptr, "malloc failed!");
    cArray->mediaKeyIdCount = licenseIds.size();
    OH_DRM_Uint8Buffer *dest = &((cArray->mediaKeyIds)[0]);
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

OH_DrmErrCode OH_MediaKeySystem_GetOfflineMediaKeyIds(OH_MediaKeySystem *mediaKeySystem,
    OH_DRM_MediakeyIdArray **mediaKeyIds)
{
    DRM_INFO_LOG("OH_MediaKeySystem_GetOfflineMediaKeyIds enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetOfflineMediaKeyIds mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeyIds != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetOfflineMediaKeyIds mediaKeyIds is nullptr!");
    std::vector<std::vector<uint8_t>> licenseIds;

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetCertificateStatus systemObject is nullptr!");
    int32_t result = systemObject->systemImpl_->GetOfflineLicenseIds(licenseIds);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetOfflineMediaKeyIds faild!");
    if (licenseIds.size() == 0) {
        DRM_DEBUG_LOG("licenseIds.data() is nullptr!");
        DRM_INFO_LOG("OH_MediaKeySystem_GetOfflineMediaKeyIds exit.");
        return DRM_ERR_OK;
    }
    *mediaKeyIds = vectorToC2DArray(licenseIds);
    DRM_INFO_LOG("OH_MediaKeySystem_GetOfflineMediaKeyIds exit.");
    return DRM_ERR_OK;
}

OH_DrmErrCode OH_MediaKeySystem_GetOfflineMediaKeyStatus(OH_MediaKeySystem *mediaKeySystem,
    OH_DRM_Uint8Buffer *mediaKeyId, OH_DRM_OfflineMediaKeyStatus *status)
{
    DRM_INFO_LOG("OH_MediaKeySystem_GetOfflineMediaKeyStatus enter");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetOfflineMediaKeyStatus mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeyId->buffer != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetOfflineMediaKeyStatus mediaKeyId.buffer is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeyId->bufferLen != 0, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetOfflineMediaKeyStatus mediaKeyId.bufferLen is zero!");
    DRM_CHECK_AND_RETURN_RET_LOG(status != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetOfflineMediaKeyStatus status is nullptr!");
    int32_t result = OFFLINELICENSE_STATUS_UNKNOWN;

    std::vector<uint8_t> licenseIdVec(mediaKeyId->buffer, mediaKeyId->buffer + mediaKeyId->bufferLen);
    IMediaKeySessionService::OfflineLicenseStatus offlineLicenseStatus =
        IMediaKeySessionService::OFFLINELICENSESTATUS_UNKNOWN;

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetCertificateStatus systemObject is nullptr!");
    result = systemObject->systemImpl_->GetOfflineLicenseStatus(licenseIdVec, offlineLicenseStatus);
    if (result != DRM_ERR_OK) {
        DRM_ERR_LOG("OH_MediaKeySystem_GetOfflineMediaKeyStatus faild!");
        return DRM_ERR_INVALID_VAL;
    }
    OH_DRM_OfflineMediaKeyStatus CofflineLicenseStatus =
        (OH_DRM_OfflineMediaKeyStatus)((int32_t)(offlineLicenseStatus));
    if (CofflineLicenseStatus < OFFLINE_MEDIA_KEY_STATUS_UNKNOWN ||
        CofflineLicenseStatus > OFFLINE_MEDIA_KEY_STATUS_INACTIVE) {
        DRM_ERR_LOG("OH_MediaKeySystem_GetOfflineMediaKeyStatus faild!");
        return DRM_ERR_INVALID_VAL;
    }
    *status = CofflineLicenseStatus;
    DRM_INFO_LOG("OH_MediaKeySystem_GetOfflineMediaKeyStatus exit");
    return DRM_ERR_OK;
}


OH_DrmErrCode OH_MediaKeySystem_ClearOfflineMediaKeys(OH_MediaKeySystem *mediaKeySystem, OH_DRM_Uint8Buffer *mediaKeyId)
{
    DRM_INFO_LOG("OH_RemoveOfflineLicense enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetOfflineLicenseStatus mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeyId->buffer != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetOfflineLicenseStatus mediaKeyId is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeyId->bufferLen != 0, DRM_ERR_INVALID_VAL,
        "OH_GetOfflineLicenseStatus mediaKeyIdLen is zero!");
    int32_t result = DRM_ERR_ERROR;
    std::vector<uint8_t> licenseIdVec(mediaKeyId->buffer, mediaKeyId->buffer + mediaKeyId->bufferLen);
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
