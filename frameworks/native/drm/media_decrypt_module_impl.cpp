/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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
#include "media_decrypt_module_impl.h"
#include "drm_log.h"
#include "drm_error_code.h"

namespace OHOS {
namespace DrmStandard {
MediaDecryptModuleImpl::MediaDecryptModuleImpl(sptr<IMediaDecryptModuleService> &decryptModule)
{
    DRM_DEBUG_LOG("Enter Init MediaDecryptModuleImpl()");
    serviceProxy_ = decryptModule;
}

MediaDecryptModuleImpl::~MediaDecryptModuleImpl()
{
    DRM_DEBUG_LOG("~MediaDecryptModuleImpl");
    if (serviceProxy_ != nullptr) {
        serviceProxy_ = nullptr;
    }
}

int32_t MediaDecryptModuleImpl::Release()
{
    DRM_INFO_LOG("MediaDecryptModuleImpl::Release enter.");
    int errCode = DRM_OK;
    if (serviceProxy_ != nullptr) {
        errCode = serviceProxy_->Release();
        serviceProxy_ = nullptr;
    }
    DRM_INFO_LOG("MediaDecryptModuleImpl::Release exit.");
    return errCode;
}

int32_t MediaDecryptModuleImpl::DecryptMediaData(bool secureDecodrtState,
    IMediaDecryptModuleService::CryptInfo &cryptInfo, uint64_t srcBuffer, uint64_t dstBuffer)
{
    DRM_INFO_LOG("MediaDecryptModuleImpl::DecryptMediaData enter");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaDecryptModuleImpl::DecryptMediaData serviceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = serviceProxy_->DecryptMediaData(secureDecodrtState, cryptInfo, srcBuffer, dstBuffer);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaDecryptModuleImpl::DecryptMediaData failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }

    DRM_INFO_LOG("MediaDecryptModuleImpl::DecryptMediaData exit");
    return DRM_OK;
}
} // DrmStandard
} // OHOS