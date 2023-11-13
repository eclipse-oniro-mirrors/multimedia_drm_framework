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
#include "key_session_napi.h"
#include "media_decrypt_module_napi.h"
#include "key_session_impl.h"
#include "media_key_system_impl.h"
#include "ipc_skeleton.h"
#include "i_keysession_service.h"

namespace OHOS {
namespace DrmStandard {
thread_local napi_ref KeySessionNapi::sConstructor_ = nullptr;
thread_local sptr<KeySessionImpl> KeySessionNapi::sKeySessionImpl_ = nullptr;

KeySessionNapi::KeySessionNapi() : env_(nullptr), wrapper_(nullptr)
{
    DRM_INFO_LOG("0x%{public}06" PRIXPTR " Instances create", FAKE_POINTER(this));
}

KeySessionNapi::~KeySessionNapi()
{
    DRM_INFO_LOG("KeySessionNapi::~KeySessionNapi Init enter.");
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " Instances destroy", FAKE_POINTER(this));
    if (wrapper_ != nullptr) {
        napi_delete_reference(env_, wrapper_);
    }
    if (keySessionImpl_) {
        keySessionImpl_ = nullptr;
    }
    DRM_INFO_LOG("KeySessionNapi::~KeySessionNapi Init exit.");
}

napi_value KeySessionNapi::Init(napi_env env, napi_value exports)
{
    DRM_INFO_LOG("KeySessionNapi::KeySessionNapi Init enter.");
    napi_status status;
    napi_value ctorObj;
    int32_t refCount = 1;

    napi_property_descriptor session_props[] = {
        DECLARE_NAPI_FUNCTION("release", Release),
        DECLARE_NAPI_FUNCTION("generateLicenseRequest", GenerateLicenseRequest),
        DECLARE_NAPI_FUNCTION("processLicenseResponse", ProcessLicenseResponse),
        DECLARE_NAPI_FUNCTION("generateOfflineReleaseRequest", GenerateOfflineReleaseRequest),
        DECLARE_NAPI_FUNCTION("processOfflineReleaseResponse", ProcessOfflineReleaseResponse),
        DECLARE_NAPI_FUNCTION("checkLicenseStatus", CheckLicenseStatus),
        DECLARE_NAPI_FUNCTION("restoreOfflineKeys", RestoreOfflineKeys),
        DECLARE_NAPI_FUNCTION("removeOfflineKeys", RemoveOfflineKeys),
        DECLARE_NAPI_FUNCTION("getOfflineKeyIds", GetOfflineKeyIds),
        DECLARE_NAPI_FUNCTION("removeLicenses", RemoveLicenses),
        DECLARE_NAPI_FUNCTION("getDecryptModule", GetDecryptModule),
        DECLARE_NAPI_FUNCTION("addEventListener", AddEventListener),
        DECLARE_NAPI_FUNCTION("deleteEventListener", DeleteEventListener),
        DECLARE_NAPI_FUNCTION("getOfflineKeyState", GetOfflineKeyState),
        DECLARE_NAPI_FUNCTION("on", SetEventCallback),
        DECLARE_NAPI_FUNCTION("off", UnsetEventCallback),
    };
    status = napi_define_class(env, KEY_SESSION_NAPI_CLASS_NAME, NAPI_AUTO_LENGTH,
                               KeySessionNapiConstructor, nullptr,
                               sizeof(session_props) / sizeof(session_props[PARAM0]),
                               session_props, &ctorObj);
    if (status == napi_ok) {
        status = napi_create_reference(env, ctorObj, refCount, &sConstructor_);
        if (status == napi_ok) {
            status = napi_set_named_property(env, exports, KEY_SESSION_NAPI_CLASS_NAME, ctorObj);
            if (status == napi_ok) {
                DRM_DEBUG_LOG("KeySessionNapi return exports");
                return exports;
            }
        }
    }
    DRM_INFO_LOG("KeySessionNapi::KeySessionNapi Init call Failed!");
    return nullptr;
}

napi_value KeySessionNapi::KeySessionNapiConstructor(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("KeySessionNapi::KeySessionNapiConstructor enter.");

    napi_status status;
    napi_value result = nullptr;
    napi_value thisVar = nullptr;

    napi_get_undefined(env, &result);
    DRM_NAPI_GET_JS_OBJ_WITH_ZERO_ARGS(env, info, status, thisVar);

    if (status == napi_ok && thisVar != nullptr) {
        std::unique_ptr<KeySessionNapi> obj = std::make_unique<KeySessionNapi>();
        if (obj != nullptr) {
            obj->env_ = env;
            if (sKeySessionImpl_ == nullptr) {
                DRM_ERR_LOG("sKeySessionImpl_ is null");
                return result;
            }
            obj->keySessionImpl_ = sKeySessionImpl_;
            obj->keySessionCallbackNapi_ = new KeySessionCallbackNapi();
            obj->keySessionImpl_->SetKeySessionCallback(obj->keySessionCallbackNapi_);
            status = napi_wrap(env, thisVar, reinterpret_cast<void*>(obj.get()),
                               KeySessionNapi::KeySessionNapiDestructor, nullptr, nullptr);
            if (status == napi_ok) {
                obj.release();
                const std::string propertyName = "KeySessionNative";
                SetKeySessionNativeProperty(env, thisVar, propertyName, sKeySessionImpl_);
                return thisVar;
            } else {
                DRM_ERR_LOG("KeySessionNapi Failure wrapping js to native napi");
            }
        }
    }
    DRM_INFO_LOG("KeySessionNapi::KeySessionNapiConstructor faild, exit.");
    return result;
}

void KeySessionNapi::KeySessionNapiDestructor(napi_env env, void *nativeObject, void *finalize)
{
    DRM_INFO_LOG("KeySessionNapi::KeySessionNapiDestructor enter.");
    KeySessionNapi* keySessionNapiObj = reinterpret_cast<KeySessionNapi*>(nativeObject);
    if (keySessionNapiObj != nullptr) {
        keySessionNapiObj->~KeySessionNapi();
    }
    DRM_INFO_LOG("KeySessionNapi::KeySessionNapiDestructor exit.");
}

bool KeySessionNapi::SetKeySessionNativeProperty(napi_env env, napi_value obj, const std::string &name,
    sptr<KeySessionImpl> keySessionImpl)
{
    DRM_INFO_LOG("KeySessionNapi::SetKeySessionNativeProperty enter.");
    CHECK_AND_RETURN_RET_LOG(obj != nullptr, false, "obj is nullptr");

    napi_value keySessionImplNative = nullptr;
    int64_t nativePointer = reinterpret_cast<int64_t>(keySessionImpl.GetRefPtr());
    DRM_DEBUG_LOG("SetKeySessionNativeProperty get nativePointer:%{public}lld", nativePointer);
    napi_status status = napi_create_int64(env, nativePointer, &keySessionImplNative);
    CHECK_AND_RETURN_RET_LOG(status == napi_ok, false, "create int failed");

    napi_value nameStr = nullptr;
    status = napi_create_string_utf8(env, name.c_str(), NAPI_AUTO_LENGTH, &nameStr);
    CHECK_AND_RETURN_RET_LOG(status == napi_ok, false, "create string failed");

    status = napi_set_property(env, obj, nameStr, keySessionImplNative);
    CHECK_AND_RETURN_RET_LOG(status == napi_ok, false, "set property failed");

    DRM_INFO_LOG("KeySessionNapi::SetKeySessionNativeProperty exit.");
    return true;
}

napi_value KeySessionNapi::CreateKeySession(napi_env env, sptr<KeySessionImpl> keySessionImpl)
{
    DRM_INFO_LOG("KeySessionNapi::CreateKeySession enter.");
    napi_status status;
    napi_value result = nullptr;
    napi_value constructor;

    status = napi_get_reference_value(env, sConstructor_, &constructor);
    if (status == napi_ok) {
        sKeySessionImpl_ = keySessionImpl;
        if (sKeySessionImpl_ == nullptr) {
            DRM_ERR_LOG("Failed to CreateKeySession instance");
            napi_get_undefined(env, &result);
            return result;
        }
        status = napi_new_instance(env, constructor, 0, nullptr, &result);
        sKeySessionImpl_ = nullptr;
        if (status == napi_ok && result != nullptr) {
            DRM_INFO_LOG("success to CreateKeySession napi instance");
            return result;
        } else {
            DRM_ERR_LOG("Failed to CreateKeySession napi instance");
        }
    }
    DRM_DEBUG_LOG("Failed to create CreateKeySession napi instance last");
    napi_get_undefined(env, &result);
    DRM_INFO_LOG("KeySessionNapi::CreateKeySession exit.");
    return result;
}

napi_value KeySessionNapi::Release(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("KeySessionNapi::Release enter.");
    int32_t currentPid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("KeySessionNapi GetCallingPID: %{public}d", currentPid);
    napi_status status;
    napi_value result = nullptr;
    size_t argc = ARGS_ZERO;
    napi_value argv[ARGS_ZERO];
    napi_value thisVar = nullptr;
    KeySessionNapi* keySessionNapi = nullptr;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);

    napi_get_undefined(env, &result);
    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr) {
        keySessionNapi->keySessionImpl_->Release();
    } else {
        DRM_ERR_LOG("KeySessionNapi Release call Failed!");
    }
    DRM_INFO_LOG("KeySessionNapi::Release exit.");
    return result;
}

napi_value KeySessionNapi::GenerateLicenseRequest(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("KeySessionNapi::GenerateLicenseRequest enter");
    napi_value result = nullptr;
    size_t argc = ARGS_THREE;
    napi_value argv[ARGS_THREE] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    char buffer[PATH_MAX];
    size_t length = 0;
    bool isTypeArray;
    void *initData = nullptr;
    size_t initDataLen;
    size_t offset;
    napi_value arraybuffer = nullptr;
    napi_typedarray_type type;
    napi_value requestType;
    napi_value mData;
    napi_value mDefaultURL;
    const char *requestTypeEnum;
    IKeySessionService::DrmInfo drmInfo;
    IKeySessionService::LicenseInfo licenseInfo;
    KeySessionNapi* keySessionNapi = nullptr;

    napi_create_object(env, &result);
    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_THREE, "requires 3 parameters maximum");

    if (napi_get_value_string_utf8(env, argv[PARAM0], buffer, PATH_MAX, &length) != napi_ok) {
        DRM_ERR_LOG("Could not able to read mimeType argument!");
        return nullptr;
    }
    napi_is_typedarray(env, argv[PARAM1], &isTypeArray);
    if (!isTypeArray) {
        DRM_ERR_LOG("argv[PARAM1] is not array!");
        return nullptr;
    }
    napi_get_typedarray_info(env, argv[PARAM1], &type, &initDataLen, &initData, &arraybuffer, &offset);
    if (initData == nullptr) {
        DRM_ERR_LOG("napi_get_typedarray_info faild!");
        return nullptr;
    }
    uint8_t* initDataPtr = reinterpret_cast<uint8_t*>(initData);
    std::vector<uint8_t> initDataStr(initDataPtr, initDataPtr + initDataLen);
    int32_t keyType = 0;
    status = napi_get_value_int32(env, argv[PARAM2], &keyType);
    if (status != napi_ok) {
        DRM_ERR_LOG("-Could not able to read keyType argument!");
        return nullptr;
    }
    drmInfo.keyType = (IKeySessionService::KeyType)keyType;
    drmInfo.mimeType = std::string(buffer);
    drmInfo.indexInfo = initDataStr;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr) {
        keySessionNapi->keySessionImpl_->GenerateLicenseRequest(drmInfo, licenseInfo);
    } else {
        DRM_ERR_LOG("KeySessionNapi GenerateLicenseRequest call Failed!");
    }

    if (licenseInfo.requestType == IKeySessionService::REQUEST_TYPE_UNKNOWN) {
        requestTypeEnum = "REQUEST_TYPE_UNKNOWN";
    } else if (licenseInfo.requestType == IKeySessionService::REQUEST_TYPE_INITIAL) {
        requestTypeEnum = "REQUEST_TYPE_INITIAL";
    } else if (licenseInfo.requestType == IKeySessionService::REQUEST_TYPE_RENEWAL) {
        requestTypeEnum = "REQUEST_TYPE_RENEWAL";
    } else if (licenseInfo.requestType == IKeySessionService::REQUEST_TYPE_RELEASE) {
        requestTypeEnum = "REQUEST_TYPE_RELEASE";
    } else if (licenseInfo.requestType == IKeySessionService::REQUEST_TYPE_NONE) {
        requestTypeEnum = "REQUEST_TYPE_NONE";
    } else if (licenseInfo.requestType == IKeySessionService::REQUEST_TYPE_UPDATE) {
        requestTypeEnum = "REQUEST_TYPE_UPDATE";
    } else if (licenseInfo.requestType == IKeySessionService::REQUEST_TYPE_DOWNLOADCERT) {
        requestTypeEnum = "REQUEST_TYPE_DOWNLOADCERT";
    }
    napi_create_string_utf8(env, requestTypeEnum, NAPI_AUTO_LENGTH, &requestType);
    napi_set_named_property(env, result, "requestType", requestType);

    size_t mDataLen = licenseInfo.mData.size();
    NAPI_CALL(env, napi_create_array(env, &mData));
    for (size_t i = 0; i < mDataLen; i++) {
        napi_value item;
        napi_create_int32(env, licenseInfo.mData[i], &item);
        napi_set_element(env, mData, i, item);
    }
    napi_set_named_property(env, result, "mData", mData);

    napi_create_string_utf8(env, licenseInfo.mDefaultURL.c_str(), NAPI_AUTO_LENGTH, &mDefaultURL);
    napi_set_named_property(env, result, "mDefaultURL", mDefaultURL);

    DRM_INFO_LOG("KeySessionNapi::GenerateLicenseRequest exit");
    return result;
}

napi_value KeySessionNapi::ProcessLicenseResponse(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("KeySessionNapi::ProcessLicenseResponse enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_ONE;
    napi_value argv[ARGS_ONE] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    bool isTypeArray;
    void *reponseData = nullptr;
    size_t reponseDataLen;
    size_t offset;
    napi_value arraybuffer = nullptr;
    napi_typedarray_type type;
    std::vector<uint8_t> keyId;
    KeySessionNapi* keySessionNapi = nullptr;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_ONE, "requires 1 parameters maximum");

    napi_is_typedarray(env, argv[PARAM0], &isTypeArray);
    if (!isTypeArray) {
        DRM_ERR_LOG("argv[PARAM0] is not array!");
        return nullptr;
    }

    napi_get_typedarray_info(env, argv[PARAM0], &type, &reponseDataLen, &reponseData, &arraybuffer,
        &offset);
    if (reponseData == nullptr) {
        DRM_ERR_LOG("napi_get_typedarray_info faild!");
        return nullptr;
    }
    uint8_t* reponseDataPtr = reinterpret_cast<uint8_t*>(reponseData);
    std::vector<uint8_t> licenseResponse(reponseDataPtr, reponseDataPtr + reponseDataLen);

    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr) {
        int ret = keySessionNapi->keySessionImpl_->ProcessLicenseResponse(keyId, licenseResponse);
        if (ret != napi_ok) {
            DRM_ERR_LOG("napi ProcessLicenseResponse faild!");
            return nullptr;
        }
        size_t keyIdLen = keyId.size();
        NAPI_CALL(env, napi_create_array(env, &result));
        for (size_t i = 0; i < keyIdLen; i++) {
            napi_value item;
            napi_create_int32(env, keyId[i], &item);
            napi_set_element(env, result, i, item);
        }
    } else {
        DRM_ERR_LOG("KeySessionNapi ProcessLicenseResponse call Failed!");
    }

    DRM_INFO_LOG("KeySessionNapi::ProcessLicenseResponse exit.");
    return result;
}

napi_value KeySessionNapi::GenerateOfflineReleaseRequest(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("KeySessionNapi::GenerateOfflineReleaseRequest enter");
    napi_value result = nullptr;
    size_t argc = ARGS_ONE;
    napi_value argv[ARGS_ONE] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    bool isTypeArray;
    void *keyId = nullptr;
    size_t keyIdLen;
    size_t offset;
    napi_value arraybuffer = nullptr;
    napi_typedarray_type type;
    std::vector<uint8_t> releaseRequest;
    KeySessionNapi* keySessionNapi = nullptr;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_ONE, "requires 1 parameters maximum");

    napi_is_typedarray(env, argv[PARAM0], &isTypeArray);
    if (!isTypeArray) {
        DRM_ERR_LOG("argv[PARAM0] is not array!");
        return nullptr;
    }
    napi_get_typedarray_info(env, argv[PARAM0], &type, &keyIdLen, &keyId, &arraybuffer, &offset);
    if (keyId == nullptr) {
        DRM_ERR_LOG("napi_get_typedarray_info faild!");
        return nullptr;
    }
    uint8_t* keyIdPtr = reinterpret_cast<uint8_t*>(keyId);
    std::vector<uint8_t> keyIdVec(keyIdPtr, keyIdPtr + keyIdLen);

    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr) {
        int ret = keySessionNapi->keySessionImpl_->GenerateOfflineReleaseRequest(keyIdVec,
            releaseRequest);
        if (ret != napi_ok) {
            DRM_ERR_LOG("napi GenerateOfflineReleaseRequest faild!");
            return nullptr;
        }
    } else {
        DRM_ERR_LOG("KeySessionNapi ProcessLicenseResponse call Failed!");
    }

    size_t releaseRequestLen = releaseRequest.size();
    NAPI_CALL(env, napi_create_array(env, &result));
    for (size_t i = 0; i < releaseRequestLen; i++) {
        napi_value item;
        napi_create_int32(env, releaseRequest[i], &item);
        napi_set_element(env, result, i, item);
    }

    DRM_INFO_LOG("KeySessionNapi::GenerateOfflineReleaseRequest exit");
    return result;
}

napi_value KeySessionNapi::ProcessOfflineReleaseResponse(napi_env env, napi_callback_info info)
{
    napi_value result = nullptr;
    size_t argc = ARGS_TWO;
    napi_value argv[ARGS_TWO] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    bool isTypeArray;
    void *keyId = nullptr;
    size_t keyIdLen;
    size_t offset;
    napi_value arraybuffer = nullptr;
    napi_typedarray_type type;
    void *response = nullptr;
    size_t responseLen;
    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_TWO, "requires 2 parameters maximum");
    napi_is_typedarray(env, argv[PARAM0], &isTypeArray);
    if (!isTypeArray) {
        DRM_ERR_LOG("argv[PARAM0] is not array!");
        return nullptr;
    }
    napi_get_typedarray_info(env, argv[PARAM0], &type, &keyIdLen, &keyId, &arraybuffer, &offset);
    if (keyId == nullptr) {
        DRM_ERR_LOG("napi_get_typedarray_info faild!");
        return nullptr;
    }
    uint8_t* keyIdPtr = reinterpret_cast<uint8_t*>(keyId);
    std::vector<uint8_t> keyIdVec(keyIdPtr, keyIdPtr + keyIdLen);
    napi_is_typedarray(env, argv[PARAM1], &isTypeArray);
    if (!isTypeArray) {
        DRM_ERR_LOG("argv[PARAM1] is not array!");
        return nullptr;
    }
    napi_get_typedarray_info(env, argv[PARAM1], &type, &responseLen, &response, &arraybuffer, &offset);
    uint8_t* responsePtr = reinterpret_cast<uint8_t*>(response);
    std::vector<uint8_t> responseVec(responsePtr, responsePtr + responseLen);
    KeySessionNapi* keySessionNapi = nullptr;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&keySessionNapi));
    if (status != napi_ok || keySessionNapi == nullptr) {
        return nullptr;
    }
    int ret = keySessionNapi->keySessionImpl_->ProcessOfflineReleaseResponse(keyIdVec, responseVec);
    if (ret != napi_ok) {
        DRM_ERR_LOG("napi ProcessOfflineReleaseResponse faild!");
        return nullptr;
    }
    return result;
}

static napi_value vectorToJsArray(napi_env env, std::vector<IKeySessionService::KeyValue> &infoMap)
{
    DRM_INFO_LOG("vectorToJsArray enter.");
    napi_value jsArray;
    napi_value jsName;
    napi_value jsValue;
    napi_create_array_with_length(env, infoMap.size(), &jsArray);
    for (size_t i = 0; i < infoMap.size(); i++) {
        napi_value jsObject;
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

napi_value KeySessionNapi::CheckLicenseStatus(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("KeySessionNapi::CheckLicenseStatus enter");
    napi_value result = nullptr;
    size_t argc = ARGS_ZERO;
    napi_value argv[ARGS_ZERO];
    napi_value thisVar = nullptr;
    napi_status status;
    KeySessionNapi* keySessionNapi = nullptr;
    std::vector<IKeySessionService::KeyValue> infoMap;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    
    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr) {
        int ret = keySessionNapi->keySessionImpl_->CheckLicenseStatus(infoMap);
        if (ret != napi_ok) {
            DRM_ERR_LOG("napi CheckLicenseStatus faild!");
            return nullptr;
        }
    } else {
        DRM_ERR_LOG("KeySessionNapi CheckLicenseStatus call Failed!");
    }

    result = vectorToJsArray(env, infoMap);
    DRM_INFO_LOG("KeySessionNapi::CheckLicenseStatus exit.");
    return result;
}

napi_value KeySessionNapi::RestoreOfflineKeys(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("KeySessionNapi::RestoreOfflineKeys enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_ONE;
    napi_value argv[ARGS_ONE] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    bool isTypeArray;
    void *keyId = nullptr;
    size_t keyIdLen;
    size_t offset;
    napi_value arraybuffer = nullptr;
    napi_typedarray_type type;
    KeySessionNapi* keySessionNapi = nullptr;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_ONE, "requires 1 parameters maximum");

    napi_is_typedarray(env, argv[PARAM0], &isTypeArray);
    if (!isTypeArray) {
        DRM_ERR_LOG("argv[PARAM0] is not array!");
        return nullptr;
    }
    napi_get_typedarray_info(env, argv[PARAM0], &type, &keyIdLen, &keyId, &arraybuffer, &offset);
    if (keyId == nullptr) {
        DRM_ERR_LOG("napi_get_typedarray_info faild!");
        return nullptr;
    }
    uint8_t* keyIdPtr = reinterpret_cast<uint8_t*>(keyId);
    std::vector<uint8_t> keyIdVec(keyIdPtr, keyIdPtr + keyIdLen);
    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr) {
        int ret = keySessionNapi->keySessionImpl_->RestoreOfflineKeys(keyIdVec);
        if (ret != napi_ok) {
            DRM_ERR_LOG("napi RestoreOfflineKeys faild!");
            return nullptr;
        }
    } else {
        DRM_ERR_LOG("KeySessionNapi RestoreOfflineKeys call Failed!");
    }

    DRM_INFO_LOG("KeySessionNapi::RestoreOfflineKeys exit.");
    return result;
}

napi_value KeySessionNapi::RemoveOfflineKeys(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("KeySessionNapi::RemoveOfflineKeys enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_ONE;
    napi_value argv[ARGS_ONE] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    bool isTypeArray;
    void *keyId = nullptr;
    size_t keyIdLen;
    size_t offset;
    napi_value arraybuffer = nullptr;
    napi_typedarray_type type;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_ONE, "requires 2 parameters maximum");
    napi_is_typedarray(env, argv[PARAM0], &isTypeArray);
    if (!isTypeArray) {
        DRM_ERR_LOG("argv[PARAM0] is not array!");
        return nullptr;
    }
    napi_get_typedarray_info(env, argv[PARAM0], &type, &keyIdLen, &keyId, &arraybuffer, &offset);
    if (keyId == nullptr) {
        DRM_ERR_LOG("napi_get_typedarray_info faild!");
        return nullptr;
    }
    uint8_t* keyIdPtr = reinterpret_cast<uint8_t*>(keyId);
    std::vector<uint8_t> keyIdVec(keyIdPtr, keyIdPtr + keyIdLen);
    KeySessionNapi* keySessionNapi = nullptr;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr) {
        int ret = keySessionNapi->keySessionImpl_->RemoveOfflineKeys(keyIdVec);
        if (ret != napi_ok) {
            DRM_ERR_LOG("napi RemoveOfflineKeys faild!");
            return nullptr;
        }
    } else {
        DRM_ERR_LOG("KeySessionNapi RemoveOfflineKeys call Failed!");
    }

    DRM_INFO_LOG("KeySessionNapi::RemoveOfflineKeys exit.");
    return result;
}

static napi_value vectorToJs2DArray(napi_env env, std::vector<std::vector<uint8_t>> vec)
{
    DRM_INFO_LOG("vectorToJs2DArray enter.");
    napi_value outArray;
    napi_value inArray;
    napi_status status = napi_create_array(env, &outArray);
    for (size_t i = 0; i < vec.size(); i++) {
        status = napi_create_array(env, &inArray);
        for (size_t j = 0; j < vec[i].size(); j++) {
            napi_value elem;
            status = napi_create_uint32(env, vec[i][j], &elem);
            status = napi_set_element(env, inArray, j, elem);
        }
        status = napi_set_element(env, outArray, i, inArray);
    }
    DRM_INFO_LOG("vectorToJs2DArray exit.");
    return outArray;
}

napi_value KeySessionNapi::GetOfflineKeyIds(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("KeySessionNapi::GetOfflineKeyIds enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_ZERO;
    napi_value argv[ARGS_ZERO];
    napi_value thisVar = nullptr;
    napi_status status;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc >= ARGS_ZERO, "requires 0 parameters maximum");
    std::vector<std::vector<uint8_t>> keyIds;
    KeySessionNapi* keySessionNapi = nullptr;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr) {
        int ret = keySessionNapi->keySessionImpl_->GetOfflineKeyIds(keyIds);
        if (ret != napi_ok) {
            DRM_ERR_LOG("napi GetOfflineKeyIds faild!");
            return nullptr;
        }
    } else {
        DRM_ERR_LOG("KeySessionNapi GetOfflineKeyIds call Failed!");
    }

    result = vectorToJs2DArray(env, keyIds);
    DRM_INFO_LOG("KeySessionNapi::GetOfflineKeyIds exit.");
    return result;
}

napi_value KeySessionNapi::RemoveLicenses(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("KeySessionNapi::RemoveLicenses enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_ZERO;
    napi_value argv[ARGS_ZERO];
    napi_value thisVar = nullptr;
    napi_status status;
    int32_t currentPid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("KeySessionNapi GetCallingPID: %{public}d", currentPid);

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    KeySessionNapi* keySessionNapi = nullptr;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr) {
        int ret = keySessionNapi->keySessionImpl_->RemoveLicenses();
        if (ret != napi_ok) {
            DRM_ERR_LOG("napi RemoveLicenses faild!");
            return nullptr;
        }
    } else {
        DRM_ERR_LOG("KeySessionNapi RemoveLicenses call Failed!");
    }

    DRM_INFO_LOG("KeySessionNapi::RemoveLicenses exit.");
    return result;
}

napi_value KeySessionNapi::GetDecryptModule(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("KeySessionNapi::GetDecryptModule enter.");
    napi_status status;
    napi_value result = nullptr;
    size_t argc = ARGS_ZERO;
    napi_value argv[ARGS_ZERO];
    napi_value thisVar = nullptr;
    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    napi_get_undefined(env, &result);
    KeySessionNapi* keySessionNapi = nullptr;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&keySessionNapi));
    sptr<MediaDecryptModuleImpl> mediaDecryptModuleImpl = nullptr;
    if (status == napi_ok && keySessionNapi != nullptr) {
        mediaDecryptModuleImpl = keySessionNapi->keySessionImpl_->GetDecryptModule();
        if (mediaDecryptModuleImpl == nullptr) {
            DRM_ERR_LOG("KeySessionNapi mediaDecryptModuleImpl get failed!!!");
            return nullptr;
        }
    } else {
        DRM_ERR_LOG("KeySessionNapi RemoveLicenses call Failed!");
    }

    result = MediaDecryptModuleNapi::GetDecryptModule(env, mediaDecryptModuleImpl);
    DRM_INFO_LOG("KeySessionNapi::GetDecryptModule exit.");
    return result;
}

napi_value KeySessionNapi::GetOfflineKeyState(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("KeySessionNapi::GetOfflineKeyState enter");
    napi_value result = nullptr;
    size_t argc = ARGS_ONE;
    napi_value argv[ARGS_ONE] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    void *keyId = nullptr;
    size_t keyIdLen;
    size_t offset;
    napi_value arraybuffer = nullptr;
    napi_typedarray_type type;
    KeySessionNapi* keySessionNapi = nullptr;
    bool isTypeArray;
    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_ONE, "requires 1 parameters maximum");
    napi_is_typedarray(env, argv[PARAM0], &isTypeArray);
    if (!isTypeArray) {
        DRM_ERR_LOG("argv[PARAM0] is not array!");
        return nullptr;
    }
    napi_get_typedarray_info(env, argv[PARAM0], &type, &keyIdLen, &keyId, &arraybuffer, &offset);
    if (keyId == nullptr) {
        DRM_ERR_LOG("napi_get_typedarray_info faild!");
        return nullptr;
    }
    uint8_t* keyIdPtr = reinterpret_cast<uint8_t*>(keyId);
    std::vector<uint8_t> keyIdVec(keyIdPtr, keyIdPtr + keyIdLen);
    IKeySessionService::OfflineKeyState offlineKeyState = IKeySessionService::OFFLINEKEYSTATE_UNKNOWN;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr) {
        int ret = keySessionNapi->keySessionImpl_->GetOfflineKeyState(keyIdVec, offlineKeyState);
        if (ret != napi_ok) {
            DRM_ERR_LOG("napi GetOfflineKeyState faild!");
            return nullptr;
        }
    } else {
        DRM_ERR_LOG("KeySessionNapi GetOfflineKeyState call Failed!");
        return nullptr;
    }

    NAPI_CALL(env, napi_create_int32(env, (int)offlineKeyState, &result));
    DRM_INFO_LOG("KeySessionNapi::GetOfflineKeyState exit");
    return result;
}


napi_value KeySessionNapi::AddEventListener(napi_env env, napi_callback_info info)
{
    napi_value result = nullptr;
    return result;
}

napi_value KeySessionNapi::DeleteEventListener(napi_env env, napi_callback_info info)
{
    napi_value result = nullptr;
    return result;
}

void KeySessionNapi::SetEventCallbackReference(const std::string eventType,
    sptr<CallBackPair> callbackPair)
{
    DRM_INFO_LOG("KeySessionNapi::SetCallbackReference");
    std::lock_guard<std::mutex> lock(mutex_);
    if (keySessionCallbackNapi_ != nullptr) {
        keySessionCallbackNapi_->SetCallbackReference(eventType, callbackPair);
    } else {
        DRM_ERR_LOG("KeySessionNapi::SetCallbackReference failed");
    }
}

void KeySessionNapi::ClearEventCallbackReference(const std::string eventType)
{
    DRM_INFO_LOG("KeySessionNapi::ClearEventCallbackReference");
    if (keySessionCallbackNapi_ != nullptr) {
        keySessionCallbackNapi_->ClearCallbackReference(eventType);
    } else {
        DRM_ERR_LOG("KeySessionNapi::ClearEventCallbackReference failed");
    }
}

napi_value KeySessionNapi::SetEventCallback(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("KeySessionNapi::SetEventCallback");
    napi_value result = nullptr;
    napi_get_undefined(env, &result);
    char buffer[PATH_MAX];
    size_t length = 0;
    size_t argc = ARGS_TWO;
    napi_value thisVar = nullptr;
    napi_value argv[ARGS_TWO] = { nullptr };
    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc == ARGS_TWO, "only requires 2 parameters");
    if (thisVar == nullptr || argv[PARAM0] == nullptr || argv[PARAM1] == nullptr) {
        DRM_ERR_LOG("Failed to retrieve arguments in SetEventCallback!");
        return result;
    }
    napi_valuetype valueType = napi_undefined;
    if (napi_typeof(env, argv[PARAM0], &valueType) != napi_ok || valueType != napi_string ||
        napi_typeof(env, argv[PARAM1], &valueType) != napi_ok || valueType != napi_function) {
        return result;
    }

    KeySessionNapi* keySessionNapi = nullptr;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr) {
        napi_get_value_string_utf8(env, argv[PARAM0], buffer, PATH_MAX, &length);
        std::string eventType = std::string(buffer);
        napi_ref callbackRef;
        napi_create_reference(env, argv[PARAM1], 1, &callbackRef);
        DRM_INFO_LOG("SetEventCallback event is %{public}s", eventType.c_str());

        sptr<CallBackPair> callbackPair = new CallBackPair(env, callbackRef);
        keySessionNapi->SetEventCallbackReference(eventType, callbackPair);
        DRM_INFO_LOG("KeySessionNapi::SetEventCallback out");
    } else {
        DRM_ERR_LOG("KeySessionNapi SetEventCallback failed!");
    }
    return result;
}

napi_value KeySessionNapi::UnsetEventCallback(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("KeySessionNapi::UnsetEventCallback");
    napi_value result = nullptr;
    napi_get_undefined(env, &result);
    napi_value thisVar = nullptr;
    napi_value argv[1] = { nullptr };
    size_t argc = 1;
    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc == ARGS_ONE, "only requires 1 parameters");
    if (thisVar == nullptr || argv[PARAM0] == nullptr) {
        DRM_ERR_LOG("Failed to retrieve arguments in UnsetEventCallback!");
        return result;
    }
    napi_valuetype valueType = napi_undefined;
    if (napi_typeof(env, argv[PARAM0], &valueType) != napi_ok || valueType != napi_string) {
        DRM_ERR_LOG("Failed to retrieve reasonable arguments in UnsetEventCallback!");
        return result;
    }

    KeySessionNapi* keySessionNapi = nullptr;
    char buffer[PATH_MAX];
    size_t length = 0;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr) {
        napi_get_value_string_utf8(env, argv[PARAM0], buffer, PATH_MAX, &length);
        std::string eventType = std::string(buffer);
        keySessionNapi->ClearEventCallbackReference(eventType);
        DRM_INFO_LOG("KeySessionNapi::UnsetEventCallback out");
    } else {
        DRM_ERR_LOG("KeySessionNapi UnsetEventCallback failed!");
    }
    return result;
}
} // DrmStandardr
} // OHOS