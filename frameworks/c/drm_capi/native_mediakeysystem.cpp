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
#include <map>
#include <shared_mutex>
#include <string>
#include <refbase.h>
#include <securec.h>
#include "drm_log.h"
#include "drm_trace.h"
#include "native_drm_common.h"
#include "native_drm_base.h"
#include "native_drm_object.h"
#include "native_mediakeysystem.h"
#include "drm_api_operation.h"
#include "media_key_system_factory_impl.h"

using namespace OHOS::DrmStandard;

bool OH_MediaKeySystem_IsSupported(const char *uuid)
{
    DRM_INFO_LOG("OH_MediaKeySystem_IsSupported enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(uuid != nullptr, false, "MediaKeySystem uuid is nullptr!");
    std::string uuidPtr(uuid);
    bool isSurooprtted = false;
    DRM_CHECK_AND_RETURN_RET_LOG(uuidPtr.size() != 0, false, "MediaKeySystem uuidPtr.size is nullptr!");
    OHOS::sptr<MediaKeySystemFactoryImpl> fatory = MediaKeySystemFactoryImpl::GetInstance();
    isSurooprtted = fatory->IsMediaKeySystemSupported(uuidPtr);
    return isSurooprtted;
}

bool OH_MediaKeySystem_IsSupported2(const char *uuid, const char *mimeType)
{
    DRM_INFO_LOG("OH_MediaKeySystem_IsSupported2 enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(((uuid != nullptr) && (mimeType != nullptr)), false,
        "OH_MediaKeySystem_IsSupported2 uuid is nullptr!");
    bool isSurooprtted = false;
    std::string uuidPtr(uuid);
    DRM_CHECK_AND_RETURN_RET_LOG(uuidPtr.size() != 0, false, "MediaKeySystem uuidPtr.size is nullptr!");
    std::string mimeTypePtr = std::string(mimeType);
    DRM_CHECK_AND_RETURN_RET_LOG(mimeTypePtr.size() != 0, false, "MediaKeySystem mimeTypePtr.size is nullptr!");

    OHOS::sptr<MediaKeySystemFactoryImpl> fatory = MediaKeySystemFactoryImpl::GetInstance();
    isSurooprtted = fatory->IsMediaKeySystemSupported(uuidPtr, mimeTypePtr);
    return isSurooprtted;
}

bool OH_MediaKeySystem_IsSupported3(const char *uuid, const char *mimeType,
    DRM_ContentProtectionLevel ContentProtectionLevel)
{
    DRM_INFO_LOG("OH_MediaKeySystem_IsSupported3 enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(((uuid != nullptr) && (mimeType != nullptr)), false,
        "OH_MediaKeySystem_IsSupported3 uuid is nullptr!");
    bool isSurooprtted = false;
    std::string uuidPtr(uuid);
    DRM_CHECK_AND_RETURN_RET_LOG(uuidPtr.size() != 0, false, "MediaKeySystem Uuid.size is nullptr!");
    std::string mimeTypePtr = std::string(mimeType);
    DRM_CHECK_AND_RETURN_RET_LOG(mimeTypePtr.size() != 0, false, "MediaKeySystem mimeTypePtr.size is nullptr!");

    OHOS::sptr<MediaKeySystemFactoryImpl> fatory = MediaKeySystemFactoryImpl::GetInstance();

    IMediaKeySessionService::ContentProtectionLevel securityLevel =
        (IMediaKeySessionService::ContentProtectionLevel)ContentProtectionLevel;
    if ((securityLevel <= IMediaKeySessionService::CONTENT_PROTECTION_LEVEL_UNKNOWN) ||
        (securityLevel >= IMediaKeySessionService::CONTENT_PROTECTION_LEVEL_MAX)) {
        DRM_ERR_LOG("ContentProtectionLevel is invalid");
        return false;
    }
    isSurooprtted = fatory->IsMediaKeySystemSupported(uuidPtr, mimeTypePtr, securityLevel);
    return isSurooprtted;
}

Drm_ErrCode OH_MediaKeySystem_GetMediaKeySystems(DRM_MediaKeySystemDescription *description, uint32_t *count)
{
    DRM_INFO_LOG("OH_MediaKeySystem_GetMediaKeySystems enter");
    DRM_CHECK_AND_RETURN_RET_LOG((description != nullptr), DRM_ERR_INVALID_VAL, "description is nullptr");
    DRM_CHECK_AND_RETURN_RET_LOG((count != nullptr), DRM_ERR_INVALID_VAL, "count is nullptr");
    std::map<std::string, std::string> keySystemNames;
    OHOS::sptr<MediaKeySystemFactoryImpl> fatory = MediaKeySystemFactoryImpl::GetInstance();
    int32_t ret = fatory->GetMediaKeySystems(keySystemNames);
    DRM_CHECK_AND_RETURN_RET_LOG((*count >= keySystemNames.size()), DRM_ERR_INVALID_VAL,
        "GetMediaKeySystems failed because the count passed by is too small.");
    int32_t times = 0;
    DRM_CHECK_AND_RETURN_RET_LOG((ret == DRM_ERR_OK), DRM_ERR_UNKNOWN,
        "GetMediaKeySystems call Failed!");
    for (auto it = keySystemNames.begin(); it != keySystemNames.end(); it++) {
        if (it->first.size() != 0) {
            ret = memcpy_s(description[times].name, sizeof(description[times].name), it->first.c_str(),
                it->first.size());
            if (ret != 0) {
                DRM_ERR_LOG("OH_MediaKeySystem_GetMediaKeySystems memcpy_s description faild!");
                return DRM_ERR_NO_MEMORY;
            }
        }
        if (it->second.size() != 0) {
            for (size_t i = 0; i < sizeof(description[times].uuid) * BASE_CONVERSION_OPERATOR;
                i += BASE_CONVERSION_OPERATOR) {
                std::string byteStr = it->second.substr(i, BASE_CONVERSION_OPERATOR);
                uint8_t byte = static_cast<u_int8_t>(std::stoi(byteStr, nullptr, HEXADECIMAL));
                description[times].uuid[i/BASE_CONVERSION_OPERATOR] = byte;
            }
        }
        times++;
    }
    if (keySystemNames.size() == 0) {
        DRM_ERR_LOG("plugin not exist.");
        return DRM_ERR_UNKNOWN;
    }
    *count = keySystemNames.size();
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_Create(const char *name, MediaKeySystem **mediaKeySystem)
{
    DRM_INFO_LOG("OH_MediaKeySystem_Create enter.");
    DrmTrace trace("OH_MediaKeySystem_Create");
    std::map<int32_t, Drm_ErrCode> errCodeMaps = {
        {401, DRM_ERR_INVALID_VAL},
        {24700201, DRM_ERR_SERVICE_DIED},
        {24700103, DRM_ERR_MAX_SYSTEM_NUM_REACHED},
        {24700101, DRM_ERR_UNKNOWN},
        {0, DRM_ERR_OK}
    };
    DRM_CHECK_AND_RETURN_RET_LOG(((name != nullptr) && (mediaKeySystem != nullptr)), DRM_ERR_INVALID_VAL,
        "parameter is error!");
    std::string nameStr = name;
    DRM_CHECK_AND_RETURN_RET_LOG(nameStr.size() != 0, DRM_ERR_INVALID_VAL, "the size of nameStr is zero");

    OHOS::sptr<MediaKeySystemFactoryImpl> factory = MediaKeySystemFactoryImpl::GetInstance();
    DRM_CHECK_AND_RETURN_RET_LOG(factory != nullptr, DRM_ERR_UNKNOWN, "factory is nullptr!");
    OHOS::sptr<OHOS::DrmStandard::MediaKeySystemImpl> system = nullptr;
    int32_t result = factory->CreateMediaKeySystem(nameStr, &system);
    Drm_ErrCode retCode = DRM_ERR_UNKNOWN;
    if (errCodeMaps.find(result) != errCodeMaps.end()) {
        retCode = errCodeMaps[result];
    }
    DRM_CHECK_AND_RETURN_RET_LOG(system != nullptr, retCode, "system create by name failed!");

    struct MediaKeySystemObject *object = new (std::nothrow) MediaKeySystemObject(system);
    DRM_CHECK_AND_RETURN_RET_LOG(object != nullptr, DRM_ERR_UNKNOWN, "MediaKeySystemObject create failed!");

    object->systemCallback_ = new (std::nothrow) MediaKeySystemCallbackCapi();
    if (object->systemCallback_ == nullptr) {
        delete object;
        DRM_ERR_LOG("MediaKeySystemObject create systemCallback failed!");
        return DRM_ERR_UNKNOWN;
    }
    int32_t ret = object->systemImpl_->SetCallback(object->systemCallback_);
    if (ret != DRM_ERR_OK) {
        delete object;
        DRM_ERR_LOG("system set callback failed!");
        return DRM_ERR_UNKNOWN;
    }

    *mediaKeySystem = object;
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_SetConfigurationString(MediaKeySystem *mediaKeySystem, const char *configName,
    const char *value)
{
    DRM_INFO_LOG("OH_MediaKeySystem_SetConfigurationString enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (configName != nullptr) && (value != nullptr)),
        DRM_ERR_INVALID_VAL, "OH_MediaKeySystem_SetConfigurationString parameter is error!");

    int32_t result = DRM_ERR_OK;
    std::string name(configName);
    DRM_CHECK_AND_RETURN_RET_LOG(name.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_SetConfigurationString configName.size is not zero!");
    std::string valuePtr(value);
    DRM_CHECK_AND_RETURN_RET_LOG(valuePtr.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_SetConfigurationString value.size is not zero!");

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_SetConfigurationString inner systemImpl is nullptr!");
    result = systemObject->systemImpl_->SetConfigurationString(name, valuePtr);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_UNKNOWN,
        "OH_MediaKeySystem_SetConfigurationString mediaKeySystemImpl::SetConfigurationString faild!");
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_GetConfigurationString(MediaKeySystem *mediaKeySystem, const char *configName,
    char *value, int32_t valueLen)
{
    DRM_INFO_LOG("OH_MediaKeySystem_GetConfigurationString enter");
    DRM_CHECK_AND_RETURN_RET_LOG(
        ((mediaKeySystem != nullptr) && (configName != nullptr) && (value != nullptr) && (valueLen > 0)),
        DRM_ERR_INVALID_VAL, "OH_MediaKeySystem_GetConfigurationString params is error!");

    std::string valuePtr;
    int32_t result = DRM_ERR_UNKNOWN;
    std::string name = std::string(configName);
    DRM_CHECK_AND_RETURN_RET_LOG(name.size() != 0, DRM_ERR_INVALID_VAL,
        "configName.size is not zero!");
    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetConfigurationString faild!");
    result = systemObject->systemImpl_->GetConfigurationString(name, valuePtr);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_UNKNOWN,
        "mediaKeySystemImpl::GetConfigurationString faild!");
    DRM_CHECK_AND_RETURN_RET_LOG(valueLen >= (int32_t)valuePtr.size(), DRM_ERR_INVALID_VAL,
        "The space for value is too small");
    memset_s(value, valueLen, 0, valueLen);
    int32_t ret = memcpy_s(value, valuePtr.size(), valuePtr.c_str(), valuePtr.size());
    if (ret != 0) {
        DRM_ERR_LOG("OH_MediaKeySystem_GetConfigurationString memcpy_s value failed!");
        return DRM_ERR_NO_MEMORY;
    }
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_SetConfigurationByteArray(MediaKeySystem *mediaKeySystem,
    const char *configName, uint8_t *value, int32_t valueLen)

{
    DRM_INFO_LOG("OH_MediaKeySystem_SetConfigurationByteArray enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(
        ((mediaKeySystem != nullptr) && (configName != nullptr) && (value != nullptr) && (valueLen > 0)),
        DRM_ERR_INVALID_VAL, "OH_MediaKeySystem_SetConfigurationByteArray params is error!");

    int32_t result = DRM_ERR_OK;
    std::string name(configName);
    DRM_CHECK_AND_RETURN_RET_LOG(name.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_SetConfigurationByteArray configName.size is not zero!");
    uint8_t *valueDataPtr = reinterpret_cast<uint8_t *>(value);
    DRM_CHECK_AND_RETURN_RET_LOG(valueDataPtr != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_SetConfigurationByteArray value is nullptr!");
    std::vector<uint8_t> valueptr(valueDataPtr, valueDataPtr + valueLen);
    DRM_CHECK_AND_RETURN_RET_LOG(valueptr.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_SetConfigurationByteArray value.size is not zero!");

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "mediaKeySystemImpl::SetConfigurationByteArray faild!");
    result = systemObject->systemImpl_->SetConfigurationByteArray(name, valueptr);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_UNKNOWN,
        "OH_MediaKeySystem_SetConfigurationByteArray mediaKeySystemImpl::SetConfigurationByteArray faild!");
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_GetConfigurationByteArray(MediaKeySystem *mediaKeySystem,
    const char *configName, uint8_t *value, int32_t *valueLen)
{
    DRM_INFO_LOG("OH_MediaKeySystem_GetConfigurationByteArray enter");
    DRM_CHECK_AND_RETURN_RET_LOG(
        ((mediaKeySystem != nullptr) && (configName != nullptr) && (value != nullptr) && (valueLen != nullptr)),
        DRM_ERR_INVALID_VAL, "OH_MediaKeySystem_GetConfigurationByteArray parameter is error!");

    std::vector<uint8_t> valuePtr;
    int32_t result = DRM_ERR_OK;
    std::string name = std::string(configName);
    DRM_CHECK_AND_RETURN_RET_LOG(name.size() != 0, DRM_ERR_INVALID_VAL,
        "configName.size is not zero!");

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetConfigurationByteArray faild!");
    result = systemObject->systemImpl_->GetConfigurationByteArray(name, valuePtr);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_UNKNOWN,
        "mediaKeySystemImpl::GetConfigurationByteArray faild!");
    DRM_CHECK_AND_RETURN_RET_LOG(*valueLen >= (int32_t)valuePtr.size(), DRM_ERR_INVALID_VAL,
        "The space for value is too small!");
    *valueLen = valuePtr.size();
    int32_t ret = memcpy_s(value, valuePtr.size(), valuePtr.data(), valuePtr.size());
    if (ret != 0) {
        DRM_ERR_LOG("OH_MediaKeySystem_GetConfigurationByteArray memcpy_s value faild!");
        return DRM_ERR_NO_MEMORY;
    }
    return DRM_ERR_OK;
}

static Drm_ErrCode vectorToClist(std::vector<IMediaKeySystemService::MetircKeyValue> &metrics,
    DRM_Statistics *statistics)
{
    DRM_INFO_LOG("vectorToCArray start.");
    memset_s(statistics, sizeof(DRM_Statistics), 0, sizeof(DRM_Statistics));
    statistics->statisticsCount = metrics.size();
    DRM_CHECK_AND_RETURN_RET_LOG((statistics->statisticsCount <= MAX_STATISTICS_COUNT), DRM_ERR_NO_MEMORY,
        "statisticsCount err!");
    for (size_t i = 0; i < metrics.size(); i++) {
        if (metrics[i].name.size() == 0) {
            continue;
        }
        int32_t ret = memcpy_s(statistics->statisticsName[i],
            sizeof(statistics->statisticsName[i]), metrics[i].name.c_str(), metrics[i].name.size());
        if (ret != 0) {
            DRM_ERR_LOG(" memcpy_s faild!");
            return DRM_ERR_NO_MEMORY;
        }
        if (metrics[i].value.size() != 0) {
            ret = memcpy_s(statistics->statisticsDescription[i],
                sizeof(statistics->statisticsDescription[i]), metrics[i].value.c_str(), metrics[i].value.size());
            if (ret != 0) {
                DRM_ERR_LOG(" memcpy_s faild!");
                return DRM_ERR_NO_MEMORY;
            }
        }
    }
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_GetStatistics(MediaKeySystem *mediaKeySystem, DRM_Statistics *statistics)
{
    DRM_INFO_LOG("OH_MediaKeySystem_GetStatistics enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (statistics != nullptr)), DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetStatistics params is error!");
    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    std::vector<IMediaKeySystemService::MetircKeyValue> metrics;
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "mediaKeySystemImpl::GetStatistics inner systemImpl is nullptr!");
    int32_t result = systemObject->systemImpl_->GetStatistics(metrics);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_UNKNOWN,
        "OH_MediaKeySystem_GetStatistics systemObject is nullptr!");
    Drm_ErrCode ret = vectorToClist(metrics, statistics);
    DRM_CHECK_AND_RETURN_RET_LOG(statistics != nullptr, DRM_ERR_UNKNOWN,
        "OH_MediaKeySystem_GetStatistics statistics obtained is nullptr!");
    return ret;
}

Drm_ErrCode OH_MediaKeySystem_GetMaxContentProtectionLevel(MediaKeySystem *mediaKeySystem,
    DRM_ContentProtectionLevel *contentProtectionLevel)
{
    DRM_INFO_LOG("GetMaxContentProtectionLevel enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (contentProtectionLevel != nullptr)),
        DRM_ERR_INVALID_VAL, "OH_MediaKeySystem_GetMaxContentProtectionLevel parameter is error!");
    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "mediaKeySystemImpl::GetMaxContentProtectionLevel faild!");
    int32_t result = DRM_ERR_OK;
    IMediaKeySessionService::ContentProtectionLevel level =
        IMediaKeySessionService::CONTENT_PROTECTION_LEVEL_UNKNOWN;
    result = systemObject->systemImpl_->GetMaxContentProtectionLevel(&level);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_UNKNOWN,
        "OH_MediaKeySystem_GetMaxContentProtectionLevel fail!");
    if (level <= IMediaKeySessionService::CONTENT_PROTECTION_LEVEL_UNKNOWN ||
        level >= IMediaKeySessionService::CONTENT_PROTECTION_LEVEL_MAX) {
        DRM_ERR_LOG("the level obtained is beyond reasonable range!");
        return DRM_ERR_UNKNOWN;
    }
    *contentProtectionLevel = static_cast<DRM_ContentProtectionLevel>(level);
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_GenerateKeySystemRequest(MediaKeySystem *mediaKeySystem, uint8_t *request,
    int32_t *requestLen, char *defaultUrl, int32_t defaultUrlLen)
{
    DRM_INFO_LOG("OH_MediaKeySystem_GenerateKeySystemRequest enter");
    DrmTrace trace("OH_MediaKeySystem_GenerateKeySystemRequest");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (request != nullptr) && (requestLen != nullptr) &&
        (*requestLen > 0) && (defaultUrl != nullptr) && (defaultUrlLen > 0)),
        DRM_ERR_INVALID_VAL, "Incorrect parameters of OH_MediaKeySystem_GenerateKeySystemRequest!");
    std::vector<uint8_t> requestData;
    std::string defaultUrlData;

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "mediaKeySystemImpl::GenerateKeySystemRequest faild!");
    int32_t result = systemObject->systemImpl_->GenerateKeySystemRequest(requestData, defaultUrlData);
    DRM_CHECK_AND_RETURN_RET_LOG(((result == DRM_ERR_OK) && (requestData.size() != 0)), DRM_ERR_UNKNOWN,
        "MediaKeySystemImpl GenerateKeySystemRequest failed!");
    int32_t ret = memcpy_s(request, *requestLen, requestData.data(), requestData.size());
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, DRM_ERR_NO_MEMORY,
        "OH_MediaKeySystem_GenerateKeySystemRequest memcpy_s request failed!");
    *requestLen = requestData.size();
    ret = memset_s(defaultUrl, defaultUrlLen, 0, defaultUrlLen);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, DRM_ERR_NO_MEMORY,
        "OH_MediaKeySystem_GenerateKeySystemRequest memset_s defaultUrl failed!");
    if (defaultUrlData.size() != 0) {
        ret = memcpy_s(defaultUrl, defaultUrlLen, defaultUrlData.data(), defaultUrlData.size());
        DRM_CHECK_AND_RETURN_RET_LOG(ret == 0, DRM_ERR_NO_MEMORY,
            "OH_MediaKeySystem_GenerateKeySystemRequest memcpy_s defaultUrl failed!");
    }
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_ProcessKeySystemResponse(MediaKeySystem *mediaKeySystem,
    uint8_t *response, int32_t responseLen)
{
    DRM_INFO_LOG("OH_MediaKeySystem_ProcessKeySystemResponse enter.");
    int64_t beginTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    DrmTrace trace("OH_MediaKeySystem_ProcessKeySystemResponse");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (response != nullptr) && (responseLen > 0)),
        DRM_ERR_INVALID_VAL, "OH_MediaKeySystem_ProcessKeySystemResponse parameter is error!");
    int32_t result = DRM_ERR_OK;
    std::vector<uint8_t> keySystemResponse(response, response + responseLen);
    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "mediaKeySystemImpl::ProcessKeySystemResponse faild!");
    result = systemObject->systemImpl_->ProcessKeySystemResponse(keySystemResponse);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_UNKNOWN,
        "OH_MediaKeySystem_ProcessKeySystemResponse systemObject is nullptr!");
    ConfigParser::WriteEndEvent(0, 0, std::string("OH_MediaKeySystem_ProcessKeySystemResponse"), beginTime);
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_GetCertificateStatus(MediaKeySystem *mediaKeySystem, DRM_CertificateStatus *certStatus)
{
    DRM_INFO_LOG("OH_MediaKeySystem_GetCertificateStatus enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (certStatus != nullptr)), DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetCertificateStatus parameter is error!");
    IMediaKeySystemService::CertificateStatus CertStatus;
    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "mediaKeySystemImpl::GetCertificateStatus faild!");
    int32_t result = systemObject->systemImpl_->GetCertificateStatus(&CertStatus);
    if (result != DRM_ERR_OK) {
        *certStatus = CERT_STATUS_UNAVAILABLE;
        DRM_ERR_LOG("OH_MediaKeySystem_GetCertificateStatus faild!");
        return DRM_ERR_UNKNOWN;
    }
    *certStatus = (DRM_CertificateStatus)((int32_t)(CertStatus));
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_SetMediaKeySystemCallback(MediaKeySystem *mediaKeySystem,
    MediaKeySystem_Callback callback)
{
    DRM_INFO_LOG("OH_MediaKeySystem_SetMediaKeySystemCallback enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (callback != nullptr)), DRM_ERR_INVALID_VAL,
        "parameter is error!");
    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_SetMediaKeySystemCallback faild!");
    systemObject->systemCallback_->SetCallbackReference(callback);
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_SetCallback(MediaKeySystem *mediaKeySystem, OH_MediaKeySystem_Callback callback)
{
    DRM_INFO_LOG("OH_MediaKeySystem_SetCallback enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (callback != nullptr)), DRM_ERR_INVALID_VAL,
        "parameter is error!");
    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_SetCallback faild!");
    systemObject->systemCallback_->SetCallbackReference(mediaKeySystem, callback);
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_CreateMediaKeySession(MediaKeySystem *mediaKeySystem, DRM_ContentProtectionLevel *level,
    MediaKeySession **mediaKeySession)
{
    DRM_INFO_LOG("OH_MediaKeySystem_CreateMediaKeySession enter.");
    DrmTrace trace("OH_MediaKeySystem_CreateMediaKeySession");
    std::map<int32_t, Drm_ErrCode> errCodeMaps = {
        {24700201, DRM_ERR_SERVICE_DIED},
        {24700104, DRM_ERR_MAX_SESSION_NUM_REACHED},
        {24700101, DRM_ERR_UNKNOWN},
        {0, DRM_ERR_OK}
    };
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (level != nullptr) && (mediaKeySession != nullptr) &&
        (*level > CONTENT_PROTECTION_LEVEL_UNKNOWN) && (*level < CONTENT_PROTECTION_LEVEL_MAX)),
        DRM_ERR_INVALID_VAL, "mediaKeySystem is nullptr!");
    struct MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "mediaKeySystemImpl::CreateMediaKeySession faild!");
    int32_t secure = static_cast<int32_t>(*level);
    IMediaKeySessionService::ContentProtectionLevel secureLevel =
        static_cast<IMediaKeySessionService::ContentProtectionLevel>(secure);
    OHOS::sptr<MediaKeySessionImpl> keySessionImpl = nullptr;
    int32_t ret = systemObject->systemImpl_->CreateMediaKeySession(secureLevel, &keySessionImpl);
    Drm_ErrCode retCode = DRM_ERR_UNKNOWN;
    if (errCodeMaps.find(ret) != errCodeMaps.end()) {
        retCode = errCodeMaps[ret];
    }
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_ERR_OK, retCode, "session create return failed!");
    DRM_CHECK_AND_RETURN_RET_LOG(keySessionImpl != nullptr, DRM_ERR_INVALID_VAL, "session create failed!");

    struct MediaKeySessionObject *sessionObject = new (std::nothrow) MediaKeySessionObject(keySessionImpl);
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject != nullptr, DRM_ERR_NO_MEMORY, "MediaKeySessionObject create failed!");

    sessionObject->sessionCallback_ = new (std::nothrow) MediaKeySessionCallbackCapi();
    if (sessionObject->sessionCallback_ == nullptr) {
        delete sessionObject;
        DRM_ERR_LOG("MediaKeySessionObject create sessionCallback failed!");
        return DRM_ERR_NO_MEMORY;
    }
    ret = sessionObject->sessionImpl_->SetCallback(sessionObject->sessionCallback_);
    if (ret != DRM_ERR_OK) {
        delete sessionObject;
        DRM_ERR_LOG("session set callback failed!");
        return DRM_ERR_UNKNOWN;
    }

    *mediaKeySession = static_cast<MediaKeySession *>(sessionObject);
    return DRM_ERR_OK;
}

static Drm_ErrCode vectorToC2DArray(std::vector<std::vector<uint8_t>> licenseIds,
    DRM_OfflineMediakeyIdArray *offlineMediaKeyIds)
{
    DRM_INFO_LOG("vectorToC2DArray enter.");
    if (licenseIds.size() >= MAX_OFFLINE_MEDIA_KEY_ID_COUNT) {
        DRM_ERR_LOG("licenseIds size too large!");
        return DRM_ERR_NO_MEMORY;
    }

    offlineMediaKeyIds->idsCount = (uint32_t)(licenseIds.size());
    for (size_t i = 0; i < licenseIds.size(); i++) {
        if (licenseIds[i].size() != 0) {
            offlineMediaKeyIds->idsLen[i] = (int32_t)(licenseIds[i].size());
            int32_t ret = memcpy_s(offlineMediaKeyIds->ids[i], MAX_OFFLINE_MEDIA_KEY_ID_LEN, licenseIds[i].data(),
                licenseIds[i].size());
            if (ret != 0) {
                DRM_ERR_LOG("memcpy_s faild!");
                return DRM_ERR_NO_MEMORY;
            }
        }
    }
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_GetOfflineMediaKeyIds(MediaKeySystem *mediaKeySystem,
    DRM_OfflineMediakeyIdArray *offlineMediaKeyIds)
{
    DRM_INFO_LOG("OH_MediaKeySystem_GetOfflineMediaKeyIds enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (offlineMediaKeyIds != nullptr)), DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_GetOfflineMediaKeyIds parameter is error!");
    std::vector<std::vector<uint8_t>> licenseIds;

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "mediaKeySystemImpl::GetOfflineMediaKeyIds faild!");
    int32_t result = systemObject->systemImpl_->GetOfflineMediaKeyIds(licenseIds);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_UNKNOWN,
        "OH_MediaKeySystem_GetOfflineMediaKeyIds faild!");
    if (licenseIds.size() == 0) {
        DRM_DEBUG_LOG("licenseIds.data() is nullptr!");
        return DRM_ERR_OK;
    }
    result = vectorToC2DArray(licenseIds, offlineMediaKeyIds);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_NO_MEMORY,
        "vectorToC2DArray faild!");
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_GetOfflineMediaKeyStatus(MediaKeySystem *mediaKeySystem,
    uint8_t *offlineMediaKeyId, int32_t offlineMediaKeyIdLen, DRM_OfflineMediaKeyStatus *status)
{
    DRM_INFO_LOG("OH_MediaKeySystem_GetOfflineMediaKeyStatus enter");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (offlineMediaKeyId != nullptr) &&
        (offlineMediaKeyIdLen > 0) && (status != nullptr)),
        DRM_ERR_INVALID_VAL, "OH_MediaKeySystem_GetOfflineMediaKeyStatus parameter is error!");
    int32_t result = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;

    std::vector<uint8_t> licenseIdVec(offlineMediaKeyId, offlineMediaKeyId + offlineMediaKeyIdLen);
    IMediaKeySessionService::OfflineMediaKeyStatus offlineMediaKeyStatus =
        IMediaKeySessionService::OFFLINELICENSESTATUS_UNKNOWN;

    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "mediaKeySystemImpl::GetOfflineMediaKeyStatus faild!");
    result = systemObject->systemImpl_->GetOfflineMediaKeyStatus(licenseIdVec, offlineMediaKeyStatus);
    if (result != DRM_ERR_OK) {
        DRM_ERR_LOG("OH_MediaKeySystem_GetOfflineMediaKeyStatus faild!");
        return DRM_ERR_UNKNOWN;
    }
    DRM_OfflineMediaKeyStatus CofflineMediaKeyStatus = (DRM_OfflineMediaKeyStatus)((int32_t)(offlineMediaKeyStatus));
    if (CofflineMediaKeyStatus < OFFLINE_MEDIA_KEY_STATUS_UNKNOWN ||
        CofflineMediaKeyStatus > OFFLINE_MEDIA_KEY_STATUS_INACTIVE) {
        DRM_ERR_LOG("OH_MediaKeySystem_GetOfflineMediaKeyStatus faild!");
        return DRM_ERR_UNKNOWN;
    }
    *status = CofflineMediaKeyStatus;
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_ClearOfflineMediaKeys(MediaKeySystem *mediaKeySystem,
    uint8_t *offlineMediaKeyId, int32_t offlineMediaKeyIdLen)
{
    DRM_INFO_LOG("OH_MediaKeySystem_ClearOfflineMediaKeys enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(((mediaKeySystem != nullptr) && (offlineMediaKeyId != nullptr) &&
        (offlineMediaKeyIdLen > 0)),
        DRM_ERR_INVALID_VAL, "OH_MediaKeySystem_ClearOfflineMediaKeys parameter is error!");
    int32_t result = DRM_ERR_OK;
    std::vector<uint8_t> licenseIdVec(offlineMediaKeyId, offlineMediaKeyId + offlineMediaKeyIdLen);
    DRM_CHECK_AND_RETURN_RET_LOG(licenseIdVec.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySystem_ClearOfflineMediaKeys configName.size is not zero!");
    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "mediaKeySystemImpl::OH_MediaKeySystem_ClearOfflineMediaKeys inner systemImpl is nullptr!");
    result = systemObject->systemImpl_->ClearOfflineMediaKeys(licenseIdVec);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_UNKNOWN,
        "OH_MediaKeySystem_ClearOfflineMediaKeys mediaKeySystemImpl::ClearOfflineMediaKeys faild!");
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySystem_Destroy(MediaKeySystem *mediaKeySystem)
{
    DRM_INFO_LOG("OH_MediaKeySystem_Destroy enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySystem != nullptr, DRM_ERR_INVALID_VAL, "mediaKeySystem is nullptr!");

    struct MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    DRM_CHECK_AND_RETURN_RET_LOG(systemObject->systemImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "mediaKeySystemImpl::OH_MediaKeySystem_Destroy faild!");
    int32_t ret = systemObject->systemImpl_->Release();
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_ERR_OK, DRM_ERR_UNKNOWN, "call media key system release failed!");
    delete mediaKeySystem;
    mediaKeySystem = nullptr;
    return DRM_ERR_OK;
}