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

#ifndef OHOS_DRM_KEY_SESSION_SERVICE_H
#define OHOS_DRM_KEY_SESSION_SERVICE_H

#include <refbase.h>
#include <iostream>
#include "key_session_service_stub.h"
#include "drm_log.h"
#include "drm_napi_utils.h"
#include "drm_host_manager.h"
#include "media_decrypt_module_service.h"

namespace OHOS {
namespace DrmStandard {
class IKeySessionServiceOperatorsCallback;
class KeySessionService : public KeySessionServiceStub {
public:
    explicit KeySessionService();
    ~KeySessionService();
    int32_t Release();
    int32_t CreateMediaDecryptModule(sptr<IMediaDecryptModuleService> &decryptModule);
    int32_t SetKeySessionServiceOperatorsCallback(wptr<IKeySessionServiceOperatorsCallback> callback);
    int32_t CloseKeySessionServiceByCallback();
    int32_t GenerateLicenseRequest(IKeySessionService::DrmInfo &drmInfo, IKeySessionService::LicenseInfo &licenseInfo);
    int32_t ProcessLicenseResponse(std::vector<uint8_t> &keyId, std::vector<uint8_t> &licenseResponse);
    int32_t GenerateOfflineReleaseRequest(std::vector<uint8_t> &keyId, std::vector<uint8_t> &releaseRequest);
    int32_t ProcessOfflineReleaseResponse(std::vector<uint8_t> &keyId, std::vector<uint8_t> &releaseResponse);
    int32_t CheckLicenseStatus(std::vector<KeyValue> &infoMap);
    int32_t RestoreOfflineKeys(std::vector<uint8_t> &keyId);
    int32_t RemoveOfflineKeys(std::vector<uint8_t> &keyId);
    int32_t GetOfflineKeyIds(std::vector<std::vector<uint8_t>> &keyIds);
    int32_t RemoveLicenses();
    int32_t GetOfflineKeyState(std::vector<uint8_t> &keyId, OfflineKeyState &state);
    int32_t SetKeySessionServiceCallback(sptr<IKeySessionServiceCallback> &callback);
    void OnKeySessionKeyExpiredStatus(const KeyStatus status);
    void OnKeySessionReclaimed(const SessionStatus status);
private:
};

class IKeySessionServiceOperatorsCallback : public virtual RefBase {
public:
    IKeySessionServiceOperatorsCallback() = default;
    virtual ~IKeySessionServiceOperatorsCallback() = default;
    virtual int32_t CloseKeySessionService(sptr<KeySessionService> sessionService) = 0;
};
} // DrmStandard
} // OHOS

#endif // OHOS_DRM_KEY_SESSION_SERVICE_H