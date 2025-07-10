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

#ifndef OHOS_DRM_KEY_SESSION_TAIHE_H
#define OHOS_DRM_KEY_SESSION_TAIHE_H

#include <refbase.h>
#include "ohos.multimedia.drm.proj.hpp"
#include "ohos.multimedia.drm.impl.hpp"
#include "taihe/runtime.hpp"
#include "stdexcept"
#include "key_session_impl.h"
#include "drm_log.h"
#include "drm_error_code_taihe.h"
#include "drm_taihe_utils.h"
#include "key_session_callback_taihe.h"

namespace ANI {
namespace DrmStandard {
using namespace taihe;
using namespace ohos::multimedia::drm;

class MediaKeySessionImpl {
public:
    MediaKeySessionImpl();
    ~MediaKeySessionImpl();

    int64_t GetMediaKeySessionNative();
    void SetMediaKeySessionNative(int64_t mediaKeySessionNative);
    MediaKeyRequest GenerateMediaKeyRequestSync(string_view mimeType, array_view<uint8_t> initData,
        int32_t mediaKeyType, optional_view<array<OptionsData>> options);
    array<uint8_t> ProcessMediaKeyResponseSync(array_view<uint8_t> response);
    array<MediaKeyStatus> CheckMediaKeyStatus();
    void ClearMediaKeys();
    array<uint8_t> GenerateOfflineReleaseRequestSync(array_view<uint8_t> mediaKeyId);
    void ProcessOfflineReleaseResponseSync(array_view<uint8_t> mediaKeyId, array_view<uint8_t> response);
    void RestoreOfflineMediaKeysSync(array_view<uint8_t> mediaKeyId);

    ContentProtectionLevel GetContentProtectionLevel();
    bool RequireSecureDecoderModule(string_view mimeType);
    void Destroy();
    static optional<MediaKeySession> CreateMediaKeySession(
        OHOS::sptr<OHOS::DrmStandard::MediaKeySessionImpl> keySessionImpl);
    array<MediaKeyStatus>
        vectorToJsArray(std::map<std::string, std::string> &licenseStatus);

    void OnKeyRequired(callback_view<void(EventInfo const&)> callback);
    void OnKeyExpired(callback_view<void(EventInfo const&)> callback);
    void OnVendorDefined(callback_view<void(EventInfo const&)> callback);
    void OnExpirationUpdate(callback_view<void(EventInfo const&)> callback);
    void OnKeysChange(callback_view<void(
        array_view<KeysInfo>, bool)> callback);

    void OffKeyRequired(optional_view<callback<void(
        EventInfo const&)>> callback);
    void OffKeysChange(optional_view<callback<void(
        array_view<KeysInfo>, bool)>> callback);
    void OffKeyExpired(optional_view<callback<void(
        EventInfo const&)>> callback);
    void OffVendorDefined(optional_view<callback<void(
        EventInfo const&)>> callback);
    void OffExpirationUpdate(optional_view<callback<void(
        EventInfo const&)>> callback);

    void SetEventCallbackReference(const std::string eventType, std::shared_ptr<AutoRef> callbackPair);
    void ClearEventCallbackReference(const std::string eventType);
    bool CheckMediaKeySessionStatus(MediaKeySessionImpl *keySessionTaihe);
    MediaKeyRequest getEmptyMediaKeyRequest();
    array<MediaKeyStatus> getEmptyMediaKeyStatusArray();
    array<uint8_t> getEmptyArray();
private:
    OHOS::sptr<OHOS::DrmStandard::MediaKeySessionImpl> keySessionImpl_;
    static thread_local OHOS::sptr<OHOS::DrmStandard::MediaKeySessionImpl> sMediaKeySessionImpl_;
    OHOS::sptr<ANI::DrmStandard::MediaKeySessionCallbackTaihe> keySessionCallbackTaihe_;
    int64_t mediaKeySessionNative_;
    std::mutex mutex_;
};
} // namespace DrmStandard
} // namespace ANI
#endif // OHOS_DRM_KEY_SESSION_TAIHE_H_
