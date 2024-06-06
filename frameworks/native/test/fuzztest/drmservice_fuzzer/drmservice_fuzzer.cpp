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


#include <string>
#include <refbase.h>
#include <securec.h>
#include "drm_log.h"
#include "ipc_skeleton.h"
#include "mediakeysystem_service.h"
#include "mediakeysystemfactory_service.h"
#include "key_session_service.h"
#include "drmservice_fuzzer.h"
#include "remote_request_code.h"
#include "iremote_proxy.h"

using namespace std;
using namespace OHOS;
using namespace DrmStandard;

namespace OHOS {
namespace DrmStandard {
const int32_t MEDIA_KEY_SYSTEM_SERVICE_ID = 3012;
const int32_t DATAMAXSIZE = 12288;
const std::u16string MEDIA_KEY_SYSTEM_FACTORY_TOKEN = u"IMediaKeySystemSystemFactoryService";
const std::u16string MEDIA_KEY_SYSTEM_TOKEN = u"IMediaKeySystemSystemService";
const std::u16string MEDIA_KEY_SESSION_TOKEN = u"IMediaKeySessionService";
const std::u16string MEDIA_DECRYPT_MODULE_TOKEN = u"IMediaDecryptModuleService";
DrmServiceNdkFuzzer::DrmServiceNdkFuzzer() {}

DrmServiceNdkFuzzer::~DrmServiceNdkFuzzer() {}


bool DrmServiceNdkFuzzer::DrmserviceIsMediaKeySystemSupportedTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemFactoryService> mediaKeySystemFactoryServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_FACTORY_TOKEN);
    data.WriteInt32(ARGS_NUM_TWO);
    std::string uuid(reinterpret_cast<const char *>(rawData), size);
    std::string mimeType(reinterpret_cast<const char *>(rawData), size);
    data.WriteString(uuid);
    data.WriteString(mimeType);
    mediaKeySystemFactoryServicePtr->OnRemoteRequest(MEDIA_KEY_SYSTEM_FACTORY_IS_MEDIA_KEY_SYSTEM_SURPPORTED, data,
        reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceCreateMediaKeySystemTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemFactoryService> mediaKeySystemFactoryServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_FACTORY_TOKEN);
    std::string name(reinterpret_cast<const char *>(rawData), size);
    data.WriteString(name);
    mediaKeySystemFactoryServicePtr->OnRemoteRequest(MEDIA_KEY_SYSTEM_FACTORY_CREATE_MEDIA_KEYSYSTEM, data, reply,
        option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceGetMediaKeySystemsTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemFactoryService> mediaKeySystemFactoryServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_FACTORY_TOKEN);
    mediaKeySystemFactoryServicePtr->OnRemoteRequest(MEDIA_KEY_SYSTEM_FACTORY_GET_MEDIA_KEYSYSTEM_NAME, data, reply,
        option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceGetMediaKeySystemUuidTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemFactoryService> mediaKeySystemFactoryServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_FACTORY_TOKEN);
    std::string name(reinterpret_cast<const char *>(rawData), size);
    data.WriteString(name);
    mediaKeySystemFactoryServicePtr->OnRemoteRequest(MEDIA_KEY_SYSTEM_FACTORY_GET_MEDIA_KEYSYSTEM_UUID, data, reply,
        option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceCreateMediaKeySessionTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_TOKEN);
    int32_t securityLevel = *reinterpret_cast<const int32_t *>(rawData);
    data.WriteInt32(securityLevel);
    mediaKeySystemServicePtr->OnRemoteRequest(MEDIA_KEY_SYSTEM_CREATE_KEY_SESSION, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceGenerateKeySystemRequestTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_TOKEN);
    mediaKeySystemServicePtr->OnRemoteRequest(MEDIA_KEY_SYSTEM_GENERATE_KEYSYSTEM_REQUEST, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceProcessKeySystemResponseTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    int32_t responsesize = *reinterpret_cast<const int32_t *>(rawData);
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_TOKEN);
    data.WriteInt32(responsesize);
    uint8_t *response = rawData;
    data.WriteBuffer(response, responsesize);
    mediaKeySystemServicePtr->OnRemoteRequest(MEDIA_KEY_SYSTEM_PROCESS_KEYSYSTEM_RESPONSE, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceGenerateSetConfigurationStringTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_TOKEN);
    std::string configName(reinterpret_cast<const char *>(rawData), size);
    std::string value(reinterpret_cast<const char *>(rawData), size);
    data.WriteString(configName);
    data.WriteString(value);
    mediaKeySystemServicePtr->OnRemoteRequest(MEDIA_KEY_SYSTEM_SETCONFIGURATION_STRING, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceSetConfigurationStringTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_TOKEN);
    std::string configName(reinterpret_cast<const char *>(rawData), size);
    std::string value(reinterpret_cast<const char *>(rawData), size);
    data.WriteString(configName);
    data.WriteString(value);
    mediaKeySystemServicePtr->OnRemoteRequest(MEDIA_KEY_SYSTEM_SETCONFIGURATION_STRING, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceGetConfigurationStringTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_TOKEN);
    std::string configName(reinterpret_cast<const char *>(rawData), size);
    data.WriteString(configName);
    mediaKeySystemServicePtr->OnRemoteRequest(MEDIA_KEY_SYSTEM_GETCONFIGURATION_STRING, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceSetConfigurationByteArrayTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_TOKEN);
    std::string configName(reinterpret_cast<const char *>(rawData), size);
    data.WriteString(configName);
    data.WriteBuffer(rawData, size);
    mediaKeySystemServicePtr->OnRemoteRequest(MEDIA_KEY_SYSTEM_SETCONFIGURATION_BYTEARRAY, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceGetConfigurationByteArrayTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_TOKEN);
    std::string configName(reinterpret_cast<const char *>(rawData), size);
    data.WriteString(configName);
    mediaKeySystemServicePtr->OnRemoteRequest(MEDIA_KEY_SYSTEM_GETCONFIGURATION_BYTEARRAY, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceGetStatisticsTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_TOKEN);
    mediaKeySystemServicePtr->OnRemoteRequest(MEDIA_KEY_SYSTEM_GETMETRIC, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceReleaseTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_TOKEN);
    mediaKeySystemServicePtr->OnRemoteRequest(MEDIA_KEY_SYSTEM_RELEASE, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceGetMaxContentProtectionLevelTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_TOKEN);
    mediaKeySystemServicePtr->OnRemoteRequest(MEDIA_KEY_SYSTEM_GETMAXSECURITYLEVEL, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceGetCertificateStatusTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_TOKEN);
    mediaKeySystemServicePtr->OnRemoteRequest(MEDIA_KEY_SYSTEM_GETCERTIFICATESTATUS, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceGetOfflineMediaKeyIdsTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_TOKEN);
    mediaKeySystemServicePtr->OnRemoteRequest(MEDIA_KEY_SYSTEM_GET_OFFLINELICENSEIDS, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceGetOfflineMediaKeyStatusTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInt32(size);
    data.WriteBuffer(rawData, size);
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_TOKEN);
    mediaKeySystemServicePtr->OnRemoteRequest(MEDIA_KEY_SYSTEM_GET_OFFLINEKEY_STATUS, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceClearOfflineMediaKeysTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_TOKEN);
    data.WriteInt32(size);
    data.WriteBuffer(rawData, size);
    mediaKeySystemServicePtr->OnRemoteRequest(MEDIA_KEY_SYSTEM_REMOVE_OFFLINELICENSE, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceCreateMediaDecryptModuleTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySessionService> mediaKeySessionService)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SESSION_TOKEN);
    mediaKeySessionService->OnRemoteRequest(CREATE_MEDIA_DECRYPT_MODULE, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceGenerateMediaKeyRequestTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySessionService> mediaKeySessionService)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SESSION_TOKEN);
    data.WriteInt32(size);
    std::string mimeType(reinterpret_cast<const char *>(rawData), size);
    data.WriteString(mimeType);
    data.WriteString(mimeType);
    data.WriteInt32(size);
    data.WriteString(mimeType);
    data.WriteInt32(size);
    data.WriteBuffer(rawData, size);
    mediaKeySessionService->OnRemoteRequest(MEDIA_KEY_SESSION_GENERATE_LICENSE_REQUEST, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceGenerateOfflineReleaseRequestTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySessionService> mediaKeySessionService)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SESSION_TOKEN);
    data.WriteInt32(size);
    data.WriteBuffer(rawData, size);
    mediaKeySessionService->OnRemoteRequest(MEDIA_KEY_SESSION_GENERATE_OFFLINE_RELEASE_REQUEST, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceProcessOfflineReleaseResponseTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySessionService> mediaKeySessionService)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SESSION_TOKEN);
    data.WriteInt32(size);
    data.WriteBuffer(rawData, size);
    data.WriteInt32(size);
    data.WriteBuffer(rawData, size);
    mediaKeySessionService->OnRemoteRequest(MEDIA_KEY_SESSION_PROCESS_OFFLINE_RELEASE_RESPONSE, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceCheckMediaKeyStatusTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySessionService> mediaKeySessionService)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SESSION_TOKEN);
    mediaKeySessionService->OnRemoteRequest(MEDIA_KEY_SESSION_GENERATE_CHECK_LICENSE_STATUS, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceRestoreOfflineMediaKeysTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySessionService> mediaKeySessionService)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SESSION_TOKEN);
    data.WriteInt32(size);
    data.WriteBuffer(rawData, size);
    mediaKeySessionService->OnRemoteRequest(MEDIA_KEY_SESSION_RESTORE_OFFLINEKEYS, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceClearMediaKeysTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySessionService> mediaKeySessionService)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SESSION_TOKEN);
    mediaKeySessionService->OnRemoteRequest(MEDIA_KEY_SESSION_REMOVE_LICENSE, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceRequireSecureDecoderModuleTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySessionService> mediaKeySessionService)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SESSION_TOKEN);
    std::string mimeType(reinterpret_cast<const char *>(rawData), size);
    data.WriteString(mimeType);
    mediaKeySessionService->OnRemoteRequest(MEDIA_KEY_SESSION_REQUIRE_SECURE_DECODER, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceGetContentProtectionLevelTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySessionService> mediaKeySessionService)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SESSION_TOKEN);
    mediaKeySessionService->OnRemoteRequest(MEDIA_KEY_SESSION_GETSECURITYLEVEL, data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceDecryptMediaDataTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaDecryptModuleService> mediaDecryptService)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_DECRYPT_MODULE_TOKEN);
    mediaDecryptService->OnRemoteRequest(DECRYPT_MODULE_DECRYPT_DATA, data, reply, option);
    return true;
}
} // namespace DrmStandard

bool FuzzSystemFactoryNdk(uint8_t *data, size_t size)
{
    if (data == nullptr) {
        return true;
    }
    DrmServiceNdkFuzzer testMediaSystemFactory;
    /* KeySystemFactory */
    /* KeySystem */
    sptr<IMediaKeySystem> hdiMediaKeySystem = new (std::nothrow) IMediaKeySystemMock();
    std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr =
        std::make_shared<MediaKeySystemService>(hdiMediaKeySystem);
    testMediaSystemFactory.DrmserviceCreateMediaKeySessionTest(data, size, mediaKeySystemServicePtr);
    testMediaSystemFactory.DrmserviceGenerateKeySystemRequestTest(data, size, mediaKeySystemServicePtr);
    testMediaSystemFactory.DrmserviceGenerateSetConfigurationStringTest(data, size, mediaKeySystemServicePtr);
    testMediaSystemFactory.DrmserviceSetConfigurationStringTest(data, size, mediaKeySystemServicePtr);
    testMediaSystemFactory.DrmserviceGetConfigurationStringTest(data, size, mediaKeySystemServicePtr);
    testMediaSystemFactory.DrmserviceSetConfigurationByteArrayTest(data, size, mediaKeySystemServicePtr);
    testMediaSystemFactory.DrmserviceGetConfigurationByteArrayTest(data, size, mediaKeySystemServicePtr);
    testMediaSystemFactory.DrmserviceGetStatisticsTest(data, size, mediaKeySystemServicePtr);
    testMediaSystemFactory.DrmserviceGetMaxContentProtectionLevelTest(data, size, mediaKeySystemServicePtr);
    testMediaSystemFactory.DrmserviceGetCertificateStatusTest(data, size, mediaKeySystemServicePtr);
    testMediaSystemFactory.DrmserviceGetOfflineMediaKeyIdsTest(data, size, mediaKeySystemServicePtr);
    testMediaSystemFactory.DrmserviceGetOfflineMediaKeyStatusTest(data, size, mediaKeySystemServicePtr);
    testMediaSystemFactory.DrmserviceClearOfflineMediaKeysTest(data, size, mediaKeySystemServicePtr);
    testMediaSystemFactory.DrmserviceReleaseTest(data, size, mediaKeySystemServicePtr);
    /* MediaKeySession */
    sptr<IMediaKeySession> hdiMediaKeySession = new (std::nothrow) IMediaKeySessionMock();
    std::shared_ptr<MediaKeySessionService> mediaKeySessionService =
        std::make_shared<MediaKeySessionService>(hdiMediaKeySession);
    testMediaSystemFactory.DrmserviceCreateMediaDecryptModuleTest(data, size, mediaKeySessionService);
    testMediaSystemFactory.DrmserviceGenerateMediaKeyRequestTest(data, size, mediaKeySessionService);
    testMediaSystemFactory.DrmserviceGenerateOfflineReleaseRequestTest(data, size, mediaKeySessionService);
    testMediaSystemFactory.DrmserviceProcessOfflineReleaseResponseTest(data, size, mediaKeySessionService);
    testMediaSystemFactory.DrmserviceCheckMediaKeyStatusTest(data, size, mediaKeySessionService);
    testMediaSystemFactory.DrmserviceRestoreOfflineMediaKeysTest(data, size, mediaKeySessionService);
    testMediaSystemFactory.DrmserviceClearMediaKeysTest(data, size, mediaKeySessionService);
    testMediaSystemFactory.DrmserviceRequireSecureDecoderModuleTest(data, size, mediaKeySessionService);
    testMediaSystemFactory.DrmserviceGetContentProtectionLevelTest(data, size, mediaKeySessionService);
    /* DecryptModule */
    sptr<IMediaDecryptModule> hdiDecryptModule = new (std::nothrow) IMediaDecryptModuleMock();
    std::shared_ptr<MediaDecryptModuleService> mediaDecryptService =
        std::make_shared<MediaDecryptModuleService>(hdiDecryptModule);
    testMediaSystemFactory.DrmserviceDecryptMediaDataTest(data, size, mediaDecryptService);
    return true;
}
} // namesapce OHOS

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(uint8_t *data, size_t size)
{
    /* Run your code on data */
    uint8_t rawData[DATAMAXSIZE] = { 0 };
    int32_t ret = memcpy_s(rawData, DATAMAXSIZE - 1, data, size);
    if (ret != 0) {
        return -1;
    }
    OHOS::FuzzSystemFactoryNdk(rawData, size);
    return 0;
}