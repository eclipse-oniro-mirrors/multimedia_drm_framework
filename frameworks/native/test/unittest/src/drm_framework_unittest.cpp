/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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
#include <mutex>
#include <shared_mutex>
#include <string>
#include <refbase.h>
#include "cstdio"
#include "cstdlib"
#include <securec.h>
#include "native_drm_common.h"
#include "native_drm_err.h"
#include "gmock/gmock.h"
#include "native_drm_base.h"
#include "native_drm_object.h"
#include "i_mediadecryptmodule_service.h"
#include "native_mediakeysession.h"
#include "native_mediakeysystem.h"
#include "cstdbool"
#include "drm_types.h"
#include "drm_framework_unittest.h"
#include "system_ability_definition.h"
#include "drm_log.h"
#include "drm_death_recipient.h"
#include "key_session_impl.h"
#include "i_mediakeysystem_service.h"
#include "media_key_system_factory_impl.h"
#include "mediakeysystem_service_callback_stub.h"
#include "http.h"
#include "ashmem.h"
#include "media_decrypt_module_service_proxy.h"
#include <cstring>
#include <map>
#include <unordered_map>
#include "nocopyable.h"
#include "ipc_skeleton.h"
#include "iservice_registry.h"
#include "i_keysession_service.h"
#include "i_keysession_service_callback.h"
#include "key_session_service_callback_stub.h"

#define DRM_SAMPLE_CHECK_AND_RETURN_RET_LOG(cond, ret, fmt, ...) \
    do {                                                         \
        if ((cond)) {                                            \
            printf(fmt, ##__VA_ARGS__);                          \
            return ret;                                          \
        }                                                        \
    } while (0)
#define DRM_SAMPLE_INFO_LOG(fmt, ...) fprintf(stdout, "[INFO] " fmt "\n", ##__VA_ARGS__)
#define DRM_SAMPLE_ERROR_LOG(fmt, ...) fprintf(stdout, "[ERROR] " fmt "\n", ##__VA_ARGS__)
#define OFFRESPONSE                                                                                            \
    {                                                                                                          \
        0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47,                                      \
            0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33,                                        \
            0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63,                                        \
            0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45,                                        \
            0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D                                               \
    }
#define REQUESTINFODATA                                                                                        \
    {                                                                                                          \
        0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00,                                      \
            0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8,                                        \
            0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00,                                        \
            0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E,                                        \
            0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22,                                        \
            0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22,                                        \
            0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30,                                        \
            0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57,                                        \
            0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22,                                        \
            0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58,                                        \
            0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59,                                        \
            0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C,                                        \
            0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22,                                        \
            0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D                                                     \
    }
#define OFFREQUESTINFODATA                                                                                     \
    {                                                                                                          \
        0x00, 0x00, 0x00, 0x8b, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00,                                      \
            0x00, 0x3d, 0x5e, 0x6d, 0x35, 0x9b, 0x9a, 0x41, 0xe8, 0xb8,                                        \
            0x43, 0xdd, 0x3c, 0x6e, 0x72, 0xc4, 0x2c, 0x00, 0x00, 0x00,                                        \
            0x6b, 0x7b, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e,                                        \
            0x22, 0x3a, 0x22, 0x56, 0x31, 0x2e, 0x30, 0x22, 0x2c, 0x22,                                        \
            0x63, 0x6f, 0x6e, 0x74, 0x65, 0x6e, 0x74, 0x49, 0x44, 0x22,                                        \
            0x3a, 0x22, 0x64, 0x48, 0x4d, 0x74, 0x4d, 0x6a, 0x59, 0x30,                                        \
            0x4c, 0x54, 0x45, 0x77, 0x4f, 0x44, 0x41, 0x74, 0x59, 0x57,                                        \
            0x56, 0x7a, 0x22, 0x2c, 0x22, 0x6b, 0x69, 0x64, 0x73, 0x22,                                        \
            0x3a, 0x5b, 0x22, 0x47, 0x2b, 0x45, 0x6b, 0x2f, 0x2b, 0x58,                                        \
            0x6d, 0x55, 0x6b, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59,                                        \
            0x57, 0x51, 0x51, 0x49, 0x67, 0x3d, 0x3d, 0x22, 0x5d, 0x2c,                                        \
            0x22, 0x65, 0x6e, 0x73, 0x63, 0x68, 0x65, 0x6d, 0x61, 0x22,                                        \
            0x3a, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7d                                                     \
    }
#define ONRESPONSE                                                                                             \
    {                                                                                                          \
        0x30, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47,                                      \
            0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33,                                        \
            0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63,                                        \
            0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45,                                        \
            0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D                                               \
    }


#define LICENSE_URL "http://license.dev.trustdta.com:8080/drmproxy/v3/getLicense"
#define PROVISION_URL "https://wiseplay-prv.cloud.huawei.com/provision/v1/wiseplay"
using namespace testing::ext;
using namespace std;

namespace {
const int32_t STEP_ONE = 1;
const int32_t STEP_TWO = 2;
const int32_t STEP_THREE = 3;
const int32_t STEP_FOUR = 4;
}

namespace OHOS {
namespace DrmStandard {

bool g_isWisePlay = false;
void DrmFrameworkUnitTest::SetUpTestCase(void) {}

void DrmFrameworkUnitTest::TearDownTestCase(void) {}

static const char *GetUuid()
{
    if (OH_MediaKeySystem_IsSupported("com.clearplay.drm")) {
        return "com.clearplay.drm";
    } else if (OH_MediaKeySystem_IsSupported("com.wiseplay.drm")) {
        return "com.wiseplay.drm";
    } else {
        return "ERROR";
    }
}

void DrmFrameworkUnitTest::SetUp()
{
    if (strcmp(GetUuid(), "com.wiseplay.drm") == 0) {
        g_isWisePlay = true;
    }
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create(GetUuid(), &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char request[12288] = { 0 }; // 12288:request len
    int32_t requestLen = 12288; // 12288:request len
    char defaultUrl[2048] = { 0 }; // 2048:url len
    int32_t defaultUrlLen = 2048; // 2048:url len
    DRM_CertificateStatus certStatus = CERT_STATUS_INVALID;
    errNo = OH_MediaKeySystem_GetCertificateStatus(mediaKeySystem, &certStatus);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (certStatus == CERT_STATUS_NOT_PROVISIONED) {
        errNo = OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, request, &requestLen, defaultUrl,
            defaultUrlLen);
        unsigned char KeySystemResponse[12288] = OFFRESPONSE;
        int32_t KeySystemResponseLen = 12288;
        if (g_isWisePlay) {
            int rett = HttpPost(PROVISION_URL, request, requestLen, KeySystemResponse, &KeySystemResponseLen, 10);
            EXPECT_EQ(rett, 0);
        } else {
            KeySystemResponseLen = 50;
        }        
        errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, 0);
        EXPECT_NE(errNo, DRM_ERR_OK);
        errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
        EXPECT_EQ(errNo, DRM_ERR_OK);
    }
    OH_MediaKeySystem_Destroy(mediaKeySystem);
    mediaKeySystem = nullptr;
}

void DrmFrameworkUnitTest::TearDown() {}

static const char *GetDrmPlugin()
{
    if (OH_MediaKeySystem_IsSupported("com.clearplay.drm")) {
        return "killall -9 clearplay_host";
    } else if (OH_MediaKeySystem_IsSupported("com.wiseplay.drm")) {
        return "killall -9 wiseplay_host";
    } else {
        return "ERROR";
    }
}

Drm_ErrCode TestSystemEventCallBack(DRM_EventType eventType, unsigned char *info,
    int32_t infoLen, char *extra)
{
    DRM_SAMPLE_INFO_LOG("TestSystemEventCallBack ok");
    return DRM_ERR_OK;
}

Drm_ErrCode TestSessoinEventCallBack(DRM_EventType eventType, unsigned char *info,
    int32_t infoLen, char *extra)
{
    DRM_SAMPLE_INFO_LOG("TestSessoinEventCallBack ok");
    return DRM_ERR_OK;
}

Drm_ErrCode TestSessoinKeyChangeCallBack(DRM_KeysInfo *keysInfo, bool hasNewGoodKeys)
{
    DRM_SAMPLE_INFO_LOG("TestSessoinKeyChangeCallBack ok");
    return DRM_ERR_OK;
}

Drm_ErrCode TestSystemEventCallBackWithObj(MediaKeySystem *mediaKeySystem, DRM_EventType eventType,
    uint8_t *info, int32_t infoLen, char *extra)
{
    DRM_SAMPLE_INFO_LOG("TestSystemEventCallBackWithObj ok");
    if (mediaKeySystem == nullptr) {
        return DRM_ERR_UNKNOWN;
    }
    DRM_SAMPLE_INFO_LOG("Event: event type: %d", eventType);
    DRM_SAMPLE_INFO_LOG("Event: the info body is: ");
    if (info != nullptr) {
        for (int32_t i = 0; i < infoLen; i++) {
            DRM_SAMPLE_INFO_LOG("%x", info[i]);
        }
    }
    if (extra != nullptr) {
        DRM_SAMPLE_INFO_LOG("Event: the extra is: %s", extra);
    }
    return DRM_ERR_OK;
}

Drm_ErrCode TestSessoinEventCallBackWithObj(MediaKeySession *mediaKeySessoin, DRM_EventType eventType,
    uint8_t *info, int32_t infoLen, char *extra)
{
    DRM_SAMPLE_INFO_LOG("TestSessoinEventCallBackWithObj ok");
    if (mediaKeySessoin == nullptr) {
        return DRM_ERR_UNKNOWN;
    }
    DRM_SAMPLE_INFO_LOG("Event: the event type: %d", eventType);
    DRM_SAMPLE_INFO_LOG("Event: the info body is: ");
    if (info != nullptr) {
        for (int32_t i = 0; i < infoLen; i++) {
            DRM_SAMPLE_INFO_LOG("%x", info[i]);
        }
    }
    if (extra != nullptr) {
        DRM_SAMPLE_INFO_LOG("Event: the extra is: %s", extra);
    }
    return DRM_ERR_OK;
}

Drm_ErrCode TestSessoinKeyChangeCallBackWithObj(MediaKeySession *mediaKeySessoin, DRM_KeysInfo *keysInfo,
    bool hasNewGoodKeys)
{
    DRM_SAMPLE_INFO_LOG("TestSessoinKeyChangeCallBackWithObj ok");
    if (mediaKeySessoin == nullptr) {
        return DRM_ERR_UNKNOWN;
    }
    for (uint32_t i = 0; i < keysInfo->keysInfoCount; i++) {
        for (uint32_t j = 0; j < MAX_KEY_ID_LEN; j += STEP_FOUR) {
            DRM_SAMPLE_INFO_LOG("KeyChangedEvent: keyid is: ");
            DRM_SAMPLE_INFO_LOG("%x %x %x %x",
                keysInfo->keyId[i][j], keysInfo->keyId[i][j + STEP_ONE],
                keysInfo->keyId[i][j + STEP_TWO], keysInfo->keyId[i][j + STEP_THREE]);
        }
        DRM_SAMPLE_INFO_LOG("KeyChangedEvent: statusValue %s", keysInfo->statusValue[i]);
    }
    return DRM_ERR_OK;
}

/*
 * Feature: Framework
 * Function: Test to determine if the creation of this DRM instance is supported
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case description: Test to determine if it supports creating the DRM instance interface OH_MediaKeySystem_IsSupported
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_MediaKeySystemIsSupportedNormal_001, TestSize.Level0)
{
    bool supported;
    supported = OH_MediaKeySystem_IsSupported(GetUuid());
    EXPECT_EQ(supported, true);
}

/*
 * Feature: Framework
 * Function: Test to determine if the creation of this DRM instance is supported
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case description: Test to determine if it supports creating the DRM instance interface OH_MediaKeySystem_IsSupported
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_MediaKeySystemIsSupported2Normal_002, TestSize.Level0)
{
    bool supported;
    supported = OH_MediaKeySystem_IsSupported2(GetUuid(), "video/mp4");
    EXPECT_EQ(supported, true);
}

/*
 * Feature: Framework
 * Function: Test to determine if the creation of this DRM instance is supported
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case description: Test to determine if it supports creating the DRM instance interface OH_MediaKeySystem_IsSupported
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_MediaKeySystemIsSupported3Normal_003, TestSize.Level0)
{
    bool supported;
    supported = OH_MediaKeySystem_IsSupported3(GetUuid(), "video/mp4", CONTENT_PROTECTION_LEVEL_HW_CRYPTO);
    EXPECT_EQ(supported, true);
}

/*
 * Feature: Framework
 * Function: Test to determine if the creation of this DRM instance is supported
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case description: Test to determine if it supports creating the DRM instance interface OH_MediaKeySystem_IsSupported
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_MediaKeySystemIsSupportedAbNormal_004, TestSize.Level0)
{
    bool supported = true;
    supported = OH_MediaKeySystem_IsSupported("com.drm.clearpla");
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported("");
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported(nullptr);
    EXPECT_EQ(supported, false);
}

/*
 * Feature: Framework
 * Function: Test to determine if the creation of this DRM instance is supported
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case description: Test to determine if it supports creating the DRM instance interface OH_MediaKeySystem_IsSupported
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_MediaKeySystemIsSupported2AbNormal_005, TestSize.Level0)
{
    bool supported = true;
    supported = OH_MediaKeySystem_IsSupported2("com.drm.clearpla", "video/mp4");
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported2("", "video/mp4");
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported2(nullptr, "video/mp4");
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported2(GetUuid(), "ideo/mp4");
    if (g_isWisePlay) {
        EXPECT_EQ(supported, true);
    } else {
        EXPECT_EQ(supported, false);
    }
    supported = OH_MediaKeySystem_IsSupported2(GetUuid(), "");
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported2(GetUuid(), nullptr);
    EXPECT_EQ(supported, false);
}

/*
 * Feature: Framework
 * Function: Test to determine if the creation of this DRM instance is supported
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case description: Test to determine if it supports creating the DRM instance interface OH_MediaKeySystem_IsSupported
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_MediaKeySystemIsSupported3AbNormal_006, TestSize.Level0)
{
    bool supported = true;
    supported = OH_MediaKeySystem_IsSupported3("com.drm.clearpla", "video/mp4", CONTENT_PROTECTION_LEVEL_HW_CRYPTO);
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported3("", "video/mp4", CONTENT_PROTECTION_LEVEL_HW_CRYPTO);
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported3(nullptr, "video/mp4", CONTENT_PROTECTION_LEVEL_HW_CRYPTO);
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported3(GetUuid(), "ideo/mp4", CONTENT_PROTECTION_LEVEL_HW_CRYPTO);
    if (g_isWisePlay) {
        EXPECT_EQ(supported, true);
    } else {
        EXPECT_EQ(supported, false);
    }
    supported = OH_MediaKeySystem_IsSupported3(GetUuid(), "", CONTENT_PROTECTION_LEVEL_HW_CRYPTO);
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported3(GetUuid(), nullptr, CONTENT_PROTECTION_LEVEL_HW_CRYPTO);
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported3(GetUuid(), "video/mp4",
        (DRM_ContentProtectionLevel)(CONTENT_PROTECTION_LEVEL_UNKNOWN - 1));
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported3(GetUuid(), "video/mp4",
        (DRM_ContentProtectionLevel)CONTENT_PROTECTION_LEVEL_MAX);
    EXPECT_EQ(supported, false);
}

/*
 * Feature: Framework
 * Function: Test and create DRM instances
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case Description: Test Creating DRM Instance
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_CreateMediaKeySystemNormal_007, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_UNKNOWN;
    errNo = OH_MediaKeySystem_Create(GetUuid(), &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_GetMaxContentProtectionLevel(mediaKeySystem, &contentProtectionLevel);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    EXPECT_GT(contentProtectionLevel, 0);
    if (mediaKeySystem != nullptr) {
        OH_MediaKeySystem_Destroy(mediaKeySystem);
        mediaKeySystem = nullptr;
    }
}

/*
 * Feature: Framework
 * Function: Test and create DRM instances
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case Description: Testing for an incorrect uuid and unsuccessful creation of DRM instance.
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_CreateMediaKeySystemAbNormal_008, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_UNKNOWN;
    errNo = OH_MediaKeySystem_Create("com.drm.clerpay", &mediaKeySystem);
    EXPECT_EQ(mediaKeySystem, nullptr);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_GetMaxContentProtectionLevel(mediaKeySystem, &contentProtectionLevel);
    EXPECT_NE(errNo, DRM_ERR_OK);
}

/*
 * Feature: Framework
 * Function: Test and create DRM instances
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case Description: Test passed in an empty uuid and failed to create a DRM instance.
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_CreateMediaKeySystemAbNormal_009, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_UNKNOWN;
    errNo = OH_MediaKeySystem_Create("", &mediaKeySystem);
    EXPECT_EQ(mediaKeySystem, nullptr);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_GetMaxContentProtectionLevel(mediaKeySystem, &contentProtectionLevel);
    EXPECT_NE(errNo, DRM_ERR_OK);
}

/*
 * Feature: Framework
 * Function: Test and create DRM instances
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case description: After the DRM instance is created, it is destroyed, and then the subsequent interface is called.
 * The interface returns unsuccessful.
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_CreateMediaKeySystemNormal_010, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_UNKNOWN;
    errNo = OH_MediaKeySystem_Create(GetUuid(), &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    mediaKeySystem = nullptr;
    EXPECT_EQ(errNo, DRM_ERR_OK);
    mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_GetMaxContentProtectionLevel(mediaKeySystem, &contentProtectionLevel);
    EXPECT_NE(errNo, DRM_ERR_OK);
}

/*
 * Feature: Framework
 * Function: Test and create sessions
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case Description: Test Creating Sessions
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_CreateMediaKeySessionNormal_011, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create(GetUuid(), &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_SetMediaKeySystemCallback(mediaKeySystem, &TestSystemEventCallBack);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    mediaKeySystem = nullptr;
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

/*
 * Feature: Framework
 * Function: Test that the RequireSecureDecoderModule interface is functioning properly
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case description: Testing that the RequireSecureDecoderModule interface is functioning properly
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_RequireSecureDecoderModuleNormal_012, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create(GetUuid(), &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    bool requireSecureDecoder;
    errNo = OH_MediaKeySession_RequireSecureDecoderModule(mediaKeySession, "video/mp4", &requireSecureDecoder);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    mediaKeySystem = nullptr;
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

/*
 * Feature: Framework
 * Function: RequireSecureDecoderModule Exception Test Input Error mimtype
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case Description: RequireSecureDecoderModule Exception Test
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_RequireSecureDecoderModuleAbNormal_013, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create(GetUuid(), &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    bool requireSecureDecoder = false;
    errNo = OH_MediaKeySession_RequireSecureDecoderModule(mediaKeySession, "asdssadadsa", &requireSecureDecoder);
    if (g_isWisePlay) {
        EXPECT_NE(errNo, DRM_ERR_OK);
    } else {
        EXPECT_EQ(errNo, DRM_ERR_OK);
    }
    EXPECT_EQ(requireSecureDecoder, false);
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    mediaKeySystem = nullptr;
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

/*
 * Feature: Framework
 * Function: RequireSecureDecoderModule Exception Test Input Error mimtype
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case Description: RequireSecureDecoderModule Exception Test
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_RequireSecureDecoderModuleAbNormal_014, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create(GetUuid(), &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    bool requireSecureDecoder = false;
    errNo = OH_MediaKeySession_RequireSecureDecoderModule(mediaKeySession, "", &requireSecureDecoder);
    EXPECT_NE(errNo, DRM_ERR_OK);
    EXPECT_EQ(requireSecureDecoder, false);
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    mediaKeySystem = nullptr;
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

/*
 * Feature: Framework
 * Function: RequireSecureDecoderModule Exception Test Input Error mimtype
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case Description: RequireSecureDecoderModule Exception Test
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_RequireSecureDecoderModuleAbNormal_015, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create(GetUuid(), &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    bool requireSecureDecoder = false;
    errNo = OH_MediaKeySession_RequireSecureDecoderModule(mediaKeySession, nullptr, &requireSecureDecoder);
    EXPECT_NE(errNo, DRM_ERR_OK);
    EXPECT_EQ(requireSecureDecoder, false);
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    mediaKeySystem = nullptr;
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

/*
 * Feature: Framework
 * Function: RequireSecureDecoderModule Exception Test Input Error mimtype
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case Description: RequireSecureDecoderModule Exception Test
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_RequireSecureDecoderModuleAbNormal_016, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create(GetUuid(), &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    bool requireSecureDecoder = false;
    errNo = OH_MediaKeySession_RequireSecureDecoderModule(mediaKeySession, "video/mp4", nullptr);
    EXPECT_NE(errNo, DRM_ERR_OK);
    EXPECT_EQ(requireSecureDecoder, false);
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

/*
 * Feature: Framework
 * Function: RequireSecureDecoderModule Exception Test Input Error mimtype
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case Description: RequireSecureDecoderModule Exception Test
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_RequireSecureDecoderModuleAbNormal_017, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create(GetUuid(), &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    bool requireSecureDecoder = false;
    errNo = OH_MediaKeySession_RequireSecureDecoderModule(nullptr, "video/mp4", &requireSecureDecoder);
    EXPECT_NE(errNo, DRM_ERR_OK);
    EXPECT_EQ(requireSecureDecoder, false);
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

/*
 * Feature: Framework
 * Function: GenerateKeySystemRequest normal testing to obtain normal requests
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case description: GenerateKeySystemRequest obtained a normal request through normal testing
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GenerateKeySystemRequestNormal_018, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create(GetUuid(), &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

/*
 * Feature: Framework
 * Function: GenerateKeySystemRequest parameter exception testing
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case Description: GenerateKeySystemRequest Parameter Exception Test
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GenerateKeySystemRequestAbNormal_019, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create(GetUuid(), &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char request[12288] = { 0 }; // 12288:request len
    int32_t requestLen = 12288; // 12288:request len
    char defaultUrl[2048] = { 0 }; // 2048:url len
    int32_t defaultUrlLen = 2048; // 2048:url len
    errNo = OH_MediaKeySystem_GenerateKeySystemRequest(nullptr, request, &requestLen, defaultUrl, defaultUrlLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    requestLen = 12288; // 12288:request len
    errNo = OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, nullptr, &requestLen, defaultUrl,
        defaultUrlLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, request, nullptr, defaultUrl, defaultUrlLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    requestLen = 12288; // 12288:request len
    errNo = OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, request, &requestLen, nullptr, defaultUrlLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    requestLen = 12288; // 12288:request len
    errNo = OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, request, &requestLen, defaultUrl, 0);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

/*
 * Feature: Framework
 * Function: Processing device certificate response testing
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case Description: Processing Device Certificate Response Testing
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_ProcessMediaKeyResponseNormal_020, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create(GetUuid(), &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_SetMediaKeySystemCallback(mediaKeySystem, &TestSystemEventCallBack);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}


/*
 * Feature: Framework
 * Function: Processing device certificate response testing
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case Description: Processing Device Certificate Response Testing
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_ProcessMediaKeyResponseAbNormal_021, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create(GetUuid(), &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char KeySystemResponse[12288] = OFFRESPONSE;
    int32_t KeySystemResponseLen = 12288;
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(nullptr, KeySystemResponse, KeySystemResponseLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

/*
 * Feature: Framework
 * Function: Processing device certificate response testing
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case Description: Processing Device Certificate Response Testing
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_ProcessMediaKeyResponseAbNormal_022, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create(GetUuid(), &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    int32_t KeySystemResponseLen = 12288;
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, nullptr, KeySystemResponseLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

/*
 * Feature: Framework
 * Function: Processing device certificate response testing
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case Description: Processing Device Certificate Response Testing
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_ProcessMediaKeyResponseAbNormal_023, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create(GetUuid(), &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

/*
 * Feature: Framework
 * Function: Processing device certificate response testing
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case Description: Processing Device Certificate Response Testing
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GenerateMediaKeyRequestNormal_024, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create(GetUuid(), &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    // mediakeysession
    DRM_MediaKeyRequest mediaKeyRequest;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = REQUESTINFODATA;
    memset_s(&info, sizeof(DRM_MediaKeyRequestInfo), 0, sizeof(DRM_MediaKeyRequestInfo));
    info.initDataLen = sizeof(testData);
    info.type = MEDIA_KEY_TYPE_ONLINE;
    memcpy_s(info.mimeType, sizeof("video/mp4"), (char *)"video/mp4", sizeof("video/mp4"));
    memcpy_s(info.initData, sizeof(testData), testData, sizeof(testData));
    memcpy_s(info.optionName[0], sizeof("optionalDataName"), (char *)"optionalDataName", sizeof("optionalDataName"));
    memcpy_s(info.optionData[0], sizeof("optionalDataValue"), (char *)"optionalDataValue", sizeof("optionalDataValue"));
    info.optionsCount = 1;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

/*
 * Feature: Framework
 * Function: Processing device certificate response testing
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case Description: Processing Device Certificate Response Testing
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GenerateMediaKeyRequestAbNormal_025, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create(GetUuid(), &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    // mediakeysession
    DRM_MediaKeyRequest mediaKeyRequest;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = REQUESTINFODATA;
    memset_s(&info, sizeof(DRM_MediaKeyRequestInfo), 0, sizeof(DRM_MediaKeyRequestInfo));
    info.initDataLen = sizeof(testData);
    info.type = MEDIA_KEY_TYPE_ONLINE;
    memcpy_s(info.mimeType, sizeof("video/mp4"), (char *)"video/mp4", sizeof("video/mp4"));
    memcpy_s(info.initData, sizeof(testData), testData, sizeof(testData));
    memcpy_s(info.optionName[0], sizeof("optionalDataName"), (char *)"optionalDataName", sizeof("optionalDataName"));
    memcpy_s(info.optionData[0], sizeof("optionalDataValue"), (char *)"optionalDataValue", sizeof("optionalDataValue"));
    info.optionsCount = 1;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(nullptr, &info, &mediaKeyRequest);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

/*
 * Feature: Framework
 * Function: Processing device certificate response testing
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case Description: Processing Device Certificate Response Testing
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GenerateMediaKeyRequestAbNormal_026, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create(GetUuid(), &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    // mediakeysession
    DRM_MediaKeyRequest mediaKeyRequest;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = REQUESTINFODATA;
    memset_s(&info, sizeof(DRM_MediaKeyRequestInfo), 0, sizeof(DRM_MediaKeyRequestInfo));
    info.initDataLen = sizeof(testData);
    info.type = MEDIA_KEY_TYPE_ONLINE;
    memcpy_s(info.mimeType, sizeof("video/mp4"), (char *)"video/mp4", sizeof("video/mp4"));
    memcpy_s(info.initData, sizeof(testData), testData, sizeof(testData));
    memcpy_s(info.optionName[0], sizeof("optionalDataName"), (char *)"optionalDataName", sizeof("optionalDataName"));
    memcpy_s(info.optionData[0], sizeof("optionalDataValue"), (char *)"optionalDataValue", sizeof("optionalDataValue"));
    info.optionsCount = 1;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, nullptr, &mediaKeyRequest);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}
