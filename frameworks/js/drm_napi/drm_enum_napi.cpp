/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#include <map>
#include <vector>
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
    { "LISTENER_DRM_EVENT", IKeySessionService::ListenerType::LISTENER_DRM_EVENT },
    { "LISTENER_PROVISION_REQUIRED", IKeySessionService::ListenerType::LISTENER_PROVISION_REQUIRED },
    { "LISTENER_KEY_NEEDED", IKeySessionService::ListenerType::LISTENER_KEY_NEEDED },
    { "LISTENER_KEY_EXPIRED", IKeySessionService::ListenerType::LISTENER_KEY_EXPIRED },
    { "LISTENER_VENDOR_DEFINED", IKeySessionService::ListenerType::LISTENER_VENDOR_DEFINED },
    { "LISTENER_KEYSESSION_RECLAIMED", IKeySessionService::ListenerType::LISTENER_KEYSESSION_RECLAIMED },
    { "LISTENER_EXPIRATION_UPDATE", IKeySessionService::ListenerType::LISTENER_EXPIRATION_UPDATE },
    { "LISTENER_KEY_CHANGE", IKeySessionService::ListenerType::LISTENER_KEY_CHANGE },
    { "LISTENER_KEYSESSION_LOSE", IKeySessionService::ListenerType::LISTENER_KEYSESSION_LOSE },
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

static const std::vector<struct JsEnumInt> g_configType = {
    { "CONFIGTYPE_DEVICEPROPERTY", IMediaKeySystemService::ConfigType::CONFIGTYPE_DEVICEPROPERTY },
    { "CONFIGTYPE_KEYSESSION", IMediaKeySystemService::ConfigType::CONFIGTYPE_KEYSESSION },
    { "CONFIGTYPE_OUTPUTPROTECTTYPE", IMediaKeySystemService::ConfigType::CONFIGTYPE_OUTPUTPROTECTTYPE },
};

static const std::vector<struct JsEnumInt> g_keyType = {
    { "KEYTYPE_OFFLINE", IKeySessionService::KeyType::KEYTYPE_OFFLINE },
    { "KEYTYPE_ONLINE", IKeySessionService::KeyType::KEYTYPE_ONLINE },
};

static const std::vector<struct JsEnumInt> g_offlineKeyState = {
    { "OFFLINEKEYSTATE_UNKNOWN", IKeySessionService::OfflineKeyState::OFFLINEKEYSTATE_UNKNOWN },
    { "OFFLINEKEYSTATE_USABLE", IKeySessionService::OfflineKeyState::OFFLINEKEYSTATE_USABLE },
    { "OFFLINEKEYSTATE_INACTIVE", IKeySessionService::OfflineKeyState::OFFLINEKEYSTATE_INACTIVE },
};

static const std::vector<struct JsEnumInt> g_requestType = {
    { "REQUEST_TYPE_UNKNOWN", IKeySessionService::RequestType::REQUEST_TYPE_UNKNOWN },
    { "REQUEST_TYPE_INITIAL", IKeySessionService::RequestType::REQUEST_TYPE_INITIAL },
    { "REQUEST_TYPE_RENEWAL", IKeySessionService::RequestType::REQUEST_TYPE_RENEWAL },
    { "REQUEST_TYPE_RELEASE", IKeySessionService::RequestType::REQUEST_TYPE_RELEASE },
    { "REQUEST_TYPE_NONE", IKeySessionService::RequestType::REQUEST_TYPE_NONE },
    { "REQUEST_TYPE_UPDATE", IKeySessionService::RequestType::REQUEST_TYPE_UPDATE },
    { "REQUEST_TYPE_DOWNLOADCERT", IKeySessionService::RequestType::REQUEST_TYPE_DOWNLOADCERT },
};

static const std::vector<struct JsEnumInt> g_securityLevel = {
    { "SECURITY_LEVEL_UNKNOWN", IKeySessionService::SecurityLevel::SECURITY_LEVEL_UNKNOWN },
    { "SECURITY_LEVEL_SW_CRYPTO", IKeySessionService::SecurityLevel::SECURITY_LEVEL_SW_CRYPTO },
    { "SECURITY_LEVEL_SW_DECODE", IKeySessionService::SecurityLevel::SECURITY_LEVEL_SW_DECODE },
    { "SECURITY_LEVEL_HW_CRYPTO", IKeySessionService::SecurityLevel::SECURITY_LEVEL_HW_CRYPTO },
    { "SECURITY_LEVEL_HW_DECODE", IKeySessionService::SecurityLevel::SECURITY_LEVEL_HW_DECODE },
    { "SECURITY_LEVEL_HW_ALL", IKeySessionService::SecurityLevel::SECURITY_LEVEL_HW_ALL },
    { "SECURITY_LEVEL_MAX", IKeySessionService::SecurityLevel::SECURITY_LEVEL_MAX },
};

static const std::vector<struct JsEnumInt> g_cryptAlgorithmType = {
    { "ALGTYPE_UNENCRYPTED", IMediaDecryptModuleService::CryptAlgorithmType::ALGTYPE_UNENCRYPTED },
    { "ALGTYPE_AES_CTR", IMediaDecryptModuleService::CryptAlgorithmType::ALGTYPE_AES_CTR },
    { "ALGTYPE_AES_WV", IMediaDecryptModuleService::CryptAlgorithmType::ALGTYPE_AES_WV },
    { "ALGTYPE_AES_CBC", IMediaDecryptModuleService::CryptAlgorithmType::ALGTYPE_AES_CBC },
    { "ALGTYPE_SM4_CBC", IMediaDecryptModuleService::CryptAlgorithmType::ALGTYPE_SM4_CBC },
};

static const std::vector<struct JsEnumString> g_propertyName = {
    { "PROPERTY_DEVICE_VENDOR", "vendor" },
    { "PROPERTY_DEVICE_VERSION", "version" },
    { "PROPERTY_DEVICE_DESCRIPTION", "description" },
    { "PROPERTY_DEVICE_ALGORITHMS", "algorithms" },
    { "PROPERTY_DEVICE_UNIQUE_ID", "deviceUniqueId" },
    { "PROPERTY_SESSION_MAX", "maxSessionNum" },
    { "PROPERTY_SESSION_CURRENT", "currentSessionNum" },
    { "PROPERTY_OUTPUT_HDCP_MAX", "maxHDCPLevel" },
    { "PROPERTY_OUTPUT_HDCP_CURRENT", "currentHDCPLevel" },
};

static const std::vector<struct JsEnumString> g_metricsName = {
    { "METRICS_SESSION_CURRENT", "currentSessionNum" },
    { "METRICS_DEVICE_VERSION", "version" },
    { "METRICS_DECRYPT_NUMBER", "decryptNumber" },
    { "METRICS_ERRO_DECRYPT_NUMBER", "errorDecryptNumber" },
};

static const std::map<std::string_view, const std::vector<struct JsEnumInt>&> g_intEnumClassMap = {
    { "ListenerType", g_listenerType },
    { "HdcpLevel", g_hdcpLevel },
    { "ConfigType", g_configType },
    { "KeyType", g_keyType },
    { "OfflineKeyState", g_offlineKeyState },
    { "RequestType", g_requestType },
    { "SecurityLevel", g_securityLevel },
    { "CryptAlgorithmType", g_cryptAlgorithmType },
};

static const std::map<std::string_view, const std::vector<struct JsEnumString>&> g_stringEnumClassMap = {
    { "PropertyName", g_propertyName },
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
            property[index] = napi_property_descriptor DECLARE_NAPI_STATIC_PROPERTY(
                enumItemVec[index].enumName.data(), value[index]);
        }

        auto constructor = [](napi_env env, napi_callback_info info) {
            napi_value jsThis = nullptr;
            napi_get_cb_info(env, info, nullptr, nullptr, &jsThis, nullptr);
            return jsThis;
        };

        napi_value result = nullptr;
        napi_status status = napi_define_class(env, enumClassName.data(), NAPI_AUTO_LENGTH, constructor,
            nullptr, property.size(), property.data(), &result);
        CHECK_AND_RETURN_RET_LOG(status == napi_ok, nullptr, "Failed to define enum");

        status = napi_set_named_property(env, exports, enumClassName.data(), result);
        CHECK_AND_RETURN_RET_LOG(status == napi_ok, nullptr, "Failed to set result");
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
            property[index] = napi_property_descriptor DECLARE_NAPI_STATIC_PROPERTY(
                enumItemVec[index].enumName.data(), value[index]);
        }

        auto constructor = [](napi_env env, napi_callback_info info) {
            napi_value jsThis = nullptr;
            napi_get_cb_info(env, info, nullptr, nullptr, &jsThis, nullptr);
            return jsThis;
        };

        napi_value result = nullptr;
        napi_status status = napi_define_class(env, enumClassName.data(), NAPI_AUTO_LENGTH, constructor,
            nullptr, property.size(), property.data(), &result);
        CHECK_AND_RETURN_RET_LOG(status == napi_ok, nullptr, "Failed to define enum");

        status = napi_set_named_property(env, exports, enumClassName.data(), result);
        CHECK_AND_RETURN_RET_LOG(status == napi_ok, nullptr, "Failed to set result");
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