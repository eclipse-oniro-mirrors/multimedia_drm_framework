/*
 * Copyright (C) 2023  Huawei Device Co., Ltd.
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

#include <map>
#include <vector>
#include "drm_napi_utils.h"
#include "drm_enum_napi.h"

namespace OHOS {
namespace DrmStandard {
struct JsEnumInt {
    std::string_view enumName;
    int32_t enumInt;
};

struct JsEnumString {
    std::string_view enumName;
    std::string_view enumString;
};

static const std::vector<struct JsEnumInt> g_listenerType = {
    { "LISTENER_DRM_EVENT", IMediaKeySessionService::ListenerType::LISTENER_DRM_EVENT },
    { "LISTENER_PROVISION_REQUIRED", IMediaKeySessionService::ListenerType::LISTENER_PROVISION_REQUIRED },
    { "LISTENER_KEY_NEEDED", IMediaKeySessionService::ListenerType::LISTENER_KEY_NEEDED },
    { "LISTENER_KEY_EXPIRED", IMediaKeySessionService::ListenerType::LISTENER_KEY_EXPIRED },
    { "LISTENER_VENDOR_DEFINED", IMediaKeySessionService::ListenerType::LISTENER_VENDOR_DEFINED },
    { "LISTENER_KEYSESSION_RECLAIMED", IMediaKeySessionService::ListenerType::LISTENER_KEYSESSION_RECLAIMED },
    { "LISTENER_EXPIRATION_UPDATE", IMediaKeySessionService::ListenerType::LISTENER_EXPIRATION_UPDATE },
    { "LISTENER_KEY_CHANGE", IMediaKeySessionService::ListenerType::LISTENER_KEY_CHANGE },
    { "LISTENER_KEYSESSION_LOST", IMediaKeySessionService::ListenerType::LISTENER_KEYSESSION_LOST },
};

static const std::vector<struct JsEnumInt> g_hdcpLevel = {
    { "HDCP_UNKNOWN", IMediaKeySystemService::HdcpLevel::HDCP_UNKNOWN },
    { "HDCP_NONE", IMediaKeySystemService::HdcpLevel::HDCP_NONE },
    { "HDCP_V1", IMediaKeySystemService::HdcpLevel::HDCP_V1 },
    { "HDCP_V2", IMediaKeySystemService::HdcpLevel::HDCP_V2 },
    { "HDCP_V2_1", IMediaKeySystemService::HdcpLevel::HDCP_V2_1 },
    { "HDCP_V2_2", IMediaKeySystemService::HdcpLevel::HDCP_V2_2 },
    { "HDCP_V2_3", IMediaKeySystemService::HdcpLevel::HDCP_V2_3 },
    { "HDCP_NO_OUTPUT", IMediaKeySystemService::HdcpLevel::HDCP_NO_OUTPUT },
};

static const std::vector<struct JsEnumInt> g_adcpLevel = {
    { "ADCP_UNKNOWN", IMediaKeySystemService::AdcpLevel::ADCP_UNKNOWN },
    { "ADCP_V1_L1", IMediaKeySystemService::AdcpLevel::ADCP_V1_L1 },
    { "ADCP_V1_L2", IMediaKeySystemService::AdcpLevel::ADCP_V1_L2 },
    { "ADCP_V1_L3", IMediaKeySystemService::AdcpLevel::ADCP_V1_L3 },
    { "ADCP_NO_OUTPUT", IMediaKeySystemService::AdcpLevel::ADCP_NO_OUTPUT },
};

static const std::vector<struct JsEnumInt> g_licenseType = {
    { "LICENSE_TYPE_OFFLINE", IMediaKeySessionService::LicenseType::LICENSETYPE_OFFLINE },
    { "LICENSE_TYPE_ONLINE", IMediaKeySessionService::LicenseType::LICENSETYPE_ONLINE },
};

static const std::vector<struct JsEnumInt> g_offlineLicenseStatus = {
    { "OFFLINELICENSESTATE_UNKNOWN", IMediaKeySessionService::OfflineLicenseStatus::OFFLINELICENSESTATUS_UNKNOWN },
    { "OFFLINELICENSESTATE_USABLE", IMediaKeySessionService::OfflineLicenseStatus::OFFLINELICENSESTATUS_USABLE },
    { "OFFLINELICENSESTATE_INACTIVE", IMediaKeySessionService::OfflineLicenseStatus::OFFLINELICENSESTATUS_INACTIVE },
};

static const std::vector<struct JsEnumInt> g_certificateStatus = {
    { "CERT_STATUS_PROVISIONED", IMediaKeySystemService::CertificateStatus::CERT_STATUS_PROVISIONED },
    { "CERT_STATUS_NOT_PROVISIONED", IMediaKeySystemService::CertificateStatus::CERT_STATUS_NOT_PROVISIONED },
    { "CERT_STATUS_EXPIRED", IMediaKeySystemService::CertificateStatus::CERT_STATUS_EXPIRED },
    { "CERT_STATUS_INVALID", IMediaKeySystemService::CertificateStatus::CERT_STATUS_INVALID },
    { "CERT_STATUS_GET_FAILED", IMediaKeySystemService::CertificateStatus::CERT_STATUS_GET_FAILED },
};

static const std::vector<struct JsEnumInt> g_requestType = {
    { "REQUEST_TYPE_UNKNOWN", IMediaKeySessionService::RequestType::REQUEST_TYPE_UNKNOWN },
    { "REQUEST_TYPE_INITIAL", IMediaKeySessionService::RequestType::REQUEST_TYPE_INITIAL },
    { "REQUEST_TYPE_RENEWAL", IMediaKeySessionService::RequestType::REQUEST_TYPE_RENEWAL },
    { "REQUEST_TYPE_RELEASE", IMediaKeySessionService::RequestType::REQUEST_TYPE_RELEASE },
    { "REQUEST_TYPE_NONE", IMediaKeySessionService::RequestType::REQUEST_TYPE_NONE },
    { "REQUEST_TYPE_UPDATE", IMediaKeySessionService::RequestType::REQUEST_TYPE_UPDATE },
};

static const std::vector<struct JsEnumInt> g_securityLevel = {
    { "SECURITY_LEVEL_UNKNOWN", IMediaKeySessionService::SecurityLevel::SECURITY_LEVEL_UNKNOWN },
    { "SECURITY_LEVEL_SW_CRYPTO", IMediaKeySessionService::SecurityLevel::SECURITY_LEVEL_SW_CRYPTO },
    { "SECURITY_LEVEL_SW_DECODE", IMediaKeySessionService::SecurityLevel::SECURITY_LEVEL_SW_DECODE },
    { "SECURITY_LEVEL_HW_CRYPTO", IMediaKeySessionService::SecurityLevel::SECURITY_LEVEL_HW_CRYPTO },
    { "SECURITY_LEVEL_HW_DECODE", IMediaKeySessionService::SecurityLevel::SECURITY_LEVEL_HW_DECODE },
    { "SECURITY_LEVEL_HW_ALL", IMediaKeySessionService::SecurityLevel::SECURITY_LEVEL_HW_ALL },
    { "SECURITY_LEVEL_MAX", IMediaKeySessionService::SecurityLevel::SECURITY_LEVEL_MAX },
};

static const std::vector<struct JsEnumString> g_configName = {
    { "CONFIG_DEVICE_VENDOR", "vendor" },
    { "CONFIG_DEVICE_VERSION", "version" },
    { "CONFIG_DEVICE_DESCRIPTION", "description" },
    { "CONFIG_DEVICE_ALGORITHMS", "algorithms" },
    { "CONFIG_DEVICE_UNIQUE_ID", "deviceUniqueId" },
    { "CONFIG_SESSION_MAX", "maxSessionNum" },
    { "CONFIG_SESSION_CURRENT", "currentSessionNum" },
    { "CONFIG_OUTPUT_HDCP_MAX", "maxHDCPLevel" },
    { "CONFIG_OUTPUT_HDCP_CURRENT", "currentHDCPLevel" },
    { "CONFIG_OUTPUT_ADCP_MAX", "maxADCPLevel" },
    { "CONFIG_OUTPUT_ADCP_CURRENT", "currentADCPLevel" },
};

static const std::vector<struct JsEnumString> g_metricsName = {
    { "METRICS_SESSION_CURRENT", "currentSessionNum" },
    { "METRICS_DEVICE_VERSION", "version" },
    { "METRICS_DECRYPT_NUMBER", "decryptNumber" },
    { "METRICS_ERRO_DECRYPT_NUMBER", "errorDecryptNumber" },
};

static const std::map<std::string_view, const std::vector<struct JsEnumInt> &> g_intEnumClassMap = {
    { "ListenerType", g_listenerType },
    { "HdcpLevel", g_hdcpLevel },
    { "AdcpLevel", g_adcpLevel },
    { "LicenseType", g_licenseType },
    { "OfflineLicenseStatus", g_offlineLicenseStatus },
    { "CertificateStatus", g_certificateStatus },
    { "RequestType", g_requestType },
    { "SecurityLevel", g_securityLevel },
};

static const std::map<std::string_view, const std::vector<struct JsEnumString> &> g_stringEnumClassMap = {
    { "ConfigName", g_configName },
    { "MetricsName", g_metricsName },
};

napi_value DrmEnumNapi::JsEnumIntInit(napi_env env, napi_value exports)
{
    for (auto it = g_intEnumClassMap.begin(); it != g_intEnumClassMap.end(); it++) {
        auto &enumClassName = it->first;
        auto &enumItemVec = it->second;
        int32_t vecSize = enumItemVec.size();
        std::vector<napi_value> value;
        value.resize(vecSize);
        for (int32_t index = 0; index < vecSize; ++index) {
            napi_create_int32(env, enumItemVec[index].enumInt, &value[index]);
        }

        std::vector<napi_property_descriptor> property;
        property.resize(vecSize);
        for (int32_t index = 0; index < vecSize; ++index) {
            property[index] =
                napi_property_descriptor DECLARE_NAPI_STATIC_PROPERTY(enumItemVec[index].enumName.data(), value[index]);
        }

        auto constructor = [](napi_env env, napi_callback_info info) {
            napi_value jsThis = nullptr;
            napi_get_cb_info(env, info, nullptr, nullptr, &jsThis, nullptr);
            return jsThis;
        };

        napi_value result = nullptr;
        napi_status status = napi_define_class(env, enumClassName.data(), NAPI_AUTO_LENGTH, constructor, nullptr,
            property.size(), property.data(), &result);
        DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, nullptr, "Failed to define enum");

        status = napi_set_named_property(env, exports, enumClassName.data(), result);
        DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, nullptr, "Failed to set result");
    }
    return exports;
}

napi_value DrmEnumNapi::JsEnumStringInit(napi_env env, napi_value exports)
{
    for (auto it = g_stringEnumClassMap.begin(); it != g_stringEnumClassMap.end(); it++) {
        auto &enumClassName = it->first;
        auto &enumItemVec = it->second;
        int32_t vecSize = enumItemVec.size();
        std::vector<napi_value> value;
        value.resize(vecSize);
        for (int32_t index = 0; index < vecSize; ++index) {
            napi_create_string_utf8(env, enumItemVec[index].enumString.data(), NAPI_AUTO_LENGTH, &value[index]);
        }

        std::vector<napi_property_descriptor> property;
        property.resize(vecSize);
        for (int32_t index = 0; index < vecSize; ++index) {
            property[index] =
                napi_property_descriptor DECLARE_NAPI_STATIC_PROPERTY(enumItemVec[index].enumName.data(), value[index]);
        }

        auto constructor = [](napi_env env, napi_callback_info info) {
            napi_value jsThis = nullptr;
            napi_get_cb_info(env, info, nullptr, nullptr, &jsThis, nullptr);
            return jsThis;
        };

        napi_value result = nullptr;
        napi_status status = napi_define_class(env, enumClassName.data(), NAPI_AUTO_LENGTH, constructor, nullptr,
            property.size(), property.data(), &result);
        DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, nullptr, "Failed to define enum");

        status = napi_set_named_property(env, exports, enumClassName.data(), result);
        DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, nullptr, "Failed to set result");
    }
    return exports;
}

napi_value DrmEnumNapi::Init(napi_env env, napi_value exports)
{
    JsEnumIntInit(env, exports);
    JsEnumStringInit(env, exports);
    return exports;
}
} // namespace drm
} // namespace OHOS