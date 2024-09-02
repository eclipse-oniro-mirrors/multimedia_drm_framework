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
#include "key_session_napi.h"
#include "media_key_system_napi.h"
#include "napi_param_utils.h"
#include "drm_trace.h"
#include "drm_error_code.h"
#include "drm_api_operation.h"

namespace OHOS {
namespace DrmStandard {
thread_local napi_ref MediaKeySystemNapi::sConstructor_ = nullptr;
thread_local sptr<MediaKeySystemImpl> MediaKeySystemNapi::sMediaKeySystemImpl_ = nullptr;

MediaKeySystemNapi::MediaKeySystemNapi() : env_(nullptr), wrapper_(nullptr)
{
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " Instances create", FAKE_POINTER(this));
}

MediaKeySystemNapi::~MediaKeySystemNapi()
{
    DRM_INFO_LOG("~MediaKeySystemNapi enter.");
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " Instances destroy", FAKE_POINTER(this));
    if (wrapper_ != nullptr) {
        napi_delete_reference(env_, wrapper_);
    }
    if (mediaKeySystemImpl_) {
        mediaKeySystemImpl_ = nullptr;
    }
    if (mediaKeySystemCallbackNapi_) {
        mediaKeySystemCallbackNapi_ = nullptr;
    }
}

napi_value MediaKeySystemNapi::Init(napi_env env, napi_value exports)
{
    DRM_INFO_LOG("Init enter.");
    napi_status status;
    napi_value ctorObj;

    napi_property_descriptor properties[] = {
        DECLARE_NAPI_FUNCTION("setConfigurationString", SetConfigurationString),
        DECLARE_NAPI_FUNCTION("getConfigurationString", GetConfigurationString),
        DECLARE_NAPI_FUNCTION("setConfigurationByteArray", SetConfigurationByteArray),
        DECLARE_NAPI_FUNCTION("getConfigurationByteArray", GetConfigurationByteArray),
        DECLARE_NAPI_FUNCTION("getMaxContentProtectionLevel", GetMaxContentProtectionLevel),
        DECLARE_NAPI_FUNCTION("generateKeySystemRequest", GenerateKeySystemRequest),
        DECLARE_NAPI_FUNCTION("processKeySystemResponse", ProcessKeySystemResponse),
        DECLARE_NAPI_FUNCTION("createMediaKeySession", CreateMediaKeySession),
        DECLARE_NAPI_FUNCTION("getStatistics", GetStatistics),
        DECLARE_NAPI_FUNCTION("getCertificateStatus", GetCertificateStatus),
        DECLARE_NAPI_FUNCTION("getOfflineMediaKeyIds", GetOfflineMediaKeyIds),
        DECLARE_NAPI_FUNCTION("getOfflineMediaKeyStatus", GetOfflineMediaKeyStatus),
        DECLARE_NAPI_FUNCTION("clearOfflineMediaKeys", ClearOfflineMediaKeys),
        DECLARE_NAPI_FUNCTION("destroy", Destroy),
        DECLARE_NAPI_FUNCTION("on", SetEventCallback),
        DECLARE_NAPI_FUNCTION("off", UnsetEventCallback),
    };

    napi_property_descriptor drm_static_properties[] = {
        DECLARE_NAPI_STATIC_FUNCTION("createMediaKeySystem", CreateMediaKeySystemInstance),
        DECLARE_NAPI_STATIC_FUNCTION("isMediaKeySystemSupported", IsMediaKeySystemSupported),
        DECLARE_NAPI_STATIC_FUNCTION("getMediaKeySystems", GetMediaKeySystems),
        DECLARE_NAPI_STATIC_FUNCTION("getMediaKeySystemUuid", GetMediaKeySystemUuid)
    };

    status = napi_define_class(env, MEDIA_KEY_SYSTEM_NAPI_CLASS_NAME, NAPI_AUTO_LENGTH, MediaKeySystemNapiConstructor,
        nullptr, sizeof(properties) / sizeof(properties[PARAM0]), properties, &ctorObj);
    DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, nullptr, "Failed to define napi class");
    status = napi_create_reference(env, ctorObj, 1, &sConstructor_);
    DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, nullptr, "Failed to create napi reference");
    status = napi_set_named_property(env, exports, MEDIA_KEY_SYSTEM_NAPI_CLASS_NAME, ctorObj);
    DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, nullptr, "Failed to set napi named property");
    status = napi_define_properties(env, exports, sizeof(drm_static_properties) / sizeof(drm_static_properties[0]),
        drm_static_properties);
    DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, nullptr, "Failed to define static DrmNapi function");
    return exports;
}

napi_value MediaKeySystemNapi::MediaKeySystemNapiConstructor(napi_env env, napi_callback_info info)
{
    DrmTrace trace("MediaKeySystemNapi::MediaKeySystemNapiConstructor");
    DRM_INFO_LOG("MediaKeySystemNapiConstructor enter.");
    napi_value result = nullptr;
    size_t argCount = 0;
    napi_value jsThis = nullptr;

    napi_get_undefined(env, &result);

    napi_status status = napi_get_cb_info(env, info, &argCount, nullptr, &jsThis, nullptr);
    DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, result, "failed to napi_get_cb_info");

    if (status == napi_ok && jsThis != nullptr) {
        std::unique_ptr<MediaKeySystemNapi> obj = std::make_unique<MediaKeySystemNapi>();
        obj->env_ = env;
        if (MediaKeySystemNapi::sMediaKeySystemImpl_ == nullptr) {
            DRM_ERR_LOG("sMediaKeySystemImpl_ is null");
            return result;
        }
        obj->mediaKeySystemImpl_ = MediaKeySystemNapi::sMediaKeySystemImpl_;
        obj->mediaKeySystemCallbackNapi_ = new (std::nothrow) MediaKeySystemCallbackNapi(env);
        obj->mediaKeySystemImpl_->SetCallback(obj->mediaKeySystemCallbackNapi_);
        status = napi_wrap(env, jsThis, reinterpret_cast<void *>(obj.get()),
            MediaKeySystemNapi::MediaKeySystemNapiDestructor, nullptr, nullptr);
        if (status == napi_ok) {
            ObjectRefMap<MediaKeySystemNapi>::Insert(obj.get());
            obj.release();
            return jsThis;
        } else {
            ObjectRefMap<MediaKeySystemNapi>::Erase(obj.get());
            DRM_ERR_LOG("Failure wrapping js to native napi");
        }
    }

    return result;
}

napi_value MediaKeySystemNapi::CreateMediaKeySystemInstance(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("CreateMediaKeySystemInstance enter.");
    napi_status status;
    napi_value result = nullptr;
    napi_value ctor = nullptr;
    napi_get_undefined(env, &result);
    status = napi_get_reference_value(env, sConstructor_, &ctor);
    if (status == napi_ok) {
        size_t argc = ARGS_ONE;
        napi_value argv[ARGS_ONE] = {0};
        napi_value thisVar = nullptr;
        DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
        std::string drmSchemaName;
        char nameBuffer[PATH_MAX] = { 0 };
        size_t nameBufferLen = 0;
        status = napi_get_value_string_utf8(env, argv[PARAM0], nameBuffer, PATH_MAX, &nameBufferLen);
        if (status != napi_ok) {
            NapiDrmError::ThrowError(env, "The param name is missing or too long.", DRM_INVALID_PARAM);
            DRM_ERR_LOG("napi_get_value_string_utf8 failed!");
            return result;
        }
        drmSchemaName = std::string(nameBuffer);
        int32_t ret = MediaKeySystemFactoryImpl::GetInstance()->CreateMediaKeySystem(drmSchemaName,
            &MediaKeySystemNapi::sMediaKeySystemImpl_);
        if (ret != DRM_OK) {
            NapiDrmError::ThrowError(env, "create MediaKeySystem failed.", ret);
            DRM_ERR_LOG("CreateMediaKeySystem failed!");
            return result;
        }
        status = napi_new_instance(env, ctor, 0, nullptr, &result);
        MediaKeySystemNapi::sMediaKeySystemImpl_ = nullptr;
        if (status == napi_ok) {
            return result;
        } else {
            DRM_ERR_LOG("New instance could not be obtained");
        }
    }
    NapiDrmError::ThrowError(env, "create MediaKeySystem failed, unknown error.", DRM_UNKNOWN_ERROR);
    napi_get_undefined(env, &result);
    return result;
}

void MediaKeySystemNapi::MediaKeySystemNapiDestructor(napi_env env, void *nativeObject, void *finalize)
{
    DrmTrace trace("MediaKeySystemNapi::MediaKeySystemNapiDestructor");
    DRM_INFO_LOG("MediaKeySystemNapiDestructor enter.");
    MediaKeySystemNapi *mediaKeySystemNapi = reinterpret_cast<MediaKeySystemNapi *>(nativeObject);
    ObjectRefMap<MediaKeySystemNapi>::DecreaseRef(mediaKeySystemNapi);
}

napi_value MediaKeySystemNapi::IsMediaKeySystemSupported(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("IsMediaKeySystemSupported enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_THREE;
    napi_value argv[ARGS_THREE] = {0, 0, 0};
    napi_value thisVar = nullptr;
    napi_get_undefined(env, &result);
    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    char buffer[PATH_MAX];
    size_t length = 0;
    if (napi_get_value_string_utf8(env, argv[PARAM0], buffer, PATH_MAX, &length) != napi_ok) {
        DRM_ERR_LOG("Could not able to read drm plugin name argument!");
        NapiDrmError::ThrowError(env, "Could not able to read drm plugin name argument!", DRM_UNKNOWN_ERROR);
        return result;
    }
    std::string pluginName = std::string(buffer);
    if (pluginName.length() == 0 || pluginName.length() > MAX_STRING_SIZE) {
        DRM_ERR_LOG("drm plugin name lenth is not able to zero or more 256!");
        NapiDrmError::ThrowError(env, "drm plugin name length exceeds reasonable range!", DRM_INVALID_PARAM);
        return result;
    }
    if (MediaKeySystemFactoryImpl::GetInstance() == nullptr) {
        DRM_ERR_LOG("get instance failed!");
        NapiDrmError::ThrowError(env, "get instance failed!", DRM_SERVICE_FATAL_ERROR);
        return result;
    }
    if (argc == ARGS_ONE) {
        bool isSurpportted = MediaKeySystemFactoryImpl::GetInstance()->IsMediaKeySystemSupported(pluginName);
        napi_get_boolean(env, isSurpportted, &result);
        return result;
    }
    buffer[0] = '\0';
    if (napi_get_value_string_utf8(env, argv[PARAM1], buffer, PATH_MAX, &length) != napi_ok) {
        DRM_ERR_LOG("Could not able to read mimeType argument!");
        NapiDrmError::ThrowError(env, "The param mimeType is missing or too long.", DRM_UNKNOWN_ERROR);
        return result;
    }
    std::string mimeType = std::string(buffer);
    if (argc == ARGS_TWO && mimeType.length() != 0) {
        bool isSurpportted = MediaKeySystemFactoryImpl::GetInstance()->IsMediaKeySystemSupported(pluginName, mimeType);
        napi_get_boolean(env, isSurpportted, &result);
        return result;
    }
    buffer[0] = '\0';

    int32_t jsContentProtectionLevel = DRM_ERROR;
    if (napi_get_value_int32(env, argv[PARAM2], &jsContentProtectionLevel) != napi_ok) {
        DRM_ERR_LOG("Could not able to read securityLevel argument!");
        NapiDrmError::ThrowError(env, "Could not able to read securityLevel argument.", DRM_UNKNOWN_ERROR);
        return result;
    }
    IMediaKeySessionService::ContentProtectionLevel securityLevel =
        (IMediaKeySessionService::ContentProtectionLevel)jsContentProtectionLevel;
    if ((securityLevel <= IMediaKeySessionService::CONTENT_PROTECTION_LEVEL_UNKNOWN) ||
        (securityLevel >= IMediaKeySessionService::CONTENT_PROTECTION_LEVEL_MAX)) {
        DRM_ERR_LOG("ContentProtectionLevel is invalid");
        NapiDrmError::ThrowError(env, "param ContentProtectionLevel exceeds reasonable range!", DRM_INVALID_PARAM);
        return result;
    }

    if (argc == ARGS_THREE) {
        bool isSurpportted =
            MediaKeySystemFactoryImpl::GetInstance()->IsMediaKeySystemSupported(pluginName, mimeType, securityLevel);
        napi_get_boolean(env, isSurpportted, &result);
        return result;
    }
    napi_get_boolean(env, false, &result);
    return result;
}

static napi_value mapToJsArray(napi_env env, std::map<std::string, std::string> &mediaKeySystemNames)
{
    DRM_INFO_LOG("mapToJsArray enter.");
    napi_value jsArray;
    int32_t i = 0;
    if (napi_create_array_with_length(env, mediaKeySystemNames.size(), &jsArray) != napi_ok) {
        DRM_ERR_LOG("napi_create_array_with_length failed");
        NapiDrmError::ThrowError(env, "napi_create_array_with_length failed",
            DRM_UNKNOWN_ERROR);
        return nullptr;
    }

    for (auto it = mediaKeySystemNames.begin(); it != mediaKeySystemNames.end(); it++) {
        napi_value jsObject;
        napi_value jsName;
        napi_value jsValue;
        napi_create_object(env, &jsObject);
        napi_create_string_utf8(env, it->first.c_str(), NAPI_AUTO_LENGTH, &jsName);
        napi_set_named_property(env, jsObject, "name", jsName);
        napi_create_string_utf8(env, it->second.c_str(), NAPI_AUTO_LENGTH, &jsValue);
        napi_set_named_property(env, jsObject, "uuid", jsValue);
        napi_set_element(env, jsArray, i++, jsObject);
    }
    return jsArray;
}

napi_value MediaKeySystemNapi::GetMediaKeySystems(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("GetMediaKeySystems enter");
    napi_value result = nullptr;
    std::map<std::string, std::string> keySystemNames;
    napi_get_undefined(env, &result);
    int32_t ret = MediaKeySystemFactoryImpl::GetInstance()->GetMediaKeySystems(keySystemNames);
    if (keySystemNames.size() == 0 || ret != DRM_OK) {
        DRM_ERR_LOG("Plugin not exist.");
        NapiDrmError::ThrowError(env, "GetMediaKeySystems call Failed!", DRM_SERVICE_FATAL_ERROR);
        return result;
    }
    result = mapToJsArray(env, keySystemNames);
    return result;
}

napi_value MediaKeySystemNapi::GetMediaKeySystemUuid(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("GetMediaKeySystemUuid enter");
    napi_value result = nullptr;
    size_t argc = ARGS_ONE;
    napi_value argv[ARGS_ONE] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    char nameStr[PATH_MAX];
    size_t nameStrLength = 0;
    std::string uuid;
    napi_get_undefined(env, &result);

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_ONE, "requires 1 parameters maximum");

    status = napi_get_value_string_utf8(env, argv[PARAM0], nameStr, PATH_MAX, &nameStrLength);
    if (status != napi_ok) {
        DRM_ERR_LOG("napi_get_value_string_utf8 failed");
        NapiDrmError::ThrowError(env, "The param configName is missing or too long.", DRM_INVALID_PARAM);
        return result;
    }
    std::string name = std::string(nameStr);
    int32_t ret = MediaKeySystemFactoryImpl::GetInstance()->GetMediaKeySystemUuid(name, uuid);
    if (ret != DRM_OK) {
        DRM_ERR_LOG("GetMediaKeySystemUuid failed");
        NapiDrmError::ThrowError(env, "GetMediaKeySystemUuid call failed", DRM_SERVICE_FATAL_ERROR);
        return result;
    }
    status = napi_create_string_utf8(env, uuid.c_str(), NAPI_AUTO_LENGTH, &result);
    if (status != napi_ok) {
        DRM_ERR_LOG("napi_create_string_utf8 failed");
        NapiDrmError::ThrowError(env, "napi_create_string_utf8 failed", DRM_UNKNOWN_ERROR);
        return result;
    }
    return result;
}

napi_value MediaKeySystemNapi::CreateMediaKeySession(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("CreateMediaKeySession enter.");
    DrmTrace trace("MediaKeySystemNapi::CreateMediaKeySession");
    napi_status status;
    napi_value result = nullptr;
    size_t argc = ARGS_ONE;
    napi_value argv[ARGS_ONE] = {0};
    napi_value thisVar = nullptr;
    int32_t jsContentProtectionLevel = 0;
    sptr<MediaKeySessionImpl> keySessionImpl = nullptr;
    MediaKeySystemNapi *mediaKeySystemNapi = nullptr;
    napi_get_undefined(env, &result);
    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    if (argc != ARGS_ZERO) {
        status = napi_get_value_int32(env, argv[PARAM0], &jsContentProtectionLevel);
        if (status != napi_ok) {
            DRM_ERR_LOG("napi_get_value_int32 failed");
            NapiDrmError::ThrowError(env, "napi_get_value_int32 failed", DRM_UNKNOWN_ERROR);
            return result;
        }
        IMediaKeySessionService::ContentProtectionLevel securityLevel =
            static_cast<IMediaKeySessionService::ContentProtectionLevel>(jsContentProtectionLevel);
        if (securityLevel <= IMediaKeySessionService::ContentProtectionLevel::CONTENT_PROTECTION_LEVEL_UNKNOWN ||
            securityLevel >= IMediaKeySessionService::ContentProtectionLevel::CONTENT_PROTECTION_LEVEL_MAX) {
            NapiDrmError::ThrowError(env, "The param ContentProtectionLevel exceeds reasonable range.", DRM_INVALID_PARAM);
            DRM_ERR_LOG("securityLevel is error!");
            return result;
        }
    } else {
        jsContentProtectionLevel = IMediaKeySessionService::ContentProtectionLevel::CONTENT_PROTECTION_LEVEL_UNKNOWN;
    }
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr && mediaKeySystemNapi->mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemNapi->mediaKeySystemImpl_->CreateMediaKeySession(
            (IMediaKeySessionService::ContentProtectionLevel)securityLevel, &keySessionImpl);
        if (ret != DRM_OK) {
            NapiDrmError::ThrowError(env, "CreateMediaKeySession failed.", ret);
            DRM_ERR_LOG("CreateMediaKeySession get failed!!!");
            return result;
        }
    } else {
        DRM_ERR_LOG("napi_unwrap call Failed!");
        NapiDrmError::ThrowError(env, "napi_get_value_int32 failed", DRM_UNKNOWN_ERROR);
        return result;
    }
    result = MediaKeySessionNapi::CreateMediaKeySession(env, keySessionImpl);
    return result;
}

napi_value MediaKeySystemNapi::SetConfigurationString(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("SetConfiguration enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_TWO;
    napi_value argv[ARGS_TWO] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    std::string name;
    std::string value;
    char nameBuffer[PATH_MAX];
    size_t nameBufferLen = 0;
    char valueBuffer[PATH_MAX];
    size_t valueBufferLen = 0;
    MediaKeySystemNapi *mediaKeySystemNapi = nullptr;
    napi_get_undefined(env, &result);

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_TWO, "Requires 2 parameters maximum");

    status = napi_get_value_string_utf8(env, argv[PARAM0], nameBuffer, PATH_MAX, &nameBufferLen);
    if (status != napi_ok) {
        DRM_ERR_LOG("Could not able to read name argument!");
        NapiDrmError::ThrowError(env, "The param configName is missing or too long", DRM_INVALID_PARAM);
        return result;
    }
    name = std::string(nameBuffer);
    status = napi_get_value_string_utf8(env, argv[PARAM1], valueBuffer, PATH_MAX, &valueBufferLen);
    if (status != napi_ok) {
        DRM_ERR_LOG("napi_get_value_string_utf8 failed!");
        NapiDrmError::ThrowError(env, "The param value is missing or too long.", DRM_INVALID_PARAM);
        return result;
    }
    value = std::string(valueBuffer);
    if (value.length() == 0) {
        DRM_ERR_LOG("String Parameter length cannot be zero!");
        NapiDrmError::ThrowError(env, "The param value length is 0.", DRM_INVALID_PARAM);
        return result;
    }
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr && mediaKeySystemNapi->mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemNapi->mediaKeySystemImpl_->SetConfigurationString(name, value);
        if (ret != napi_ok) {
            DRM_ERR_LOG("SetConfigurationString failed!");
            NapiDrmError::ThrowError(env, "SetConfigurationString failed, service error.", DRM_SERVICE_FATAL_ERROR);
            return result;
        }
    } else {
        DRM_ERR_LOG("SetConfiguration call Failed!");
        NapiDrmError::ThrowError(env, "SetConfigurationString failed, unknown error.", DRM_UNKNOWN_ERROR);
        return result;
    }
    return result;
}

napi_value MediaKeySystemNapi::GetConfigurationString(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("GetConfiguration enter");
    napi_value result = nullptr;
    size_t argc = ARGS_ONE;
    napi_value argv[ARGS_ONE] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    char nameStr[PATH_MAX];
    size_t nameStrLength = 0;
    std::string value;
    MediaKeySystemNapi *mediaKeySystemNapi = nullptr;
    napi_get_undefined(env, &result);

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_ONE, "requires 1 parameters maximum");

    status = napi_get_value_string_utf8(env, argv[PARAM0], nameStr, PATH_MAX, &nameStrLength);
    if (status != napi_ok) {
        DRM_ERR_LOG("napi_get_value_string_utf8 failed!");
        NapiDrmError::ThrowError(env, "The param configName is missing or too long.", DRM_INVALID_PARAM);
        return result;
    }
    std::string name = std::string(nameStr);
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr && mediaKeySystemNapi->mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemNapi->mediaKeySystemImpl_->GetConfigurationString(name, value);
        if (ret != napi_ok) {
            DRM_ERR_LOG("GetConfigurationString failed!");
            NapiDrmError::ThrowError(env, "GetConfigurationString failed, service error.", DRM_SERVICE_FATAL_ERROR);
            return result;
        }
    } else {
        DRM_ERR_LOG("napi_unwrap failed!");
        NapiDrmError::ThrowError(env, "Get configuration failed, unknown error.", DRM_UNKNOWN_ERROR);
        return result;
    }

    napi_create_string_utf8(env, value.c_str(), NAPI_AUTO_LENGTH, &result);
    return result;
}

napi_value MediaKeySystemNapi::SetConfigurationByteArray(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("SetConfigurationByteArray enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_TWO;
    napi_value argv[ARGS_TWO] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    std::string name;
    char nameBuffer[PATH_MAX];
    size_t nameBufferLen = 0;
    bool isTypeArray;
    void *valueData = nullptr;
    size_t valueDataLen;
    size_t offset;
    napi_value arraybuffer = nullptr;
    napi_typedarray_type type;
    MediaKeySystemNapi *mediaKeySystemNapi = nullptr;

    napi_get_undefined(env, &result);
    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_TWO, "requires 2 parameters maximum");

    status = napi_get_value_string_utf8(env, argv[PARAM0], nameBuffer, PATH_MAX, &nameBufferLen);
    if (status != napi_ok) {
        DRM_ERR_LOG("Could not able to read name argument!");
        NapiDrmError::ThrowError(env, "The param configName is missing or too long.", DRM_INVALID_PARAM);
        return result;
    }
    name = std::string(nameBuffer);
    napi_is_typedarray(env, argv[PARAM1], &isTypeArray);
    if (!isTypeArray) {
        DRM_ERR_LOG("Second parame is not array!");
        NapiDrmError::ThrowError(env, "The param value type is invalid.", DRM_INVALID_PARAM);
        return result;
    }
    napi_get_typedarray_info(env, argv[PARAM1], &type, &valueDataLen, &valueData, &arraybuffer, &offset);
    if (valueData == nullptr) {
        DRM_ERR_LOG("napi_get_typedarray_info failed!");
        NapiDrmError::ThrowError(env, "The param value is missing or too long.", DRM_INVALID_PARAM);
        return result;
    }
    uint8_t *valueDataPtr = reinterpret_cast<uint8_t *>(valueData);
    std::vector<uint8_t> value(valueDataPtr, valueDataPtr + valueDataLen);
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr && mediaKeySystemNapi->mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemNapi->mediaKeySystemImpl_->SetConfigurationByteArray(name, value);
        if (ret != napi_ok) {
            DRM_ERR_LOG("SetConfigurationByteArray failed!");
            NapiDrmError::ThrowError(env, "SetConfigurationByteArray failed, service error.", DRM_SERVICE_FATAL_ERROR);
            return result;
        }
    } else {
        DRM_ERR_LOG("napi_unwrap call Failed!");
        NapiDrmError::ThrowError(env, "Set configuration failed, unknown error.", DRM_UNKNOWN_ERROR);
        return result;
    }
    return result;
}

napi_value MediaKeySystemNapi::GetConfigurationByteArray(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("GetConfigurationByteArray enter");
    napi_value result = nullptr;
    size_t argc = ARGS_TWO;
    napi_value argv[ARGS_TWO] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    char nameStr[PATH_MAX];
    size_t nameStrLength = 0;
    std::vector<uint8_t> value;
    MediaKeySystemNapi *mediaKeySystemNapi = nullptr;
    napi_get_undefined(env, &result);

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_TWO, "requires 2 parameters maximum");

    status = napi_get_value_string_utf8(env, argv[PARAM0], nameStr, PATH_MAX, &nameStrLength);
    if (status != napi_ok) {
        DRM_ERR_LOG("Could not able to read name argument!");
        NapiDrmError::ThrowError(env, "The param configName is missing or too long.", DRM_INVALID_PARAM);
        return result;
    }
    std::string name = std::string(nameStr);
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr && mediaKeySystemNapi->mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemNapi->mediaKeySystemImpl_->GetConfigurationByteArray(name, value);
        if (ret != napi_ok) {
            DRM_ERR_LOG("GetConfigurationByteArray failed!");
            NapiDrmError::ThrowError(env, "GetConfigurationByteArray failed, service error.", DRM_SERVICE_FATAL_ERROR);
            return result;
        }
    } else {
        DRM_ERR_LOG("napi_unwrap Failed!");
        NapiDrmError::ThrowError(env, "GetConfigurationByteArray failed.", DRM_UNKNOWN_ERROR);
        return result;
    }
    size_t valueLen = value.size();
    NAPI_CALL(env, napi_create_array(env, &result));
    for (size_t i = 0; i < valueLen; i++) {
        napi_value item;
        napi_create_int32(env, value[i], &item);
        napi_set_element(env, result, i, item);
    }
    return result;
}

napi_value MediaKeySystemNapi::GetMaxContentProtectionLevel(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("GetMaxContentProtectionLevel enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_ONE;
    napi_value argv[ARGS_ONE] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    MediaKeySystemNapi *mediaKeySystemNapi = nullptr;

    napi_get_undefined(env, &result);
    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_ONE, "requires one parameters maximum");
    IMediaKeySessionService::ContentProtectionLevel level = IMediaKeySessionService::CONTENT_PROTECTION_LEVEL_UNKNOWN;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr && mediaKeySystemNapi->mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemNapi->mediaKeySystemImpl_->GetMaxContentProtectionLevel(&level);
        if (ret != napi_ok) {
            DRM_ERR_LOG("GetCertificateStatus failed!");
            NapiDrmError::ThrowError(env, "GetMaxContentProtectionLevel failed, service error.",
                DRM_SERVICE_FATAL_ERROR);
            return result;
        }
    } else {
        DRM_ERR_LOG("napi_unwrap call Failed!");
        NapiDrmError::ThrowError(env, "GetMaxContentProtectionLevel failed, unknown error.", DRM_UNKNOWN_ERROR);
        return result;
    }

    NAPI_CALL(env, napi_create_int32(env, (int32_t)level, &result));
    return result;
}

bool MediaKeySystemNapi::CheckMediaKeySystemStatus(MediaKeySystemNapi *napi,
    std::shared_ptr<MediaKeySystemAsyncContext> context)
{
    DRM_NAPI_CHECK_AND_RETURN_LOG(napi != nullptr, false, "napi object is nullptr.");
    if (napi->mediaKeySystemImpl_ == nullptr) {
        context->SignError(DRM_SERVICE_FATAL_ERROR);
        return false;
    }
    return true;
}

bool MediaKeySystemNapi::CheckContextStatus(std::shared_ptr<MediaKeySystemAsyncContext> context)
{
    DRM_NAPI_CHECK_AND_RETURN_LOG(context != nullptr, false, "context object is nullptr.");
    if (context->native == nullptr) {
        context->SignError(DRM_SERVICE_FATAL_ERROR);
        return false;
    }
    return true;
}

napi_value MediaKeySystemNapi::GenerateKeySystemRequest(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("GenerateKeySystemRequest enter");
    DrmTrace trace("MediaKeySystemNapi::GenerateKeySystemRequest");
    auto context = std::make_shared<MediaKeySystemAsyncContext>();
    if (context == nullptr) {
        DRM_ERR_LOG("GenerateKeySystemRequest failed.");
        NapiDrmError::ThrowError(env, "make context failed, unknown error.", DRM_UNKNOWN_ERROR);
        return NapiParamUtils::GetUndefinedValue(env);
    }

    context->GetCbInfo(env, info);

    auto executor = [context]() {
        DRM_NAPI_CHECK_AND_RETURN_VOID_LOG(CheckContextStatus(context), "context object state is error.");
        auto obj = reinterpret_cast<MediaKeySystemNapi *>(context->native);
        ObjectRefMap objectGuard(obj);
        auto *napiMediaKeySystem = objectGuard.GetPtr();
        DRM_NAPI_CHECK_AND_RETURN_VOID_LOG(CheckMediaKeySystemStatus(napiMediaKeySystem, context),
            "context object state is error.");
        context->intValue = napiMediaKeySystem->mediaKeySystemImpl_->GenerateKeySystemRequest(
            context->provisionRequest.data, context->provisionRequest.defaultURL);
        if (context->intValue != 0) {
            context->SignError(DRM_SERVICE_FATAL_ERROR);
        }
    };

    auto complete = [env, context](napi_value &output) {
        NapiParamUtils::SetProvisionRequest(env, context->provisionRequest, output);
    };
    return NapiAsyncWork::Enqueue(env, context, "GenerateKeySystemRequest", executor, complete);
}

napi_value MediaKeySystemNapi::ProcessKeySystemResponse(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("ProcessKeySystemResponse enter.");
    int64_t beginTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    DrmTrace trace("MediaKeySystemNapi::ProcessKeySystemResponse");
    auto context = std::make_shared<MediaKeySystemAsyncContext>();
    if (context == nullptr) {
        DRM_ERR_LOG("ProcessKeySystemResponse failed.");
        NapiDrmError::ThrowError(env, "make context failed, unknown error.", DRM_UNKNOWN_ERROR);
        return NapiParamUtils::GetUndefinedValue(env);
    }

    auto inputParser = [env, context](size_t argc, napi_value *argv) {
        NAPI_CHECK_ARGS_RETURN_VOID(context, argc == ARGS_ONE, "invalid arguments", DRM_INVALID_PARAM);
        context->status = NapiParamUtils::GetValueUint8Array(env, context->response, argv[PARAM0]);
        NAPI_CHECK_STATUS_RETURN_VOID(context, "ProcessKeySystemResponse failed.", DRM_INVALID_PARAM);
    };

    context->GetCbInfo(env, info, inputParser);

    auto executor = [context]() {
        DRM_NAPI_CHECK_AND_RETURN_VOID_LOG(CheckContextStatus(context), "context object state is error.");
        auto obj = reinterpret_cast<MediaKeySystemNapi *>(context->native);
        ObjectRefMap objectGuard(obj);
        auto *napiMediaKeySystem = objectGuard.GetPtr();
        DRM_NAPI_CHECK_AND_RETURN_VOID_LOG(CheckMediaKeySystemStatus(napiMediaKeySystem, context),
            "context object state is error.");
        context->intValue = napiMediaKeySystem->mediaKeySystemImpl_->ProcessKeySystemResponse(context->response);
        if (context->intValue != DRM_OK) {
            context->SignError(DRM_SERVICE_FATAL_ERROR);
        }
    };

    auto complete = [env](napi_value &output) { output = NapiParamUtils::GetUndefinedValue(env); };
    ConfigParser::WriteEndEvent(0, 0, std::string("processKeySystemResponse"), beginTime);
    return NapiAsyncWork::Enqueue(env, context, "ProcessKeySystemResponse", executor, complete);
}

static napi_value vectorToJsArray(napi_env env, std::vector<IMediaKeySystemService::MetircKeyValue> &metrics)
{
    DRM_INFO_LOG("vectorToJsArray enter.");
    napi_value jsArray;
    napi_create_array_with_length(env, metrics.size(), &jsArray);
    for (size_t i = 0; i < metrics.size(); i++) {
        napi_value jsObject;
        napi_value jsName;
        napi_value jsValue;
        napi_create_object(env, &jsObject);
        napi_create_string_utf8(env, metrics[i].name.c_str(), NAPI_AUTO_LENGTH, &jsName);
        napi_set_named_property(env, jsObject, "name", jsName);
        napi_create_string_utf8(env, metrics[i].value.c_str(), NAPI_AUTO_LENGTH, &jsValue);
        napi_set_named_property(env, jsObject, "value", jsValue);

        napi_set_element(env, jsArray, i, jsObject);
    }
    return jsArray;
}

napi_value MediaKeySystemNapi::GetStatistics(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("GetStatistics enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_ZERO;
    napi_value argv[ARGS_ZERO];
    napi_value thisVar = nullptr;
    napi_status status;
    std::vector<IMediaKeySystemService::MetircKeyValue> metrics;
    MediaKeySystemNapi *mediaKeySystemNapi = nullptr;
    napi_get_undefined(env, &result);

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr && mediaKeySystemNapi->mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemNapi->mediaKeySystemImpl_->GetStatistics(metrics);
        if (ret != napi_ok) {
            DRM_ERR_LOG("GetStatistics failed!");
            NapiDrmError::ThrowError(env, "Get statistics failed, service error.", DRM_SERVICE_FATAL_ERROR);
            return result;
        }
    } else {
        DRM_ERR_LOG("napi_unwrap Failed!");
        NapiDrmError::ThrowError(env, "GetStatistics failed, unknown error.", DRM_UNKNOWN_ERROR);
        return result;
    }
    result = vectorToJsArray(env, metrics);
    return result;
}

napi_value MediaKeySystemNapi::GetCertificateStatus(napi_env env, napi_callback_info info)
{
    DrmTrace trace("MediaKeySystemNapi::GetCertificateStatus");
    DRM_INFO_LOG("GetCertificateStatus enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_ZERO;
    napi_value argv[ARGS_ZERO];
    napi_value thisVar = nullptr;
    napi_status status;
    IMediaKeySystemService::CertificateStatus certStatus;
    MediaKeySystemNapi *mediaKeySystemNapi = nullptr;

    napi_get_undefined(env, &result);
    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr && mediaKeySystemNapi->mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemNapi->mediaKeySystemImpl_->GetCertificateStatus(&certStatus);
        if (ret != napi_ok) {
            DRM_ERR_LOG("GetCertificateStatus failed!");
            NapiDrmError::ThrowError(env, "GetCertificateStatus failed, service error.", DRM_SERVICE_FATAL_ERROR);
            return result;
        }
    } else {
        DRM_ERR_LOG("napi_unwrap Failed!");
        NapiDrmError::ThrowError(env, "GetCertificateStatus failed, unknown error.", DRM_UNKNOWN_ERROR);
        return result;
    }
    NAPI_CALL(env, napi_create_int32(env, (int32_t)certStatus, &result));
    return result;
}

static napi_value vectorToJs2DArray(napi_env env, std::vector<std::vector<uint8_t>> vec, napi_value result)
{
    DRM_INFO_LOG("vectorToJs2DArray enter.");
    napi_value outArray;
    napi_value inArray;
    napi_status status = napi_create_array(env, &outArray);
    if (status != napi_ok) {
        DRM_ERR_LOG("napi_create_array failed!");
        NapiDrmError::ThrowError(env, "napi_create_array failed.", DRM_UNKNOWN_ERROR);
        return result;
    }

    for (size_t i = 0; i < vec.size(); i++) {
        status = napi_create_array(env, &inArray);
        if (status != napi_ok) {
            DRM_ERR_LOG("napi_create_array failed!");
            NapiDrmError::ThrowError(env, "napi_create_array failed.", DRM_UNKNOWN_ERROR);
            return result;
        }

        for (size_t j = 0; j < vec[i].size(); j++) {
            napi_value elem;
            status = napi_create_uint32(env, vec[i][j], &elem);
            if (status != napi_ok) {
                DRM_ERR_LOG("napi_create_uint32 failed!");
                NapiDrmError::ThrowError(env, "napi_create_uint32 failed.", DRM_UNKNOWN_ERROR);
                return result;
            }
            status = napi_set_element(env, inArray, j, elem);
            if (status != napi_ok) {
                DRM_ERR_LOG("napi_set_element failed!");
                NapiDrmError::ThrowError(env, "napi_set_element failed.", DRM_UNKNOWN_ERROR);
                return result;
            }
        }
        status = napi_set_element(env, outArray, i, inArray);
        if (status != napi_ok) {
            DRM_ERR_LOG("napi_set_element failed!");
            NapiDrmError::ThrowError(env, "napi_set_element failed.", DRM_UNKNOWN_ERROR);
            return result;
        }
    }
    return outArray;
}

napi_value MediaKeySystemNapi::GetOfflineMediaKeyIds(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("GetOfflineMediaKeyIds enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_ZERO;
    napi_value argv[ARGS_ZERO];
    napi_value thisVar = nullptr;
    napi_status status;
    MediaKeySystemNapi *mediaKeySystemNapi = nullptr;

    napi_get_undefined(env, &result);
    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc >= ARGS_ZERO, "requires 0 parameters maximum");
    std::vector<std::vector<uint8_t>> licenseIds;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr && mediaKeySystemNapi->mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemNapi->mediaKeySystemImpl_->GetOfflineMediaKeyIds(licenseIds);
        if (ret != napi_ok) {
            DRM_ERR_LOG("napi GetOfflineMediaKeyIds failed!");
            NapiDrmError::ThrowError(env, "GetOfflineMediaKeyIds failed, service error.", DRM_SERVICE_FATAL_ERROR);
            return result;
        }
    } else {
        DRM_ERR_LOG("napi_unwrap call Failed!");
        NapiDrmError::ThrowError(env, "GetOfflineMediaKeyIds failed, unknown error.", DRM_UNKNOWN_ERROR);
        return result;
    }

    result = vectorToJs2DArray(env, licenseIds, result);
    return result;
}

napi_value MediaKeySystemNapi::GetOfflineMediaKeyStatus(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("GetOfflineMediaKeyStatus enter");
    napi_value result = nullptr;
    size_t argc = ARGS_ONE;
    napi_value argv[ARGS_ONE] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    void *licenseId = nullptr;
    size_t licenseIdLen;
    size_t offset;
    napi_value arraybuffer = nullptr;
    napi_typedarray_type type;
    bool isTypeArray;
    MediaKeySystemNapi *mediaKeySystemNapi = nullptr;

    napi_get_undefined(env, &result);
    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_ONE, "requires 1 parameters maximum");
    napi_is_typedarray(env, argv[PARAM0], &isTypeArray);
    if (!isTypeArray) {
        DRM_ERR_LOG("First parame is not array!");
        NapiDrmError::ThrowError(env, "The param mediaKeyId type is not array.", DRM_INVALID_PARAM);
        return result;
    }
    napi_get_typedarray_info(env, argv[PARAM0], &type, &licenseIdLen, &licenseId, &arraybuffer, &offset);
    if (licenseId == nullptr) {
        DRM_ERR_LOG("napi_get_typedarray_info failed!");
        NapiDrmError::ThrowError(env, "The param mediaKeyId is invalid.", DRM_INVALID_PARAM);
        return result;
    }
    uint8_t *licenseIdPtr = reinterpret_cast<uint8_t *>(licenseId);
    std::vector<uint8_t> licenseIdVec(licenseIdPtr, licenseIdPtr + licenseIdLen);
    IMediaKeySessionService::OfflineMediaKeyStatus offlineMediaKeyStatus =
        IMediaKeySessionService::OFFLINELICENSESTATUS_UNKNOWN;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr && mediaKeySystemNapi->mediaKeySystemImpl_ != nullptr) {
        int32_t ret =
            mediaKeySystemNapi->mediaKeySystemImpl_->GetOfflineMediaKeyStatus(licenseIdVec, offlineMediaKeyStatus);
        if (ret != napi_ok) {
            DRM_ERR_LOG("GetOfflineMediaKeyStatus failed!");
            NapiDrmError::ThrowError(env, "GetOfflineMediaKeyStatus failed, service error.", DRM_SERVICE_FATAL_ERROR);
            return result;
        }
    } else {
        DRM_ERR_LOG("napi_unwrap call Failed!");
        NapiDrmError::ThrowError(env, "GetOfflineMediaKeyStatus failed, unknown error.", DRM_UNKNOWN_ERROR);
        return result;
    }

    NAPI_CALL(env, napi_create_int32(env, (int32_t)offlineMediaKeyStatus, &result));
    return result;
}

napi_value MediaKeySystemNapi::ClearOfflineMediaKeys(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("ClearOfflineMediaKeys enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_ONE;
    napi_value argv[ARGS_ONE] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    bool isTypeArray;
    void *licenseId = nullptr;
    size_t licenseIdLen;
    size_t offset;
    napi_value arraybuffer = nullptr;
    napi_typedarray_type type;
    MediaKeySystemNapi *mediaKeySystemNapi = nullptr;

    napi_get_undefined(env, &result);
    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_ONE, "requires 2 parameters maximum");
    napi_is_typedarray(env, argv[PARAM0], &isTypeArray);
    if (!isTypeArray) {
        DRM_ERR_LOG("argv[PARAM0] is not array!");
        NapiDrmError::ThrowError(env, "The param mediaKeyId is not array.", DRM_INVALID_PARAM);
        return result;
    }
    napi_get_typedarray_info(env, argv[PARAM0], &type, &licenseIdLen, &licenseId, &arraybuffer, &offset);
    if (licenseId == nullptr) {
        DRM_ERR_LOG("napi_get_typedarray_info failed!");
        NapiDrmError::ThrowError(env, "ClearOfflineMediaKeys failed, unknown error.", DRM_UNKNOWN_ERROR);
        return result;
    }
    uint8_t *licenseIdPtr = reinterpret_cast<uint8_t *>(licenseId);
    std::vector<uint8_t> licenseIdVec(licenseIdPtr, licenseIdPtr + licenseIdLen);
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr && mediaKeySystemNapi->mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemNapi->mediaKeySystemImpl_->ClearOfflineMediaKeys(licenseIdVec);
        if (ret != napi_ok) {
            DRM_ERR_LOG("ClearOfflineMediaKeys failed!");
            NapiDrmError::ThrowError(env, "ClearOfflineMediaKeys failed, service error.", DRM_SERVICE_FATAL_ERROR);
            return result;
        }
    } else {
        DRM_ERR_LOG("ClearOfflineMediaKeys call Failed!");
        NapiDrmError::ThrowError(env, "ClearOfflineMediaKeys failed, unknown error.", DRM_UNKNOWN_ERROR);
        return result;
    }

    return result;
}

napi_value MediaKeySystemNapi::Destroy(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("Release enter.");
    int32_t currentPid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("GetCallingPID: %{public}d", currentPid);

    napi_status status;
    napi_value result = nullptr;
    size_t argc = ARGS_ZERO;
    napi_value argv[ARGS_ZERO];
    napi_value thisVar = nullptr;
    MediaKeySystemNapi *mediaKeySystemNapi = nullptr;

    napi_get_undefined(env, &result);
    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    napi_get_undefined(env, &result);
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr && mediaKeySystemNapi->mediaKeySystemImpl_ != nullptr) {
        int32_t ret = mediaKeySystemNapi->mediaKeySystemImpl_->Release();
        if (ret != napi_ok) {
            DRM_ERR_LOG("GetCertificateStatus failed!");
            NapiDrmError::ThrowError(env, "Destroy failed, service error.", DRM_SERVICE_FATAL_ERROR);
            return result;
        }
    } else {
        DRM_ERR_LOG("napi_unwrap Failed!");
        NapiDrmError::ThrowError(env, "Destroy failed.", DRM_UNKNOWN_ERROR);
        return result;
    }
    return result;
}

void MediaKeySystemNapi::SaveEventCallbackReferrence(const std::string eventType, std::shared_ptr<AutoRef> callbackPair)
{
    DRM_INFO_LOG("SaveEventCallbackReferrence");
    std::lock_guard<std::mutex> lock(mutex_);
    if (mediaKeySystemCallbackNapi_ != nullptr) {
        mediaKeySystemCallbackNapi_->SetCallbackReference(eventType, callbackPair);
    } else {
        DRM_ERR_LOG("SaveEventCallbackReferrence failed.");
    }
}

void MediaKeySystemNapi::ClearEventCallbackReferrence(const std::string eventType)
{
    DRM_INFO_LOG("ClearEventCallbackReference");
    if (mediaKeySystemCallbackNapi_ != nullptr) {
        mediaKeySystemCallbackNapi_->ClearCallbackReference(eventType);
    } else {
        DRM_ERR_LOG("ClearEventCallbackReference failed.");
    }
}

napi_value MediaKeySystemNapi::SetEventCallback(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("SetEventCallback");
    napi_value result = nullptr;
    napi_get_undefined(env, &result);
    size_t length = 0;
    size_t argc = ARGS_TWO;
    napi_value thisVar = nullptr;
    napi_value argv[ARGS_TWO] = { nullptr };
    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc == ARGS_TWO, "only requires 2 parameters");
    if (thisVar == nullptr || argv[PARAM0] == nullptr || argv[PARAM1] == nullptr) {
        NapiDrmError::ThrowError(env, "Mandatory parameters are left unspecified.", DRM_INVALID_PARAM);
        DRM_ERR_LOG("Failed to retrieve arguments in SetEventCallback!");
        return result;
    }
    napi_valuetype valueType = napi_undefined;
    if (napi_typeof(env, argv[PARAM0], &valueType) != napi_ok || valueType != napi_string ||
        napi_typeof(env, argv[PARAM1], &valueType) != napi_ok || valueType != napi_function) {
        NapiDrmError::ThrowError(env, "Mandatory parameter type is invalid.", DRM_INVALID_PARAM);
        return result;
    }

    MediaKeySystemNapi *mediaKeySystemNapi = nullptr;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr) {
        char buffer[PATH_MAX];
        napi_get_value_string_utf8(env, argv[PARAM0], buffer, PATH_MAX, &length);
        std::string eventType = std::string(buffer);
        napi_ref callbackRef;
        napi_create_reference(env, argv[PARAM1], 1, &callbackRef);
        DRM_INFO_LOG("SetEventCallback event is %{public}s", eventType.c_str());

        std::shared_ptr<AutoRef> callbackPair = std::make_shared<AutoRef>(env, callbackRef);
        mediaKeySystemNapi->SaveEventCallbackReferrence(eventType, callbackPair);
        DRM_INFO_LOG("SetEventCallback out");
    } else {
        NapiDrmError::ThrowError(env, "SetEventCallback failed, unknown error.", DRM_UNKNOWN_ERROR);
        DRM_ERR_LOG("SetEventCallback failed!");
    }
    return result;
}

napi_value MediaKeySystemNapi::UnsetEventCallback(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("UnsetEventCallback");
    napi_value result = nullptr;
    napi_get_undefined(env, &result);
    napi_value thisVar = nullptr;
    napi_value argv[ARGS_ONE] = { nullptr };
    size_t argc = 1;
    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc == ARGS_ONE, "only requires 1 parameters");
    if (thisVar == nullptr || argv[PARAM0] == nullptr) {
        DRM_ERR_LOG("Failed to retrieve arguments in UnsetEventCallback!");
        NapiDrmError::ThrowError(env, "The param callback type is missing.", DRM_INVALID_PARAM);
        return result;
    }
    napi_valuetype valueType = napi_undefined;
    if (napi_typeof(env, argv[PARAM0], &valueType) != napi_ok || valueType != napi_string) {
        DRM_ERR_LOG("Failed to retrieve reasonable arguments in UnsetEventCallback!");
        NapiDrmError::ThrowError(env, "The param callback type is not string.", DRM_INVALID_PARAM);
        return result;
    }

    MediaKeySystemNapi *mediaKeySystemNapi = nullptr;
    char buffer[PATH_MAX];
    size_t length = 0;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr) {
        napi_get_value_string_utf8(env, argv[PARAM0], buffer, PATH_MAX, &length);
        std::string eventType = std::string(buffer);
        mediaKeySystemNapi->ClearEventCallbackReferrence(eventType);
        DRM_INFO_LOG("UnsetEventCallback out");
    } else {
        NapiDrmError::ThrowError(env, "UnsetEventCallback failed, unknown error.", DRM_UNKNOWN_ERROR);
        DRM_ERR_LOG("UnsetEventCallback failed!");
    }
    return result;
}
} // namespace DrmStandard
} // namespace OHOS