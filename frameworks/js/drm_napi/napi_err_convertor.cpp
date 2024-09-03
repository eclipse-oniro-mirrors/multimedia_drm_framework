/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "drm_error_code.h"
#include "drm_error_code_napi.h"
#include "common_napi.h"

namespace OHOS {
namespace DrmStandard {

const std::unordered_map<DrmInnerErrCode, DrmNapiErrCode> INNERCODE_TO_NAPICODE = {
    {DRM_INNER_ERR_OK,                                                         DRM_NAPI_ERR_OK},
    {DRM_INNER_ERR_BASE,                                                     DRM_NAPI_ERR_BASE},
    {DRM_INNER_ERR_MAX_SYSTEM_NUM_REACHED,                 DRM_NAPI_ERR_MAX_SYSTEM_NUM_REACHED},
    {DRM_INNER_ERR_MAX_SESSION_NUM_REACHED,               DRM_NAPI_ERR_MAX_SESSION_NUM_REACHED},
    {DRM_INNER_ERR_INVALID_VAL,                                       DRM_NAPI_ERR_INVALID_VAL},
    {DRM_INNER_ERR_SERVICE_DIED,                                    DRM_NAPI_ERR_SERVICE_FATAL},
    {DRM_INNER_ERR_INVALID_KEY_SESSION,                               DRM_NAPI_ERR_INVALID_VAL},
    {DRM_INNER_ERR_INVALID_MEDIA_KEY_SYSTEM,                          DRM_NAPI_ERR_INVALID_VAL},
    {DRM_INNER_ERR_NOT_PROVISIONED,                               DRM_NAPI_ERR_NOT_PROVISIONED},
    {DRM_INNER_ERR_DEVICE_TIME_ERROR,                           DRM_NAPI_ERR_DEVICE_TIME_ERROR},
    {DRM_INNER_ERR_PROVISIONING_NO_DEVICE_CERT,                    DRM_NAPI_ERR_NO_DEVICE_CERT},
    {DRM_INNER_ERR_KEY_REQUEST_NOT_ALLOWED,                           DRM_NAPI_ERR_INVALID_VAL},
    {DRM_INNER_ERR_KEY_REQUEST_INVALID_MEDIA_KEY_TYPE,                DRM_NAPI_ERR_INVALID_VAL},
    {DRM_INNER_ERR_KEY_REQUEST_INVALID_MIMETYPE,                      DRM_NAPI_ERR_INVALID_VAL},
    {DRM_INNER_ERR_KEY_REQUEST_INVALID_INITDATA,                      DRM_NAPI_ERR_INVALID_VAL},
    {DRM_INNER_ERR_KEY_REQUEST_INVALID_DRM_CERTIFICATES,              DRM_NAPI_ERR_INVALID_VAL},
    {DRM_INNER_ERR_KEY_REQUEST_DRM_CERTIFICATES_REVOKED, DRM_NAPI_ERR_DRM_CERTIFICATES_REVOKED},
    {DRM_INNER_ERR_DECRYPT_NO_LICENSE,                                 DRM_NAPI_ERR_NO_LICENSE},
    {DRM_INNER_ERR_DECRYPT_NO_KEY,                                         DRM_NAPI_ERR_NO_KEY},
    {DRM_INNER_ERR_POLICY_CONTENT_PROTECTION_LEVEL_NOT_ALLOWED,       DRM_NAPI_ERR_INVALID_VAL},
    {DRM_INNER_ERR_SERVICE_FATAL_ERROR,                             DRM_NAPI_ERR_SERVICE_FATAL},
    {DRM_INNER_ERR_PLUGIN_ERROR,                                          DRM_NAPI_ERR_UNKNOWN},
};

const std::unordered_map<DrmInnerErrCode, DrmNapiErrCode> INNERCODE_TO_API12_NAPICODE = {
    {DRM_INNER_ERR_OK,                                                         DRM_NAPI_ERR_OK},
    {DRM_INNER_ERR_BASE,                                                     DRM_NAPI_ERR_BASE},
    {DRM_INNER_ERR_MAX_SYSTEM_NUM_REACHED,                 DRM_NAPI_ERR_MAX_SYSTEM_NUM_REACHED},
    {DRM_INNER_ERR_MAX_SESSION_NUM_REACHED,               DRM_NAPI_ERR_MAX_SESSION_NUM_REACHED},
    {DRM_INNER_ERR_SERVICE_DIED,                                    DRM_NAPI_ERR_SERVICE_FATAL},
    {DRM_INNER_ERR_SERVICE_FATAL_ERROR,                             DRM_NAPI_ERR_SERVICE_FATAL},
};

DrmNapiErrCode DrmInnerErrToNapiErr(DrmInnerErrCode errCode)
{
    if (INNERCODE_TO_NAPICODE.find(errCode) != INNERCODE_TO_NAPICODE.end()) {
        return INNERCODE_TO_NAPICODE.at(errCode);
    }
    return DRM_NAPI_ERR_UNKNOWN;
}

DrmNapiErrCode DrmInnerErrToNapiErrAPI12(DrmInnerErrCode errCode)
{
    if (INNERCODE_TO_API12_NAPICODE.find(errCode) != INNERCODE_TO_API12_NAPICODE.end()) {
        return INNERCODE_TO_API12_NAPICODE.at(errCode);
    }
    return DRM_NAPI_ERR_UNKNOWN;
}

napi_status NapiDrmError::ThrowError(napi_env env, const char *napiMessage, int32_t napiCode)
{
    napi_value message = nullptr;
    napi_value code = nullptr;
    napi_value result = nullptr;
    napi_create_string_utf8(env, napiMessage, NAPI_AUTO_LENGTH, &message);
    napi_create_error(env, nullptr, message, &result);
    napi_create_int32(env, napiCode, &code);
    napi_set_named_property(env, result, "code", code);
    napi_throw(env, result);
    return napi_ok;
}

void NapiDrmError::ThrowError(napi_env env, int32_t code)
{
    std::string messageValue = GetMessageByCode(code);
    napi_throw_error(env, (std::to_string(code)).c_str(), messageValue.c_str());
}

std::string NapiDrmError::GetMessageByCode(int32_t &code)
{
    std::string errMessage;
    switch (code) {
        case DRM_INVALID_PARAM:
            errMessage = DRM_INVALID_PARAM_INFO;
            break;
        case DRM_SERVICE_FATAL_ERROR:
            errMessage = DRM_SERVICE_FATAL_ERRO_INFO;
            break;
        case DRM_UNKNOWN_ERROR:
            errMessage = DRM_UNKNOWN_ERROR_INFO;
            break;
        case DRM_MAX_SYSTEM_NUM_REACHED:
            errMessage = DRM_MAX_SYSTEM_NUM_REACHED_INFO;
            break;
        case DRM_MAX_SESSION_NUM_REACHED:
            errMessage = DRM_MAX_SESSION_NUM_REACHED_INFO;
            break;
        default:
            errMessage = DRM_UNKNOWN_ERROR_INFO;
            code = DRM_UNKNOWN_ERROR;
            break;
    }
    return errMessage;
}
} // namespace DrmStandard
} // namespace OHOS