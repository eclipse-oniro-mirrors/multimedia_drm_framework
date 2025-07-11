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

#ifndef OHOS_DRM_MEDIA_KEY_SYSTEM_TAIHE_H
#define OHOS_DRM_MEDIA_KEY_SYSTEM_TAIHE_H

#include "ohos.multimedia.drm.proj.hpp"
#include "ohos.multimedia.drm.impl.hpp"
#include "taihe/runtime.hpp"
#include "stdexcept"
#include "drm_log.h"
#include <map>
#include "media_key_system_impl.h"
#include "media_key_system_factory_impl.h"
#include "drm_error_code.h"
#include "drm_error_code_taihe.h"
#include "drm_taihe_utils.h"
#include "media_key_system_callback_taihe.h"

namespace ANI {
namespace DrmStandard {
using namespace taihe;
using namespace ohos::multimedia::drm;

class MediaKeySystemImpl {
    public:
    MediaKeySystemImpl();
    ~MediaKeySystemImpl();

    string GetConfigurationString(string_view configName);
    void SetConfigurationString(string_view configName, string_view value);
    array<uint8_t> GetConfigurationByteArray(string_view configName);
    void SetConfigurationByteArray(string_view configName, array_view<uint8_t> value);
    array<StatisticKeyValue> GetStatistics();
    ContentProtectionLevel GetMaxContentProtectionLevel();
    ProvisionRequest GenerateKeySystemRequestSync();
    void ProcessKeySystemResponseSync(array_view<uint8_t> response);
    CertificateStatus GetCertificateStatus();
    static OHOS::DrmStandard::DrmTaiheErrCode DrmInnerErrToTaiheErrAPI12(OHOS::DrmStandard::DrmInnerErrCode errCode);
    optional<MediaKeySession> CreateMediaKeySessionWithLevel(ContentProtectionLevel level);
    optional<MediaKeySession> CreateMediaKeySession();
    array<array<uint8_t>> GetOfflineMediaKeyIds();
    OfflineMediaKeyStatus GetOfflineMediaKeyStatus(array_view<uint8_t> mediaKeyId);
    void ClearOfflineMediaKeys(array_view<uint8_t> mediaKeyId);
    void Destroy();
    void OnKeySystemRequired(callback_view<void(EventInfo const&)> callback);
    void SaveEventCallbackReferrence(const std::string eventType, std::shared_ptr<AutoRef> callbackPair);
    void OffKeySystemRequired(optional_view<callback<void(
        EventInfo const&)>> callback);
    void ClearEventCallbackReferrence(const std::string eventType);

    friend string GetMediaKeySystemUuid(string_view name);
    friend array<MediaKeySystemDescription> GetMediaKeySystems();
    friend optional<MediaKeySystem> CreateMediaKeySystem(string_view name);
    friend bool IsMediaKeySystemSupportedWithThreeArgs(string_view name, string_view mimeType,
        ContentProtectionLevel level);
    friend bool IsMediaKeySystemSupportedWithTwoArgs(string_view name, string_view mimeType);
    friend bool IsMediaKeySystemSupported(string_view name);
private:
    OHOS::sptr<OHOS::DrmStandard::MediaKeySystemImpl> mediaKeySystemImpl_;
    TaiheProvisionRequest provisionRequest;
    std::mutex mutex_;
    static thread_local OHOS::sptr<OHOS::DrmStandard::MediaKeySystemImpl> sMediaKeySystemTaihe_;
    OHOS::sptr<MediaKeySystemCallbackTaihe> mediaKeySystemCallbackTaihe_;
};
} // namespace DrmStandard
} // namespace ANI
#endif // OHOS_DRM_MEDIA_KEY_SYSTEM_TAIHE_H_
