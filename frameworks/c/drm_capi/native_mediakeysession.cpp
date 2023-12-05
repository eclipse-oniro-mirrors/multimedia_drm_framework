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
#include <vector>
#include <refbase.h>

#include "drm_log.h"
#include "drm_error_code.h"

#include "native_drm_object.h"
#include "key_session_impl.h"
#include "native_mediakeysession.h"

using namespace OHOS::DrmStandard;

OH_DrmErrCode OH_MediaKeySession_GenerateLicenseRequest(OH_MediaKeySession *keySession,
    OH_DRM_MediaKeyRequestInfo *info, OH_DRM_LicenseRequest *request)
{
    DRM_INFO_LOG("OH_MediaKeySession_GenerateLicenseRequest called");
    DRM_CHECK_AND_RETURN_RET_LOG(keySession != nullptr, DRM_ERR_INVALID_VAL, "keySession is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(info != nullptr, DRM_ERR_INVALID_VAL, "request info is nullptr!");

    struct MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(keySession);
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject != nullptr, DRM_ERR_INVALID_VAL, "sessionObject is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject->sessionImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "systemImpl_ is nullptr!");

    (void)request;
    return DRM_ERR_OK;
}

OH_DrmErrCode OH_MediaKeySession_ProcessLicenseResponse(OH_MediaKeySession *keySession, unsigned char *response,
    uint32_t &responseLen, unsigned char *keyId, uint32_t keyIdLen)
{
    DRM_INFO_LOG("OH_MediaKeySession_ProcessLicenseResponse called");
    DRM_CHECK_AND_RETURN_RET_LOG(keySession != nullptr, DRM_ERR_INVALID_VAL, "keySession is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(keyId != nullptr, DRM_ERR_INVALID_VAL, "licenseId is nullptr!");

    struct MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(keySession);
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject != nullptr, DRM_ERR_INVALID_VAL, "sessionObject is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject->sessionImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "systemImpl_ is nullptr!");

    std::vector<uint8_t> licenseResponseVec(response, response + responseLen);

    std::vector<uint8_t> keyIdVec(keyId, keyId + keyIdLen);
    DRM_INFO_LOG("OH_MediaKeySession_ProcessLicenseResponse keyid len: %{public}d", responseLen);
    for (uint32_t i = 0; i < keyIdLen; i++) {
        DRM_INFO_LOG("OH_MediaKeySession_ProcessLicenseResponse keyid len: %{public}x", keyIdVec[i]);
    }

    int32_t ret = sessionObject->sessionImpl_->ProcessLicenseResponse(keyIdVec, licenseResponseVec);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_ERR_OK, DRM_ERR_INVALID_VAL, "got licenseResponse null!");
    DRM_CHECK_AND_RETURN_RET_LOG(!licenseResponseVec.empty(), DRM_ERR_INVALID_VAL, "got licenseResponse null!");

    return DRM_ERR_OK;
}

OH_DrmErrCode OH_MediaKeySession_CheckLicenseStatus(OH_MediaKeySession *keySession, OH_DRM_MediaKeyStatus *status,
    uint32_t *licenseStatusCount)
{
    DRM_CHECK_AND_RETURN_RET_LOG(keySession != nullptr, DRM_ERR_INVALID_VAL, "keySession is nullptr!");

    struct MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(keySession);
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject != nullptr, DRM_ERR_INVALID_VAL, "sessionObject is nullptr!");
    DRM_CHECK_AND_RETURN_RET_LOG(sessionObject->sessionImpl_ != nullptr, DRM_ERR_INVALID_VAL,
        "systemImpl_ is nullptr!");

    (void)status;
    (void)licenseStatusCount;
    return DRM_ERR_OK;
}