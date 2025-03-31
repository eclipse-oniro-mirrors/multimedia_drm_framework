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
#include <string_ex.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "drm_log.h"
#include "ipc_skeleton.h"
#include "mediakeysystem_service.h"
#include "mediakeysystemfactory_service.h"
#include "key_session_service.h"
#include "drmservice_fuzzer.h"
#include "iremote_proxy.h"

using namespace std;
using namespace OHOS;
using namespace DrmStandard;

namespace OHOS {
namespace DrmStandard {
const int32_t MEDIA_KEY_SYSTEM_SERVICE_ID = 3012;
const int32_t DATAMAXSIZE = 12288;
const std::u16string MEDIA_KEY_SYSTEM_FACTORY_TOKEN = u"OHOS.DrmStandard.IMediaKeySystemFactoryService";
const std::u16string MEDIA_KEY_SYSTEM_TOKEN = u"OHOS.DrmStandard.IMediaKeySystemService";
const std::u16string MEDIA_KEY_SESSION_TOKEN = u"OHOS.DrmStandard.IMediaKeySessionService";
const std::u16string MEDIA_DECRYPT_MODULE_TOKEN = u"OHOS.DrmStandard.IMediaDecryptModuleService";
const uint32_t COMMAND_IS_MEDIA_KEY_SYSTEM_SUPPORTED_V2 = static_cast<uint32_t>(
    IMediaKeySystemFactoryServiceIpcCode::COMMAND_IS_MEDIA_KEY_SYSTEM_SUPPORTED_IN_STRING_IN_STRING_OUT_BOOLEAN
);
const uint32_t COMMAND_IS_MEDIA_KEY_SYSTEM_SUPPORTED_V3 = static_cast<uint32_t>(
    IMediaKeySystemFactoryServiceIpcCode::COMMAND_IS_MEDIA_KEY_SYSTEM_SUPPORTED_IN_STRING_IN_STRING_IN_INT_OUT_BOOLEAN
);

std::shared_ptr<MediaKeySystemFactoryService> g_mediaKeySystemFactoryServicePtr = nullptr;

DrmServiceNdkFuzzer::DrmServiceNdkFuzzer() {}

DrmServiceNdkFuzzer::~DrmServiceNdkFuzzer() {}


bool DrmServiceNdkFuzzer::DrmserviceIsMediaKeySystemSupportedV1Test(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemFactoryService> mediaKeySystemFactoryServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_FACTORY_TOKEN);
    std::string uuid(reinterpret_cast<const char *>(rawData), size);
    data.WriteString16(Str8ToStr16(uuid));
    mediaKeySystemFactoryServicePtr->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySystemFactoryServiceIpcCode::COMMAND_IS_MEDIA_KEY_SYSTEM_SUPPORTED),
        data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceIsMediaKeySystemSupportedV2Test(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemFactoryService> mediaKeySystemFactoryServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_FACTORY_TOKEN);
    FuzzedDataProvider fdp(rawData, size);
    size_t len = size / 2; // 2 half
    std::string uuid(fdp.ConsumeBytes<char>(len).data(), len);
    std::string mimeType(fdp.ConsumeBytes<char>(len).data(), len);
    data.WriteString16(Str8ToStr16(uuid));
    data.WriteString16(Str8ToStr16(mimeType));
    mediaKeySystemFactoryServicePtr->OnRemoteRequest(COMMAND_IS_MEDIA_KEY_SYSTEM_SUPPORTED_V2,
        data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceIsMediaKeySystemSupportedV3Test(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemFactoryService> mediaKeySystemFactoryServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_FACTORY_TOKEN);
    FuzzedDataProvider fdp(rawData, size);
    size_t len = (size - sizeof(int32_t)) / 2; // 2 half    
    std::string uuid(fdp.ConsumeBytes<char>(len).data(), len);
    std::string mimeType(fdp.ConsumeBytes<char>(len).data(), len);
    data.WriteString16(Str8ToStr16(uuid));
    data.WriteString16(Str8ToStr16(mimeType));
    int32_t securityLevel = fdp.ConsumeIntegral<int32_t>();
    data.WriteInt32(securityLevel);
    mediaKeySystemFactoryServicePtr->OnRemoteRequest(COMMAND_IS_MEDIA_KEY_SYSTEM_SUPPORTED_V3,
        data, reply, option);
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
    data.WriteString16(Str8ToStr16(name));
    mediaKeySystemFactoryServicePtr->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySystemFactoryServiceIpcCode::COMMAND_CREATE_MEDIA_KEY_SYSTEM),
        data, reply, option);
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
    mediaKeySystemFactoryServicePtr->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySystemFactoryServiceIpcCode::COMMAND_GET_MEDIA_KEY_SYSTEMS),
        data, reply, option);
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
    data.WriteString16(Str8ToStr16(name));
    mediaKeySystemFactoryServicePtr->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySystemFactoryServiceIpcCode::COMMAND_GET_MEDIA_KEY_SYSTEM_UUID),
        data, reply, option);
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
    mediaKeySystemServicePtr->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySystemServiceIpcCode::COMMAND_CREATE_MEDIA_KEY_SESSION),
        data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceCloseMediaKeySessionServiceTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    sptr<IMediaKeySession> hdiMediaKeySession = new (std::nothrow) IMediaKeySessionMock();
    sptr<MediaKeySessionService> mediaKeySessionService =
        new (std::nothrow) MediaKeySessionService(hdiMediaKeySession);
    mediaKeySystemServicePtr->CloseMediaKeySessionService(mediaKeySessionService);
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
    mediaKeySystemServicePtr->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySystemServiceIpcCode::COMMAND_GENERATE_KEY_SYSTEM_REQUEST),
        data, reply, option);
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
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_TOKEN);
    data.WriteInt32(size);
    uint8_t *response = rawData;
    for (size_t i = 0; i < size; i++) {
        if (!data.WriteUint8(response[i])) {
            return false;
        }
    }
    mediaKeySystemServicePtr->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySystemServiceIpcCode::COMMAND_PROCESS_KEY_SYSTEM_RESPONSE),
        data, reply, option);
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
    FuzzedDataProvider fdp(rawData, size);
    size_t len = size / 2; // 2 half
    std::string configName(fdp.ConsumeBytes<char>(len).data(), len);
    std::string value(fdp.ConsumeBytes<char>(len).data(), len);
    data.WriteString16(Str8ToStr16(configName));
    data.WriteString16(Str8ToStr16(value));
    mediaKeySystemServicePtr->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySystemServiceIpcCode::COMMAND_SET_CONFIGURATION_STRING),
        data, reply, option);
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
    FuzzedDataProvider fdp(rawData, size);
    size_t len = size / 2; // 2 half
    std::string configName(fdp.ConsumeBytes<char>(len).data(), len);
    std::string value(fdp.ConsumeBytes<char>(len).data(), len);
    data.WriteString16(Str8ToStr16(configName));
    data.WriteString16(Str8ToStr16(value));
    mediaKeySystemServicePtr->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySystemServiceIpcCode::COMMAND_SET_CONFIGURATION_STRING),
        data, reply, option);
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
    data.WriteString16(Str8ToStr16(configName));
    mediaKeySystemServicePtr->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySystemServiceIpcCode::COMMAND_GET_CONFIGURATION_STRING),
        data, reply, option);
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
    FuzzedDataProvider fdp(rawData, size);
    size_t len = size / 2; // 2 half
    std::string configName(fdp.ConsumeBytes<char>(len).data(), len);
    data.WriteString16(Str8ToStr16(configName));
    data.WriteInt32(len);
    for (size_t i = 0; i < len; i++) {
        if (!data.WriteUint8(rawData[i])) {
            return false;
        }
    }
    mediaKeySystemServicePtr->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySystemServiceIpcCode::COMMAND_SET_CONFIGURATION_BYTE_ARRAY),
        data, reply, option);
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
    data.WriteString16(Str8ToStr16(configName));
    mediaKeySystemServicePtr->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySystemServiceIpcCode::COMMAND_GET_CONFIGURATION_BYTE_ARRAY),
        data, reply, option);
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
    mediaKeySystemServicePtr->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySystemServiceIpcCode::COMMAND_GET_STATISTICS),
        data, reply, option);
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
    mediaKeySystemServicePtr->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySystemServiceIpcCode::COMMAND_RELEASE), data, reply, option);
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
    mediaKeySystemServicePtr->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySystemServiceIpcCode::COMMAND_GET_MAX_CONTENT_PROTECTION_LEVEL),
        data, reply, option);
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
    mediaKeySystemServicePtr->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySystemServiceIpcCode::COMMAND_GET_CERTIFICATE_STATUS), data, reply, option);
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
    mediaKeySystemServicePtr->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySystemServiceIpcCode::COMMAND_GET_OFFLINE_MEDIA_KEY_IDS), data, reply, option);
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
    data.WriteInterfaceToken(MEDIA_KEY_SYSTEM_TOKEN);
    data.WriteInt32(size);
    for (size_t i = 0; i < size; i++) {
        if (!data.WriteUint8(rawData[i])) {
            return false;
        }
    }
    mediaKeySystemServicePtr->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySystemServiceIpcCode::COMMAND_GET_OFFLINE_MEDIA_KEY_STATUS),
        data, reply, option);
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
    for (size_t i = 0; i < size; i++) {
        if (!data.WriteUint8(rawData[i])) {
            return false;
        }
    }
    mediaKeySystemServicePtr->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySystemServiceIpcCode::COMMAND_CLEAR_OFFLINE_MEDIA_KEYS), data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceGetMediaDecryptModuleTest(uint8_t *rawData, size_t size,
    std::shared_ptr<MediaKeySessionService> mediaKeySessionService)
{
    if (rawData == nullptr || size < sizeof(int32_t)) {
        return false;
    }
    MessageParcel reply;
    MessageOption option;
    MessageParcel data;
    data.WriteInterfaceToken(MEDIA_KEY_SESSION_TOKEN);
    mediaKeySessionService->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySessionServiceIpcCode::COMMAND_GET_MEDIA_DECRYPT_MODULE), data, reply, option);
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
    FuzzedDataProvider fdp(rawData, size);
    size_t len = size / 3; // 3 servings
    data.WriteInt32(size);
    std::string mimeType(fdp.ConsumeBytes<char>(len).data(), len);
    data.WriteString16(Str8ToStr16(mimeType));
    data.WriteInt32(size);
    for (size_t i = 0; i < size; i++) {
        if (!data.WriteUint8(rawData[i])) {
            return false;
        }
    }
    data.WriteInt32(size);
    std::string optionalKey(fdp.ConsumeBytes<char>(len).data(), len);
    std::string optionalValue(fdp.ConsumeBytes<char>(len).data(), len);
    data.WriteString16(Str8ToStr16(optionalKey));
    data.WriteString16(Str8ToStr16(optionalValue));
    mediaKeySessionService->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySessionServiceIpcCode::COMMAND_GENERATE_MEDIA_KEY_REQUEST),
        data, reply, option);
    return true;
}

bool DrmServiceNdkFuzzer::DrmserviceProcessMediaKeyResponseTest(uint8_t *rawData, size_t size,
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
    for (size_t i = 0; i < size; i++) {
        if (!data.WriteUint8(rawData[i])) {
            return false;
        }
    }
    mediaKeySessionService->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySessionServiceIpcCode::COMMAND_PROCESS_MEDIA_KEY_RESPONSE),
        data, reply, option);
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
    for (size_t i = 0; i < size; i++) {
        if (!data.WriteUint8(rawData[i])) {
            return false;
        }
    }
    mediaKeySessionService->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySessionServiceIpcCode::COMMAND_GENERATE_OFFLINE_RELEASE_REQUEST),
        data, reply, option);
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
    FuzzedDataProvider fdp(rawData, size);
    size_t len = size / 2; // 2 half
    uint8_t *licenseId = fdp.ConsumeBytes<uint8_t>(len).data();
    uint8_t *releaseReponse = fdp.ConsumeBytes<uint8_t>(len).data();
    data.WriteInt32(len);
    for (size_t i = 0; i < len; i++) {
        if (!data.WriteUint8(licenseId[i])) {
            return false;
        }
    }
    data.WriteInt32(len);
    for (size_t i = 0; i < len; i++) {
        if (!data.WriteUint8(releaseReponse[i])) {
            return false;
        }
    }
    mediaKeySessionService->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySessionServiceIpcCode::COMMAND_PROCESS_OFFLINE_RELEASE_RESPONSE),
        data, reply, option);
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
    mediaKeySessionService->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySessionServiceIpcCode::COMMAND_CHECK_MEDIA_KEY_STATUS), data, reply, option);
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
    for (size_t i = 0; i < size; i++) {
        if (!data.WriteUint8(rawData[i])) {
            return false;
        }
    }
    mediaKeySessionService->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySessionServiceIpcCode::COMMAND_RESTORE_OFFLINE_MEDIA_KEYS),
        data, reply, option);
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
    mediaKeySessionService->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySessionServiceIpcCode::COMMAND_CLEAR_MEDIA_KEYS), data, reply, option);
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
    data.WriteString16(Str8ToStr16(mimeType));
    mediaKeySessionService->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySessionServiceIpcCode::COMMAND_REQUIRE_SECURE_DECODER_MODULE),
        data, reply, option);
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
    mediaKeySessionService->OnRemoteRequest(
        static_cast<uint32_t>(IMediaKeySessionServiceIpcCode::COMMAND_GET_CONTENT_PROTECTION_LEVEL),
        data, reply, option);
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
    mediaDecryptService->OnRemoteRequest(
        static_cast<uint32_t>(IMediaDecryptModuleServiceIpcCode::COMMAND_DECRYPT_MEDIA_DATA), data, reply, option);
    return true;
}
} // namespace DrmStandard

bool FuzzMediaKeySystemFactoryNdk(uint8_t *data, size_t size)
{
    if (data == nullptr) {
        return true;
    }
    DrmServiceNdkFuzzer testMediaKeySystemFactory;
    /* KeySystemFactory */
    if (g_mediaKeySystemFactoryServicePtr == nullptr) {
        g_mediaKeySystemFactoryServicePtr =
            std::make_shared<MediaKeySystemFactoryService>(MEDIA_KEY_SYSTEM_SERVICE_ID, true);
        if (g_mediaKeySystemFactoryServicePtr == nullptr) {
            return false;
        }
    }
    testMediaKeySystemFactory.DrmserviceIsMediaKeySystemSupportedV1Test(data, size, g_mediaKeySystemFactoryServicePtr);
    testMediaKeySystemFactory.DrmserviceIsMediaKeySystemSupportedV2Test(data, size, g_mediaKeySystemFactoryServicePtr);
    testMediaKeySystemFactory.DrmserviceIsMediaKeySystemSupportedV3Test(data, size, g_mediaKeySystemFactoryServicePtr);
    testMediaKeySystemFactory.DrmserviceCreateMediaKeySystemTest(data, size, g_mediaKeySystemFactoryServicePtr);
    testMediaKeySystemFactory.DrmserviceGetMediaKeySystemsTest(data, size, g_mediaKeySystemFactoryServicePtr);
    testMediaKeySystemFactory.DrmserviceGetMediaKeySystemUuidTest(data, size, g_mediaKeySystemFactoryServicePtr);
    return true;
}

bool FuzzSystemFactoryNdk(uint8_t *data, size_t size)
{
    if (data == nullptr) {
        return true;
    }
    DrmServiceNdkFuzzer testMediaSystemFactory;
    /* KeySystem */
    sptr<IMediaKeySystem> hdiMediaKeySystem = new (std::nothrow) IMediaKeySystemMock();
    std::shared_ptr<MediaKeySystemService> mediaKeySystemServicePtr =
        std::make_shared<MediaKeySystemService>(hdiMediaKeySystem);
    testMediaSystemFactory.DrmserviceCreateMediaKeySessionTest(data, size, mediaKeySystemServicePtr);
    testMediaSystemFactory.DrmserviceCloseMediaKeySessionServiceTest(data, size, mediaKeySystemServicePtr);
    testMediaSystemFactory.DrmserviceGenerateKeySystemRequestTest(data, size, mediaKeySystemServicePtr);
    testMediaSystemFactory.DrmserviceProcessKeySystemResponseTest(data, size, mediaKeySystemServicePtr);
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
    testMediaSystemFactory.DrmserviceGetMediaDecryptModuleTest(data, size, mediaKeySessionService);
    testMediaSystemFactory.DrmserviceGenerateMediaKeyRequestTest(data, size, mediaKeySessionService);
    testMediaSystemFactory.DrmserviceProcessMediaKeyResponseTest(data, size, mediaKeySessionService);
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
    OHOS::FuzzMediaKeySystemFactoryNdk(rawData, size);
    return 0;
}
