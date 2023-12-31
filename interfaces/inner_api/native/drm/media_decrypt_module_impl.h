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
#ifndef OHOS_DRM_MEDIA_DECRYPT_MODULE_IMPL_H_
#define OHOS_DRM_MEDIA_DECRYPT_MODULE_IMPL_H_

#include <cstring>
#include "nocopyable.h"
#include "ipc_skeleton.h"
#include "iservice_registry.h"
#include "i_mediadecryptmodule_service.h"

namespace OHOS {
namespace DrmStandard {
class MediaDecryptModuleImpl : public RefBase {
public:
    explicit MediaDecryptModuleImpl(sptr<IMediaDecryptModuleService> &decryptModule);
    ~MediaDecryptModuleImpl();
    int32_t Release();
    int32_t DecryptMediaData(bool secureDecodrtState, IMediaDecryptModuleService::CryptInfo &cryptInfo,
        uint64_t srcBuffer, uint64_t dstBuffer);

private:
    std::mutex mutex_;
    sptr<OHOS::DrmStandard::IMediaDecryptModuleService> serviceProxy_;
};
} // DrmStandard
} // OHOS

#endif