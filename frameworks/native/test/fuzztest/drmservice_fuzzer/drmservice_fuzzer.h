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


#ifndef DRM_DRMSERVICE_FUZZER_H
#define DRM_DRMSERVICE_FUZZER_H

#include "imedia_key_session.h"
#include "imedia_key_system.h"
#include "imedia_decrypt_module.h"

#define FUZZ_PROJECT_NAME "mediakeysystemndkfactory_fuzzer"

namespace OHOS {
namespace DrmStandard {
class DrmServiceNdkFuzzer : public NoCopyable {
public:
    DrmServiceNdkFuzzer();
    ~DrmServiceNdkFuzzer();
    /* KeySystemFactory */
    bool DrmserviceIsMediaKeySystemSupportedTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySystemFactoryService> mediaKeySystemFactoryServicePtr);
    bool DrmserviceCreateMediaKeySystemTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySystemFactoryService> mediaKeySystemFactoryServicePtr);
    bool DrmserviceGetMediaKeySystemsTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySystemFactoryService> mediaKeySystemFactoryServicePtr);
    bool DrmserviceGetMediaKeySystemUuidTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySystemFactoryService> mediaKeySystemFactoryServicePtr);
    /* KeySystem */
    bool DrmserviceCreateMediaKeySessionTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr);
    bool DrmserviceGenerateKeySystemRequestTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr);
    bool DrmserviceProcessKeySystemResponseTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr);
    bool DrmserviceGenerateSetConfigurationStringTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr);
    bool DrmserviceSetConfigurationStringTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr);
    bool DrmserviceGetConfigurationStringTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr);
    bool DrmserviceSetConfigurationByteArrayTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr);
    bool DrmserviceGetConfigurationByteArrayTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr);
    bool DrmserviceGetStatisticsTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr);
    bool DrmserviceReleaseTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr);
    bool DrmserviceGetMaxContentProtectionLevelTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr);
    bool DrmserviceGetCertificateStatusTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr);
    bool DrmserviceGetOfflineMediaKeyIdsTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr);
    bool DrmserviceGetOfflineMediaKeyStatusTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr);
    bool DrmserviceClearOfflineMediaKeysTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr);
    /* MediaKeySession */
    bool DrmserviceCreateMediaDecryptModuleTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySessionService> mediaKeySessionService);
    bool DrmserviceGenerateMediaKeyRequestTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySessionService> mediaKeySessionService);
    bool DrmserviceGenerateOfflineReleaseRequestTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySessionService> mediaKeySessionService);
    bool DrmserviceProcessOfflineReleaseResponseTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySessionService> mediaKeySessionService);
    bool DrmserviceCheckMediaKeyStatusTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySessionService> mediaKeySessionService);
    bool DrmserviceRestoreOfflineMediaKeysTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySessionService> mediaKeySessionService);
    bool DrmserviceClearMediaKeysTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySessionService> mediaKeySessionService);
    bool DrmserviceRequireSecureDecoderModuleTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySessionService> mediaKeySessionService);
    bool DrmserviceGetContentProtectionLevelTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaKeySessionService> mediaKeySessionService);
    bool DrmserviceDecryptMediaDataTest(uint8_t *rawData, size_t size,
        std::shared_ptr<MediaDecryptModuleService> mediaDecryptService);
};

class IMediaKeySessionMock : public IMediaKeySession {
public:
    ~IMediaKeySessionMock() override = default;

    int32_t GenerateMediaKeyRequest(const OHOS::HDI::Drm::V1_0::MediaKeyRequestInfo &mediaKeyRequestInfo,
        OHOS::HDI::Drm::V1_0::MediaKeyRequest &mediaKeyRequest) override
    {
        // Implement the mock behavior for GenerateMediaKeyRequest
        return HDF_SUCCESS;
    }

    int32_t ProcessMediaKeyResponse(const std::vector<uint8_t> &mediaKeyResponse,
        std::vector<uint8_t> &mediaKeyId) override
    {
        // Implement the mock behavior for ProcessMediaKeyResponse
        return HDF_SUCCESS;
    }

    int32_t CheckMediaKeyStatus(std::map<std::string, std::string> &mediaKeyStatus) override
    {
        // Implement the mock behavior for CheckMediaKeyStatus
        return HDF_SUCCESS;
    }

    int32_t ClearMediaKeys() override
    {
        // Implement the mock behavior for ClearMediaKeys
        return HDF_SUCCESS;
    }

    int32_t GetOfflineReleaseRequest(const std::vector<uint8_t> &mediaKeyId,
        std::vector<uint8_t> &releaseRequest) override
    {
        // Implement the mock behavior for GetOfflineReleaseRequest
        return HDF_SUCCESS;
    }

    int32_t ProcessOfflineReleaseResponse(const std::vector<uint8_t> &mediaKeyId,
        const std::vector<uint8_t> &response) override
    {
        // Implement the mock behavior for ProcessOfflineReleaseResponse
        return HDF_SUCCESS;
    }

    int32_t RestoreOfflineMediaKeys(const std::vector<uint8_t> &mediaKeyId) override
    {
        // Implement the mock behavior for RestoreOfflineMediaKeys
        return HDF_SUCCESS;
    }

    int32_t GetContentProtectionLevel(OHOS::HDI::Drm::V1_0::ContentProtectionLevel &level) override
    {
        // Implement the mock behavior for GetContentProtectionLevel
        return HDF_SUCCESS;
    }

    int32_t RequiresSecureDecoderModule(const std::string &mimeType, bool &required) override
    {
        // Implement the mock behavior for RequiresSecureDecoderModule
        return HDF_SUCCESS;
    }

    int32_t SetCallback(const sptr<OHOS::HDI::Drm::V1_0::IMediaKeySessionCallback> &sessionCallback) override
    {
        // Implement the mock behavior for SetCallback
        return HDF_SUCCESS;
    }

    int32_t GetMediaDecryptModule(sptr<OHOS::HDI::Drm::V1_0::IMediaDecryptModule> &decryptModule) override
    {
        // Implement the mock behavior for GetMediaDecryptModule
        return HDF_SUCCESS;
    }

    int32_t Destroy() override
    {
        // Implement the mock behavior for Destroy
        return HDF_SUCCESS;
    }

    int32_t GetVersion(uint32_t &majorVer, uint32_t &minorVer) override
    {
        majorVer = 1;
        minorVer = 0;
        return HDF_SUCCESS;
    }

    bool IsProxy() override
    {
        return false;
    }

    const std::u16string GetDesc() override
    {
        return u"ohos.hdi.drm.v1_0.IMediaKeySession";
    }
};

class IMediaKeySystemMock : public IMediaKeySystem {
public:
    ~IMediaKeySystemMock() override = default;

    int32_t GetConfigurationString(const std::string &name, std::string &value) override
    {
        value = "mock_value";
        return HDF_SUCCESS;
    }

    int32_t SetConfigurationString(const std::string &name, const std::string &value) override
    {
        return HDF_SUCCESS;
    }

    int32_t GetConfigurationByteArray(const std::string &name, std::vector<uint8_t> &value) override
    {
        value = { 0x01, 0x02, 0x03 };
        return HDF_SUCCESS;
    }

    int32_t SetConfigurationByteArray(const std::string &name, const std::vector<uint8_t> &value) override
    {
        return HDF_SUCCESS;
    }

    int32_t GetStatistics(std::map<std::string, std::string> &statistics) override
    {
        statistics = { { "key1", "value1" }, { "key2", "value2" } };
        return HDF_SUCCESS;
    }

    int32_t GetMaxContentProtectionLevel(OHOS::HDI::Drm::V1_0::ContentProtectionLevel &level) override
    {
        level = OHOS::HDI::Drm::V1_0::ContentProtectionLevel::HW_ENHANCED_SECURE_CRYPTO;
        return HDF_SUCCESS;
    }

    int32_t GenerateKeySystemRequest(std::string &defaultUrl, std::vector<uint8_t> &request) override
    {
        defaultUrl = "mock_url";
        request = { 0x04, 0x05, 0x06 };
        return HDF_SUCCESS;
    }

    int32_t ProcessKeySystemResponse(const std::vector<uint8_t> &response) override
    {
        return HDF_SUCCESS;
    }

    int32_t GetOemCertificateStatus(OHOS::HDI::Drm::V1_0::CertificateStatus &status) override
    {
        status = OHOS::HDI::Drm::V1_0::CertificateStatus::CERT_STATUS_INVALID;
        return HDF_SUCCESS;
    }

    int32_t SetCallback(const sptr<OHOS::HDI::Drm::V1_0::IMediaKeySystemCallback> &systemCallback) override
    {
        return HDF_SUCCESS;
    }

    int32_t CreateMediaKeySession(OHOS::HDI::Drm::V1_0::ContentProtectionLevel level,
        sptr<OHOS::HDI::Drm::V1_0::IMediaKeySession> &keySession) override
    {
        return HDF_SUCCESS;
    }

    int32_t GetOfflineMediaKeyIds(std::vector<std::vector<uint8_t>> &mediaKeyIds) override
    {
        mediaKeyIds = { { 0x07, 0x08, 0x09 }, { 0x0A, 0x0B, 0x0C } };
        return HDF_SUCCESS;
    }

    int32_t GetOfflineMediaKeyStatus(const std::vector<uint8_t> &mediaKeyId,
        OHOS::HDI::Drm::V1_0::OfflineMediaKeyStatus &mediaKeyStatus) override
    {
        mediaKeyStatus = OHOS::HDI::Drm::V1_0::OfflineMediaKeyStatus::OFFLINE_MEDIA_KEY_STATUS_USABLE;
        return HDF_SUCCESS;
    }

    int32_t ClearOfflineMediaKeys(const std::vector<uint8_t> &mediaKeyId) override
    {
        return HDF_SUCCESS;
    }

    int32_t GetOemCertificate(sptr<OHOS::HDI::Drm::V1_0::IOemCertificate> &oemCert) override
    {
        return HDF_SUCCESS;
    }

    int32_t Destroy() override
    {
        return HDF_SUCCESS;
    }

    int32_t GetVersion(uint32_t &majorVer, uint32_t &minorVer) override
    {
        majorVer = 1;
        minorVer = 0;
        return HDF_SUCCESS;
    }

    bool IsProxy() override
    {
        return false;
    }

    const std::u16string GetDesc() override
    {
        return u"mock_descriptor";
    }
};

class IMediaDecryptModuleMock : public IMediaDecryptModule {
public:
    ~IMediaDecryptModuleMock() override = default;

    int32_t DecryptMediaData(bool secure, const OHOS::HDI::Drm::V1_0::CryptoInfo &cryptoInfo,
        const OHOS::HDI::Drm::V1_0::DrmBuffer &srcBuffer, const OHOS::HDI::Drm::V1_0::DrmBuffer &destBuffer) override
    {
        return HDF_SUCCESS;
    }

    int32_t Release() override
    {
        return HDF_SUCCESS;
    }

    int32_t GetVersion(uint32_t &majorVer, uint32_t &minorVer) override
    {
        majorVer = 1;
        minorVer = 0;
        return HDF_SUCCESS;
    }

    bool IsProxy() override
    {
        return false;
    }

    const std::u16string GetDesc() override
    {
        return u"mock_media_decrypt_module";
    }
};
} // namespace DrmStandard
bool FuzzSystemFactoryNdk(uint8_t *data, size_t size);
} // namesapce OHOS

#endif // DRM_MEDIAKEYSYSTEMNDKFACTORY_FUZZER_H