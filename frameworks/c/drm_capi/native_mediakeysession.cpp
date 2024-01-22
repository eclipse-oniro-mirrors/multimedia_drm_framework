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
#include "native_drm_base.h"
#include "native_drm_object.h"
#include "key_session_impl.h"
#include "native_mediakeysession.h"
#include "native_mediakeysystem.h"

using namespace OHOS::DrmStandard;


static DRM_MediaKeyRequest *DealMediaKeyRequest(IMediaKeySessionService::MediaKeyRequest &licenseRequest)
{
    int max = 0;
    int offset = sizeof(DRM_MediaKeyRequest);
    max = max + sizeof(DRM_MediaKeyRequest) + licenseRequest.mDefaultURL.size() + licenseRequest.mData.size();
    DRM_MediaKeyRequest *mediaKeyRequest = (DRM_MediaKeyRequest *)malloc(max);
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeyRequest != nullptr, nullptr, "mediaKeyRequest is nullptr!");
    mediaKeyRequest->type = (DRM_MediaKeyRequestType)(licenseRequest.requestType);

    mediaKeyRequest->data.buffer = (unsigned char *)((uint8_t *)mediaKeyRequest + offset);
    int ret = memcpy_s(mediaKeyRequest->data.buffer, licenseRequest.mData.size(), licenseRequest.mData.data(),
        licenseRequest.mData.size());
    if (ret != 0) {
        DRM_DEBUG_LOG("licenseRequest.mData.data() is nullptr!");
    }
    mediaKeyRequest->data.bufferLen = licenseRequest.mData.size();
    offset += licenseRequest.mData.size();
    mediaKeyRequest->defaultUrl.buffer = (char *)((uint8_t *)mediaKeyRequest + offset);
    ret = memcpy_s(mediaKeyRequest->defaultUrl.buffer, licenseRequest.mDefaultURL.size(),
        licenseRequest.mDefaultURL.data(), licenseRequest.mDefaultURL.size());
    if (ret != 0) {
        DRM_DEBUG_LOG("licenseRequest.mDefaultURL.data() is nullptr!");
    }
    mediaKeyRequest->defaultUrl.bufferLen = licenseRequest.mDefaultURL.size();
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeyRequest != nullptr, nullptr, "mediaKeyRequest is nullptr!");
    return mediaKeyRequest;
}

Drm_ErrCode OH_MediaKeySession_GenerateMediaKeyRequest(MediaKeySession *mediaKeySession,
    DRM_MediaKeyRequestInfo *info, DRM_MediaKeyRequest **mediaKeyRequest)
{
    DRM_INFO_LOG("OH_MediaKeySession_GenerateMediaKeyRequest enter");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySession != nullptr, DRM_ERR_INVALID_VAL, "mediaKeySession is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(info != nullptr, DRM_ERR_INVALID_VAL, "info is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeyRequest != nullptr, DRM_ERR_INVALID_VAL, "mediaKeyRequest is nullptr!");
    IMediaKeySessionService::MediaKeyRequestInfo licenseRequestInfo;
    IMediaKeySessionService::MediaKeyRequest licenseRequest;
    licenseRequest.requestType = OHOS::DrmStandard::IMediaKeySessionService::REQUEST_TYPE_RELEASE;
    licenseRequestInfo.mediaKeyType = (IMediaKeySessionService::MediaKeyType)info->type;
    licenseRequestInfo.mimeType = std::string(info->mimeType.buffer, info->mimeType.buffer + info->mimeType.bufferLen);
    std::vector<uint8_t> initDataStr(info->data.buffer, info->data.buffer + info->data.bufferLen);
    licenseRequestInfo.initData = initDataStr;
    for (uint32_t i = 0; i < info->optionsCount; i++) {
        std::string optionsname(info->optionsData->name.buffer,
            info->optionsData->name.buffer + info->optionsData->name.bufferLen);
        std::string optionsvalue(info->optionsData->value.buffer,
            info->optionsData->value.buffer + info->optionsData->value.bufferLen);
        licenseRequestInfo.optionalData.insert(std::make_pair(optionsname, optionsvalue));
    }
    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(mediaKeySession);
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject != nullptr, DRM_ERR_INVALID_VAL, "sessionObject is nullptr!");
    int ret = sessionObject->sessionImpl_->GenerateMediaKeyRequest(licenseRequestInfo, licenseRequest);
    DRM_CHECK_AND_RETURN_RET_LOG((ret == DRM_OK), DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_GenerateMediaKeyRequest call Failed!");
    *mediaKeyRequest = DealMediaKeyRequest(licenseRequest);
    DRM_CHECK_AND_RETURN_RET_LOG(*mediaKeyRequest != nullptr, DRM_ERR_INVALID_VAL, "*mediaKeyRequest is nullptr!");
    DRM_INFO_LOG("OH_MediaKeySession_GenerateMediaKeyRequest exit");
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySession_ProcessMediaKeyResponse(MediaKeySession *keySession, DRM_Uint8Buffer *response,
    unsigned char **mediaKeyId, int32_t *mediaKeyIdLen)
{
    DRM_INFO_LOG("OH_MediaKeySession_ProcessMediaKeyResponse called");
    DRM_CHECK_AND_RETURN_RET_LOG(keySession != nullptr, DRM_ERR_INVALID_VAL, "keySession is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeyIdLen != nullptr, DRM_ERR_INVALID_VAL, "mediaKeyIdLen is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(response != nullptr, DRM_ERR_INVALID_VAL, "response is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeyId != nullptr, DRM_ERR_INVALID_VAL, "mediaKeyId is nullptr!");

    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(keySession);
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject != nullptr, DRM_ERR_INVALID_VAL, "sessionObject is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject->sessionImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "systemImpl_ is nullptr!");
    std::vector<uint8_t> licenseResponseVec(response->buffer, response->buffer + response->bufferLen);
    std::vector<uint8_t> keyIdVec;
    int32_t ret = sessionObject->sessionImpl_->ProcessMediaKeyResponse(keyIdVec, licenseResponseVec);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_ERR_OK, DRM_ERR_INVALID_VAL, "got licenseResponse null!");
    DRM_CHECK_AND_RETURN_RET_LOG(!licenseResponseVec.empty(), DRM_ERR_INVALID_VAL, "got licenseResponse null!");
    *mediaKeyIdLen = keyIdVec.size();
    if (keyIdVec.size() == 0) {
        DRM_DEBUG_LOG("keyIdVec.data() is nullptr!");
        return DRM_ERR_OK;
    }
    *mediaKeyId = (unsigned char *)malloc(keyIdVec.size());
    DRM_CHECK_AND_RETURN_RET_LOG(*mediaKeyId != nullptr, DRM_ERR_INVALID_VAL, "malloc faild!");
    ret = memcpy_s(*mediaKeyId, keyIdVec.size(), keyIdVec.data(), keyIdVec.size());
    if (ret != 0) {
        DRM_ERR_LOG("OH_MediaKeySession_ProcessMediaKeyResponse memcpy_s faild!");
        return DRM_ERR_NO_MEMORY;
    }
    return DRM_ERR_OK;
}

static DRM_MediaKeyDescription *MapToClist(std::map<std::string, std::string> licenseStatus)
{
    DRM_INFO_LOG("MapToClist start.");
    int32_t max = sizeof(uint32_t);
    int offset = licenseStatus.size() * sizeof(DRM_CharBufferPair);
    for (auto it = licenseStatus.begin(); it != licenseStatus.end(); it++) {
        max += (sizeof(DRM_CharBufferPair) + it->first.size() + it->second.size());
    }
    DRM_MediaKeyDescription *cArray = (DRM_MediaKeyDescription *)malloc(max);
    DRM_CHECK_AND_RETURN_RET_LOG(cArray != nullptr, nullptr, "malloc faild!");
    cArray->mediaKeyCount = licenseStatus.size();
    DRM_CharBufferPair *dest = &((cArray->description)[0]);
    auto it = licenseStatus.begin();
    for (size_t i = 0; i < licenseStatus.size(); i++) {
        dest[i].name.bufferLen = it->first.size();
        dest[i].name.buffer = (char *)((uint8_t *)dest + offset);
        int32_t ret = memcpy_s(dest[i].name.buffer, it->first.size(), it->first.c_str(), it->first.size());
        if (ret != 0) {
            DRM_ERR_LOG("MapToClist memcpy_s faild!");
            return nullptr;
        }
        offset += it->first.size();

        dest[i].value.bufferLen = it->second.size();
        dest[i].value.buffer = (char *)((uint8_t *)dest + offset);
        ret = memcpy_s(dest[i].value.buffer, it->second.size(), it->second.c_str(), it->second.size());
        if (ret != 0) {
            DRM_ERR_LOG("MapToClist memcpy_s faild!");
            return nullptr;
        }
        offset += it->second.size();
        it++;
    }
    DRM_INFO_LOG("MapToClist exit.");
    return cArray;
}

Drm_ErrCode OH_MediaKeySession_CheckMediaKeyStatus(MediaKeySession *mediaKeySessoin,
    DRM_MediaKeyDescription **mediaKeyDescription)
{
    DRM_INFO_LOG("OH_MediaKeySession_CheckMediaKeyStatus enter");
    std::map<std::string, std::string> licenseStatus;
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySessoin != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_ClearMediaKeys keySession is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeyDescription != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_ClearMediaKeys mediaKeyDescription is nullptr!");
    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(mediaKeySessoin);
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_CheckMediaKeyStatus sessionObject is nullptr!");
    int32_t result = sessionObject->sessionImpl_->CheckMediaKeyStatus(licenseStatus);
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

Drm_ErrCode OH_MediaKeySession_ClearMediaKeys(MediaKeySession *mediaKeySessoin)
{
    DRM_INFO_LOG("OH_MediaKeySession_ClearMediaKeys enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySessoin != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_ClearMediaKeys keySession is nullptr!");
    int32_t currentPid = OHOS::IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySessionNapi GetCallingPID: %{public}d", currentPid);
    int32_t result = DRM_ERR_OK;
    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(mediaKeySessoin);
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_GenerateMediaKeyRequest sessionObject is nullptr!");
    result = sessionObject->sessionImpl_->ClearMediaKeys();
    DRM_CHECK_AND_RETURN_RET_LOG(result == DRM_ERR_OK, DRM_ERR_INVALID_VAL,
        "OH_SetConfigurationByteArray mediaKeySystemImpl::SetConfigurationByteArray faild!");
    DRM_INFO_LOG("OH_MediaKeySession_ClearMediaKeys exit.");
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySession_GenerateOfflineReleaseRequest(MediaKeySession *mediaKeySessoin,
    DRM_Uint8Buffer *mediaKeyId, unsigned char **releaseRequest, int32_t *releaseRequestLen)
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
    DRM_CHECK_AND_RETURN_RET_LOG(releaseRequest != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_GenerateOfflineReleaseRequest releaseRequest is zero!");
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
    *releaseRequestLen = ReleaseRequest.size();
    if (*releaseRequestLen == 0) {
        DRM_DEBUG_LOG("ReleaseRequest.data() is nullptr!");
        return DRM_ERR_OK;
    }
    *releaseRequest = (unsigned char *)malloc(ReleaseRequest.size());
    DRM_CHECK_AND_RETURN_RET_LOG(*releaseRequest != nullptr, DRM_ERR_INVALID_VAL, "malloc faild!");
    int32_t ret = memcpy_s(*releaseRequest, ReleaseRequest.size(), ReleaseRequest.data(), ReleaseRequest.size());
    if (ret != 0) {
        DRM_ERR_LOG("OH_MediaKeySession_GenerateOfflineReleaseRequest memcpy_s faild!");
        return DRM_ERR_INVALID_VAL;
    }
    DRM_INFO_LOG("OH_MediaKeySession_GenerateOfflineReleaseRequest exit");
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySession_ProcessOfflineReleaseResponse(MediaKeySession *mediaKeySessoin,
    DRM_Uint8Buffer *mediaKeyId, DRM_Uint8Buffer *releaseReponse)
{
    DRM_INFO_LOG("OH_MediaKeySession_ProcessOfflineReleaseResponse enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySessoin != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_ClearMediaKeys keySession is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeyId != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_ClearMediaKeys licenseIdLen is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeyId->buffer != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_ClearMediaKeys licenseId is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeyId->bufferLen != 0, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_ClearMediaKeys licenseIdLen is zero!");
    DRM_CHECK_AND_RETURN_RET_LOG(releaseReponse != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_ClearMediaKeys releaseReponse is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(releaseReponse->buffer != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_ClearMediaKeys response is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(releaseReponse->bufferLen != 0, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_ClearMediaKeys responseLen is zero!");
    std::vector<uint8_t> licenseIdVec(mediaKeyId->buffer, mediaKeyId->buffer + mediaKeyId->bufferLen);
    DRM_CHECK_AND_RETURN_RET_LOG(licenseIdVec.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_ClearMediaKeys licenseIdVec.size is zero!");
    std::vector<uint8_t> responseVec(releaseReponse->buffer, releaseReponse->buffer + releaseReponse->bufferLen);
    DRM_CHECK_AND_RETURN_RET_LOG(responseVec.size() != 0, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_ClearMediaKeys responseVec.size is zero!");
    int32_t result = DRM_ERR_OK;
    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(mediaKeySessoin);
    DRM_CHECK_AND_RETURN_RET_LOG((sessionObject != nullptr), DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_ClearMediaKeys get sessionObject fail!");
    result = sessionObject->sessionImpl_->ProcessOfflineReleaseResponse(licenseIdVec, responseVec);
    DRM_CHECK_AND_RETURN_RET_LOG((result == DRM_ERR_OK), DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_ProcessOfflineReleaseResponse call Failed!");
    DRM_INFO_LOG("OH_MediaKeySession_ProcessOfflineReleaseResponse exit.");
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySession_RestoreOfflineMediaKeys(MediaKeySession *mediaKeySessoin,
    DRM_Uint8Buffer *mediaKeyId)
{
    DRM_INFO_LOG("OH_MediaKeySession_restoreOfflineMediaKey enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySessoin != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_restoreOfflineMediaKey keySession is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeyId != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_restoreOfflineMediaKey mediaKeyId is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeyId->buffer != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_restoreOfflineMediaKey licenseId is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeyId->bufferLen != 0, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_restoreOfflineMediaKey licenseIdLen is zero!");
    std::vector<uint8_t> licenseIdVec(mediaKeyId->buffer, mediaKeyId->buffer + mediaKeyId->bufferLen);
    int32_t result = DRM_ERR_OK;
    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(mediaKeySessoin);
    DRM_CHECK_AND_RETURN_RET_LOG((sessionObject != nullptr), DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_restoreOfflineMediaKey get sessionObject fail!");
    result = sessionObject->sessionImpl_->RestoreOfflineMediaKeys(licenseIdVec);
    DRM_CHECK_AND_RETURN_RET_LOG((result == DRM_ERR_OK), DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_restoreOfflineMediaKey call Failed!");
    DRM_INFO_LOG("OH_MediaKeySession_restoreOfflineMediaKey exit.");
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySession_GetContentProtectionLevel(MediaKeySession *mediaKeySessoin,
    DRM_ContentProtectionLevel *contentProtectionLevel)
{
    DRM_INFO_LOG("OH_MediaKeySession_GetContentProtectionLevel enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySessoin != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_GetContentProtectionLevel keySession is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(contentProtectionLevel != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_GetContentProtectionLevel contentProtectionLevel is nullptr!");
    int32_t result = DRM_ERR_OK;
    IMediaKeySessionService::ContentProtectionLevel level =
        IMediaKeySessionService::ContentProtectionLevel::SECURITY_LEVEL_UNKNOWN;
    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(mediaKeySessoin);
    DRM_CHECK_AND_RETURN_RET_LOG((sessionObject != nullptr), DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_restoreOfflineMediaKey get sessionObject fail!");
    result = sessionObject->sessionImpl_->GetContentProtectionLevel(&level);
    DRM_CHECK_AND_RETURN_RET_LOG((result == DRM_ERR_OK), DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_GetContentProtectionLevel get level fail!");
    *contentProtectionLevel = static_cast<DRM_ContentProtectionLevel>(level);
    if (*contentProtectionLevel < CONTENT_PROTECTION_LEVEL_UNKNOWN ||
        *contentProtectionLevel > CONTENT_PROTECTION_LEVEL_MAX) {
        DRM_ERR_LOG("OH_MediaKeySession_GetContentProtectionLevel faild!");
        return DRM_ERR_INVALID_VAL;
    }
    DRM_INFO_LOG("OH_MediaKeySession_GetContentProtectionLevel exit");
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySession_RequireSecureDecoderModule(MediaKeySession *mediaKeySessoin, const char *mimeType,
    bool *status)
{
    DRM_INFO_LOG("OH_MediaKeySession_RequireSecureDecoderModule enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySessoin != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_RequireSecureDecoderModule keySession is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(mimeType != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_RequireSecureDecoderModule mimeType is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(status != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_RequireSecureDecoderModule status is nullptr!");
    std::string mimeTypeBuf = std::string(mimeType);
    DRM_CHECK_AND_RETURN_RET_LOG((mimeTypeBuf.size() != 0), DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_RequireSecureDecoderModule mimeTypesize is zero!");
    bool statusValue = false;
    int32_t result = DRM_ERR_OK;
    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(mediaKeySessoin);
    DRM_CHECK_AND_RETURN_RET_LOG((sessionObject != nullptr), DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_RequireSecureDecoderModule get sessionObject fail!");
    result = sessionObject->sessionImpl_->RequireSecureDecoderModule(mimeTypeBuf, &statusValue);
    if (result != DRM_ERR_OK) {
        DRM_ERR_LOG("OH_MediaKeySession_RequireSecureDecoderModule keySessionImpl_->RequireSecureDecoderModule faild!");
        return DRM_ERR_INVALID_VAL;
    }
    *status = statusValue;
    DRM_INFO_LOG("OH_MediaKeySession_RequireSecureDecoderModule exit.");
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySession_SetMediaKeySessionCallback(MediaKeySession *mediaKeySessoin,
    MediaKeySession_Callback *callback)
{
    DRM_INFO_LOG("OH_MediaKeySession_SetMediaKeySessionCallback enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(mediaKeySessoin != nullptr, DRM_ERR_INVALID_VAL, "mediaKeySessoin is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(callback != nullptr, DRM_ERR_INVALID_VAL, "callback is nullptr!");
    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(mediaKeySessoin);
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject != nullptr, DRM_ERR_INVALID_VAL, "sessionObject is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject->sessionCallback_ != nullptr, DRM_ERR_INVALID_VAL,
        "sessionCallback is nullptr!");
    sessionObject->sessionCallback_->SetCallbackReference(*callback);
    return DRM_ERR_OK;
}

Drm_ErrCode OH_MediaKeySession_Destroy(MediaKeySession *keySession)
{
    DRM_INFO_LOG("OH_MediaKeySession_Destroy enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(keySession != nullptr, DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_Destroy keySession is nullptr!");
    int32_t result = DRM_ERR_OK;
    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(keySession);
    DRM_CHECK_AND_RETURN_RET_LOG((sessionObject != nullptr), DRM_ERR_INVALID_VAL,
        "OH_MediaKeySession_Destroy get sessionObject fail!");
    result = sessionObject->sessionImpl_->Release();
    if (result != DRM_ERR_OK) {
        DRM_ERR_LOG("OH_MediaKeySession_Destroy keySessionImpl_->Release faild!");
        return DRM_ERR_INVALID_STATE;
    }
    delete sessionObject;
    DRM_INFO_LOG("OH_MediaKeySession_Destroy exit.");
    return DRM_ERR_OK;
}