/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#ifndef OHOS_DRM_MEDIA_KEY_SYSTEM_CALLBACK_TAIHE_H
#define OHOS_DRM_MEDIA_KEY_SYSTEM_CALLBACK_TAIHE_H

#include <map>
#include "media_key_system_impl.h"
#include "drm_ani_common.h"
#include "drm_taihe_utils.h"
#include "event_handler.h"

namespace ANI {
namespace DrmStandard {

class MediaKeySystemCallbackTaihe : public OHOS::DrmStandard::MediaKeySystemImplCallback {
public:
    MediaKeySystemCallbackTaihe();
    virtual ~MediaKeySystemCallbackTaihe();
    void SetCallbackReference(const std::string eventType, std::shared_ptr<AutoRef> callbackPair);
    void ClearCallbackReference(const std::string eventType);
    void SendEvent(const std::string &event, int32_t extra, const std::vector<uint8_t> &data) override;
private:
    struct MediaKeySystemTaiheCallback {
        std::shared_ptr<AutoRef> callback = nullptr;
        std::string callbackName = "unknown";
        DrmEventParame eventParame;
    };
    void OnTaiheCallbackInterrupt(std::unique_ptr<MediaKeySystemTaiheCallback> &taiheCb);

    std::shared_ptr<OHOS::AppExecFwk::EventHandler> mainHandler_ = nullptr;
    std::mutex mutex_;
    std::map<std::string, std::shared_ptr<AutoRef>> callbackMap_;
};
} // namespace DrmStandard
} // namespace OHOS
#endif // OHOS_DRM_MEDIA_KEY_SYSTEM_CALLBACK_TAIHE_H_