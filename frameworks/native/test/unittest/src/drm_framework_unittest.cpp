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

#include "cstdio"
#include "cstdlib"
#include "native_drm_common.h"
#include "native_drm_err.h"
#include "gmock/gmock.h"
#include "native_mediakeysession.h"
#include "native_mediakeysystem.h"
#include "cstdbool"
#include "drm_framework_unittest.h"
#include "drm_log.h"
#include "drm_error_code.h"

using namespace testing::ext;
using namespace std;

namespace OHOS {
namespace DrmStandard {
void DrmFrameworkUnitTest::SetUpTestCase(void) {}

void DrmFrameworkUnitTest::TearDownTestCase(void) {}

void DrmFrameworkUnitTest::SetUp() {}

void DrmFrameworkUnitTest::TearDown() {}

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
    supported = OH_MediaKeySystem_IsSupported("com.drm.clearplay");
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
    supported = OH_MediaKeySystem_IsSupported2("com.drm.clearplay", "video/mp4");
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
    supported = OH_MediaKeySystem_IsSupported3("com.drm.clearplay", "video/mp4", CONTENT_PROTECTION_LEVEL_HW_CRYPTO);
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

    supported = OH_MediaKeySystem_IsSupported2("com.drm.clearplay", "ideo/mp4");
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported2("com.drm.clearplay", "");
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported2("com.drm.clearplay", nullptr);
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
    supported = OH_MediaKeySystem_IsSupported3("com.drm.clearplay", "ideo/mp4", CONTENT_PROTECTION_LEVEL_HW_CRYPTO);
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported3("com.drm.clearplay", "", CONTENT_PROTECTION_LEVEL_HW_CRYPTO);
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported3("com.drm.clearplay", nullptr, CONTENT_PROTECTION_LEVEL_HW_CRYPTO);
    EXPECT_EQ(supported, false);

    supported = OH_MediaKeySystem_IsSupported3("com.drm.clearplay", "video/mp4",
        (OH_DRM_ContentProtectionLevel)(CONTENT_PROTECTION_LEVEL_UNKNOWN - 1));
    EXPECT_EQ(supported, false);

    supported = OH_MediaKeySystem_IsSupported3("com.drm.clearplay", "video/mp4",
        (OH_DRM_ContentProtectionLevel)CONTENT_PROTECTION_LEVEL_MAX);
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
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_UNKNOWN;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_GetMaxContentProtectionLevel(mediaKeySystem, &contentProtectionLevel);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    EXPECT_GT(contentProtectionLevel, 0);
    if (mediaKeySystem != nullptr) {
        OH_MediaKeySystem_Destroy(mediaKeySystem);
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
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_UNKNOWN;
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
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_UNKNOWN;
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
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_UNKNOWN;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
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
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
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
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    bool requireSecureDecoder;
    errNo = OH_MediaKeySession_RequireSecureDecoderModule(mediaKeySession, "video/mp4", &requireSecureDecoder);
    EXPECT_EQ(errNo, DRM_ERR_OK);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_RequireSecureDecoderModuleAbNormal_013, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    bool requireSecureDecoder = false;
    errNo = OH_MediaKeySession_RequireSecureDecoderModule(mediaKeySession, "asdssadadsa", &requireSecureDecoder);
    EXPECT_EQ(errNo, DRM_ERR_OK);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_RequireSecureDecoderModuleAbNormal_014, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
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
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
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
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
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
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
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
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    if (request) {
        free(request);
    }
    if (defaultUrl) {
        free(defaultUrl);
    }
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
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo = OH_MediaKeySystem_GenerateKeySystemRequest(nullptr, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, nullptr, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    errNo = OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, nullptr, &defaultUrl, &defaultUrlLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    errNo = OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, nullptr, &defaultUrlLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    errNo = OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, nullptr);
    EXPECT_NE(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
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
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
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
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
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
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
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
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = 0;
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GenerateMediaKeyRequestNormal_024, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;}
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;}
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
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
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;}
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;}
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
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
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
    }
    if (defaultUrl) {
        free(defaultUrl);
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, nullptr, &mediaKeyRequest);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GenerateMediaKeyRequestAbNormal_027, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;}
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, nullptr);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_ProcessMediaKeyResponseNormal_028, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    if (mediaKeyId) {
        free(mediaKeyId);
        mediaKeyId = nullptr;
    }
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_ProcessMediaKeyResponseAbNormal_029, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_NE(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(nullptr, &response, &mediaKeyId, &keyIdLen);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_ProcessMediaKeyResponseAbNormal_030, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_NE(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, nullptr, &mediaKeyId, &keyIdLen);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_ProcessMediaKeyResponseAbNormal_031, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, nullptr, &keyIdLen);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_ProcessMediaKeyResponseAbNormal_032, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;

    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, nullptr);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_CheckMediaKeyStatusNormal_033, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_MediaKeyDescription *mediaKeyDescription = nullptr;
    errNo = OH_MediaKeySession_CheckMediaKeyStatus(mediaKeySession, &mediaKeyDescription);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (mediaKeyDescription) {
        free(mediaKeyDescription);
        mediaKeyDescription = nullptr;
    }

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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_CheckMediaKeyStatusAbNormal_034, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_MediaKeyDescription *mediaKeyDescription = nullptr;
    errNo = OH_MediaKeySession_CheckMediaKeyStatus(nullptr, &mediaKeyDescription);
    EXPECT_NE(errNo, DRM_ERR_OK);
    if (mediaKeyDescription) {
        free(mediaKeyDescription);
        mediaKeyDescription = nullptr;
    }

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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_CheckMediaKeyStatusAbNormal_035, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySession_CheckMediaKeyStatus(mediaKeySession, nullptr);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_ClearMediaKeysNormal_036, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_MediaKeyDescription *mediaKeyDescription = nullptr;
    errNo = OH_MediaKeySession_CheckMediaKeyStatus(mediaKeySession, &mediaKeyDescription);

    if (mediaKeyDescription) {
        free(mediaKeyDescription);
        mediaKeyDescription = nullptr;
    }
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySession_ClearMediaKeys(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySession_CheckMediaKeyStatus(mediaKeySession, &mediaKeyDescription);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_ClearMediaKeysAbNormal_037, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_MediaKeyDescription *mediaKeyDescription = nullptr;
    errNo = OH_MediaKeySession_CheckMediaKeyStatus(mediaKeySession, &mediaKeyDescription);

    if (mediaKeyDescription) {
        free(mediaKeyDescription);
        mediaKeyDescription = nullptr;
    }
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySession_ClearMediaKeys(nullptr);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GetOfflineMediaKeyStatusNormal_038, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_MediaKeyDescription *mediaKeyDescription = nullptr;
    errNo = OH_MediaKeySession_CheckMediaKeyStatus(mediaKeySession, &mediaKeyDescription);
    if (mediaKeyDescription) {
        free(mediaKeyDescription);
        mediaKeyDescription = nullptr;
    }
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    OH_DRM_Uint8Buffer mediaKeyIds;
    mediaKeyIds.buffer = mediaKeyId;
    mediaKeyIds.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GetOfflineMediaKeyStatusAbNormal_039, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_MediaKeyDescription *mediaKeyDescription = nullptr;
    errNo = OH_MediaKeySession_CheckMediaKeyStatus(mediaKeySession, &mediaKeyDescription);
    if (mediaKeyDescription) {
        free(mediaKeyDescription);
        mediaKeyDescription = nullptr;
    }
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    OH_DRM_Uint8Buffer mediaKeyIds;
    mediaKeyIds.buffer = mediaKeyId;
    mediaKeyIds.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(nullptr, &mediaKeyIds, &OfflineMediaKeyStatus);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GetOfflineMediaKeyStatusAbNormal_040, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_MediaKeyDescription *mediaKeyDescription = nullptr;
    errNo = OH_MediaKeySession_CheckMediaKeyStatus(mediaKeySession, &mediaKeyDescription);
    if (mediaKeyDescription) {
        free(mediaKeyDescription);
        mediaKeyDescription = nullptr;
    }
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;

    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, nullptr, &OfflineMediaKeyStatus);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GetOfflineMediaKeyStatusAbNormal_041, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_MediaKeyDescription *mediaKeyDescription = nullptr;
    errNo = OH_MediaKeySession_CheckMediaKeyStatus(mediaKeySession, &mediaKeyDescription);
    if (mediaKeyDescription) {
        free(mediaKeyDescription);
        mediaKeyDescription = nullptr;
    }
    EXPECT_EQ(errNo, DRM_ERR_OK);


    OH_DRM_Uint8Buffer mediaKeyIds;
    mediaKeyIds.buffer = mediaKeyId;
    mediaKeyIds.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, nullptr);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GetOfflineMediaKeyIdsNormal_042, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);


    OH_DRM_MediakeyIdArray *mediaKeyIdss = nullptr;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyIds(mediaKeySystem, &mediaKeyIdss);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (mediaKeyIdss) {
        free(mediaKeyIdss);
        mediaKeyIdss = nullptr;
    }

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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GetOfflineMediaKeyIdsAbNormal_043, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);


    OH_DRM_MediakeyIdArray *mediaKeyIdss = nullptr;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyIds(nullptr, &mediaKeyIdss);
    EXPECT_NE(errNo, DRM_ERR_OK);
    if (mediaKeyIdss) {
        free(mediaKeyIdss);
        mediaKeyIdss = nullptr;
    }

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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GetOfflineMediaKeyIdsAbNormal_044, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);


    OH_DRM_MediakeyIdArray *mediaKeyIdss = nullptr;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyIds(mediaKeySystem, nullptr);
    EXPECT_NE(errNo, DRM_ERR_OK);
    if (mediaKeyIdss) {
        free(mediaKeyIdss);
        mediaKeyIdss = nullptr;
    }
    
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GenerateOfflineReleaseRequestNormal_045, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x30, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    OH_DRM_Uint8Buffer mediaKeyIds;
    mediaKeyIds.buffer = mediaKeyId;
    mediaKeyIds.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    EXPECT_EQ(OfflineMediaKeyStatus, 1);

    unsigned char *releaseRequest = nullptr;
    int32_t releaseRequestLen;
    OH_DRM_Uint8Buffer mediaKeyIdToRelease;
    mediaKeyIdToRelease.buffer = mediaKeyId;
    mediaKeyIdToRelease.bufferLen = keyIdLen;

    errNo = OH_MediaKeySession_GenerateOfflineReleaseRequest(mediaKeySession, &mediaKeyIdToRelease, &releaseRequest,
        &releaseRequestLen);
    if (releaseRequest) {
        free(releaseRequest);
        releaseRequest = nullptr;
    }
    
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GenerateOfflineReleaseRequestAbNormal_046, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x30, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    OH_DRM_Uint8Buffer mediaKeyIds;
    mediaKeyIds.buffer = mediaKeyId;
    mediaKeyIds.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    EXPECT_EQ(OfflineMediaKeyStatus, 1);
    unsigned char *releaseRequest = nullptr;
    int32_t releaseRequestLen;
    OH_DRM_Uint8Buffer mediaKeyIdToRelease;
    mediaKeyIdToRelease.buffer = mediaKeyId;
    mediaKeyIdToRelease.bufferLen = keyIdLen;
    errNo = OH_MediaKeySession_GenerateOfflineReleaseRequest(nullptr, &mediaKeyIdToRelease, &releaseRequest,
        &releaseRequestLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo =
        OH_MediaKeySession_GenerateOfflineReleaseRequest(mediaKeySession, nullptr, &releaseRequest, &releaseRequestLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySession_GenerateOfflineReleaseRequest(mediaKeySession, &mediaKeyIdToRelease, nullptr,
        &releaseRequestLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySession_GenerateOfflineReleaseRequest(mediaKeySession, &mediaKeyIdToRelease, &releaseRequest,
        nullptr);
    EXPECT_NE(errNo, DRM_ERR_OK);
    if (mediaKeyId) {
        free(mediaKeyId);
        mediaKeyId = nullptr;
    }

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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_ProcessOfflineReleaseResponseNormal_047, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x30, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    OH_DRM_Uint8Buffer mediaKeyIds;
    mediaKeyIds.buffer = mediaKeyId;
    mediaKeyIds.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    EXPECT_EQ(OfflineMediaKeyStatus, 1);
    unsigned char *releaseRequest = nullptr;
    int32_t releaseRequestLen;
    OH_DRM_Uint8Buffer mediaKeyIdToRelease;
    mediaKeyIdToRelease.buffer = mediaKeyId;
    mediaKeyIdToRelease.bufferLen = keyIdLen;

    errNo = OH_MediaKeySession_GenerateOfflineReleaseRequest(mediaKeySession, &mediaKeyIdToRelease, &releaseRequest,
        &releaseRequestLen);
    if (releaseRequest) {
        free(releaseRequest);
        releaseRequest = nullptr;
    }
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char testKeyReleaseResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer keyReleaseResponse;
    keyReleaseResponse.buffer = testKeyReleaseResponse;
    keyReleaseResponse.bufferLen = sizeof(testKeyReleaseResponse);
    errNo =
        OH_MediaKeySession_ProcessOfflineReleaseResponse(mediaKeySession, &mediaKeyIdToRelease, &keyReleaseResponse);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(OfflineMediaKeyStatus, 2);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_ProcessOfflineReleaseResponseAbNormal_048, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x30, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    OH_DRM_Uint8Buffer mediaKeyIds;
    mediaKeyIds.buffer = mediaKeyId;
    mediaKeyIds.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    EXPECT_EQ(OfflineMediaKeyStatus, 1);

    unsigned char *releaseRequest = nullptr;
    int32_t releaseRequestLen;
    OH_DRM_Uint8Buffer mediaKeyIdToRelease;
    mediaKeyIdToRelease.buffer = mediaKeyId;
    mediaKeyIdToRelease.bufferLen = keyIdLen;

    errNo = OH_MediaKeySession_GenerateOfflineReleaseRequest(mediaKeySession, &mediaKeyIdToRelease, &releaseRequest,
        &releaseRequestLen);
    if (releaseRequest) {
        free(releaseRequest);
        releaseRequest = nullptr;
    }
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char testKeyReleaseResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer keyReleaseResponse;
    keyReleaseResponse.buffer = testKeyReleaseResponse;
    keyReleaseResponse.bufferLen = sizeof(testKeyReleaseResponse);
    errNo = OH_MediaKeySession_ProcessOfflineReleaseResponse(nullptr, &mediaKeyIdToRelease, &keyReleaseResponse);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySession_ProcessOfflineReleaseResponse(mediaKeySession, nullptr, &keyReleaseResponse);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySession_ProcessOfflineReleaseResponse(mediaKeySession, &mediaKeyIdToRelease, nullptr);
    EXPECT_NE(errNo, DRM_ERR_OK);
    if (mediaKeyId) {
        free(mediaKeyId);
        mediaKeyId = nullptr;
    }

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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GetCertificateStatusNormal_049, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_CertificateStatus certStatus = CERT_STATUS_GET_FAILED;
    errNo = OH_MediaKeySystem_GetCertificateStatus(mediaKeySystem, &certStatus);
    EXPECT_EQ(certStatus, 0);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GetCertificateStatusAbNormal_050, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_CertificateStatus certStatus = CERT_STATUS_GET_FAILED;
    errNo = OH_MediaKeySystem_GetCertificateStatus(nullptr, &certStatus);
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_GetCertificateStatus(mediaKeySystem, nullptr);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_SetConfigurationStringNormal_051, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_SetConfigurationString(mediaKeySystem, "testConfigurationString", "gezhegezhegezhe");
    EXPECT_EQ(errNo, DRM_ERR_OK);
    char *value = nullptr;
    int32_t valueLen = 0;
    errNo = OH_MediaKeySystem_GetConfigurationString(mediaKeySystem, "testConfigurationString", &value, &valueLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (value) {
        free(value);
        value = nullptr;
    }

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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_SetConfigurationStringAbNormal_052, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_SetConfigurationString(nullptr, "testConfigurationString", "gezhegezhegezhe");
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_SetConfigurationString(mediaKeySystem, "", "gezhegezhegezhe");
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_SetConfigurationString(mediaKeySystem, nullptr, "gezhegezhegezhe");
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_SetConfigurationString(mediaKeySystem, "testConfigurationString", "");
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_SetConfigurationString(mediaKeySystem, "testConfigurationString", nullptr);
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GetConfigurationStringAbNormal_053, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_SetConfigurationString(mediaKeySystem, "testConfigurationString", "gezhegezhegezhe");
    EXPECT_EQ(errNo, DRM_ERR_OK);

    char *value = nullptr;
    int32_t valueLen = 0;
    errNo = OH_MediaKeySystem_GetConfigurationString(nullptr, "testConfigurationString", &value, &valueLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_GetConfigurationString(mediaKeySystem, "", &value, &valueLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_GetConfigurationString(mediaKeySystem, nullptr, &value, &valueLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_GetConfigurationString(mediaKeySystem, "testConfigurationString", nullptr, &valueLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_GetConfigurationString(mediaKeySystem, "testConfigurationString", &value, nullptr);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GetConfigurationByteArrayNormal_054, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char testArray[5] = {1, 2, 3, 4, 5};
    errNo = OH_MediaKeySystem_SetConfigurationByteArray(mediaKeySystem, "testConfigurationByteArray", testArray, 5);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char *configByteArray = nullptr;
    uint32_t byteArrayLen = 0;
    errNo = OH_MediaKeySystem_GetConfigurationByteArray(mediaKeySystem, "testConfigurationByteArray", &configByteArray,
        &byteArrayLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (configByteArray) {
        free(configByteArray);
        configByteArray = nullptr;
    }

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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_SetConfigurationByteArrayAbNormal_055, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char testArray[5] = {1, 2, 3, 4, 5};
    errNo = OH_MediaKeySystem_SetConfigurationByteArray(nullptr, "testConfigurationByteArray", testArray, 5);
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_SetConfigurationByteArray(mediaKeySystem, "", testArray, 5);
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_SetConfigurationByteArray(mediaKeySystem, nullptr, testArray, 5);
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_SetConfigurationByteArray(mediaKeySystem, "testConfigurationByteArray", nullptr, 5);
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_SetConfigurationByteArray(mediaKeySystem, "testConfigurationByteArray", testArray, 0);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GetConfigurationByteArrayAbNormal_056, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char testArray[5] = {1, 2, 3, 4, 5};
    errNo = OH_MediaKeySystem_SetConfigurationByteArray(mediaKeySystem, "testConfigurationByteArray", testArray, 5);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char *configByteArray = nullptr;
    uint32_t byteArrayLen = 0;
    errNo = OH_MediaKeySystem_GetConfigurationByteArray(nullptr, "testConfigurationByteArray", &configByteArray,
        &byteArrayLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_GetConfigurationByteArray(mediaKeySystem, "", &configByteArray, &byteArrayLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_GetConfigurationByteArray(mediaKeySystem, nullptr, &configByteArray, &byteArrayLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_GetConfigurationByteArray(mediaKeySystem, "testConfigurationByteArray", nullptr,
        &byteArrayLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_GetConfigurationByteArray(mediaKeySystem, "testConfigurationByteArray", &configByteArray,
        nullptr);
    EXPECT_NE(errNo, DRM_ERR_OK);
    if (configByteArray) {
        free(configByteArray);
        configByteArray = nullptr;
    }

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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GetStatisticsNormal_057, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_Statistics *statistics;
    errNo = OH_MediaKeySystem_GetStatistics(mediaKeySystem, &statistics);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (statistics) {
        free(statistics);
        statistics = nullptr;
    }

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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GetStatisticsAbNormal_058, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_Statistics *statistics;
    errNo = OH_MediaKeySystem_GetStatistics(nullptr, &statistics);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_GetStatistics(mediaKeySystem, nullptr);
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

/*
 * Feature: Framework
 * Function: Test and create DRM instances
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case Description: Test Creating DRM Instance
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GetMaxContentProtectionLevelAbNormal_059, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_UNKNOWN;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_GetMaxContentProtectionLevel(nullptr, &contentProtectionLevel);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_GetMaxContentProtectionLevel(mediaKeySystem, nullptr);
    EXPECT_NE(errNo, DRM_ERR_OK);

    if (mediaKeySystem != nullptr) {
        OH_MediaKeySystem_Destroy(mediaKeySystem);
    }
}

/*
 * Feature: Framework
 * Function: Processing device certificate response testing
 * Sub function: NA
 * Function point: NA
 * Environmental conditions: NA
 * Case Description: Processing Device Certificate Response Testing
 */
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_ClearOfflineMediaKeysNormal_060, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    OH_DRM_Uint8Buffer mediaKeyIds;
    mediaKeyIds.buffer = mediaKeyId;
    mediaKeyIds.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(OfflineMediaKeyStatus, 1);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *releaseRequest = nullptr;
    int32_t releaseRequestLen;
    OH_DRM_Uint8Buffer mediaKeyIdToRelease;
    mediaKeyIdToRelease.buffer = mediaKeyId;
    mediaKeyIdToRelease.bufferLen = keyIdLen;

    errNo = OH_MediaKeySession_GenerateOfflineReleaseRequest(mediaKeySession, &mediaKeyIdToRelease, &releaseRequest,
        &releaseRequestLen);
    if (releaseRequest) {
        free(releaseRequest);
        releaseRequest = nullptr;
    }
    
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char testKeyReleaseResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer keyReleaseResponse;
    keyReleaseResponse.buffer = testKeyReleaseResponse;
    keyReleaseResponse.bufferLen = sizeof(testKeyReleaseResponse);
    errNo =
        OH_MediaKeySession_ProcessOfflineReleaseResponse(mediaKeySession, &mediaKeyIdToRelease, &keyReleaseResponse);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(OfflineMediaKeyStatus, 2);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_Uint8Buffer mediaKeyIdToClear;
    mediaKeyIdToClear.buffer = mediaKeyId;
    mediaKeyIdToClear.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_ClearOfflineMediaKeys(mediaKeySystem, &mediaKeyIdToClear);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(OfflineMediaKeyStatus, 0);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_ClearOfflineMediaKeysAbNormal_061, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    OH_DRM_Uint8Buffer mediaKeyIds;
    mediaKeyIds.buffer = mediaKeyId;
    mediaKeyIds.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(OfflineMediaKeyStatus, 1);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *releaseRequest = nullptr;
    int32_t releaseRequestLen;
    OH_DRM_Uint8Buffer mediaKeyIdToRelease;
    mediaKeyIdToRelease.buffer = mediaKeyId;
    mediaKeyIdToRelease.bufferLen = keyIdLen;

    errNo = OH_MediaKeySession_GenerateOfflineReleaseRequest(mediaKeySession, &mediaKeyIdToRelease, &releaseRequest,
        &releaseRequestLen);
    if (releaseRequest) {
        free(releaseRequest);
        releaseRequest = nullptr;
    }
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char testKeyReleaseResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer keyReleaseResponse;
    keyReleaseResponse.buffer = testKeyReleaseResponse;
    keyReleaseResponse.bufferLen = sizeof(testKeyReleaseResponse);
    errNo =
        OH_MediaKeySession_ProcessOfflineReleaseResponse(mediaKeySession, &mediaKeyIdToRelease, &keyReleaseResponse);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(OfflineMediaKeyStatus, 2);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_Uint8Buffer mediaKeyIdToClear;
    mediaKeyIdToClear.buffer = mediaKeyId;
    mediaKeyIdToClear.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_ClearOfflineMediaKeys(nullptr, &mediaKeyIdToClear);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_ClearOfflineMediaKeys(mediaKeySystem, nullptr);
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(OfflineMediaKeyStatus, 2);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_RestoreOfflineMediaKeysNormal_062, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    OH_DRM_Uint8Buffer mediaKeyIds;
    mediaKeyIds.buffer = mediaKeyId;
    mediaKeyIds.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(OfflineMediaKeyStatus, 1);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *releaseRequest = nullptr;
    int32_t releaseRequestLen;
    OH_DRM_Uint8Buffer mediaKeyIdToRelease;
    mediaKeyIdToRelease.buffer = mediaKeyId;
    mediaKeyIdToRelease.bufferLen = keyIdLen;

    errNo = OH_MediaKeySession_GenerateOfflineReleaseRequest(mediaKeySession, &mediaKeyIdToRelease, &releaseRequest,
        &releaseRequestLen);
    if (releaseRequest) {
        free(releaseRequest);
        releaseRequest = nullptr;
    }
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char testKeyReleaseResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer keyReleaseResponse;
    keyReleaseResponse.buffer = testKeyReleaseResponse;
    keyReleaseResponse.bufferLen = sizeof(testKeyReleaseResponse);
    errNo =
        OH_MediaKeySession_ProcessOfflineReleaseResponse(mediaKeySession, &mediaKeyIdToRelease, &keyReleaseResponse);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(OfflineMediaKeyStatus, 2);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySession_RestoreOfflineMediaKeys(mediaKeySession, &mediaKeyIdToRelease);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(OfflineMediaKeyStatus, 1);
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
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_RestoreOfflineMediaKeysAbNormal_063, TestSize.Level0)
{
    OH_DrmErrCode errNo = DRM_ERR_ERROR;
    OH_MediaKeySystem *mediaKeySystem = nullptr;
    OH_MediaKeySession *mediaKeySession = nullptr;
    OH_DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.drm.clearplay", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);
        request = nullptr;
    }
    if (defaultUrl) {
        free(defaultUrl);
        defaultUrl = nullptr;
    }
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    OH_DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    OH_DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    OH_DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    OH_DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    OH_DRM_Uint8Buffer mediaKeyIds;
    mediaKeyIds.buffer = mediaKeyId;
    mediaKeyIds.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(OfflineMediaKeyStatus, 1);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *releaseRequest = nullptr;
    int32_t releaseRequestLen;
    OH_DRM_Uint8Buffer mediaKeyIdToRelease;
    mediaKeyIdToRelease.buffer = mediaKeyId;
    mediaKeyIdToRelease.bufferLen = keyIdLen;

    errNo = OH_MediaKeySession_GenerateOfflineReleaseRequest(mediaKeySession, &mediaKeyIdToRelease, &releaseRequest,
        &releaseRequestLen);
    if (releaseRequest) {
        free(releaseRequest);
        releaseRequest = nullptr;
    }
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char testKeyReleaseResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    OH_DRM_Uint8Buffer keyReleaseResponse;
    keyReleaseResponse.buffer = testKeyReleaseResponse;
    keyReleaseResponse.bufferLen = sizeof(testKeyReleaseResponse);
    errNo =
        OH_MediaKeySession_ProcessOfflineReleaseResponse(mediaKeySession, &mediaKeyIdToRelease, &keyReleaseResponse);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(OfflineMediaKeyStatus, 2);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySession_RestoreOfflineMediaKeys(nullptr, &mediaKeyIdToRelease);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySession_RestoreOfflineMediaKeys(mediaKeySession, nullptr);
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(OfflineMediaKeyStatus, 2);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}
} // DrmStandard
} // OHOS
