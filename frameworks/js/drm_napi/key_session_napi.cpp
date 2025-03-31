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
#include "napi_param_utils.h"
#include "key_session_impl.h"
#include "media_key_system_impl.h"
#include "key_session_napi.h"
#include "drm_trace.h"
#include "drm_error_code_napi.h"
#include "drm_api_operation.h"

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
    DRM_INFO_LOG("~MediaKeySessionNapi Init enter.");
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " Instances destroy", FAKE_POINTER(this));
    if (wrapper_ != nullptr) {
        napi_delete_reference(env_, wrapper_);
    }
    if (keySessionImpl_) {
        keySessionImpl_ = nullptr;
    }
    if (keySessionCallbackNapi_) {
        keySessionCallbackNapi_ = nullptr;
    }
}

napi_value MediaKeySessionNapi::Init(napi_env env, napi_value exports)
{
    DRM_INFO_LOG("MediaKeySessionNapi Init enter.");
    napi_status status;
    napi_value ctorObj;

    napi_property_descriptor session_props[] = {
        DECLARE_NAPI_FUNCTION("generateMediaKeyRequest", GenerateMediaKeyRequest),
        DECLARE_NAPI_FUNCTION("processMediaKeyResponse", ProcessMediaKeyResponse),
        DECLARE_NAPI_FUNCTION("generateOfflineReleaseRequest", GenerateOfflineReleaseRequest),
        DECLARE_NAPI_FUNCTION("processOfflineReleaseResponse", ProcessOfflineReleaseResponse),
        DECLARE_NAPI_FUNCTION("checkMediaKeyStatus", CheckMediaKeyStatus),
        DECLARE_NAPI_FUNCTION("restoreOfflineMediaKeys", RestoreOfflineMediaKeys),
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
    DRM_INFO_LOG("MediaKeySessionNapi Init call Failed!");
    return nullptr;
}

napi_value MediaKeySessionNapi::MediaKeySessionNapiConstructor(napi_env env, napi_callback_info info)
{
    DrmTrace trace("MediaKeySessionNapi::MediaKeySessionNapiConstructor");
    DRM_INFO_LOG("MediaKeySessionNapiConstructor enter.");

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
            obj->keySessionCallbackNapi_ = new MediaKeySessionCallbackNapi(env);
            obj->keySessionImpl_->SetCallback(obj->keySessionCallbackNapi_);

            status = napi_wrap(env, thisVar, reinterpret_cast<void *>(obj.get()),
                MediaKeySessionNapi::MediaKeySessionNapiDestructor, nullptr, nullptr);
            if (status == napi_ok) {
                ObjectRefMap<MediaKeySessionNapi>::Insert(obj.get());
                obj.release();
                const std::string propertyName = "MediaKeySessionNative";
                SetMediaKeySessionNativeProperty(env, thisVar, propertyName, sMediaKeySessionImpl_);
                return thisVar;
            } else {
                ObjectRefMap<MediaKeySessionNapi>::Erase(obj.get());
                DRM_ERR_LOG("Failure wrapping js to native napi");
            }
        }
    }
    return result;
}

void MediaKeySessionNapi::MediaKeySessionNapiDestructor(napi_env env, void *nativeObject, void *finalize)
{
    DrmTrace trace("MediaKeySessionNapiDestructor");
    DRM_INFO_LOG("MediaKeySessionNapiDestructor enter.");
    MediaKeySessionNapi *keySessionNapiObj = reinterpret_cast<MediaKeySessionNapi *>(nativeObject);
    ObjectRefMap<MediaKeySessionNapi>::DecreaseRef(keySessionNapiObj);
}

bool MediaKeySessionNapi::SetMediaKeySessionNativeProperty(napi_env env, napi_value obj, const std::string &name,
    sptr<MediaKeySessionImpl> keySessionImpl)
{
    DRM_INFO_LOG("SetMediaKeySessionNativeProperty enter.");
    DRM_CHECK_AND_RETURN_RET_LOG(obj != nullptr, false, "obj is nullptr");

    napi_value keySessionImplNative = nullptr;
    int64_t nativePointer = reinterpret_cast<int64_t>(keySessionImpl.GetRefPtr());
    napi_status status = napi_create_int64(env, nativePointer, &keySessionImplNative);
    DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, false, "create int failed.");

    napi_value nameStr = nullptr;
    status = napi_create_string_utf8(env, name.c_str(), NAPI_AUTO_LENGTH, &nameStr);
    DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, false, "create string failed.");

    status = napi_set_property(env, obj, nameStr, keySessionImplNative);
    DRM_CHECK_AND_RETURN_RET_LOG(status == napi_ok, false, "set property failed.");

    return true;
}

napi_value MediaKeySessionNapi::CreateMediaKeySession(napi_env env, sptr<MediaKeySessionImpl> keySessionImpl)
{
    DrmTrace trace("MediaKeySessionNapi::CreateMediaKeySession");
    DRM_INFO_LOG("CreateMediaKeySession enter.");
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
            return result;
        } else {
            DRM_ERR_LOG("Failed to CreateMediaKeySession napi instance");
        }
    }
    DRM_DEBUG_LOG("Failed to create CreateMediaKeySession napi instance last");
    napi_get_undefined(env, &result);
    return result;
}

napi_value MediaKeySessionNapi::Destroy(napi_env env, napi_callback_info info)
{
    DrmTrace trace("MediaKeySessionNapi::Destroy");
    DRM_INFO_LOG("Destroy enter.");
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
        int32_t ret = keySessionNapi->keySessionImpl_->Release();
        if (ret != DRM_NAPI_ERR_OK) {
            DRM_ERR_LOG("keySessionImpl_ Release call Failed!");
            NapiDrmError::ThrowError(env, "Release failed, service error.", DRM_NAPI_ERR_SERVICE_FATAL);
            return result;
        }
    } else {
        DRM_ERR_LOG("Release call Failed!");
        NapiDrmError::ThrowError(env, "Release failed, unknown error.", DRM_NAPI_ERR_UNKNOWN);
        return result;
    }
    return result;
}

bool MediaKeySessionNapi::CheckMediaKeySessionStatus(MediaKeySessionNapi *napi,
    std::shared_ptr<MediaKeySessionAsyncContext> context)
{
    DRM_NAPI_CHECK_AND_RETURN_LOG(napi != nullptr, false, "napi object is nullptr.");
    if (napi->keySessionImpl_ == nullptr) {
        context->SignError(DRM_NAPI_ERR_SERVICE_FATAL);
        return false;
    }
    return true;
}

bool MediaKeySessionNapi::CheckContextStatus(std::shared_ptr<MediaKeySessionAsyncContext> context)
{
    DRM_NAPI_CHECK_AND_RETURN_LOG(context != nullptr, false, "context object is nullptr.");
    if (context->native == nullptr) {
        context->SignError(DRM_NAPI_ERR_SERVICE_FATAL);
        return false;
    }
    return true;
}

napi_value MediaKeySessionNapi::GenerateMediaKeyRequest(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("GenerateMediaKeyRequest enter");
    DrmTrace trace("MediaKeySessionNapi::GenerateMediaKeyRequest");
    auto context = std::make_shared<MediaKeySessionAsyncContext>();
    if (context == nullptr) {
        DRM_ERR_LOG("GenerateMediaKeyRequest failed.");
        NapiDrmError::ThrowError(env, "make context failed, unknown error.", DRM_NAPI_ERR_UNKNOWN);
        return NapiParamUtils::GetUndefinedValue(env);
    }

    auto inputParser = [env, context](size_t argc, napi_value *argv) {
        NAPI_CHECK_ARGS_RETURN_VOID(context, argc == ARGS_THREE || argc == ARGS_FOUR, "invalid arguments",
            DRM_NAPI_ERR_INVALID_VAL);
        context->mediaKeyRequestInfo.mimeType = NapiParamUtils::GetStringArgument(env, argv[PARAM0]);
        context->status = NapiParamUtils::GetValueUint8Array(env, context->mediaKeyRequestInfo.initData, argv[PARAM1]);
        NAPI_CHECK_STATUS_RETURN_VOID(context, "generateMediaKeyRequest failed!", DRM_NAPI_ERR_INVALID_VAL);
        int32_t mediaKeyType = 0;
        context->status = NapiParamUtils::GetValueInt32(env, mediaKeyType, argv[PARAM2]);
        NAPI_CHECK_STATUS_RETURN_VOID(context, "generateMediaKeyRequest failed!", DRM_NAPI_ERR_INVALID_VAL);
        context->mediaKeyRequestInfo.mediaKeyType = (MediaKeyType)mediaKeyType;
        if (argc == ARGS_FOUR) {
            context->status =
            NapiParamUtils::GetValueOptionsData(env, context->mediaKeyRequestInfo.optionalData, argv[PARAM3]);
            NAPI_CHECK_STATUS_RETURN_VOID(context, "generateMediaKeyRequest failed!", DRM_NAPI_ERR_INVALID_VAL);
        }
    };
    context->GetCbInfo(env, info, inputParser);

    auto executor = [context]() {
        DRM_NAPI_CHECK_AND_RETURN_VOID_LOG(CheckContextStatus(context), "context object state is error.");
        auto obj = reinterpret_cast<MediaKeySessionNapi *>(context->native);
        ObjectRefMap objectGuard(obj);
        auto *napiMediaKeySession = objectGuard.GetPtr();
        DRM_NAPI_CHECK_AND_RETURN_VOID_LOG(CheckMediaKeySessionStatus(napiMediaKeySession, context),
            "MediaKeySession state is error.");
        context->intValue = napiMediaKeySession->keySessionImpl_->GenerateMediaKeyRequest(context->mediaKeyRequestInfo,
            context->mediaKeyRequest);
        if (context->intValue != DRM_NAPI_ERR_OK) {
            context->SignError(DRM_NAPI_ERR_SERVICE_FATAL);
        }
    };

    auto complete = [env, context](napi_value &output) {
        NapiParamUtils::SetMediaKeyRequest(env, context->mediaKeyRequest, output);
    };
    return NapiAsyncWork::Enqueue(env, context, "GenerateMediaKeyRequest", executor, complete);
}

napi_value MediaKeySessionNapi::ProcessMediaKeyResponse(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("ProcessMediaKeyResponse enter.");
    int64_t beginTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    DrmTrace trace("MediaKeySessionNapi::ProcessMediaKeyResponse");
    auto context = std::make_shared<MediaKeySessionAsyncContext>();
    if (context == nullptr) {
        DRM_ERR_LOG("ProcessMediaKeyResponse failed.");
        NapiDrmError::ThrowError(env, "make context failed, unknown error.", DRM_NAPI_ERR_UNKNOWN);
        return NapiParamUtils::GetUndefinedValue(env);
    }

    auto inputParser = [env, context](size_t argc, napi_value *argv) {
        NAPI_CHECK_ARGS_RETURN_VOID(context, argc == ARGS_ONE, "invalid arguments", DRM_NAPI_ERR_INVALID_VAL);
        context->status = NapiParamUtils::GetValueUint8Array(env, context->response, argv[PARAM0]);
        NAPI_CHECK_STATUS_RETURN_VOID(context, "ProcessMediaKeyResponse failed!", DRM_NAPI_ERR_INVALID_VAL);
    };
    context->GetCbInfo(env, info, inputParser);

    auto executor = [context]() {
        DRM_NAPI_CHECK_AND_RETURN_VOID_LOG(CheckContextStatus(context), "context object state is error.");
        auto obj = reinterpret_cast<MediaKeySessionNapi *>(context->native);
        ObjectRefMap objectGuard(obj);
        auto *napiMediaKeySession = objectGuard.GetPtr();
        DRM_NAPI_CHECK_AND_RETURN_VOID_LOG(CheckMediaKeySessionStatus(napiMediaKeySession, context),
            "MediaKeySession state is error.");
        context->intValue =
            napiMediaKeySession->keySessionImpl_->ProcessMediaKeyResponse(context->licenseId, context->response);
        if (context->intValue != DRM_NAPI_ERR_OK) {
            context->SignError(DRM_NAPI_ERR_SERVICE_FATAL);
        }
    };

    auto complete = [env, context](napi_value &output) {
        NapiParamUtils::SetValueUint8Array(env, context->licenseId, output);
    };
    ConfigParser::WriteEndEvent(0, 0, std::string("processMediaKeyResponse"), beginTime);
    return NapiAsyncWork::Enqueue(env, context, "ProcessMediaKeyResponse", executor, complete);
}

napi_value MediaKeySessionNapi::GenerateOfflineReleaseRequest(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("GenerateOfflineReleaseRequest enter");
    auto context = std::make_shared<MediaKeySessionAsyncContext>();
    if (context == nullptr) {
        DRM_ERR_LOG("GenerateOfflineReleaseRequest failed.");
        NapiDrmError::ThrowError(env, "make context failed, unknown error.", DRM_NAPI_ERR_UNKNOWN);
        return NapiParamUtils::GetUndefinedValue(env);
    }

    auto inputParser = [env, context](size_t argc, napi_value *argv) {
        NAPI_CHECK_ARGS_RETURN_VOID(context, argc == ARGS_ONE, "invalid arguments", DRM_NAPI_ERR_INVALID_VAL);
        context->status = NapiParamUtils::GetValueUint8Array(env, context->releaseLicenseId, argv[PARAM0]);
        NAPI_CHECK_STATUS_RETURN_VOID(context, "GenerateOfflineReleaseRequest failed.", DRM_NAPI_ERR_INVALID_VAL);
    };
    context->GetCbInfo(env, info, inputParser);

    auto executor = [context]() {
        DRM_NAPI_CHECK_AND_RETURN_VOID_LOG(CheckContextStatus(context), "context object state is error.");
        auto obj = reinterpret_cast<MediaKeySessionNapi *>(context->native);
        ObjectRefMap objectGuard(obj);
        auto *napiMediaKeySession = objectGuard.GetPtr();
        DRM_NAPI_CHECK_AND_RETURN_VOID_LOG(CheckMediaKeySessionStatus(napiMediaKeySession, context),
            "MediaKeySession state is error.");
        context->intValue = napiMediaKeySession->keySessionImpl_->GenerateOfflineReleaseRequest(
            context->releaseLicenseId, context->releaseRequest);
        if (context->intValue != DRM_NAPI_ERR_OK) {
            context->SignError(DRM_NAPI_ERR_SERVICE_FATAL);
        }
    };

    auto complete = [env, context](napi_value &output) {
        NapiParamUtils::SetValueUint8Array(env, context->releaseRequest, output);
    };
    return NapiAsyncWork::Enqueue(env, context, "GenerateOfflineReleaseRequest", executor, complete);
}

napi_value MediaKeySessionNapi::ProcessOfflineReleaseResponse(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("ProcessOfflineReleaseResponse enter.");
    auto context = std::make_shared<MediaKeySessionAsyncContext>();
    if (context == nullptr) {
        DRM_ERR_LOG("ProcessOfflineReleaseResponse failed.");
        NapiDrmError::ThrowError(env, "make context failed, unknown error.", DRM_NAPI_ERR_UNKNOWN);
        return NapiParamUtils::GetUndefinedValue(env);
    }

    auto inputParser = [env, context](size_t argc, napi_value *argv) {
        NAPI_CHECK_ARGS_RETURN_VOID(context, argc == ARGS_TWO, "invalid arguments", DRM_NAPI_ERR_INVALID_VAL);
        context->status = NapiParamUtils::GetValueUint8Array(env, context->releaseResponseLicenseId, argv[PARAM0]);
        NAPI_CHECK_STATUS_RETURN_VOID(context, "GetValueUint8Array failed.", DRM_NAPI_ERR_INVALID_VAL);
        context->status = NapiParamUtils::GetValueUint8Array(env, context->releaseResponse, argv[PARAM1]);
        NAPI_CHECK_STATUS_RETURN_VOID(context, "ProcessOfflineReleaseResponse failed.", DRM_NAPI_ERR_INVALID_VAL);
    };
    context->GetCbInfo(env, info, inputParser);

    auto executor = [context]() {
        DRM_NAPI_CHECK_AND_RETURN_VOID_LOG(CheckContextStatus(context), "context object state is error.");
        auto obj = reinterpret_cast<MediaKeySessionNapi *>(context->native);
        ObjectRefMap objectGuard(obj);
        auto *napiMediaKeySession = objectGuard.GetPtr();
        DRM_NAPI_CHECK_AND_RETURN_VOID_LOG(CheckMediaKeySessionStatus(napiMediaKeySession, context),
            "MediaKeySession state is error.");
        context->intValue = napiMediaKeySession->keySessionImpl_->ProcessOfflineReleaseResponse(
            context->releaseResponseLicenseId, context->releaseResponse);
        if (context->intValue != DRM_NAPI_ERR_OK) {
            context->SignError(DRM_NAPI_ERR_SERVICE_FATAL);
        }
    };

    auto complete = [env, context](napi_value &output) { output = NapiParamUtils::GetUndefinedValue(env); };
    return NapiAsyncWork::Enqueue(env, context, "ProcessOfflineReleaseResponse", executor, complete);
}

static napi_value vectorToJsArray(napi_env env, std::map<std::string, std::string> &licenseStatus)
{
    DRM_INFO_LOG("vectorToJsArray enter.");
    napi_value jsArray;
    napi_value jsName;
    napi_value jsValue;
    napi_create_array_with_length(env, licenseStatus.size(), &jsArray);
    size_t index = 0;
    for (auto it = licenseStatus.begin(); it != licenseStatus.end(); ++it) {
        napi_value jsObject;
        napi_create_object(env, &jsObject);
        std::string name = it->first;
        std::string status = it->second;
        napi_create_string_utf8(env, name.c_str(), NAPI_AUTO_LENGTH, &jsName);
        napi_set_named_property(env, jsObject, "name", jsName);
        napi_create_string_utf8(env, status.c_str(), NAPI_AUTO_LENGTH, &jsValue);
        napi_set_named_property(env, jsObject, "value", jsValue);
        napi_set_element(env, jsArray, index++, jsObject);
    }
    return jsArray;
}

napi_value MediaKeySessionNapi::CheckMediaKeyStatus(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("CheckMediaKeyStatus enter");
    napi_value result = nullptr;
    size_t argc = ARGS_ZERO;
    napi_value argv[ARGS_ZERO];
    napi_value thisVar = nullptr;
    napi_status status;
    MediaKeySessionNapi *keySessionNapi = nullptr;
    std::map<std::string, std::string> licenseStatus;

    napi_get_undefined(env, &result);
    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr && keySessionNapi->keySessionImpl_ != nullptr) {
        int32_t ret = keySessionNapi->keySessionImpl_->CheckMediaKeyStatus(licenseStatus);
        if (ret != DRM_NAPI_ERR_OK) {
            DRM_ERR_LOG("CheckMediaKeyStatus failed!");
            NapiDrmError::ThrowError(env, "CheckMediaKeyStatus failed.", DRM_NAPI_ERR_SERVICE_FATAL);
            return result;
        }
    } else {
        DRM_ERR_LOG("CheckMediaKeyStatus call Failed!");
        NapiDrmError::ThrowError(env, "CheckMediaKeyStatus call failed.", DRM_NAPI_ERR_UNKNOWN);
        return result;
    }

    if (licenseStatus.size() == 0) {
        DRM_ERR_LOG("Licence not exist.");
        NapiDrmError::ThrowError(env, "CheckMediaKeyStatus call failed, service error.", DRM_NAPI_ERR_SERVICE_FATAL);
        return result;
    }
    result = vectorToJsArray(env, licenseStatus);
    return result;
}

napi_value MediaKeySessionNapi::RestoreOfflineMediaKeys(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("RestoreOfflineMediaKeys enter.");
    int64_t beginTime = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    auto context = std::make_shared<MediaKeySessionAsyncContext>();
    if (context == nullptr) {
        DRM_ERR_LOG("RestoreOfflineMediaKeys failed.");
        NapiDrmError::ThrowError(env, "make_shared failed, unknown error.", DRM_NAPI_ERR_UNKNOWN);
        return NapiParamUtils::GetUndefinedValue(env);
    }

    auto inputParser = [env, context](size_t argc, napi_value *argv) {
        NAPI_CHECK_ARGS_RETURN_VOID(context, argc == ARGS_ONE, "invalid arguments", DRM_NAPI_ERR_INVALID_VAL);
        context->status = NapiParamUtils::GetValueUint8Array(env, context->restoreLicenseId, argv[PARAM0]);
        NAPI_CHECK_STATUS_RETURN_VOID(context, "RestoreOfflineMediaKeys failed.", DRM_NAPI_ERR_INVALID_VAL);
    };
    context->GetCbInfo(env, info, inputParser);

    auto executor = [context]() {
        DRM_NAPI_CHECK_AND_RETURN_VOID_LOG(CheckContextStatus(context), "context object state is error.");
        auto obj = reinterpret_cast<MediaKeySessionNapi *>(context->native);
        ObjectRefMap objectGuard(obj);
        auto *napiMediaKeySession = objectGuard.GetPtr();
        DRM_NAPI_CHECK_AND_RETURN_VOID_LOG(CheckMediaKeySessionStatus(napiMediaKeySession, context),
            "MediaKeySession state is error.");
        context->intValue = napiMediaKeySession->keySessionImpl_->RestoreOfflineMediaKeys(context->restoreLicenseId);
        if (context->intValue != DRM_NAPI_ERR_OK) {
            context->SignError(DRM_NAPI_ERR_SERVICE_FATAL);
        }
    };

    auto complete = [env, context](napi_value &output) { output = NapiParamUtils::GetUndefinedValue(env); };
    ConfigParser::WriteEndEvent(0, 0, std::string("restoreOfflineMediaKeys"), beginTime);
    return NapiAsyncWork::Enqueue(env, context, "RestoreOfflineMediaKeys", executor, complete);
}

napi_value MediaKeySessionNapi::ClearMediaKeys(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("ClearMediaKeys enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_ZERO;
    napi_value argv[ARGS_ZERO];
    napi_value thisVar = nullptr;
    napi_status status;

    napi_get_undefined(env, &result);
    int32_t currentPid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaKeySessionNapi GetCallingPID: %{public}d", currentPid);
    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    MediaKeySessionNapi *keySessionNapi = nullptr;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr && keySessionNapi->keySessionImpl_ != nullptr) {
        int32_t ret = keySessionNapi->keySessionImpl_->ClearMediaKeys();
        if (ret != DRM_NAPI_ERR_OK) {
            DRM_ERR_LOG("ClearMediaKeys call Failed!");
            NapiDrmError::ThrowError(env, "ClearMediaKeys call failed, service error.",
                DRM_NAPI_ERR_SERVICE_FATAL);
            return result;
        }
    } else {
        DRM_ERR_LOG("napi_unwrap call Failed!");
        NapiDrmError::ThrowError(env, "napi_unwrap call failed.", DRM_NAPI_ERR_UNKNOWN);
        return result;
    }
    return result;
}

napi_value MediaKeySessionNapi::RequireSecureDecoderModule(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("RequireSecureDecoderModule enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_ONE;
    napi_value argv[ARGS_ONE] = {0};
    napi_value thisVar = nullptr;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    napi_get_undefined(env, &result);
    if (argc != ARGS_ONE) {
        DRM_ERR_LOG("invalid arguments.");
        NapiDrmError::ThrowError(env, "Invalid params, only need one param.", DRM_NAPI_ERR_INVALID_VAL);
        return result;
    }
    char mimeTypeBuf[PATH_MAX];
    size_t length = 0;
    if (napi_get_value_string_utf8(env, argv[PARAM0], mimeTypeBuf, PATH_MAX, &length) != napi_ok) {
        DRM_ERR_LOG("Could not able to read mimetype!");
        NapiDrmError::ThrowError(env, "Could not able to read mimetype.", DRM_NAPI_ERR_UNKNOWN);
        return result;
    }

    std::string mimeType = std::string(mimeTypeBuf);
    bool statusValue = false;
    MediaKeySessionNapi *keySessionNapi = nullptr;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&keySessionNapi));
    if (keySessionNapi == nullptr || keySessionNapi->keySessionImpl_ == nullptr) {
        DRM_ERR_LOG("napi_unwrapcall Failed!");
        NapiDrmError::ThrowError(env, "napi_unwrapcall failed.", DRM_NAPI_ERR_UNKNOWN);
        return result;
    }
    int32_t ret = keySessionNapi->keySessionImpl_->RequireSecureDecoderModule(mimeType, &statusValue);
    if (ret != DRM_NAPI_ERR_OK) {
        DRM_ERR_LOG("keySessionImpl_ RequireSecureDecoderModule call Failed!");
        NapiDrmError::ThrowError(env, "RequireSecureDecoderModule failed, service error.", DRM_NAPI_ERR_SERVICE_FATAL);
        return result;
    }
    status = napi_get_boolean(env, statusValue, &result);
    DRM_INFO_LOG("napi_get_boolean call success!,statusValue:%{public}d.", statusValue);
    return result;
}

napi_value MediaKeySessionNapi::GetContentProtectionLevel(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("GetContentProtectionLevel enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_ONE;
    napi_value argv[ARGS_ONE] = {0};
    napi_value thisVar = nullptr;
    MediaKeySessionNapi *keySessionNapi = nullptr;

    napi_get_undefined(env, &result);
    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    ContentProtectionLevel level = (ContentProtectionLevel)0;
    napi_status status = napi_unwrap(env, thisVar, reinterpret_cast<void **>(&keySessionNapi));
    if (status == napi_ok && keySessionNapi != nullptr && keySessionNapi->keySessionImpl_ != nullptr) {
        int32_t ret = keySessionNapi->keySessionImpl_->GetContentProtectionLevel(&level);
        if (ret != DRM_NAPI_ERR_OK) {
            DRM_ERR_LOG("GetContentProtectionLevel call Failed!");
            NapiDrmError::ThrowError(env, "GetContentProtectionLevel failed, service error.",
                DRM_NAPI_ERR_SERVICE_FATAL);
            return result;
        }
    } else {
        DRM_ERR_LOG("napi_unwrap call Failed!");
        NapiDrmError::ThrowError(env, "napi_unwrap failed.", DRM_NAPI_ERR_UNKNOWN);
        return result;
    }

    NAPI_CALL(env, napi_create_int32(env, (int32_t)level, &result));
    return result;
}

void MediaKeySessionNapi::SetEventCallbackReference(const std::string eventType, std::shared_ptr<AutoRef> callbackPair)
{
    DRM_INFO_LOG("SetEventCallbackReference");
    std::lock_guard<std::mutex> lock(mutex_);
    if (keySessionCallbackNapi_ != nullptr) {
        keySessionCallbackNapi_->SetCallbackReference(eventType, callbackPair);
    } else {
        DRM_ERR_LOG("SetEventCallbackReference failed.");
    }
}

void MediaKeySessionNapi::ClearEventCallbackReference(const std::string eventType)
{
    DRM_INFO_LOG("ClearEventCallbackReference");
    if (keySessionCallbackNapi_ != nullptr) {
        keySessionCallbackNapi_->ClearCallbackReference(eventType);
    } else {
        DRM_ERR_LOG("ClearEventCallbackReference failed.");
    }
}

napi_value MediaKeySessionNapi::SetEventCallback(napi_env env, napi_callback_info info)
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
        DRM_ERR_LOG("Failed to retrieve arguments in SetEventCallback!");
        NapiDrmError::ThrowError(env, "Mandatory parameters are left unspecified.", DRM_NAPI_ERR_INVALID_VAL);
        return result;
    }
    napi_valuetype valueType = napi_undefined;
    if (napi_typeof(env, argv[PARAM0], &valueType) != napi_ok || valueType != napi_string ||
        napi_typeof(env, argv[PARAM1], &valueType) != napi_ok || valueType != napi_function) {
        NapiDrmError::ThrowError(
            env, "Incorrect parameter types, check if callback type is a string, and callback is a function.",
            DRM_NAPI_ERR_INVALID_VAL);
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
        DRM_DEBUG_LOG("SetEventCallback event is %{public}s", eventType.c_str());

        std::shared_ptr<AutoRef> callbackPair = std::make_shared<AutoRef>(env, callbackRef);
        keySessionNapi->SetEventCallbackReference(eventType, callbackPair);
    } else {
        DRM_ERR_LOG("napi_unwrap failed!");
        NapiDrmError::ThrowError(env, "napi_unwrap failed!", DRM_NAPI_ERR_UNKNOWN);
    }
    return result;
}

napi_value MediaKeySessionNapi::UnsetEventCallback(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("UnsetEventCallback");
    napi_value result = nullptr;
    napi_get_undefined(env, &result);
    napi_value thisVar = nullptr;
    napi_value argv[1] = { nullptr };
    size_t argc = 1;
    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    NAPI_ASSERT(env, argc == ARGS_ONE, "only requires 1 parameters");
    if (thisVar == nullptr || argv[PARAM0] == nullptr) {
        DRM_ERR_LOG("Failed to retrieve arguments in UnsetEventCallback!");
        NapiDrmError::ThrowError(env, "Mandatory parameters are left unspecified.", DRM_NAPI_ERR_INVALID_VAL);
        return result;
    }
    napi_valuetype valueType = napi_undefined;
    if (napi_typeof(env, argv[PARAM0], &valueType) != napi_ok || valueType != napi_string) {
        DRM_ERR_LOG("Failed to retrieve reasonable arguments in UnsetEventCallback!");
        NapiDrmError::ThrowError(env, "the param is not a string", DRM_NAPI_ERR_INVALID_VAL);
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
        DRM_INFO_LOG("UnsetEventCallback out");
    } else {
        DRM_ERR_LOG("UnsetEventCallback failed!");
        NapiDrmError::ThrowError(env, "UnsetEventCallback failed, unknown error.", DRM_NAPI_ERR_UNKNOWN);
    }
    return result;
}
} // DrmStandardr
} // OHOS