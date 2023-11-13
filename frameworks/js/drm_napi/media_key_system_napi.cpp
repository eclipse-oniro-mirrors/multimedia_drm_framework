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
#include "media_key_system_napi.h"
#include "key_session_napi.h"

namespace OHOS {
namespace DrmStandard {
thread_local napi_ref MediaKeySystemNapi::sConstructor_ = nullptr;
thread_local sptr<MediaKeySystemImpl> MediaKeySystemNapi::sKeySystem_ = nullptr;

MediaKeySystemNapi::MediaKeySystemNapi() : env_(nullptr), wrapper_(nullptr)
{
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " Instances create", FAKE_POINTER(this));
}

MediaKeySystemNapi::~MediaKeySystemNapi()
{
    DRM_INFO_LOG("MediaKeySystemNapi::~MediaKeySystemNapi enter.");
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " Instances destroy", FAKE_POINTER(this));
    if (wrapper_ != nullptr) {
        napi_delete_reference(env_, wrapper_);
    }
    if (keySystem_) {
        keySystem_ = nullptr;
    }
    DRM_INFO_LOG("MediaKeySystemNapi::~MediaKeySystemNapi exit.");
}

napi_value MediaKeySystemNapi::Init(napi_env env, napi_value exports)
{
    DRM_INFO_LOG("MediaKeySystemNapi Init enter.");
    napi_status status;
    napi_value ctorObj;
    int32_t refCount = 1;

    napi_property_descriptor properties[] = {
        DECLARE_NAPI_FUNCTION("setConfiguration", SetConfiguration),
        DECLARE_NAPI_FUNCTION("getConfiguration", GetConfiguration),
        DECLARE_NAPI_FUNCTION("generateKeySystemRequest", GenerateKeySystemRequest),
        DECLARE_NAPI_FUNCTION("processKeySystemResponse", ProcessKeySystemResponse),
        DECLARE_NAPI_FUNCTION("createKeySession", CreateKeySession),
        DECLARE_NAPI_FUNCTION("getMetric", GetMetric),
        DECLARE_NAPI_FUNCTION("getSecurityLevel", GetSecurityLevel),
    };

    status = napi_define_class(env, MEDIA_KEY_SYSTEM_NAPI_CLASS_NAME, NAPI_AUTO_LENGTH,
                               MediaKeySystemNapiConstructor, nullptr,
                               sizeof(properties) / sizeof(properties[PARAM0]),
                               properties, &ctorObj);
    if (status == napi_ok) {
        if (napi_create_reference(env, ctorObj, refCount, &sConstructor_) == napi_ok) {
            status = napi_set_named_property(env, exports, MEDIA_KEY_SYSTEM_NAPI_CLASS_NAME, ctorObj);
            if (status == napi_ok) {
                return exports;
            }
        }
    }
    DRM_INFO_LOG("MediaKeySystemNapi Init exit.");
    return nullptr;
}

napi_value MediaKeySystemNapi::MediaKeySystemNapiConstructor(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySystemNapi::MediaKeySystemNapiConstructor enter.");
    napi_value result = nullptr;
    size_t argCount = 0;
    napi_value jsThis = nullptr;

    napi_get_undefined(env, &result);

    napi_status status = napi_get_cb_info(env, info, &argCount, nullptr, &jsThis, nullptr);
    CHECK_AND_RETURN_RET_LOG(status == napi_ok, result, "failed to napi_get_cb_info");

    if (status == napi_ok && jsThis != nullptr) {
        std::unique_ptr<MediaKeySystemNapi> obj = std::make_unique<MediaKeySystemNapi>();
        obj->env_ = env;
        if (MediaKeySystemNapi::sKeySystem_ == nullptr) {
            DRM_ERR_LOG("sKeySystem_ is null");
            return result;
        }
        obj->keySystem_ = MediaKeySystemNapi::sKeySystem_;

        status = napi_wrap(env, jsThis, reinterpret_cast<void*>(obj.get()),
                           MediaKeySystemNapi::MediaKeySystemNapiDestructor, nullptr, nullptr);
        if (status == napi_ok) {
            obj.release();
            return jsThis;
        } else {
            DRM_ERR_LOG("Failure wrapping js to native napi");
        }
        DRM_ERR_LOG("will call IsMediaKeySystemSupported");
    }
    DRM_INFO_LOG("MediaKeySystemNapi::MediaKeySystemNapiConstructor exit.");
    return result;
}

napi_value MediaKeySystemNapi::CreateMediaKeySystemInstance(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySystemNapi::CreateMediaKeySystemInstance enter.");
    napi_status status;
    napi_value result = nullptr;
    napi_value ctor;

    status = napi_get_reference_value(env, sConstructor_, &ctor);
    if (status == napi_ok) {
        size_t argc = ARGS_ONE;
        napi_value argv[ARGS_ONE] = {0};
        napi_value thisVar = nullptr;
        DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
        napi_get_undefined(env, &result);
        std::string uuid;
        char uuidBuffer[PATH_MAX];
        size_t uuidBufferLen = 0;
        status = napi_get_value_string_utf8(env, argv[PARAM0], uuidBuffer, PATH_MAX, &uuidBufferLen);
        if (status != napi_ok) {
            DRM_ERR_LOG("Could not able to read uuid argument!");
            return nullptr;
        }
        uuid = std::string(uuidBuffer);
        int retCode = MediaKeySystemFactoryImpl::GetInstance()->CreateMediaKeySystem(uuid, 
            &MediaKeySystemNapi::sKeySystem_);
        if (retCode != DRM_OK || MediaKeySystemNapi::sKeySystem_ == nullptr) {
            DRM_ERR_LOG("MediaKeySystemNapi sKeySystem_ get failed!!!");
            return nullptr;
        }
        status = napi_new_instance(env, ctor, 0, nullptr, &result);
        MediaKeySystemNapi::sKeySystem_ = nullptr;
        if (status == napi_ok) {
            DRM_ERR_LOG("CreateMediaKeySystemInstance 164");
            return result;
        } else {
            DRM_ERR_LOG("New instance could not be obtained");
        }
    }
    napi_get_undefined(env, &result);
    DRM_INFO_LOG("CreateMediaKeySystemInstance exit.");
    return result;
}

void MediaKeySystemNapi::MediaKeySystemNapiDestructor(napi_env env, void *nativeObject, void *finalize)
{
    DRM_INFO_LOG("MediaKeySystemNapi::MediaKeySystemNapiDestructor enter.");
    MediaKeySystemNapi* mediaKeySystemNapi = reinterpret_cast<MediaKeySystemNapi*>(nativeObject);
    if (mediaKeySystemNapi != nullptr) {
        mediaKeySystemNapi->~MediaKeySystemNapi();
    }
    DRM_INFO_LOG("MediaKeySystemNapi::MediaKeySystemNapiDestructor exit.");
}

napi_value MediaKeySystemNapi::Release(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySystemNapi::Release enter.");
    int32_t currentPid = IPCSkeleton::GetCallingPid();     
    DRM_DEBUG_LOG("MediaKeySystemNapi GetCallingPID: %{public}d", currentPid);

    napi_status status;
    napi_value result = nullptr;
    size_t argc = ARGS_ZERO;
    napi_value argv[ARGS_ZERO];
    napi_value thisVar = nullptr;
    MediaKeySystemNapi* mediaKeySystemNapi = nullptr;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    napi_get_undefined(env, &result);
    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr) {
        mediaKeySystemNapi->keySystem_->Release();
    } else {
        DRM_ERR_LOG("mediaKeySystemNapi Release call Failed!");
    }
    DRM_INFO_LOG("MediaKeySystemNapi::Release exit.");
    return result;
}

napi_value MediaKeySystemNapi::IsMediaKeySystemSupported(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySystemNapi::IsMediaKeySystemSupported enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_THREE;
    napi_value argv[ARGS_THREE] = {0, 0, 0};
    napi_value thisVar = nullptr;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    char buffer[PATH_MAX];
    size_t length = 0;
    // get uuid
    if (napi_get_value_string_utf8(env, argv[PARAM0], buffer, PATH_MAX, &length) != napi_ok) {
        DRM_ERR_LOG("Could not able to read uuid argument!");
        return nullptr;
    }
    std::string uuid = std::string(buffer);
    // search by uuid
    if (argc == ARGS_ONE) {
        bool isSurpportted = MediaKeySystemFactoryImpl::GetInstance()->IsMediaKeySystemSupported(uuid);
        napi_get_boolean(env, isSurpportted, &result);
        return result;
    }
    // get mimeType
    if (napi_get_value_string_utf8(env, argv[PARAM1], buffer, PATH_MAX, &length) != napi_ok) {
        DRM_ERR_LOG("Could not able to read mimeType argument!");
        return nullptr;
    }
    std::string mimeType = std::string(buffer);
    // search by uuid and mineType
    if (argc == ARGS_TWO) {
        bool isSurpportted = MediaKeySystemFactoryImpl::GetInstance()->IsMediaKeySystemSupported(uuid, mimeType);
        napi_get_boolean(env, isSurpportted, &result);
        return result;
    }

    // get securityLevel
    int32_t jsSecurityLevel = -1;
    if (napi_get_value_int32(env, argv[PARAM2], &jsSecurityLevel) != napi_ok) {
        DRM_ERR_LOG("Could not able to read securityLevel argument!");
        return nullptr;
    }
    IKeySessionService::SecurityLevel securityLevel = IKeySessionService::SECURITY_LEVEL_UNKNOWN;
    switch (jsSecurityLevel) {
        case IKeySessionService::SECURITY_LEVEL_UNKNOWN:
            securityLevel = IKeySessionService::SECURITY_LEVEL_UNKNOWN;
            break;
        case IKeySessionService::SECURITY_LEVEL_SW_CRYPTO:
            securityLevel = IKeySessionService::SECURITY_LEVEL_SW_CRYPTO;
            break;
        case IKeySessionService::SECURITY_LEVEL_SW_DECODE:
            securityLevel = IKeySessionService::SECURITY_LEVEL_SW_DECODE;
            break;
        case IKeySessionService::SECURITY_LEVEL_HW_CRYPTO:
            securityLevel = IKeySessionService::SECURITY_LEVEL_HW_CRYPTO;
            break;
        case IKeySessionService::SECURITY_LEVEL_HW_DECODE:
            securityLevel = IKeySessionService::SECURITY_LEVEL_HW_DECODE;
            break;
        case IKeySessionService::SECURITY_LEVEL_HW_ALL:
            securityLevel = IKeySessionService::SECURITY_LEVEL_HW_ALL;
            break;
        case IKeySessionService::SECURITY_LEVEL_MAX:
            securityLevel = IKeySessionService::SECURITY_LEVEL_MAX;
            break;
        default:
            DRM_ERR_LOG("jsSecurityLevel is invalid");
            return nullptr;
    }

    // search by uuid, mineType and securityLevel
    if (argc == ARGS_THREE) {
        bool isSurpportted =
            MediaKeySystemFactoryImpl::GetInstance()->IsMediaKeySystemSupported(uuid, mimeType, securityLevel);
        napi_get_boolean(env, isSurpportted, &result);
        return result;
    }
    DRM_INFO_LOG("MediaKeySystemNapi::IsMediaKeySystemSupported exit.");
    return nullptr;
}

napi_value MediaKeySystemNapi::CreateKeySession(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySystemNapi::CreateKeySession enter.");
    napi_status status;
    napi_value result = nullptr;
    size_t argc = ARGS_ONE;
    napi_value argv[ARGS_ONE] = {0};
    napi_value thisVar = nullptr;
    int32_t jsSecurityLevel = 0;
    sptr<KeySessionImpl> keySessionImpl = nullptr;
    MediaKeySystemNapi* mediaKeySystemNapi = nullptr;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    napi_get_undefined(env, &result);

    status = napi_get_value_int32(env, argv[PARAM0], &jsSecurityLevel);
    if (status != napi_ok) {
        DRM_ERR_LOG("MediaKeySystemNapi napi get jsSecurityLevel failure!");
        return nullptr;
    }
    // not check the securityLevel is valid.
    IKeySessionService::SecurityLevel securityLevel = static_cast<IKeySessionService::SecurityLevel>(jsSecurityLevel);
    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr) {
        int ret = mediaKeySystemNapi->keySystem_->CreateKeySession((IKeySessionService::SecurityLevel)securityLevel,
            &keySessionImpl);
        if (ret != DRM_OK || keySessionImpl == nullptr) {
            DRM_ERR_LOG("MediaKeySystemNapi CreateKeySession get failed!!!");
            return nullptr;
        }
    } else {
        DRM_ERR_LOG("mediaKeySystemNapi CreateKeySession call Failed!");
        return nullptr;
    }

    result = KeySessionNapi::CreateKeySession(env, keySessionImpl);
    DRM_INFO_LOG("MediaKeySystemNapi::CreateKeySession exit.");
    return result;
}

napi_value MediaKeySystemNapi::SetConfiguration(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySystemNapi::SetConfiguration enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_THREE;
    napi_value argv[ARGS_THREE] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    int32_t configType = 0;
    std::string name;
    std::string value;
    char nameBuffer[PATH_MAX];
    size_t nameBufferLen = 0;
    char valueBuffer[PATH_MAX];
    size_t valueBufferLen = 0;
    MediaKeySystemNapi* mediaKeySystemNapi = nullptr;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_THREE, "requires 3 parameters maximum");

    status = napi_get_value_int32(env, argv[PARAM0], &configType);
    if (status != napi_ok) {
        DRM_ERR_LOG("Could not able to read configType argument!");
        return nullptr;
    }
    status = napi_get_value_string_utf8(env, argv[PARAM1], nameBuffer, PATH_MAX, &nameBufferLen);
    if (status != napi_ok) {
        DRM_ERR_LOG("Could not able to read name argument!");
        return nullptr;
    }
    name = std::string(nameBuffer);
    status = napi_get_value_string_utf8(env, argv[PARAM2], valueBuffer, PATH_MAX, &valueBufferLen);
    if (status != napi_ok) {
        DRM_ERR_LOG("Could not able to read value argument!");
        return nullptr;
    }
    value = std::string(valueBuffer);
    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr) {
        int ret = mediaKeySystemNapi->keySystem_->SetConfiguration(IMediaKeySystemService::ConfigType(configType),
            name, value);
        if (ret != napi_ok) {
            DRM_ERR_LOG("napi SetConfiguration faild!");
            return nullptr;
        }
    } else {
        DRM_ERR_LOG("mediaKeySystemNapi SetConfiguration call Failed!");
        return nullptr;
    }

    DRM_INFO_LOG("MediaKeySystemNapi::SetConfiguration exit.");
    return result;
}

napi_value MediaKeySystemNapi::GetConfiguration(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySystemNapi::GetConfiguration enter");
    napi_value result = nullptr;
    size_t argc = ARGS_THREE;
    napi_value argv[ARGS_THREE] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    int32_t configType = 0;
    char nameStr[PATH_MAX];
    size_t nameStrLength = 0;
    std::string value;
    MediaKeySystemNapi* mediaKeySystemNapi = nullptr;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_THREE, "requires 3 parameters maximum");

    status = napi_get_value_int32(env, argv[PARAM0], &configType);
    if (status != napi_ok) {
        DRM_ERR_LOG("Could not able to read requestType argument!");
        return nullptr;
    }

    napi_get_value_string_utf8(env, argv[PARAM1], nameStr, PATH_MAX, &nameStrLength);
    std::string name = std::string(nameStr);
    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr) {
        int ret =
            mediaKeySystemNapi->keySystem_->GetConfiguration(IMediaKeySystemService::ConfigType(configType),
            name, value);
        if (ret != napi_ok) {
            DRM_ERR_LOG("napi GetConfiguration faild!");
            return nullptr;
        }
    } else {
        DRM_ERR_LOG("mediaKeySystemNapi Release call Failed!");
        return nullptr;
    }

    napi_create_string_utf8(env, value.c_str(), NAPI_AUTO_LENGTH, &result);
    DRM_INFO_LOG("MediaKeySystemNapi::GetConfiguration exit");
    return result;
}

napi_value MediaKeySystemNapi::GetSecurityLevel(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySystemNapi::GetSecurityLevel enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_ONE;
    napi_value argv[ARGS_ONE] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    MediaKeySystemNapi* mediaKeySystemNapi = nullptr;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);

    NAPI_ASSERT(env, argc <= ARGS_ONE, "requires 1 parameters maximum");
    IKeySessionService::SecurityLevel level = (IKeySessionService::SecurityLevel)0;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr) {
        mediaKeySystemNapi->keySystem_->GetSecurityLevel(&level);
    } else {
        DRM_ERR_LOG("MediaKeySystemNapi GetSecurityLevel call Failed!");
        return nullptr;
    }

    NAPI_CALL(env, napi_create_int32(env, (int)level, &result));
    DRM_INFO_LOG("MediaKeySystemNapi::GetSecurityLevel exit");
    return result;
}

napi_value MediaKeySystemNapi::GenerateKeySystemRequest(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySystemNapi::GenerateKeySystemRequest enter");
    napi_value result = nullptr;
    size_t argc = ARGS_THREE;
    napi_value argv[ARGS_THREE] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    int32_t requestType = 0;
    std::vector<uint8_t> request;
    std::string defaultUrl;
    napi_value keyRequestType;
    napi_value mDefaultURL;
    napi_value mData;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_THREE, "requires 3 parameters maximum");
    status = napi_get_value_int32(env, argv[PARAM0], &requestType);
    if (status != napi_ok) {
        DRM_ERR_LOG("Could not able to read requestType argument!");
        return nullptr;
    }
    MediaKeySystemNapi* mediaKeySystemNapi = nullptr;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr) {
        int ret =
            mediaKeySystemNapi->keySystem_->GenerateKeySystemRequest(IMediaKeySystemService::RequestType(requestType),
            request, defaultUrl);
        if (ret != napi_ok) {
            DRM_ERR_LOG("napi GenerateKeySystemRequest faild!");
            return nullptr;
        }
    } else {
        DRM_ERR_LOG("mediaKeySystemNapi GenerateKeySystemRequest call Failed!");
        return nullptr;
    }

    NAPI_CALL(env, napi_create_object(env, &result));
    NAPI_CALL(env, napi_create_int32(env, static_cast<int32_t>(requestType), &keyRequestType));
    size_t requestLen = request.size();
    NAPI_CALL(env, napi_create_array(env, &mData));
    for (size_t i = 0; i < requestLen; i++) {
        napi_value item;
        napi_create_int32(env, request[i], &item);
        napi_set_element(env, mData, i, item);
    }
    NAPI_CALL(env, napi_create_string_utf8(env, defaultUrl.c_str(), NAPI_AUTO_LENGTH, &mDefaultURL));

    NAPI_CALL(env, napi_set_named_property(env, result, "keyRequestType", keyRequestType));
    NAPI_CALL(env, napi_set_named_property(env, result, "mDefaultURL", mDefaultURL));
    NAPI_CALL(env, napi_set_named_property(env, result, "mData", mData));

    DRM_INFO_LOG("MediaKeySystemNapi::GenerateKeySystemRequest exit");
    return result;
}

napi_value MediaKeySystemNapi::ProcessKeySystemResponse(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySystemNapi::ProcessKeySystemResponse enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_TWO;
    napi_value argv[ARGS_TWO] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    int32_t requestType = 0;
    bool isTypeArray;
    void *reponseData = nullptr;
    size_t reponseDataLen;
    size_t offset;
    napi_value arraybuffer = nullptr;
    napi_typedarray_type type;
    MediaKeySystemNapi* mediaKeySystemNapi = nullptr;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_TWO, "requires 2 parameters maximum");
    status = napi_get_value_int32(env, argv[PARAM0], &requestType);
    if (status != napi_ok) {
        DRM_ERR_LOG("Could not able to read requestType argument!");
        return nullptr;
    }
    napi_is_typedarray(env, argv[PARAM1], &isTypeArray);
    if (!isTypeArray) {
        DRM_ERR_LOG("argv[PARAM1] reponse is not array!");
        return nullptr;
    }
    napi_get_typedarray_info(env, argv[PARAM1], &type, &reponseDataLen, &reponseData, &arraybuffer, &offset);
    if (reponseData == nullptr) {
        DRM_ERR_LOG("napi_get_typedarray_info faild!");
        return nullptr;
    }
    uint8_t* reponseDataPtr = reinterpret_cast<uint8_t*>(reponseData);
    std::vector<uint8_t> keySystemResponse(reponseDataPtr, reponseDataPtr + reponseDataLen);
    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr) {
        int ret =
            mediaKeySystemNapi->keySystem_->ProcessKeySystemResponse(IMediaKeySystemService::RequestType(requestType),
            keySystemResponse);
        if (ret != napi_ok) {
            DRM_ERR_LOG("napi ProcessKeySystemResponse faild!");
            return nullptr;
        }
    } else {
        DRM_ERR_LOG("mediaKeySystemNapi ProcessKeySystemResponse call Failed!");
        return nullptr;
    }

    DRM_INFO_LOG("MediaKeySystemNapi::ProcessKeySystemResponse exit.");
    return result;
}

static napi_value vectorToJsArray(napi_env env, std::vector<IMediaKeySystemService::KeyValue> &infoMap)
{
    DRM_INFO_LOG("vectorToJsArray enter.");
    napi_value jsArray;
    napi_create_array_with_length(env, infoMap.size(), &jsArray);
    for (size_t i = 0; i < infoMap.size(); i++) {
        napi_value jsObject;
        napi_value jsName;
        napi_value jsValue;
        napi_create_object(env, &jsObject);
        napi_create_string_utf8(env, infoMap[i].name.c_str(), NAPI_AUTO_LENGTH, &jsName);
        napi_set_named_property(env, jsObject, "name", jsName);
        napi_create_string_utf8(env, infoMap[i].value.c_str(), NAPI_AUTO_LENGTH, &jsValue);
        napi_set_named_property(env, jsObject, "value", jsValue);

        napi_set_element(env, jsArray, i, jsObject);
    }
    DRM_INFO_LOG("vectorToJsArray exit.");
    return jsArray;
}

napi_value MediaKeySystemNapi::GetMetric(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySystemNapi::GetMetric enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_ZERO;
    napi_value argv[ARGS_ZERO];
    napi_value thisVar = nullptr;
    napi_status status;
    std::vector<IMediaKeySystemService::KeyValue> infoMap;
    MediaKeySystemNapi* mediaKeySystemNapi = nullptr;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&mediaKeySystemNapi));
    if (status == napi_ok && mediaKeySystemNapi != nullptr) {
        int ret = mediaKeySystemNapi->keySystem_->GetMetric(infoMap);
        if (ret != napi_ok) {
            DRM_ERR_LOG("keySystem_->GetMetric faild!");
            return nullptr;
        }
    } else {
        DRM_ERR_LOG("mediaKeySystemNapi Release call Failed!");
        return nullptr;
    }
    result = vectorToJsArray(env, infoMap);
    DRM_INFO_LOG("MediaKeySystemNapi::GetMetric exit.");
    return result;
}
} // namespace DrmStandard
} // namespace OHOS
