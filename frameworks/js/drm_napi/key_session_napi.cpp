/*
 * Copyright (c) 2023  Huawei Device Co., Ltd.
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
#include "ipc_skeleton.h"
#include "drm_napi_utils.h"
#include "key_session_impl.h"
#include "media_key_system_impl.h"
#include "key_session_napi.h"

namespace OHOS {
namespace DrmStandard {
thread_local napi_ref MediaKeySessionNapi::sConstructor_ = nullptr;
thread_local sptr<MediaKeySessionImpl> MediaKeySessionNapi::sMediaKeySessionImpl_ = nullptr;

MediaKeySessionNapi::MediaKeySessionNapi() : env_(nullptr), wrapper_(nullptr)
{
    DRM_INFO_LOG("0x%{public}06" PRIXPTR " Instances create", FAKE_POINTER(this));
}

MediaKeySessionNapi::~MediaKeySessionNapi()
{
    DRM_INFO_LOG("MediaKeySessionNapi::~MediaKeySessionNapi Init enter.");
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " Instances destroy", FAKE_POINTER(this));
    if (wrapper_ != nullptr) {
        napi_delete_reference(env_, wrapper_);
    }
    if (keySessionImpl_) {
        keySessionImpl_ = nullptr;
    }
    DRM_INFO_LOG("MediaKeySessionNapi::~MediaKeySessionNapi Init exit.");
}

napi_value MediaKeySessionNapi::Init(napi_env env, napi_value exports)
{
    DRM_INFO_LOG("MediaKeySessionNapi::MediaKeySessionNapi Init enter.");
    napi_status status;
    napi_value ctorObj;

    napi_property_descriptor session_props[] = {
        DECLARE_NAPI_FUNCTION("generateMediaKeyRequest", GenerateMediaKeyRequest),
        DECLARE_NAPI_FUNCTION("processMediaKeyResponse", ProcessMediaKeyResponse),
        DECLARE_NAPI_FUNCTION("generateOfflineReleaseRequest", GenerateOfflineReleaseRequest),
        DECLARE_NAPI_FUNCTION("processOfflineReleaseResponse", ProcessOfflineReleaseResponse),
        DECLARE_NAPI_FUNCTION("checkMediaKeyStatus", CheckMediaKeyStatus),
        DECLARE_NAPI_FUNCTION("restoreOfflineMediaKey", RestoreOfflineMediaKeys),
        DECLARE_NAPI_FUNCTION("clearMediaKeys", ClearMediaKeys),
        DECLARE_NAPI_FUNCTION("getContentProtectionLevel", GetContentProtectionLevel),
        DECLARE_NAPI_FUNCTION("requireSecureDecoderModule", RequireSecureDecoderModule),
        DECLARE_NAPI_FUNCTION("destroy", Destroy),
        DECLARE_NAPI_FUNCTION("on", SetEventCallback),
        DECLARE_NAPI_FUNCTION("off", UnsetEventCallback),
    };
    status = napi_define_class(env, KEY_SESSION_NAPI_CLASS_NAME, NAPI_AUTO_LENGTH, MediaKeySessionNapiConstructor,
        nullptr, sizeof(session_props) / sizeof(session_props[PARAM0]), session_props, &ctorObj);
    if (status == napi_ok) {
        status = napi_create_reference(env, ctorObj, 1, &sConstructor_);
        if (status == napi_ok) {
            status = napi_set_named_property(env, exports, KEY_SESSION_NAPI_CLASS_NAME, ctorObj);
            if (status == napi_ok) {
                DRM_DEBUG_LOG("MediaKeySessionNapi return exports");
                return exports;
            }
        }
    }
    DRM_INFO_LOG("MediaKeySessionNapi::MediaKeySessionNapi Init call Failed!");
    return nullptr;
}

napi_value MediaKeySessionNapi::MediaKeySessionNapiConstructor(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySessionNapi::MediaKeySessionNapiConstructor enter.");

    napi_status status;
    napi_value result = nullptr;
    napi_value thisVar = nullptr;

    napi_get_undefined(env, &result);
    DRM_NAPI_GET_JS_OBJ_WITH_ZERO_ARGS(env, info, status, thisVar);

    if (status == napi_ok && thisVar != nullptr) {
        std::unique_ptr<MediaKeySessionNapi> obj = std::make_unique<MediaKeySessionNapi>();
        if (obj != nullptr) {
            obj->env_ = env;
            if (sMediaKeySessionImpl_ == nullptr) {
                DRM_ERR_LOG("sMediaKeySessionImpl_ is null");
                return result;
            }
            obj->keySessionImpl_ = sMediaKeySessionImpl_;
            obj->keySessionCallbackNapi_ = new MediaKeySessionCallbackNapi();
            obj->keySessionImpl_->SetCallback(obj->keySessionCallbackNapi_);

            status = napi_wrap(env, thisVar, reinterpret_cast<void *>(obj.get()),
                MediaKeySessionNapi::MediaKeySessionNapiDestructor, nullptr, nullptr);
            if (status == napi_ok) {
                obj.release();
                const std::string propertyName = "MediaKeySessionNative";
                SetMediaKeySessionNativeProperty(env, thisVar, propertyName, sMediaKeySessionImpl_);
                return thisVar;
            } else {
                DRM_ERR_LOG("MediaKeySessionNapi Failure wrapping js to native napi");
            }
        }
    }
    DRM_INFO_LOG("MediaKeySessionNapi::MediaKeySessionNapiConstructor faild, exit.");
    return result;
}

void MediaKeySessionNapi::MediaKeySessionNapiDestructor(napi_env env, void *nativeObject, void *finalize)
{
    DRM_INFO_LOG("MediaKeySessionNapi::MediaKeySessionNapiDestructor enter.");
    MediaKeySessionNapi *keySessionNapiObj = reinterpret_cast<MediaKeySessionNapi *>(nativeObject);
    if (keySessionNapiObj != nullptr) {
        keySessionNapiObj->~MediaKeySessionNapi();
        keySessionNapiObj = nullptr;
    }
    DRM_INFO_LOG("MediaKeySessionNapi::MediaKeySessionNapiDestructor exit.");
}

bool MediaKeySessionNapi::SetMediaKeySessionNativeProperty(napi_env env, napi_value obj, const std::string &name,
    sptr<MediaKeySessionImpl> keySessionImpl)
{
    DRM_INFO_LOG("MediaKeySessionNapi::SetMediaKeySessionNativeProperty enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(obj != nullptr, false, "obj is nullptr");

    napi_value keySessionImplNative = nullptr;
    int64_t nativePointer = reinterpret_cast<int64_t>(keySessionImpl.GetRefPtr());
    napi_status status = napi_create_int64(env, nativePointer, &keySessionImplNative);
    DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, false, "create int failed");

    napi_value nameStr = nullptr;
    status = napi_create_string_utf8(env, name.c_str(), NAPI_AUTO_LENGTH, &nameStr);
    DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, false, "create string failed");

    status = napi_set_property(env, obj, nameStr, keySessionImplNative);
    DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, false, "set property failed");

    DRM_INFO_LOG("MediaKeySessionNapi::SetMediaKeySessionNativeProperty exit.");
    return true;
}

napi_value MediaKeySessionNapi::CreateMediaKeySession(napi_env env, sptr<MediaKeySessionImpl> keySessionImpl)
{
    DRM_INFO_LOG("MediaKeySessionNapi::CreateMediaKeySession enter.");
    napi_status status;
    napi_value result = nullptr;
    napi_value constructor;
    status = napi_get_reference_value(env, sConstructor_, &constructor);
    if (status == napi_ok) {
        sMediaKeySessionImpl_ = keySessionImpl;
        if (sMediaKeySessionImpl_ == nullptr) {
            DRM_ERR_LOG("Failed to CreateMediaKeySession instance");
            napi_get_undefined(env, &result);
            return result;
        }
        status = napi_new_instance(env, constructor, 0, nullptr, &result);
        sMediaKeySessionImpl_ = nullptr;
        if (status == napi_ok && result != nullptr) {
            DRM_INFO_LOG("success to CreateMediaKeySession napi instance");
            DRM_INFO_LOG("MediaKeySessionNapi::CreateMediaKeySession exit.");
            return result;
        } else {
            DRM_ERR_LOG("Failed to CreateMediaKeySession napi instance");
        }
    }
    DRM_DEBUG_LOG("Failed to create CreateMediaKeySession napi instance last");
    napi_get_undefined(env, &result);
    DRM_INFO_LOG("MediaKeySessionNapi::CreateMediaKeySession exit.");
    return result;
}

napi_value MediaKeySessionNapi::Destroy(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySessionNapi::Release enter.");
    int32_t currentPid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySessionNapi GetCallingPID: %{public}d", currentPid);

    napi_status status;
    napi_value result = nullptr;
    size_t argc = ARGS_ZERO;
    napi_value argv[ARGS_ZERO];
    napi_value thisVar = nullptr;
    MediaKeySessionNapi *keySessionNapi = nullptr;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);

    napi_get_undefined(env, &result);
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr && keySessionNapi->keySessionImpl_ != nullptr) {
        keySessionNapi->keySessionImpl_->Release();
        keySessionNapi->~MediaKeySessionNapi();
        keySessionNapi = nullptr;
    } else {
        DRM_ERR_LOG("MediaKeySessionNapi Release call Failed!");
    }
    DRM_INFO_LOG("MediaKeySessionNapi::Release exit.");
    return result;
}

static napi_value DealMediaKeyRequest(napi_env env, IMediaKeySessionService::MediaKeyRequest &licenseRequest)
{
    const char *requestTypeEnum;
    napi_value requestType;
    napi_value mData;
    napi_value mDefaultURL;
    napi_value result = nullptr;

    if (licenseRequest.requestType == IMediaKeySessionService::REQUEST_TYPE_UNKNOWN) {
        requestTypeEnum = "ALGTYPE_UNENCRYPTED";
    } else if (licenseRequest.requestType == IMediaKeySessionService::REQUEST_TYPE_INITIAL) {
        requestTypeEnum = "REQUEST_TYPE_INITIAL";
    } else if (licenseRequest.requestType == IMediaKeySessionService::REQUEST_TYPE_RENEWAL) {
        requestTypeEnum = "REQUEST_TYPE_RENEWAL";
    } else if (licenseRequest.requestType == IMediaKeySessionService::REQUEST_TYPE_RELEASE) {
        requestTypeEnum = "REQUEST_TYPE_RELEASE";
    } else if (licenseRequest.requestType == IMediaKeySessionService::REQUEST_TYPE_NONE) {
        requestTypeEnum = "REQUEST_TYPE_NONE";
    } else if (licenseRequest.requestType == IMediaKeySessionService::REQUEST_TYPE_UPDATE) {
        requestTypeEnum = "REQUEST_TYPE_UPDATE";
    } else if (licenseRequest.requestType < IMediaKeySessionService::REQUEST_TYPE_UNKNOWN ||
        licenseRequest.requestType > IMediaKeySessionService::REQUEST_TYPE_UPDATE) {
        DRM_ERR_LOG("Do not understand licenseRequest.requestType enum!");
        return result;
    }

    napi_status status = napi_create_string_utf8(env, requestTypeEnum, NAPI_AUTO_LENGTH, &requestType);
    if (status != napi_ok) {
        DRM_ERR_LOG("requestType error!");
        return result;
    }
    napi_create_object(env, &result);
    napi_set_named_property(env, result, "mediaKeyRequestType", requestType);

    size_t mDataLen = licenseRequest.mData.size();
    NAPI_CALL(env, napi_create_array(env, &mData));
    for (size_t i = 0; i < mDataLen; i++) {
        napi_value item;
        napi_create_int32(env, licenseRequest.mData[i], &item);
        napi_set_element(env, mData, i, item);
    }
    napi_set_named_property(env, result, "data", mData);

    status = napi_create_string_utf8(env, licenseRequest.mDefaultURL.c_str(), NAPI_AUTO_LENGTH, &mDefaultURL);
    if (status != napi_ok) {
        DRM_ERR_LOG("mDefaultURL error!");
        return result;
    }
    napi_set_named_property(env, result, "defaultURL", mDefaultURL);
    return result;
}

napi_value MediaKeySessionNapi::GenerateMediaKeyRequest(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySessionNapi::GenerateMediaKeyRequest enter");

    size_t argc = ARGS_FOUR;
    napi_value argv[ARGS_FOUR] = {0};
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

    IMediaKeySessionService::MediaKeyRequestInfo licenseRequestInfo;
    IMediaKeySessionService::MediaKeyRequest licenseRequest;
    MediaKeySessionNapi *keySessionNapi = nullptr;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_FOUR, "requires 4 parameters maximum");

    DRM_CHECK_AND_RETURN_RET_LOG(napi_get_value_string_utf8(env, argv[PARAM0], buffer, PATH_MAX, &length) == napi_ok,
        nullptr, "Could not able to read mimeType argument!");
    napi_is_typedarray(env, argv[PARAM1], &isTypeArray);
    DRM_CHECK_AND_RETURN_RET_LOG(isTypeArray, nullptr, "argv[PARAM1] is not array!");
    napi_get_typedarray_info(env, argv[PARAM1], &type, &initDataLen, &initData, &arraybuffer, &offset);
    DRM_CHECK_AND_RETURN_RET_LOG(initData != nullptr, nullptr, "napi_get_typedarray_info faild!");
    uint8_t *initDataPtr = reinterpret_cast<uint8_t *>(initData);
    std::vector<uint8_t> initDataStr(initDataPtr, initDataPtr + initDataLen);
    int32_t licenseType = 0;
    status = napi_get_value_int32(env, argv[PARAM2], &licenseType);
    DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, nullptr, "Could not able to read licenseType argument!");
    // optional data
    std::map<std::string, std::string> tmpOptionalData;

    uint32_t optionalDataCount = 0;
    status = napi_get_array_length(env, argv[PARAM3], &optionalDataCount);
    DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, nullptr, "Could not able to read optionalData argument!");
    if (optionalDataCount > 0) {
        for (size_t i = 0; i < optionalDataCount; i++) {
            napi_value tmpData;
            napi_value tmpName;
            napi_value tmpValue;
            size_t nameLength = 0;
            size_t valueLength = 0;
            status = napi_get_element(env, argv[PARAM3], i, &tmpData);
            DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, nullptr, "Could not able to read optionalData element!");
            status = napi_get_named_property(env, tmpData, "name", &tmpName);
            DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, nullptr, "Could not able to read optionalData property!");
            status = napi_get_named_property(env, tmpData, "value", &tmpValue);
            DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, nullptr, "Could not able to read optionalData property!");
            status = napi_get_value_string_utf8(env, tmpName, nullptr, 0, &nameLength);
            DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, nullptr,
                "Could not able to transfer optionalData buffer info!");
            status = napi_get_value_string_utf8(env, tmpValue, nullptr, 0, &valueLength);
            DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, nullptr,
                "Could not able to transfer optionalData buffer info!");
            std::string name(nameLength, 0);
            status = napi_get_value_string_utf8(env, tmpName, &name[0], nameLength + 1, &nameLength);
            DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, nullptr,
                "Could not able to transfer optionalData buffer info!");
            std::string value(valueLength, 0);
            status = napi_get_value_string_utf8(env, tmpValue, &value[0], valueLength + 1, &valueLength);
            DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, nullptr,
                "Could not able to transfer optionalData buffer info!");
            DRM_INFO_LOG("%{public}s: name : %{public}s", __func__, name.c_str());
            DRM_INFO_LOG("%{public}s: value : %{public}s", __func__, value.c_str());
            tmpOptionalData.insert(std::make_pair(name, value));
        }
    }

    licenseRequestInfo.mediaKeyType = (IMediaKeySessionService::MediaKeyType)licenseType;
    licenseRequestInfo.mimeType = std::string(buffer);
    licenseRequestInfo.initData = initDataStr;
    licenseRequestInfo.optionalData = tmpOptionalData;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&keySessionNapi));
    DRM_CHECK_AND_RETURN_RET_LOG((keySessionNapi != nullptr && keySessionNapi->keySessionImpl_ != nullptr), nullptr,
        "MediaKeySessionNapi get keySessionNapi fail!");
    int32_t ret = keySessionNapi->keySessionImpl_->GenerateMediaKeyRequest(licenseRequestInfo, licenseRequest);
    DRM_CHECK_AND_RETURN_RET_LOG((ret == DRM_OK), nullptr, "MediaKeySessionNapi GenerateMediaKeyRequest call Failed!");
    napi_value result = DealMediaKeyRequest(env, licenseRequest);
    DRM_INFO_LOG("MediaKeySessionNapi::GenerateMediaKeyRequest exit");
    return result;
}

napi_value MediaKeySessionNapi::ProcessMediaKeyResponse(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySessionNapi::ProcessMediaKeyResponse enter.");
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
    std::vector<uint8_t> licenseId;
    MediaKeySessionNapi *keySessionNapi = nullptr;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_ONE, "requires 1 parameters maximum");

    napi_is_typedarray(env, argv[PARAM0], &isTypeArray);
    if (!isTypeArray) {
        DRM_ERR_LOG("reponse is not array! ");
        return nullptr;
    }

    napi_get_typedarray_info(env, argv[PARAM0], &type, &reponseDataLen, &reponseData, &arraybuffer, &offset);
    if (reponseData == nullptr) {
        DRM_ERR_LOG("napi_get_typedarray_info faild! Could not able to read reponseData argument!");
        return nullptr;
    }
    uint8_t *reponseDataPtr = reinterpret_cast<uint8_t *>(reponseData);
    std::vector<uint8_t> licenseResponse(reponseDataPtr, reponseDataPtr + reponseDataLen);
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr && keySessionNapi->keySessionImpl_ != nullptr) {
        int32_t ret = keySessionNapi->keySessionImpl_->ProcessMediaKeyResponse(licenseId, licenseResponse);
        DRM_CHECK_AND_RETURN_RET_LOG((ret == DRM_OK), nullptr,
            "MediaKeySessionNapi ProcessMediaKeyResponse call Failed!");
        size_t licenseIdLen = licenseId.size();
        NAPI_CALL(env, napi_create_array(env, &result));
        for (size_t i = 0; i < licenseIdLen; i++) {
            napi_value item;
            napi_create_int32(env, licenseId[i], &item);
            napi_set_element(env, result, i, item);
        }
    } else {
        DRM_ERR_LOG("MediaKeySessionNapi ProcessMediaKeyResponse call Failed!");
    }
    DRM_INFO_LOG("MediaKeySessionNapi::ProcessMediaKeyResponse exit.");
    return result;
}

napi_value MediaKeySessionNapi::GenerateOfflineReleaseRequest(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySessionNapi::GenerateOfflineReleaseRequest enter");
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
    std::vector<uint8_t> releaseRequest;
    MediaKeySessionNapi *keySessionNapi = nullptr;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_ONE, "requires 1 parameters maximum");

    napi_is_typedarray(env, argv[PARAM0], &isTypeArray);
    if (!isTypeArray) {
        DRM_ERR_LOG("licenseId is not array!");
        return nullptr;
    }
    napi_get_typedarray_info(env, argv[PARAM0], &type, &licenseIdLen, &licenseId, &arraybuffer, &offset);
    if (licenseId == nullptr) {
        DRM_ERR_LOG("napi_get_typedarray_info faild! Could not able to read licenseId argument!");
        return nullptr;
    }
    uint8_t *licenseIdPtr = reinterpret_cast<uint8_t *>(licenseId);
    std::vector<uint8_t> licenseIdVec(licenseIdPtr, licenseIdPtr + licenseIdLen);

    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr && keySessionNapi->keySessionImpl_ != nullptr) {
        int32_t ret = keySessionNapi->keySessionImpl_->GenerateOfflineReleaseRequest(licenseIdVec, releaseRequest);
        DRM_CHECK_AND_RETURN_RET_LOG((ret == DRM_OK), nullptr,
            "MediaKeySessionNapi GenerateOfflineReleaseRequest call Failed!");
    } else {
        DRM_ERR_LOG("MediaKeySessionNapi ProcessMediaKeyResponse call Failed!");
        return nullptr;
    }

    size_t releaseRequestLen = releaseRequest.size();
    NAPI_CALL(env, napi_create_array(env, &result));
    for (size_t i = 0; i < releaseRequestLen; i++) {
        napi_value item;
        napi_create_int32(env, releaseRequest[i], &item);
        napi_set_element(env, result, i, item);
    }

    DRM_INFO_LOG("MediaKeySessionNapi::GenerateOfflineReleaseRequest exit");
    return result;
}

napi_value MediaKeySessionNapi::ProcessOfflineReleaseResponse(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySessionNapi::ProcessOfflineReleaseResponse enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_TWO;
    napi_value argv[ARGS_TWO] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    bool isTypeArray;
    void *licenseId = nullptr;
    size_t licenseIdLen;
    size_t offset;
    napi_value arraybuffer = nullptr;
    napi_typedarray_type type;
    void *response = nullptr;
    size_t responseLen;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_TWO, "requires 2 parameters maximum");

    napi_is_typedarray(env, argv[PARAM0], &isTypeArray);
    DRM_CHECK_AND_RETURN_RET_LOG(isTypeArray, nullptr, "argv[PARAM0] is not array!");
    napi_get_typedarray_info(env, argv[PARAM0], &type, &licenseIdLen, &licenseId, &arraybuffer, &offset);
    DRM_CHECK_AND_RETURN_RET_LOG(licenseId != nullptr, nullptr, "napi_get_typedarray_info faild!");
    uint8_t *licenseIdPtr = reinterpret_cast<uint8_t *>(licenseId);
    std::vector<uint8_t> licenseIdVec(licenseIdPtr, licenseIdPtr + licenseIdLen);
    napi_is_typedarray(env, argv[PARAM1], &isTypeArray);
    DRM_CHECK_AND_RETURN_RET_LOG(isTypeArray, nullptr, "argv[PARAM1] is not array!");
    napi_get_typedarray_info(env, argv[PARAM1], &type, &responseLen, &response, &arraybuffer, &offset);
    DRM_CHECK_AND_RETURN_RET_LOG(response != nullptr, nullptr, "napi_get_typedarray_info faild!");
    uint8_t *responsePtr = reinterpret_cast<uint8_t *>(response);
    std::vector<uint8_t> responseVec(responsePtr, responsePtr + responseLen);

    MediaKeySessionNapi *keySessionNapi = nullptr;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&keySessionNapi));
    DRM_CHECK_AND_RETURN_RET_LOG((keySessionNapi != nullptr && keySessionNapi->keySessionImpl_ != nullptr), nullptr,
        "MediaKeySessionNapi get keySessionNapi fail!");
    int32_t ret = keySessionNapi->keySessionImpl_->ProcessOfflineReleaseResponse(licenseIdVec, responseVec);
    DRM_CHECK_AND_RETURN_RET_LOG((ret == DRM_OK), nullptr,
        "MediaKeySessionNapi ProcessOfflineReleaseResponse call Failed!");

    DRM_INFO_LOG("MediaKeySessionNapi::ProcessOfflineReleaseResponse exit.");
    return result;
}

static napi_value vectorToJsArray(napi_env env, std::map<std::string, std::string> &licenseStatus)
{
    DRM_INFO_LOG("vectorToJsArray enter.");
    napi_value jsArray;
    napi_value jsName;
    napi_value jsValue;
    napi_create_array_with_length(env, licenseStatus.size(), &jsArray);
    size_t i = 0;
    for (auto it = licenseStatus.begin(); it != licenseStatus.end(); ++it) {
        napi_value jsObject;
        napi_create_object(env, &jsObject);
        std::string name = it->first;
        std::string status = it->second;
        napi_create_string_utf8(env, name.c_str(), NAPI_AUTO_LENGTH, &jsName);
        napi_set_named_property(env, jsObject, "name", jsName);
        napi_create_string_utf8(env, status.c_str(), NAPI_AUTO_LENGTH, &jsValue);
        napi_set_named_property(env, jsObject, "value", jsValue);
        napi_set_element(env, jsArray, i++, jsObject);
    }
    DRM_INFO_LOG("vectorToJsArray exit.");
    return jsArray;
}

napi_value MediaKeySessionNapi::CheckMediaKeyStatus(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySessionNapi::CheckMediaKeyStatus enter");
    napi_value result = nullptr;
    size_t argc = ARGS_ZERO;
    napi_value argv[ARGS_ZERO];
    napi_value thisVar = nullptr;
    napi_status status;
    MediaKeySessionNapi *keySessionNapi = nullptr;
    std::map<std::string, std::string> licenseStatus;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);

    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr && keySessionNapi->keySessionImpl_ != nullptr) {
        int32_t ret = keySessionNapi->keySessionImpl_->CheckMediaKeyStatus(licenseStatus);
        DRM_CHECK_AND_RETURN_RET_LOG((ret == DRM_OK), nullptr, "MediaKeySessionNapi CheckMediaKeyStatus call Failed!");
    } else {
        DRM_ERR_LOG("MediaKeySessionNapi CheckMediaKeyStatus call Failed!");
    }

    if (licenseStatus.size() == 0) {
        DRM_ERR_LOG("Licence not exist.");
        return nullptr;
    }
    result = vectorToJsArray(env, licenseStatus);
    DRM_INFO_LOG("MediaKeySessionNapi::CheckMediaKeyStatus exit.");
    return result;
}

napi_value MediaKeySessionNapi::RestoreOfflineMediaKeys(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySessionNapi::RestoreOfflineMediaKeys enter.");
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
    MediaKeySessionNapi *keySessionNapi = nullptr;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc <= ARGS_ONE, "requires 1 parameters maximum");

    napi_is_typedarray(env, argv[PARAM0], &isTypeArray);
    if (!isTypeArray) {
        DRM_ERR_LOG("licenseId is not array!");
        return nullptr;
    }
    napi_get_typedarray_info(env, argv[PARAM0], &type, &licenseIdLen, &licenseId, &arraybuffer, &offset);
    if (licenseId == nullptr) {
        DRM_ERR_LOG("napi_get_typedarray_info faild! Could not able to read licenseId argument!");
        return nullptr;
    }
    uint8_t *licenseIdPtr = reinterpret_cast<uint8_t *>(licenseId);
    std::vector<uint8_t> licenseIdVec(licenseIdPtr, licenseIdPtr + licenseIdLen);
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr && keySessionNapi->keySessionImpl_ != nullptr) {
        int32_t ret = keySessionNapi->keySessionImpl_->RestoreOfflineMediaKeys(licenseIdVec);
        DRM_CHECK_AND_RETURN_RET_LOG((ret == DRM_OK), nullptr,
            "MediaKeySessionNapi RestoreOfflineMediaKeys call Failed!");
    } else {
        DRM_ERR_LOG("MediaKeySessionNapi RestoreOfflineMediaKeys call Failed!");
    }

    DRM_INFO_LOG("MediaKeySessionNapi::RestoreOfflineMediaKeys exit.");
    return result;
}

napi_value MediaKeySessionNapi::ClearMediaKeys(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySessionNapi::ClearMediaKeys enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_ZERO;
    napi_value argv[ARGS_ZERO];
    napi_value thisVar = nullptr;
    napi_status status;

    int32_t currentPid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySessionNapi GetCallingPID: %{public}d", currentPid);

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    MediaKeySessionNapi *keySessionNapi = nullptr;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr && keySessionNapi->keySessionImpl_ != nullptr) {
        int32_t ret = keySessionNapi->keySessionImpl_->ClearMediaKeys();
        DRM_CHECK_AND_RETURN_RET_LOG((ret == DRM_OK), nullptr, "MediaKeySessionNapi ClearMediaKeys call Failed!");
    } else {
        DRM_ERR_LOG("MediaKeySessionNapi ClearMediaKeys call Failed!");
    }

    DRM_INFO_LOG("MediaKeySessionNapi::ClearMediaKeys exit.");
    return result;
}

napi_value MediaKeySessionNapi::RequireSecureDecoderModule(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySessionNapi::RequireSecureDecoderModule enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_ONE;
    napi_value argv[ARGS_ONE] = {0};
    napi_value thisVar = nullptr;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    napi_get_undefined(env, &result);
    NAPI_ASSERT(env, argc <= ARGS_ONE, "requires 1 parameters maximum");
    char mimeTypeBuf[PATH_MAX];
    size_t length = 0;

    DRM_CHECK_AND_RETURN_RET_LOG(napi_get_value_string_utf8(env, argv[PARAM0], mimeTypeBuf, PATH_MAX, &length) ==
        napi_ok,
        nullptr, "Could not able to read mimetype!");

    std::string mimeType = std::string(mimeTypeBuf);
    bool statusValue;
    MediaKeySessionNapi *keySessionNapi = nullptr;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&keySessionNapi));
    DRM_CHECK_AND_RETURN_RET_LOG((keySessionNapi != nullptr), nullptr, "MediaKeySessionNapi get keySessionNapi fail!");
    DRM_CHECK_AND_RETURN_RET_LOG((keySessionNapi->keySessionImpl_ != nullptr), nullptr, "keySessionImpl_ == nullptr.");

    int32_t ret = keySessionNapi->keySessionImpl_->RequireSecureDecoderModule(mimeType, &statusValue);
    DRM_CHECK_AND_RETURN_RET_LOG((ret == DRM_OK), nullptr,
        "MediaKeySessionNapi RequireSecureDecoderModule call Failed!");
    status = napi_get_boolean(env, statusValue, &result);
    DRM_INFO_LOG("napi_get_boolean call success!,statusValue:%{public}d.", statusValue);

    DRM_INFO_LOG("MediaKeySessionNapi::RequireSecureDecoderModule exit.");
    return result;
}

napi_value MediaKeySessionNapi::GetContentProtectionLevel(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySessionNapi::GetContentProtectionLevel enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_ONE;
    napi_value argv[ARGS_ONE] = {0};
    napi_value thisVar = nullptr;
    MediaKeySessionNapi *keySessionNapi = nullptr;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);

    NAPI_ASSERT(env, argc <= ARGS_ONE, "requires 1 parameters maximum");
    IMediaKeySessionService::ContentProtectionLevel level = (IMediaKeySessionService::ContentProtectionLevel)0;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr && keySessionNapi->keySessionImpl_ != nullptr) {
        int32_t ret = keySessionNapi->keySessionImpl_->GetContentProtectionLevel(&level);
        DRM_CHECK_AND_RETURN_RET_LOG((ret == DRM_OK), nullptr,
            "MediaKeySessionNapi GetContentProtectionLevel call Failed!");
    } else {
        DRM_ERR_LOG("MediaKeySessionNapi GetContentProtectionLevel call Failed!");
        return nullptr;
    }

    NAPI_CALL(env, napi_create_int32(env, (int32_t)level, &result));
    DRM_INFO_LOG("MediaKeySessionNapi::GetContentProtectionLevel exit");
    return result;
}

void MediaKeySessionNapi::SetEventCallbackReference(const std::string eventType, sptr<CallBackPair> callbackPair)
{
    DRM_INFO_LOG("MediaKeySessionNapi::SetEventCallbackReference");
    std::lock_guard<std::mutex> lock(mutex_);
    if (keySessionCallbackNapi_ != nullptr) {
        keySessionCallbackNapi_->SetCallbackReference(eventType, callbackPair);
    } else {
        DRM_ERR_LOG("MediaKeySessionNapi::SetEventCallbackReference failed");
    }
}

void MediaKeySessionNapi::ClearEventCallbackReference(const std::string eventType)
{
    DRM_INFO_LOG("MediaKeySessionNapi::ClearEventCallbackReference");
    if (keySessionCallbackNapi_ != nullptr) {
        keySessionCallbackNapi_->ClearCallbackReference(eventType);
    } else {
        DRM_ERR_LOG("MediaKeySessionNapi::ClearEventCallbackReference failed");
    }
}

napi_value MediaKeySessionNapi::SetEventCallback(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySessionNapi::SetEventCallback");
    napi_value result = nullptr;
    napi_get_undefined(env, &result);
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

    MediaKeySessionNapi *keySessionNapi = nullptr;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr) {
        char buffer[PATH_MAX];
        napi_get_value_string_utf8(env, argv[PARAM0], buffer, PATH_MAX, &length);
        std::string eventType = std::string(buffer);
        napi_ref callbackRef;
        napi_create_reference(env, argv[PARAM1], 1, &callbackRef);
        DRM_INFO_LOG("SetEventCallback event is %{public}s", eventType.c_str());

        sptr<CallBackPair> callbackPair = new CallBackPair(env, callbackRef);
        keySessionNapi->SetEventCallbackReference(eventType, callbackPair);
        DRM_INFO_LOG("MediaKeySessionNapi::SetEventCallback out");
    } else {
        DRM_ERR_LOG("MediaKeySessionNapi SetEventCallback failed!");
    }
    return result;
}

napi_value MediaKeySessionNapi::UnsetEventCallback(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaKeySessionNapi::UnsetEventCallback");
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

    MediaKeySessionNapi *keySessionNapi = nullptr;
    size_t length = 0;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr) {
        char buffer[PATH_MAX];
        napi_get_value_string_utf8(env, argv[PARAM0], buffer, PATH_MAX, &length);
        std::string eventType = std::string(buffer);
        keySessionNapi->ClearEventCallbackReference(eventType);
        DRM_INFO_LOG("MediaKeySessionNapi::UnsetEventCallback out");
    } else {
        DRM_ERR_LOG("MediaKeySessionNapi UnsetEventCallback failed!");
    }
    return result;
}
} // DrmStandardr
} // OHOS
