/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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
#ifndef OHOS_DRM_MediaDecryptModuleService_H_
#define OHOS_DRM_MediaDecryptModuleService_H_

#include <iostream>
#include <refbase.h>
#include "drm_host_manager.h"
#include "drm_log.h"
#include "drm_error_code.h"
#include "media_decrypt_module_service_stub.h"

namespace OHOS {
namespace DrmStandard {
class MediaDecryptModuleService : public MediaDecryptModuleServiceStub {
public:
    MediaDecryptModuleService();
    ~MediaDecryptModuleService();
    int32_t Release() override;
    int32_t DecryptData(bool secureDecodrtState, IMediaDecryptModuleService::CryptInfo &cryptInfo, uint64_t srcBuffer,
        uint64_t dstBuffer) override;
    int32_t RequireSecureDecoderModule(std::string &mimeType, bool *status) override;
private:
};
} // DrmStandard
} // OHOS
#endif // OHOS_DRM_MediaDecryptModuleService_H_