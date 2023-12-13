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
#include "native_drm_base.h"
#include "native_drm_object.h"
#include "key_session_impl.h"
#include "native_mediakeysession.h"


using namespace OHOS::DrmStandard;


OH_DrmErrCode OH_MediaKeySession_GenerateLicenseRequest(OH_MediaKeySession *mediaKeySession,
    OH_DRM_MediaKeyRequestInfo *info, unsigned char **mediaKeyRequest, int32_t *mediaKeyRequestLen)
{
    DRM_INFO_LOG("OH_MediaKeySession_GenerateLicenseRequest enter");
    DRM_INFO_LOG("OH_MediaKeySession_GenerateLicenseRequest exit");
    return DRM_ERR_OK;
}

OH_DrmErrCode OH_MediaKeySession_ProcessLicenseResponse(OH_MediaKeySession *keySession, unsigned char *response,
    uint32_t responseLen, unsigned char *keyId, uint32_t *keyIdLen)
{
    DRM_INFO_LOG("OH_MediaKeySession_ProcessLicenseResponse called");
    DRM_CHECK_AND_RETURN_RET_LOG(keySession != nullptr, DRM_ERR_INVALID_VAL, "keySession is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(keyId != nullptr, DRM_ERR_INVALID_VAL, "licenseId is nullptr!");

    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(keySession);
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject != nullptr, DRM_ERR_INVALID_VAL, "sessionObject is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject->sessionImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "systemImpl_ is nullptr!");
    std::vector<uint8_t> licenseResponseVec(response, response + responseLen);
    std::vector<uint8_t> keyIdVec;
    int32_t ret = sessionObject->sessionImpl_->ProcessLicenseResponse(keyIdVec, licenseResponseVec);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_ERR_OK, DRM_ERR_INVALID_VAL, "got licenseResponse null!");
    DRM_CHECK_AND_RETURN_RET_LOG(!licenseResponseVec.empty(), DRM_ERR_INVALID_VAL, "got licenseResponse null!");
    ret = memcpy_s(keyId, keyIdVec.size(), keyIdVec.data(), keyIdVec.size());
    if (ret != 0) {
        DRM_ERR_LOG("OH_MediaKeySession_ProcessLicenseResponse memcpy_s faild!");
        return DRM_ERR_ERROR;
    }
    *keyIdLen = keyIdVec.size();
    return DRM_ERR_OK;
}

static OH_DRM_MediaKeyDescription *MapToClist(
    std::map<std::string, MediaKeySessionKeyStatus> licenseStatus)
{
    DRM_INFO_LOG("MapToClist start.");
    int32_t max = sizeof(uint32_t);
    int offset = licenseStatus.size() * sizeof(OH_DRM_EnumBufferPair);
    for (auto it = licenseStatus.begin(); it != licenseStatus.end(); it++) {
        max += (sizeof(OH_DRM_EnumBufferPair) + it->first.size());
    }
    OH_DRM_MediaKeyDescription *cArray = (OH_DRM_MediaKeyDescription *)malloc(max);
    DRM_CHECK_AND_RETURN_RET_LOG(cArray != nullptr, nullptr, "malloc faild!");
    cArray->mediaKeyCount = licenseStatus.size();
    OH_DRM_EnumBufferPair *dest = &((cArray->description)[0]);
    auto it = licenseStatus.begin();
    for (size_t i = 0; i < licenseStatus.size(); i++) {
        dest[i].name.bufferLen = it->first.size();
        dest[i].name.buffer = (char *)((uint8_t *)dest + offset);
        int32_t ret = memcpy_s(dest[i].name.buffer, it->first.size(), it->first.c_str(), it->first.size());
        if (ret != 0) {
            DRM_ERR_LOG("MapToClist memcpy_s faild!");
            return nullptr;
        }
        dest[i].value = (int32_t)(it->second);
        offset += it->first.size();
        it++;
    }
    DRM_INFO_LOG("MapToClist exit.");
    return cArray;
}

OH_DrmErrCode OH_MediaKeySession_CheckMediaKeyStatus(OH_MediaKeySession *mediaKeySessoin,
    OH_DRM_MediaKeyDescription **mediaKeyDescription)
{
    DRM_INFO_LOG("OH_MediaKeySession_CheckMediaKeyStatus enter");
    std::map<std::string, MediaKeySessionKeyStatus> licenseStatus;

    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(mediaKeySessoin);
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_CheckMediaKeyStatus sessionObject is nullptr!");

    int32_t result = sessionObject->sessionImpl_->CheckLicenseStatus(licenseStatus);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray mediaKeySystemImpl::SetConfigurationByteArray faild!");
    if (licenseStatus.size() == 0) {
        DRM_ERR_LOG("Licence not exist.");
        return DRM_ERR_INVALID_VAL;
    }
    *mediaKeyDescription = MapToClist(licenseStatus);
    DRM_INFO_LOG("OH_MediaKeySession_CheckMediaKeyStatus exit.");
    return DRM_ERR_OK;
}

OH_DrmErrCode OH_MediaKeySession_RemoveLicense(OH_MediaKeySession *keySession)
{
    DRM_INFO_LOG("OH_MediaKeySession_RemoveLicense enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(keySession != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_RemoveLicense keySession is nullptr!");
    int32_t currentPid = OHOS::IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySessionNapi GetCallingPID: %{public}d", currentPid);
    int32_t result = DRM_ERR_ERROR;
    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(keySession);
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_GenerateLicenseRequest sessionObject is nullptr!");
    result = sessionObject->sessionImpl_->RemoveLicense();
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray mediaKeySystemImpl::SetConfigurationByteArray faild!");
    DRM_INFO_LOG("OH_MediaKeySession_RemoveLicense exit.");
    return DRM_ERR_OK;
}

OH_DrmErrCode OH_MediaKeySession_GenerateOfflineReleaseRequest(OH_MediaKeySession *mediaKeySessoin,
    OH_DRM_Uint8Buffer *mediaKeyId, unsigned char **releaseRequest, int32_t *releaseRequestLen)
{
    DRM_INFO_LOG("OH_MediaKeySession_GenerateOfflineReleaseRequest enter");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySessoin != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_GenerateOfflineReleaseRequest keySession is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeyId != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_GenerateOfflineReleaseRequest mediaKeyId is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeyId->buffer != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_GenerateOfflineReleaseRequest mediaKeyId->buffer is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeyId->bufferLen != 0, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_GenerateOfflineReleaseRequest mediaKeyId->bufferLen is zero!");
    DRM_CHECK_AND_RETURN_RET_LOG(releaseRequestLen != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_GenerateOfflineReleaseRequest releaseRequestLen is nullptr!");
    std::vector<uint8_t> ReleaseRequest;
    uint8_t *licenseIdPtr = reinterpret_cast<uint8_t *>(mediaKeyId->buffer);
    std::vector<uint8_t> licenseIdVec(licenseIdPtr, licenseIdPtr + mediaKeyId->bufferLen);

    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(mediaKeySessoin);
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_GenerateOfflineReleaseRequest sessionObject is nullptr!");

    uint32_t result = sessionObject->sessionImpl_->GenerateOfflineReleaseRequest(licenseIdVec, ReleaseRequest);
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_GenerateOfflineReleaseRequest GenerateOfflineReleaseRequest faild!");
    *releaseRequest = (unsigned char *)malloc(ReleaseRequest.size());
    int32_t ret = memcpy_s(*releaseRequest, ReleaseRequest.size(), ReleaseRequest.data(), ReleaseRequest.size());
    if (ret != 0) {
        DRM_ERR_LOG("OH_MediaKeySession_GenerateOfflineReleaseRequest memcpy_s faild!");
        return DRM_ERR_INVALID_VAL;
    }
    *releaseRequestLen = ReleaseRequest.size();
    DRM_INFO_LOG("OH_MediaKeySession_GenerateOfflineReleaseRequest exit");
    return DRM_ERR_OK;
}

OH_DrmErrCode OH_MediaKeySession_ProcessOfflineReleaseResponse(OH_MediaKeySession *keySession, uint8_t *licenseId,
    size_t licenseIdLen, uint8_t *response, size_t responseLen)
{
    DRM_INFO_LOG("OH_MediaKeySession_ProcessOfflineReleaseResponse enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(keySession != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_RemoveLicense keySession is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(licenseId != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_RemoveLicense licenseId is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(licenseIdLen != 0, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_RemoveLicense licenseIdLen is zero!");
    DRM_CHECK_AND_RETURN_RET_LOG(response != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_RemoveLicense response is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(responseLen != 0, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_RemoveLicense responseLen is zero!");
    std::vector<uint8_t> licenseIdVec(licenseId, licenseId + licenseIdLen);
    DRM_CHECK_AND_RETURN_RET_LOG(licenseIdVec.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_RemoveLicense licenseIdVec.size is zero!");
    std::vector<uint8_t> responseVec(response, response + responseLen);
    DRM_CHECK_AND_RETURN_RET_LOG(responseVec.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_RemoveLicense responseVec.size is zero!");
    int32_t result = DRM_ERR_ERROR;
    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(keySession);
    DRM_CHECK_AND_RETURN_RET_LOG((sessionObject != nullptr), DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_RemoveLicense get sessionObject fail!");
    result = sessionObject->sessionImpl_->ProcessOfflineReleaseResponse(licenseIdVec, responseVec);
    DRM_CHECK_AND_RETURN_RET_LOG((result == DRM_ERR_OK), DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_ProcessOfflineReleaseResponse call Failed!");
    DRM_INFO_LOG("OH_MediaKeySession_ProcessOfflineReleaseResponse exit.");
    return DRM_ERR_OK;
}

OH_DrmErrCode OH_MediaKeySession_RestoreOfflineLicense(OH_MediaKeySession *keySession, uint8_t *licenseId,
    size_t licenseIdLen)
{
    DRM_INFO_LOG("OH_MediaKeySession_RestoreOfflineLicense enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(keySession != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_RestoreOfflineLicense keySession is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(licenseId != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_RestoreOfflineLicense licenseId is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(licenseIdLen != 0, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_RestoreOfflineLicense licenseIdLen is zero!");
    std::vector<uint8_t> licenseIdVec(licenseId, licenseId + licenseIdLen);
    int32_t result = DRM_ERR_ERROR;
    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(keySession);
    DRM_CHECK_AND_RETURN_RET_LOG((sessionObject != nullptr), DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_RestoreOfflineLicense get sessionObject fail!");
    result = sessionObject->sessionImpl_->RestoreOfflineLicense(licenseIdVec);
    DRM_CHECK_AND_RETURN_RET_LOG((result == DRM_ERR_OK), DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_RestoreOfflineLicense call Failed!");
    DRM_INFO_LOG("OH_MediaKeySession_RestoreOfflineLicense exit.");
    return DRM_ERR_OK;
}

OH_SecurityLevel OH_MediaKeySession_GetContentProtectionLevel(OH_MediaKeySession *mediaKeySessoin)
{
    DRM_INFO_LOG("OH_MediaKeySession_GetSecurityLevel enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySessoin != nullptr, SECURITY_LEVEL_UNKNOWN,
        "OH_MediaKeySession_GetSecurityLevel keySession is nullptr!");
    IMediaKeySessionService::SecurityLevel level = IMediaKeySessionService::SecurityLevel::SECURITY_LEVEL_UNKNOWN;
    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(mediaKeySessoin);
    DRM_CHECK_AND_RETURN_RET_LOG((sessionObject != nullptr), SECURITY_LEVEL_UNKNOWN,
        "OH_MediaKeySession_RestoreOfflineLicense get sessionObject fail!");
    sessionObject->sessionImpl_->GetSecurityLevel(&level);
    OH_SecurityLevel SecurityLevel = static_cast<OH_SecurityLevel>(level);
    if (SecurityLevel < SECURITY_LEVEL_UNKNOWN || SecurityLevel > SECURITY_LEVEL_MAX) {
        DRM_ERR_LOG("OH_MediaKeySession_GetSecurityLevel faild!");
        return SECURITY_LEVEL_UNKNOWN;
    }
    DRM_INFO_LOG("OH_MediaKeySession_GetSecurityLevel exit");
    return SecurityLevel;
}

OH_DrmBool OH_MediaKeySession_RequireSecureDecoderModule(OH_MediaKeySession *keySession, const char *mimeType)
{
    DRM_INFO_LOG("OH_MediaKeySession_RequireSecureDecoderModule enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(keySession != nullptr, DRM_BOOL_FALSE,
        "OH_MediaKeySession_RequireSecureDecoderModule keySession is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mimeType != nullptr, DRM_BOOL_FALSE,
        "OH_MediaKeySession_RequireSecureDecoderModule mimeType is nullptr!");
    OH_DrmBool StatusValue = DRM_BOOL_FALSE;
    std::string mimeTypeBuf = std::string(mimeType);
    bool statusValue = false;
    int32_t result = DRM_ERR_ERROR;
    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(keySession);
    DRM_CHECK_AND_RETURN_RET_LOG((sessionObject != nullptr), DRM_BOOL_FALSE,
        "OH_MediaKeySession_RequireSecureDecoderModule get sessionObject fail!");
    result = sessionObject->sessionImpl_->RequireSecureDecoderModule(mimeTypeBuf, &statusValue);
    if (result != DRM_ERR_OK) {
        DRM_ERR_LOG("OH_MediaKeySession_RequireSecureDecoderModule keySessionImpl_->RequireSecureDecoderModule faild!");
        return DRM_BOOL_FALSE;
    }
    if (statusValue) {
        StatusValue = DRM_BOOL_TRUE;
    }
    DRM_INFO_LOG("OH_MediaKeySession_RequireSecureDecoderModule exit.");
    return StatusValue;
}

OH_DrmErrCode OH_MediaKeySession_SetMediaKeySessionCallback(OH_MediaKeySession *mediaKeySessoin,
    OH_MediaKeySessionCallback *callback)
{
    return DRM_ERR_OK;
}

OH_DrmErrCode OH_MediaKeySession_Destroy(OH_MediaKeySession *keySession)
{
    DRM_INFO_LOG("OH_MediaKeySession_Destroy enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(keySession != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_Destroy keySession is nullptr!");
    int32_t result = DRM_ERR_ERROR;
    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(keySession);
    DRM_CHECK_AND_RETURN_RET_LOG((sessionObject != nullptr), DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_Destroy get sessionObject fail!");
    result = sessionObject->sessionImpl_->Release();
    if (result != DRM_ERR_OK) {
        DRM_ERR_LOG("OH_MediaKeySession_Destroy keySessionImpl_->Release faild!");
        return DRM_ERR_ERROR;
    }
    DRM_INFO_LOG("OH_MediaKeySession_Destroy exit.");
    return DRM_ERR_OK;
}