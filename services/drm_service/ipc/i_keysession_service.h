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

#ifndef OHOS_DRM_IKEY_SESSION_H_
#define OHOS_DRM_IKEY_SESSION_H_

#include "i_mediadecryptmodule_service.h"
#include "i_keysession_service_callback.h"
#include "iremote_broker.h"

namespace OHOS {
namespace DrmStandard {
class IKeySessionService : public IRemoteBroker {
public:
    enum ListenerType {
        LISTENER_DRM_EVENT = 200,
        LISTENER_PROVISION_REQUIRED = 201,
        LISTENER_KEY_NEEDED = 202,
        LISTENER_KEY_EXPIRED = 203,
        LISTENER_VENDOR_DEFINED = 204,
        LISTENER_KEYSESSION_RECLAIMED = 205,
        LISTENER_EXPIRATION_UPDATE = 206,
        LISTENER_KEY_CHANGE = 207,
        LISTENER_KEYSESSION_LOSE = 208,
    };

    struct KeyValue {
        std::string name;
        std::string value;
    };

    enum KeyType {
        KEYTYPE_OFFLINE = 0,
        KEYTYPE_ONLINE,
    };

    enum OfflineKeyState {
        OFFLINEKEYSTATE_UNKNOWN = 0,
        OFFLINEKEYSTATE_USABLE = 1,
        OFFLINEKEYSTATE_INACTIVE = 2,
    };

    enum RequestType {
        REQUEST_TYPE_UNKNOWN = 0,
        REQUEST_TYPE_INITIAL = 1,
        REQUEST_TYPE_RENEWAL = 2,
        REQUEST_TYPE_RELEASE = 3,
        REQUEST_TYPE_NONE = 4,
        REQUEST_TYPE_UPDATE = 5,
        REQUEST_TYPE_DOWNLOADCERT = 6,
    };

    enum SecurityLevel {
        SECURITY_LEVEL_UNKNOWN = 0,
        SECURITY_LEVEL_SW_CRYPTO = 1,
        SECURITY_LEVEL_SW_DECODE = 2,
        SECURITY_LEVEL_HW_CRYPTO = 3,
        SECURITY_LEVEL_HW_DECODE = 4,
        SECURITY_LEVEL_HW_ALL = 5,
        SECURITY_LEVEL_MAX = 6,
    };

    struct DrmInfo {
        KeyType keyType;
        std::string mimeType;
        std::vector<uint8_t> indexInfo;
    };

    struct LicenseInfo {
        RequestType requestType;
        std::vector<uint8_t> mData;
        std::string mDefaultURL;
    };

    virtual ~IKeySessionService() = default;
    virtual int32_t Release() = 0;
    virtual int32_t CreateMediaDecryptModule(sptr<IMediaDecryptModuleService> &decryptModule) = 0;
    virtual int32_t GenerateLicenseRequest(DrmInfo &drmInfo, LicenseInfo &licenseInfo) = 0;
    virtual int32_t ProcessLicenseResponse(std::vector<uint8_t> &keyId, std::vector<uint8_t> &licenseResponse) = 0;
    virtual int32_t GenerateOfflineReleaseRequest(std::vector<uint8_t> &keyId,
        std::vector<uint8_t> &releaseRequest) = 0;
    virtual int32_t ProcessOfflineReleaseResponse(std::vector<uint8_t> &keyId,
        std::vector<uint8_t> &releaseReponse) = 0;
    virtual int32_t CheckLicenseStatus(std::vector<KeyValue> &infoMap) = 0;
    virtual int32_t RestoreOfflineKeys(std::vector<uint8_t> &keyId) = 0;
    virtual int32_t RemoveOfflineKeys(std::vector<uint8_t> &keyId) = 0;
    virtual int32_t GetOfflineKeyIds(std::vector<std::vector<uint8_t>> &keyIds) = 0;
    virtual int32_t RemoveLicenses() = 0;
    virtual int32_t GetOfflineKeyState(std::vector<uint8_t> &keyId, IKeySessionService::OfflineKeyState &state) = 0;
    virtual int32_t SetKeySessionServiceCallback(sptr<IKeySessionServiceCallback> &callback) = 0;
    DECLARE_INTERFACE_DESCRIPTOR(u"IKeySessionService");
};
} // DrmStandard
} // OHOS


#endif // OHOS_DRM_IKEY_SESSION_H_