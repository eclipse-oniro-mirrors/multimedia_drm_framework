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
#include "media_decrypt_module_napi.h"
#include "media_decrypt_module_impl.h"
#include "key_session_impl.h"
#include "ipc_skeleton.h"

namespace OHOS {
namespace DrmStandard {
thread_local napi_ref MediaDecryptModuleNapi::sConstructor_ = nullptr;
thread_local sptr<MediaDecryptModuleImpl> MediaDecryptModuleNapi::sMediaDecryptModuleImpl_ = nullptr;

MediaDecryptModuleNapi::MediaDecryptModuleNapi() : env_(nullptr), wrapper_(nullptr)
{
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " Instances create", FAKE_POINTER(this));
}

MediaDecryptModuleNapi::~MediaDecryptModuleNapi()
{
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR " Instances destroy", FAKE_POINTER(this));
    if (wrapper_ != nullptr) {
        napi_delete_reference(env_, wrapper_);
    }
    if (sMediaDecryptModuleImpl_) {
        sMediaDecryptModuleImpl_ = nullptr;
    }
}

napi_value MediaDecryptModuleNapi::Init(napi_env env, napi_value exports)
{
    DRM_INFO_LOG("MediaDecryptModuleNapi Init ENTER");

    napi_status status;
    napi_value ctorObj;
    int32_t refCount = 1;

    napi_property_descriptor decryptModule_props[] = {
        DECLARE_NAPI_FUNCTION("release", Release),
        DECLARE_NAPI_FUNCTION("requireSecureDecoderModule", RequireSecureDecoderModule),
        DECLARE_NAPI_FUNCTION("decryptData", DecryptData),
    };

    DRM_DEBUG_LOG("MediaDecryptModuleNapi napi_define_class");
    status = napi_define_class(env, DECYPT_MODULE_NAPI_CLASS_NAME, NAPI_AUTO_LENGTH,
                               MediaDecryptModuleNapiConstructor, nullptr,
                               sizeof(decryptModule_props) / sizeof(decryptModule_props[PARAM0]),
                               decryptModule_props, &ctorObj);
    if (status == napi_ok) {
        status = napi_create_reference(env, ctorObj, refCount, &sConstructor_);
        if (status == napi_ok) {
            status = napi_set_named_property(env, exports, DECYPT_MODULE_NAPI_CLASS_NAME, ctorObj);
            if (status == napi_ok) {
                DRM_INFO_LOG("MediaDecryptModuleNapi return");
                return exports;
            }
        }
    }
    DRM_INFO_LOG("MediaDecryptModuleNapi Init call Failed!");
    return nullptr;
}

napi_value MediaDecryptModuleNapi::MediaDecryptModuleNapiConstructor(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaDecryptModuleNapi::MediaDecryptModuleNapiConstructor enter.");
    napi_status status;
    napi_value result = nullptr;
    napi_value thisVar = nullptr;

    napi_get_undefined(env, &result);
    DRM_NAPI_GET_JS_OBJ_WITH_ZERO_ARGS(env, info, status, thisVar);

    if (status == napi_ok && thisVar != nullptr) {
        std::unique_ptr<MediaDecryptModuleNapi> obj = std::make_unique<MediaDecryptModuleNapi>();
        if (obj != nullptr) {
            obj->env_ = env;
            if (sMediaDecryptModuleImpl_ == nullptr) {
                DRM_ERR_LOG("sMediaDecryptModuleImpl_ is null");
                return result;
            }
            obj->mediaDecryptModuleImpl_ = sMediaDecryptModuleImpl_;
            status = napi_wrap(env, thisVar, reinterpret_cast<void*>(obj.get()),
                               MediaDecryptModuleNapi::MediaDecryptModuleNapiDestructor, nullptr, nullptr);
            if (status == napi_ok) {
                obj.release();
                return thisVar;
            } else {
                DRM_ERR_LOG("MediaDecryptModuleNapi Failure wrapping js to native napi");
            }
        }
    }
    DRM_INFO_LOG("MediaDecryptModuleNapi::MediaDecryptModuleNapiConstructor exit.");
    return result;
}

void MediaDecryptModuleNapi::MediaDecryptModuleNapiDestructor(napi_env env, void *nativeObject, void *finalize)
{
    DRM_INFO_LOG("MediaDecryptModuleNapi::MediaDecryptModuleNapiDestructor enter.");
    MediaDecryptModuleNapi* decryptModuleNapiObj = reinterpret_cast<MediaDecryptModuleNapi*>(nativeObject);
    if (decryptModuleNapiObj != nullptr) {
        decryptModuleNapiObj->~MediaDecryptModuleNapi();
    }
    DRM_INFO_LOG("MediaDecryptModuleNapi::MediaDecryptModuleNapiDestructor exit.");
}

napi_value MediaDecryptModuleNapi::GetDecryptModule(napi_env env, sptr<MediaDecryptModuleImpl> decryptModuleImpl)
{
    DRM_INFO_LOG("MediaDecryptModuleNapi::GetDecryptModule enter.");
    napi_status status;
    napi_value result = nullptr;
    napi_value constructor;

    status = napi_get_reference_value(env, sConstructor_, &constructor);
    if (status == napi_ok) {
        sMediaDecryptModuleImpl_ = decryptModuleImpl;
        if (sMediaDecryptModuleImpl_ == nullptr) {
            DRM_ERR_LOG("Failed to GetDecryptModule instance");
            napi_get_undefined(env, &result);
            return result;
        }
        status = napi_new_instance(env, constructor, 0, nullptr, &result);
        sMediaDecryptModuleImpl_ = nullptr;
        if (status == napi_ok && result != nullptr) {
            DRM_INFO_LOG("success to GetDecryptModule napi instance");
            return result;
        } else {
            DRM_ERR_LOG("Failed to GetDecryptModule napi instance");
        }
    }
    DRM_ERR_LOG("Failed to create GetDecryptModule napi instance last");
    napi_get_undefined(env, &result);
    DRM_INFO_LOG("MediaDecryptModuleNapi::GetDecryptModule exit.");
    return result;
}

napi_value MediaDecryptModuleNapi::Release(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaDecryptModuleNapi::Release enter.");
    int32_t currentPid = IPCSkeleton::GetCallingPid();
    DRM_DEBUG_LOG("MediaDecryptModuleNapi GetCallingPID: %{public}d", currentPid);

    napi_status status;
    napi_value result = nullptr;
    size_t argc = ARGS_ZERO;
    napi_value argv[ARGS_ZERO];
    napi_value thisVar = nullptr;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);

    napi_get_undefined(env, &result);
    MediaDecryptModuleNapi* decryptModuleNapi = nullptr;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&decryptModuleNapi));
    if (status == napi_ok && decryptModuleNapi != nullptr) {
        decryptModuleNapi->mediaDecryptModuleImpl_->Release();
    } else {
        DRM_ERR_LOG("MediaDecryptModuleNapi Release call Failed!");
    }
    DRM_INFO_LOG("MediaDecryptModuleNapi::Release exit.");
    return result;
}

napi_value MediaDecryptModuleNapi::RequireSecureDecoderModule(napi_env env, napi_callback_info info)
{
    DRM_INFO_LOG("MediaDecryptModuleNapi::RequireSecureDecoderModule enter.");
    napi_value result = nullptr;
    size_t argc = ARGS_ONE;
    napi_value argv[ARGS_ONE] = {0};
    napi_value thisVar = nullptr;
    napi_status status;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    napi_get_undefined(env, &result);
    NAPI_ASSERT(env, argc <= ARGS_ONE, "requires 1 parameters maximum");
    char mimeTypeBuf[PATH_MAX];
    size_t length = 0;
    if (napi_get_value_string_utf8(env, argv[PARAM0], mimeTypeBuf, PATH_MAX, &length) == napi_ok) {
        std::string mimeType = std::string(mimeTypeBuf);
        bool statusValue;
        MediaDecryptModuleNapi* decryptModuleNapi = nullptr;
        status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&decryptModuleNapi));
        if (status == napi_ok && decryptModuleNapi != nullptr) {
            if (decryptModuleNapi->mediaDecryptModuleImpl_ == nullptr) {
                DRM_ERR_LOG("mediaDecryptModuleImpl_ == nullptr.");
                return nullptr;
            }
            decryptModuleNapi->mediaDecryptModuleImpl_->RequireSecureDecoderModule(mimeType, &statusValue);
            status = napi_get_boolean(env, statusValue, &result);
            if (status != napi_ok) {
                DRM_ERR_LOG("napi_get_boolean call faild!");
                return nullptr;
            }
        } else {
            DRM_ERR_LOG("MediaDecryptModuleNapi DecryptData call Failed!");
            return nullptr;
        }
    } else {
        DRM_ERR_LOG("-Could not able to read response argument!");
        return nullptr;
    }
    DRM_INFO_LOG("MediaDecryptModuleNapi::RequireSecureDecoderModule exit.");
    return result;
}

napi_value MediaDecryptModuleNapi::DecryptData(napi_env env, napi_callback_info info)
{
    napi_value result = nullptr;
    size_t argc = ARGS_FOUR;
    napi_value argv[ARGS_FOUR] = {0};
    napi_value thisVar = nullptr;
    napi_status status;
    IMediaDecryptModuleService::CryptInfo cryptInfo;
    napi_value type_property;
    napi_value keyId_property;
    napi_value iv_property;
    napi_value pattern_property;
    napi_value subSample_property;
    napi_value subSampleNumber_property;
    bool secureDecodrtState = false;
    int32_t type = 0;
    bool isTypeArray;
    void *keyId = nullptr;
    size_t keyIdLen;
    size_t offset;
    napi_value arraybuffer = nullptr;
    napi_typedarray_type array_type;
    void *iv = nullptr;
    size_t ivLen;
    napi_value pattern_property_encryptBlocks;
    napi_value pattern_property_skipBlocks;
    int32_t subSampleLen = 0;
    napi_value jsClearHeaderLen;
    napi_value jsPayLoadLen;
    int32_t srcBuffer = 0;
    int32_t dstBuffer = 0;

    DRM_NAPI_GET_JS_ARGS(env, info, argc, argv, thisVar);
    napi_get_undefined(env, &result);
    NAPI_ASSERT(env, argc <= ARGS_FOUR, "requires 4 parameters maximum");
    status = napi_get_value_bool(env, argv[PARAM0], &secureDecodrtState);
    if (status != napi_ok) {
        DRM_ERR_LOG("Could not able to read secureDecodrtState argument!");
        return nullptr;
    }
    napi_get_named_property(env, argv[PARAM1], "type", &type_property);
    napi_get_value_int32(env, type_property, &type);
    if (type == 0) {
        cryptInfo.type = IMediaDecryptModuleService::ALGTYPE_UNENCRYPTED;
        DRM_DEBUG_LOG("cryptInfo.type:%{public}d.", (int)cryptInfo.type);
    } else if (type == IMediaDecryptModuleService::ALGTYPE_AES_CTR) {
        cryptInfo.type = IMediaDecryptModuleService::ALGTYPE_AES_CTR;
        DRM_DEBUG_LOG("cryptInfo.type:%{public}d.", (int)cryptInfo.type);
    } else if (type == IMediaDecryptModuleService::ALGTYPE_AES_WV) {
        cryptInfo.type = IMediaDecryptModuleService::ALGTYPE_AES_WV;
        DRM_DEBUG_LOG("cryptInfo.type:%{public}d.", (int)cryptInfo.type);
    } else if (type == IMediaDecryptModuleService::ALGTYPE_AES_CBC) {
        cryptInfo.type = IMediaDecryptModuleService::ALGTYPE_AES_CBC;
        DRM_DEBUG_LOG("cryptInfo.type:%{public}d.", (int)cryptInfo.type);
    } else if (type == IMediaDecryptModuleService::ALGTYPE_SM4_CBC) {
        cryptInfo.type = IMediaDecryptModuleService::ALGTYPE_SM4_CBC;
        DRM_DEBUG_LOG("cryptInfo.type:%{public}d.", (int)cryptInfo.type);
    }

    napi_get_named_property(env, argv[PARAM1], "keyId", &keyId_property);
    napi_is_typedarray(env, keyId_property, &isTypeArray);
    if (!isTypeArray) {
        DRM_ERR_LOG("argv[PARAM1] keyId is not array!");
        return nullptr;
    }
    napi_get_typedarray_info(env, keyId_property, &array_type, &keyIdLen, &keyId, &arraybuffer, &offset);
    if (keyId == nullptr) {
        DRM_ERR_LOG("napi_get_typedarray_info faild!");
        return nullptr;
    }
    uint8_t* keyIdPtr = reinterpret_cast<uint8_t*>(keyId);
    std::vector<uint8_t> keyIdVec(keyIdPtr, keyIdPtr + keyIdLen);
    cryptInfo.keyId.assign(keyIdVec.begin(), keyIdVec.end());

    napi_get_named_property(env, argv[PARAM1], "iv", &iv_property);
    napi_is_typedarray(env, iv_property, &isTypeArray);
    if (!isTypeArray) {
        DRM_ERR_LOG("argv[PARAM1] iv is not array!");
        return nullptr;
    }
    napi_get_typedarray_info(env, iv_property, &array_type, &ivLen, &iv, &arraybuffer, &offset);
    if (iv == nullptr) {
        DRM_ERR_LOG("napi_get_typedarray_info faild!");
        return nullptr;
    }
    uint8_t* ivPtr = reinterpret_cast<uint8_t*>(iv);
    std::vector<uint8_t> ivVec(ivPtr, ivPtr + ivLen);
    cryptInfo.iv.assign(ivVec.begin(), ivVec.end());

    napi_get_named_property(env, argv[PARAM1], "pattern", &pattern_property);
    napi_get_named_property(env, pattern_property, "encryptBlocks", &pattern_property_encryptBlocks);
    napi_get_named_property(env, pattern_property, "skipBlocks", &pattern_property_skipBlocks);
    napi_get_value_int32(env, pattern_property_encryptBlocks, &cryptInfo.pattern.encryptBlocks);
    napi_get_value_int32(env, pattern_property_skipBlocks, &cryptInfo.pattern.skipBlocks);
    napi_get_named_property(env, argv[PARAM1], "subSampleNumber", &subSampleNumber_property);
    napi_get_value_int32(env, subSampleNumber_property, &subSampleLen);
    napi_get_named_property(env, argv[PARAM1], "subSample", &subSample_property);
    for (uint32_t i = 0; i < subSampleLen; i++) {
        napi_value jsSubSample;
        IMediaDecryptModuleService::SubSample subSampleTmp = {0};
        napi_get_element(env, subSample_property, i, &jsSubSample);
        napi_get_named_property(env, jsSubSample, "clearHeaderLen", &jsClearHeaderLen);
        napi_get_named_property(env, jsSubSample, "payLoadLen", &jsPayLoadLen);
        napi_get_value_int32(env, jsClearHeaderLen, &subSampleTmp.clearHeaderLen);
        napi_get_value_int32(env, jsPayLoadLen, &subSampleTmp.payLoadLen);
        cryptInfo.subSample.push_back(subSampleTmp);
    }
    status = napi_get_value_int32(env, argv[PARAM2], &srcBuffer);
    if (status != napi_ok) {
        DRM_ERR_LOG("Could not able to read srcBuffer argument!");
        return nullptr;
    }
    status = napi_get_value_int32(env, argv[PARAM3], &dstBuffer);
    if (status != napi_ok) {
        DRM_ERR_LOG("Could not able to read dstBuffer argument!");
        return nullptr;
    }
    MediaDecryptModuleNapi* decryptModuleNapi = nullptr;
    status = napi_unwrap(env, thisVar, reinterpret_cast<void**>(&decryptModuleNapi));
    if (status == napi_ok && decryptModuleNapi != nullptr) {
        if (decryptModuleNapi->mediaDecryptModuleImpl_ != nullptr) {
            decryptModuleNapi->mediaDecryptModuleImpl_->DecryptData(secureDecodrtState, cryptInfo, srcBuffer,
                dstBuffer);
        } else {
            DRM_ERR_LOG("mediaDecryptModuleImpl_ == nullptr.");
            return nullptr;
        }
    } else {
        DRM_ERR_LOG("MediaDecryptModuleNapi DecryptData call Failed!");
    }
    return result;
}
} // DrmStandardr
} // OHOS