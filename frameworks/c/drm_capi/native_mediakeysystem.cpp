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

OH_MediaKeySession *OH_MediaKeySystem_CreateMediaKeySession(OH_MediaKeySystem *mediaKeySystem, OH_SecurityLevel *level)
{
    DRM_INFO_LOG("OH_MediaKeySystem_CreateMediaKeySession called");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, nullptr, "OH_MediaKeySystem mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(level != nullptr, nullptr, "OH_MediaKeySystem level is nullptr!");

    struct MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, nullptr, "OH_MediaKeySystem systemObject is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemImpl_ != nullptr, nullptr, "systemImpl_ is nullptr!");

    int32_t secure = static_cast<int32_t>(*level);
    DRM_CHECK_AND_RETURN_RET_LOG(secure <= SECURITY_LEVEL_MAX && secure >= SECURITY_LEVEL_UNKNOWN, nullptr,
        "OH_MediaKeySystem secure input is invalid");
    IMediaKeySessionService::SecurityLevel secureLevel = static_cast<IMediaKeySessionService::SecurityLevel>(secure);
    OHOS::sptr<MediaKeySessionImpl> keySessionImpl = nullptr;
    int32_t ret = systemObject->systemImpl_->CreateMediaKeySession(secureLevel, &keySessionImpl);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_OK, nullptr, "OH_MediaKeySystem create session impl return failed!");
    DRM_CHECK_AND_RETURN_RET_LOG(keySessionImpl != nullptr, nullptr, "OH_MediaKeySystem session impl create failed!");

    struct MediaKeySessionObject *object = new (std::nothrow) MediaKeySessionObject(keySessionImpl);
    DRM_CHECK_AND_RETURN_RET_LOG(object != nullptr, nullptr, "OH_MediaKeySystem MediaKeySessionObject create failed!");
    return object;
}

OH_DrmBool OH_MediaKeySystemIsSupportedByUuid(const char *uuid)
{
    DRM_INFO_LOG("OH_MediaKeySystemIsSupportedByUuid enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(uuid != nullptr, false, "OH_MediaKeySystem uuid is nullptr!");
    std::string Uuid(uuid);
    DRM_CHECK_AND_RETURN_RET_LOG(Uuid.size() != 0, false, "OH_MediaKeySystem Uuid.size is nullptr!");
    OHOS::sptr<MediaKeySystemFactoryImpl> fatory = MediaKeySystemFactoryImpl::GetInstance();
    DRM_CHECK_AND_RETURN_RET_LOG(fatory != nullptr, false, "OH_MediaKeySystem fatory is nullptr!");
    bool isSurpportted = fatory->IsMediaKeySystemSupported(Uuid);
    DRM_INFO_LOG("OH_MediaKeySystemIsSupportedByUuid exit.");
    return isSurpportted;
}

OH_DrmBool OH_MediaKeySystemIsSupportedV2(const char *uuid, const char *mimeTypePtr)
{
    DRM_INFO_LOG("OH_MediaKeySystemIsSupportedByUuid enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(uuid != nullptr, false, "OH_MediaKeySystem uuid is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mimeTypePtr != nullptr, false, "OH_MediaKeySystem uuid is nullptr!");

    std::string Uuid(uuid);
    DRM_CHECK_AND_RETURN_RET_LOG(Uuid.size() != 0, false, "OH_MediaKeySystem Uuid.size is nullptr!");
    std::string mimeType = std::string(mimeTypePtr);
    DRM_CHECK_AND_RETURN_RET_LOG(mimeType.size() != 0, false, "OH_MediaKeySystem level is nullptr!");

    OHOS::sptr<MediaKeySystemFactoryImpl> fatory = MediaKeySystemFactoryImpl::GetInstance();
    DRM_CHECK_AND_RETURN_RET_LOG(fatory != nullptr, false, "OH_MediaKeySystem fatory is nullptr!");
    bool isSurpportted = fatory->IsMediaKeySystemSupported(Uuid, mimeType);
    DRM_INFO_LOG("OH_MediaKeySystemIsSupportedByUuid exit.");
    return isSurpportted;
}

OH_DrmBool OH_MediaKeySystemIsSupportedV3(const char *uuidPtr, const char *mimeTypePtr, int32_t cSecurityLevel)
{
    DRM_INFO_LOG("OH_MediaKeySystemIsSupportedByUuid enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(uuidPtr != nullptr, false, "OH_MediaKeySystem uuid is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mimeTypePtr != nullptr, false, "OH_MediaKeySystem uuid is nullptr!");

    std::string uuid(uuidPtr);
    DRM_CHECK_AND_RETURN_RET_LOG(uuid.size() != 0, false, "OH_MediaKeySystem Uuid.size is nullptr!");
    std::string mimeType = std::string(mimeTypePtr);
    DRM_CHECK_AND_RETURN_RET_LOG(mimeType.size() != 0, false, "OH_MediaKeySystem level is nullptr!");

    OHOS::sptr<MediaKeySystemFactoryImpl> fatory = MediaKeySystemFactoryImpl::GetInstance();
    DRM_CHECK_AND_RETURN_RET_LOG(fatory != nullptr, false, "OH_MediaKeySystem fatory is nullptr!");

    IMediaKeySessionService::SecurityLevel securityLevel;
    securityLevel = (IMediaKeySessionService::SecurityLevel)cSecurityLevel;
    if ((securityLevel < IMediaKeySessionService::SECURITY_LEVEL_UNKNOWN) ||
        (securityLevel >= IMediaKeySessionService::SECURITY_LEVEL_MAX)) {
        DRM_ERR_LOG("cSecurityLevel is invalid");
        return false;
    }
    bool isSurpportted = fatory->IsMediaKeySystemSupported(uuid, mimeType, securityLevel);
    DRM_INFO_LOG("MediaKeySystemNapi::IsMediaKeySystemSupported exit.");
    return isSurpportted;
}


OH_DrmErrCode OH_GetConfigurationString(OH_MediaKeySystem *mediaKeySystem, const char *configName, char *valuePtr)
{
    DRM_INFO_LOG("OH_GetConfigurationString enter");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationString mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(configName != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationString configName is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(valuePtr != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationString valuePtr is nullptr!");
    memset(valuePtr, 0, strlen(valuePtr));
    std::string value;
    int32_t result = DRM_ERR_ERROR;
    std::string name = std::string(configName);
    DRM_CHECK_AND_RETURN_RET_LOG(name.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString configName.size is not zero!");

    struct MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationString systemObject is nullptr!");
    result = systemObject->systemImpl_->GetConfigurationString(name, value);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString mediaKeySystemImpl::GetConfigurationString faild!");
    memcpy(valuePtr, value.c_str(), value.size());
    DRM_INFO_LOG("OH_GetConfigurationString exit");
    return DRM_ERR_OK;
}

OH_DrmErrCode OH_SetConfigurationString(OH_MediaKeySystem *mediaKeySystem, const char *configName, const char *valuePtr)
{
    DRM_INFO_LOG("OH_SetConfigurationString enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(configName != nullptr, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString configName is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(valuePtr != nullptr, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString value is nullptr!");

    int32_t result = DRM_ERR_ERROR;
    std::string name(configName);
    DRM_CHECK_AND_RETURN_RET_LOG(name.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString configName.size is not zero!");
    std::string value(valuePtr);
    DRM_CHECK_AND_RETURN_RET_LOG(value.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString value.size is not zero!");

    struct MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString systemObject is nullptr!");
    result = systemObject->systemImpl_->SetConfigurationString(name, value);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationString mediaKeySystemImpl::SetConfigurationString faild!");

    DRM_INFO_LOG("OH_SetConfigurationString exit.");
    return DRM_ERR_OK;
}

OH_DrmErrCode OH_GetConfigurationByteArray(OH_MediaKeySystem *mediaKeySystem, const char *configName, unsigned char *valuePtr, uint32_t *valueLen)
{
    DRM_INFO_LOG("OH_GetConfigurationByteArray enter");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationByteArray mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(configName != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationByteArray configName is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(valuePtr != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationByteArray valuePtr is nullptr!");

    std::vector<uint8_t> value;
    int32_t result = DRM_ERR_ERROR;
    std::string name = std::string(configName);
    DRM_CHECK_AND_RETURN_RET_LOG(name.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationByteArray configName.size is not zero!");

    struct MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationString systemObject is nullptr!");
    result = systemObject->systemImpl_->GetConfigurationByteArray(name, value);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_GetConfigurationByteArray mediaKeySystemImpl::GetConfigurationByteArray faild!");
    memcpy(valuePtr, value.data(), value.size());
    *valueLen = value.size();
    DRM_INFO_LOG("OH_GetConfigurationByteArray exit");
    return DRM_ERR_OK;
}

OH_DrmErrCode OH_SetConfigurationByteArray(OH_MediaKeySystem *mediaKeySystem, const char *configName,
    unsigned char *valuePtr, uint32_t valueLen)
{
    DRM_INFO_LOG("OH_SetConfigurationByteArray enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray mediaKeySystem is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(configName != nullptr, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray configName is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(valuePtr != nullptr, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray value is nullptr!");

    int32_t result = DRM_ERR_ERROR;
    std::string name(configName);
    DRM_CHECK_AND_RETURN_RET_LOG(name.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray configName.size is not zero!");
    uint8_t *valueDataPtr = reinterpret_cast<uint8_t *>(valuePtr);
    DRM_CHECK_AND_RETURN_RET_LOG(valueDataPtr != nullptr, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray value is nullptr!");
    std::vector<uint8_t> value(valueDataPtr, valueDataPtr + valueLen);
    DRM_CHECK_AND_RETURN_RET_LOG(value.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray value.size is not zero!");


    struct MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray systemObject is nullptr!");
    result = systemObject->systemImpl_->SetConfigurationByteArray(name, value);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray mediaKeySystemImpl::SetConfigurationByteArray faild!");
    DRM_INFO_LOG("OH_SetConfigurationByteArray exit.");
    return DRM_ERR_OK;
}



OH_SecurityLevel OH_GetMaxSecurityLevel(OH_MediaKeySystem *mediaKeySystem)
{
    DRM_INFO_LOG("GetMaxSecurityLevel enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, SECURITY_LEVEL_UNKNOWN,
        "OH_GetMaxSecurityLevel mediaKeySystem is nullptr!");
    struct MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);

    IMediaKeySessionService::SecurityLevel level = IMediaKeySessionService::SECURITY_LEVEL_UNKNOWN;
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, SECURITY_LEVEL_UNKNOWN,
        "OH_GetMaxSecurityLevel systemObject is nullptr!");
    systemObject->systemImpl_->GetMaxSecurityLevel(&level);
    if (level > IMediaKeySessionService::SECURITY_LEVEL_UNKNOWN ||
        level > IMediaKeySessionService::SECURITY_LEVEL_MAX) {
        DRM_ERR_LOG("mediaKeySystemImpl::GetMaxSecurityLevel faild!");
        return SECURITY_LEVEL_UNKNOWN;
    }
    OH_SecurityLevel SecurityLevel = static_cast<OH_SecurityLevel>(level);
    DRM_INFO_LOG("OH_GetMaxSecurityLevel exit");
    return SecurityLevel;
}


OH_DrmErrCode OH_ProcessKeySystemResponse(OH_MediaKeySystem *mediaKeySystem, uint8_t *reponseData,
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

    struct MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_ProcessKeySystemResponse systemObject is nullptr!");
    result = systemObject->systemImpl_->ProcessKeySystemResponse(keySystemResponse);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_ProcessKeySystemResponse systemObject is nullptr!");
    DRM_INFO_LOG("OH_ProcessKeySystemResponse exit.");
    return DRM_ERR_OK;
}

OH_CertificateStatus OH_GetCertificateStatus(OH_MediaKeySystem *mediaKeySystem)
{
    DRM_INFO_LOG("OH_GetCertificateStatus enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, CERT_STATUS_GET_FAILED,
        "OH_ProcessKeySystemResponse mediaKeySystem is nullptr!");
    IMediaKeySystemService::CertificateStatus certStatus;
    struct MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, CERT_STATUS_GET_FAILED,
        "OH_GetCertificateStatus systemObject is nullptr!");
    int32_t result = systemObject->systemImpl_->GetCertificateStatus(&certStatus);
    if (result != DRM_ERR_OK) {
        DRM_ERR_LOG("OH_GetOfflineLicenseStatus faild!");
        return CERT_STATUS_GET_FAILED;
    }
    OH_CertificateStatus CertStatus = (OH_CertificateStatus)((int32_t)(certStatus));
    if (CertStatus < CERT_STATUS_PROVISIONED || CertStatus > CERT_STATUS_GET_FAILED) {
        DRM_ERR_LOG("OH_GetCertificateStatus faild!");
        return CERT_STATUS_GET_FAILED;
    }
    DRM_INFO_LOG("OH_GetCertificateStatus exit.");
    return CertStatus;
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

    struct MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, OFFLINELICENSE_STATUS_UNKNOWN,
        "OH_GetCertificateStatus systemObject is nullptr!");
    result = systemObject->systemImpl_->GetOfflineLicenseStatus(licenseIdVec, offlineLicenseStatus);
    if (result != DRM_ERR_OK) {
        DRM_ERR_LOG("OH_GetOfflineLicenseStatus faild!");
        return OFFLINELICENSE_STATUS_UNKNOWN;
    }
    OH_OfflineLicenseStatus CofflineLicenseStatus = (OH_OfflineLicenseStatus)((int32_t)(offlineLicenseStatus));
    if (CofflineLicenseStatus < OFFLINELICENSE_STATUS_UNKNOWN || CofflineLicenseStatus > OFFLINELICENSE_STATUS_INACTIVE) {
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

    struct MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_GetCertificateStatus systemObject is nullptr!");
    result = systemObject->systemImpl_->RemoveOfflineLicense(licenseIdVec);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_RemoveOfflineLicense mediaKeySystemImpl::RemoveOfflineLicense faild!");
    DRM_INFO_LOG("OH_RemoveOfflineLicense exit.");
    return DRM_ERR_OK;
}
