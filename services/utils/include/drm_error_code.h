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

#ifndef DRM_ERROR_CODE_H
#define DRM_ERROR_CODE_H
#include <iostream>

namespace OHOS {
namespace DrmStandard {

enum DrmInnerErrCode : int32_t {
    /**
     * the operation completed successfully.
     */
    DRM_INNER_ERR_OK = 0,
    /**
     * DRM_INNER_ERR_BASE.
     */
    DRM_INNER_ERR_BASE = 24700800,
    /**
     * unknown error.
     */
    DRM_INNER_ERR_UNKNOWN = DRM_INNER_ERR_BASE + 1,
    /**
     * opertation not be permitted.
     */
    DRM_INNER_ERR_OPERATION_NOT_PERMITTED = DRM_INNER_ERR_BASE + 2,
    /**
     * Meet max MediaKeySystem num limit.
     */
    DRM_INNER_ERR_MAX_SYSTEM_NUM_REACHED = DRM_INNER_ERR_BASE + 3,
    /**
     * Meet max MediaKeySession num limit.
     */
    DRM_INNER_ERR_MAX_SESSION_NUM_REACHED = DRM_INNER_ERR_BASE + 4,
    /**
     * no memory.
     */
    DRM_INNER_ERR_NO_MEMORY = DRM_INNER_ERR_BASE + 5,
    /**
     * invalid argument.
     */
    DRM_INNER_ERR_INVALID_VAL = DRM_INNER_ERR_BASE + 6,
    /**
     * IO error.
     */
    DRM_INNER_ERR_IO = DRM_INNER_ERR_BASE + 7,
    /**
     * network timeout.
     */
    DRM_INNER_ERR_TIMEOUT = DRM_INNER_ERR_BASE + 8,
    /**
     * drm service died.
     */
    DRM_INNER_ERR_SERVICE_DIED = DRM_INNER_ERR_BASE + 9,
    /**
     * not support this operation in this state.
     */
    DRM_INNER_ERR_INVALID_STATE = DRM_INNER_ERR_BASE + 10,
    /**
     * mediakey system not supported.
     */
    DRM_INNER_ERR_UNSUPPORTED = DRM_INNER_ERR_BASE + 11,
    /**
     * invalid key session.
     */
    DRM_INNER_ERR_INVALID_KEY_SESSION = DRM_INNER_ERR_BASE + 12,
    /**
     * invalid media key system.
     */
    DRM_INNER_ERR_INVALID_MEDIA_KEY_SYSTEM = DRM_INNER_ERR_BASE + 13,
    /**
     * crypto error.
     */
    DRM_INNER_ERR_CRYPTO_ERROR = DRM_INNER_ERR_BASE + 14,
    /**
     * device not provisioned.
     */
    DRM_INNER_ERR_NOT_PROVISIONED = DRM_INNER_ERR_BASE + 15,
    /**
     * device time error.
     */
    DRM_INNER_ERR_DEVICE_TIME_ERROR = DRM_INNER_ERR_BASE + 16,
    /**
     * provisioning not allowed.
     */
    DRM_INNER_ERR_PROVISIONING_NOT_ALLOWED = DRM_INNER_ERR_BASE + 17,
    /**
     * not device cert when provisioning.
     */
    DRM_INNER_ERR_PROVISIONING_NO_DEVICE_CERT = DRM_INNER_ERR_BASE + 18,
    /**
     * server certificates are invalid when provisioning.
     */
    DRM_INNER_ERR_PROVISIONING_INVALID_SERVER_CERTIFICATES = DRM_INNER_ERR_BASE + 19,
    /**
     * server certificates are expired when provisioning.
     */
    DRM_INNER_ERR_PROVISIONING_SERVER_CERTIFICATES_EXPIRED = DRM_INNER_ERR_BASE + 20,
    /**
     * invalid message when provisioning.
     */
    DRM_INNER_ERR_PROVISIONING_INVALID_MESSAGE = DRM_INNER_ERR_BASE + 21,
    /**
     * drm certificates are invalid when provisioning.
     */
    DRM_INNER_ERR_PROVISIONING_INVALID_DRM_CERTIFICATES = DRM_INNER_ERR_BASE + 22,
    /**
     * time error when provisioning.
     */
    DRM_INNER_ERR_PROVISIONING_TIME_ERROR = DRM_INNER_ERR_BASE + 23,
    /**
     * storage read is failed when provisioning.
     */
    DRM_INNER_ERR_STORAGE_READ = DRM_INNER_ERR_BASE + 24,
    /**
     * storage write is failed when provisioning.
     */
    DRM_INNER_ERR_STORAGE_WRITE = DRM_INNER_ERR_BASE + 25,
    /**
     * not allowed to generate key request.
     */
    DRM_INNER_ERR_KEY_REQUEST_NOT_ALLOWED = DRM_INNER_ERR_BASE + 26,
    /**
     * invalid media key type when generating key request.
     */
    DRM_INNER_ERR_KEY_REQUEST_INVALID_MEDIA_KEY_TYPE = DRM_INNER_ERR_BASE + 27,
    /**
     * invalid mime type when generating key request.
     */
    DRM_INNER_ERR_KEY_REQUEST_INVALID_MIMETYPE = DRM_INNER_ERR_BASE + 28,
    /**
     * invalid initdata when generating key request.
     */
    DRM_INNER_ERR_KEY_REQUEST_INVALID_INITDATA = DRM_INNER_ERR_BASE + 29,
    /**
     * invalid drm certificates when generating key request.
     */
    DRM_INNER_ERR_KEY_REQUEST_INVALID_DRM_CERTIFICATES = DRM_INNER_ERR_BASE + 30,
    /**
     * drm certificates are expired when generating key request.
     */
    DRM_INNER_ERR_KEY_REQUEST_DRM_CERTIFICATES_EXPIRED = DRM_INNER_ERR_BASE + 31,
    /**
     * drm certificates are revoked when generating key request.
     */
    DRM_INNER_ERR_KEY_REQUEST_DRM_CERTIFICATES_REVOKED = DRM_INNER_ERR_BASE + 32,
    /**
     * invalid message when processing key response.
     */
    DRM_INNER_ERR_KEY_RESPONSE_INVALID_MESSAGE = DRM_INNER_ERR_BASE + 33,
    /**
     * invalid server certificates when processing key response.
     */
    DRM_INNER_ERR_KEY_RESPONSE_INVALID_SERVER_CERTIFICATES = DRM_INNER_ERR_BASE + 34,
    /**
     * server certificates expired when processing key response.
     */
    DRM_INNER_ERR_KEY_RESPONSE_SERVER_CERTIFICATES_EXPIRED = DRM_INNER_ERR_BASE + 35,
    /**
     * time error when processing key response.
     */
    DRM_INNER_ERR_KEY_RESPONSE_TIME_ERROR = DRM_INNER_ERR_BASE + 36,
    /**
     * server denied when processing key response.
     */
    DRM_INNER_ERR_KEY_RESPONSE_DENIED = DRM_INNER_ERR_BASE + 37,
    /**
     * invalid media key id when restoring offline mediakeys.
     */
    DRM_INNER_ERR_KEY_RESTORE_INVALID_MEDIA_KEY_ID = DRM_INNER_ERR_BASE + 38,
    /**
     * algorithm not support when decrypting media samples.
     */
    DRM_INNER_ERR_DECRYPT_ALGORITHM_NOT_SUPPORT = DRM_INNER_ERR_BASE + 39,
    /**
     * no license when decrypting media samples.
     */
    DRM_INNER_ERR_DECRYPT_NO_LICENSE = DRM_INNER_ERR_BASE + 40,
    /**
     * no key when decrypting media samples.
     */
    DRM_INNER_ERR_DECRYPT_NO_KEY = DRM_INNER_ERR_BASE + 41,
    /**
     * invalid iv when decrypting media samples.
     */
    DRM_INNER_ERR_DECRYPT_INVALID_IV = DRM_INNER_ERR_BASE + 42,
    /**
     * invalid subsamples when decrypting media samples.
     */
    DRM_INNER_ERR_DECRYPT_INVALID_SUBSAMPLES = DRM_INNER_ERR_BASE + 43,
    /**
     * invalid pattern when decrypting media samples.
     */
    DRM_INNER_ERR_DECRYPT_INVALID_PATTERN = DRM_INNER_ERR_BASE + 44,
    /**
     * invalid input buffer when decrypting media samples.
     */
    DRM_INNER_ERR_DECRYPT_INVALID_INPUT_BUFFER = DRM_INNER_ERR_BASE + 45,
    /**
     * invalid input buffer len when decrypting media samples.
     */
    DRM_INNER_ERR_DECRYPT_INVALID_INPUT_BUFFER_LEN = DRM_INNER_ERR_BASE + 46,
    /**
     * invalid output buffer when decrypting media samples.
     */
    DRM_INNER_ERR_DECRYPT_INVALID_OUTPUT_BUFFER = DRM_INNER_ERR_BASE + 47,
    /**
     * invalid output buffer len when decrypting media samples.
     */
    DRM_INNER_ERR_DECRYPT_INVALID_OUTPUT_BUFFER_LEN = DRM_INNER_ERR_BASE + 48,
    /**
     * secure not support when decrypting media samples.
     */
    DRM_INNER_ERR_DECRYPT_SECURE_NOT_SUPPORT = DRM_INNER_ERR_BASE + 49,
    /**
     * output protection not allowed when do policy control.
     */
    DRM_INNER_ERR_POLICY_OUTPUT_PROTECTION_NOT_ALLOWED = DRM_INNER_ERR_BASE + 50,
    /**
     * content protection level not allowed when do policy control.
     */
    DRM_INNER_ERR_POLICY_CONTENT_PROTECTION_LEVEL_NOT_ALLOWED = DRM_INNER_ERR_BASE + 51,
    /**
     * rental not allowed when do policy control.
     */
    DRM_INNER_ERR_POLICY_RENTAL_NOT_ALLOWED = DRM_INNER_ERR_BASE + 52,
    /**
     * invalid renew parameter when do policy control.
     */
    DRM_INNER_ERR_POLICY_INVALID_RENEW_PARAMETER = DRM_INNER_ERR_BASE + 53,
    /**
     * license need when do policy control.
     */
    DRM_INNER_ERR_POLICY_LICENSE_NEED = DRM_INNER_ERR_BASE + 54,
    /**
     * memory error.
     */
    DRM_INNER_ERR_MEMORY_ERROR = DRM_INNER_ERR_BASE + 55,
    /**
     * tamper found.
     */
    DRM_INNER_ERR_TAMPER_FOUND = DRM_INNER_ERR_BASE + 56,
    /**
     * drm service fatal error.
     */
    DRM_INNER_ERR_SERVICE_FATAL_ERROR = DRM_INNER_ERR_BASE + 57,
    /**
     * calling plugin error.
     */
    DRM_INNER_ERR_PLUGIN_ERROR = DRM_INNER_ERR_BASE + 58,
};
} // namespace DrmStandard
} // namespace OHOS
#endif // DRM_ERROR_CODE_H