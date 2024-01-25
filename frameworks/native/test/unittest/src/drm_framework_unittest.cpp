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
#include <securec.h>
#include "drm_log.h"
#include "drm_error_code.h"
#include "native_drm_base.h"
#include "native_drm_object.h"
#include "key_session_impl.h"
#include "native_mediakeysession.h"
#include "cstdio"
#include "cstdlib"
#include <securec.h>
#include "native_drm_common.h"
#include "native_drm_err.h"
#include "gmock/gmock.h"
#include "drm_error_code.h"
#include "native_drm_base.h"
#include "native_drm_object.h"
#include "key_session_impl.h"
#include "i_mediadecryptmodule_service.h"
#include "native_mediakeysession.h"
#include "native_mediakeysystem.h"
#include <cstring>
#include <map>
#include <unordered_map>
#include "nocopyable.h"
#include "ipc_skeleton.h"
#include "iservice_registry.h"
#include "i_keysession_service.h"
#include "i_keysession_service_callback.h"
#include "key_session_service_callback_stub.h"
#include "cstdbool"
#include "drm_types.h"
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
    supported = OH_MediaKeySystem_IsSupported("com.clearplay.drm");
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
    supported = OH_MediaKeySystem_IsSupported2("com.clearplay.drm", "video/mp4");
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
    supported = OH_MediaKeySystem_IsSupported3("com.clearplay.drm", "video/mp4", CONTENT_PROTECTION_LEVEL_HW_CRYPTO);
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

    supported = OH_MediaKeySystem_IsSupported2("com.clearplay.drm", "ideo/mp4");
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported2("com.clearplay.drm", "");
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported2("com.clearplay.drm", nullptr);
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
    supported = OH_MediaKeySystem_IsSupported3("com.clearplay.drm", "ideo/mp4", CONTENT_PROTECTION_LEVEL_HW_CRYPTO);
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported3("com.clearplay.drm", "", CONTENT_PROTECTION_LEVEL_HW_CRYPTO);
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported3("com.clearplay.drm", nullptr, CONTENT_PROTECTION_LEVEL_HW_CRYPTO);
    EXPECT_EQ(supported, false);

    supported = OH_MediaKeySystem_IsSupported3("com.clearplay.drm", "video/mp4",
        (DRM_ContentProtectionLevel)(CONTENT_PROTECTION_LEVEL_UNKNOWN - 1));
    EXPECT_EQ(supported, false);

    supported = OH_MediaKeySystem_IsSupported3("com.clearplay.drm", "video/mp4",
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
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_NE(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_NE(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;

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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_MediaKeyDescription *mediaKeyDescription = nullptr;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_MediaKeyDescription *mediaKeyDescription = nullptr;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_MediaKeyDescription *mediaKeyDescription = nullptr;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_MediaKeyDescription *mediaKeyDescription = nullptr;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_MediaKeyDescription *mediaKeyDescription = nullptr;
    errNo = OH_MediaKeySession_CheckMediaKeyStatus(mediaKeySession, &mediaKeyDescription);
    if (mediaKeyDescription) {
        free(mediaKeyDescription);
        mediaKeyDescription = nullptr;
    }
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    DRM_Uint8Buffer mediaKeyIds;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_MediaKeyDescription *mediaKeyDescription = nullptr;
    errNo = OH_MediaKeySession_CheckMediaKeyStatus(mediaKeySession, &mediaKeyDescription);
    if (mediaKeyDescription) {
        free(mediaKeyDescription);
        mediaKeyDescription = nullptr;
    }
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    DRM_Uint8Buffer mediaKeyIds;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_MediaKeyDescription *mediaKeyDescription = nullptr;
    errNo = OH_MediaKeySession_CheckMediaKeyStatus(mediaKeySession, &mediaKeyDescription);
    if (mediaKeyDescription) {
        free(mediaKeyDescription);
        mediaKeyDescription = nullptr;
    }
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;

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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_MediaKeyDescription *mediaKeyDescription = nullptr;
    errNo = OH_MediaKeySession_CheckMediaKeyStatus(mediaKeySession, &mediaKeyDescription);
    if (mediaKeyDescription) {
        free(mediaKeyDescription);
        mediaKeyDescription = nullptr;
    }
    EXPECT_EQ(errNo, DRM_ERR_OK);


    DRM_Uint8Buffer mediaKeyIds;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);


    DRM_MediakeyIdArray *mediaKeyIdss = nullptr;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);


    DRM_MediakeyIdArray *mediaKeyIdss = nullptr;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);


    DRM_MediakeyIdArray *mediaKeyIdss = nullptr;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x30, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    DRM_Uint8Buffer mediaKeyIds;
    mediaKeyIds.buffer = mediaKeyId;
    mediaKeyIds.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    EXPECT_EQ(OfflineMediaKeyStatus, 1);

    unsigned char *releaseRequest = nullptr;
    int32_t releaseRequestLen;
    DRM_Uint8Buffer mediaKeyIdToRelease;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x30, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    DRM_Uint8Buffer mediaKeyIds;
    mediaKeyIds.buffer = mediaKeyId;
    mediaKeyIds.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    EXPECT_EQ(OfflineMediaKeyStatus, 1);
    unsigned char *releaseRequest = nullptr;
    int32_t releaseRequestLen;
    DRM_Uint8Buffer mediaKeyIdToRelease;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x30, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    DRM_Uint8Buffer mediaKeyIds;
    mediaKeyIds.buffer = mediaKeyId;
    mediaKeyIds.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    EXPECT_EQ(OfflineMediaKeyStatus, 1);
    unsigned char *releaseRequest = nullptr;
    int32_t releaseRequestLen;
    DRM_Uint8Buffer mediaKeyIdToRelease;
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
    DRM_Uint8Buffer keyReleaseResponse;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x30, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    DRM_Uint8Buffer mediaKeyIds;
    mediaKeyIds.buffer = mediaKeyId;
    mediaKeyIds.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    EXPECT_EQ(OfflineMediaKeyStatus, 1);

    unsigned char *releaseRequest = nullptr;
    int32_t releaseRequestLen;
    DRM_Uint8Buffer mediaKeyIdToRelease;
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
    DRM_Uint8Buffer keyReleaseResponse;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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

    DRM_CertificateStatus certStatus = CERT_STATUS_INVALID;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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

    DRM_CertificateStatus certStatus = CERT_STATUS_INVALID;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_Uint8Buffer testArrayData;
    testArrayData.buffer = testArray;
    testArrayData.bufferLen = sizeof(testArray);
    errNo = OH_MediaKeySystem_SetConfigurationByteArray(mediaKeySystem, "testConfigurationByteArray", &testArrayData);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char *configByteArray = nullptr;
    int32_t byteArrayLen = 0;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_Uint8Buffer testArrayData;
    testArrayData.buffer = testArray;
    testArrayData.bufferLen = sizeof(testArray);
    errNo = OH_MediaKeySystem_SetConfigurationByteArray(nullptr, "testConfigurationByteArray", &testArrayData);
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_SetConfigurationByteArray(mediaKeySystem, "", &testArrayData);
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_SetConfigurationByteArray(mediaKeySystem, nullptr, &testArrayData);
    EXPECT_NE(errNo, DRM_ERR_OK);
    testArrayData.buffer = nullptr;
    errNo = OH_MediaKeySystem_SetConfigurationByteArray(mediaKeySystem, "testConfigurationByteArray", &testArrayData);
    EXPECT_NE(errNo, DRM_ERR_OK);
    testArrayData.buffer = testArray;
    testArrayData.bufferLen = 0;
    errNo = OH_MediaKeySystem_SetConfigurationByteArray(mediaKeySystem, "testConfigurationByteArray", &testArrayData);
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_Uint8Buffer testArrayData;
    testArrayData.buffer = testArray;
    testArrayData.bufferLen = sizeof(testArray);
    errNo = OH_MediaKeySystem_SetConfigurationByteArray(mediaKeySystem, "testConfigurationByteArray", &testArrayData);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char *configByteArray = nullptr;
    int32_t byteArrayLen = 0;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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

    DRM_Statistics *statistics;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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

    DRM_Statistics *statistics;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_UNKNOWN;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    DRM_Uint8Buffer mediaKeyIds;
    mediaKeyIds.buffer = mediaKeyId;
    mediaKeyIds.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(OfflineMediaKeyStatus, 1);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *releaseRequest = nullptr;
    int32_t releaseRequestLen;
    DRM_Uint8Buffer mediaKeyIdToRelease;
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
    DRM_Uint8Buffer keyReleaseResponse;
    keyReleaseResponse.buffer = testKeyReleaseResponse;
    keyReleaseResponse.bufferLen = sizeof(testKeyReleaseResponse);
    errNo =
        OH_MediaKeySession_ProcessOfflineReleaseResponse(mediaKeySession, &mediaKeyIdToRelease, &keyReleaseResponse);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(OfflineMediaKeyStatus, 2);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_Uint8Buffer mediaKeyIdToClear;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    DRM_Uint8Buffer mediaKeyIds;
    mediaKeyIds.buffer = mediaKeyId;
    mediaKeyIds.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(OfflineMediaKeyStatus, 1);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *releaseRequest = nullptr;
    int32_t releaseRequestLen;
    DRM_Uint8Buffer mediaKeyIdToRelease;
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
    DRM_Uint8Buffer keyReleaseResponse;
    keyReleaseResponse.buffer = testKeyReleaseResponse;
    keyReleaseResponse.bufferLen = sizeof(testKeyReleaseResponse);
    errNo =
        OH_MediaKeySession_ProcessOfflineReleaseResponse(mediaKeySession, &mediaKeyIdToRelease, &keyReleaseResponse);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(OfflineMediaKeyStatus, 2);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_Uint8Buffer mediaKeyIdToClear;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    DRM_Uint8Buffer mediaKeyIds;
    mediaKeyIds.buffer = mediaKeyId;
    mediaKeyIds.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(OfflineMediaKeyStatus, 1);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *releaseRequest = nullptr;
    int32_t releaseRequestLen;
    DRM_Uint8Buffer mediaKeyIdToRelease;
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
    DRM_Uint8Buffer keyReleaseResponse;
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
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    DRM_Uint8Buffer mediaKeyIds;
    mediaKeyIds.buffer = mediaKeyId;
    mediaKeyIds.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(OfflineMediaKeyStatus, 1);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *releaseRequest = nullptr;
    int32_t releaseRequestLen;
    DRM_Uint8Buffer mediaKeyIdToRelease;
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
    DRM_Uint8Buffer keyReleaseResponse;
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

#define MAX_BUFF 139
static void filldata(DRM_MediaKeyRequestInfo *info, DRM_CharBufferPair *sample, unsigned char *testData)
{
    info->type = MEDIA_KEY_TYPE_ONLINE;
    info->mimeType.buffer = (char *)"video/mp4";
    info->mimeType.bufferLen = sizeof("video/mp4");
    info->data.buffer = testData;
    info->data.bufferLen = MAX_BUFF;
    info->optionsCount = 1;
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info->optionsData = sample;
}

const unsigned char TESTKEYRELEASERESPONSE[50] = {0x31, 0x64, 0x6E, 0x5A,
    0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42,
    0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A,
    0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57,
    0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
const unsigned char TESTKEYSESSIONRESPONSE[50] = {0x31, 0x64, 0x6E, 0x5A,
    0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42,
    0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A,
    0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57,
    0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};

static void filltest3(MediaKeySystem *mediaKeySystem, MediaKeySession *mediaKeySession)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    unsigned char *mediaKeyId = nullptr;
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = (unsigned char *)TESTKEYSESSIONRESPONSE;
    response.bufferLen = sizeof(TESTKEYSESSIONRESPONSE);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    DRM_Uint8Buffer mediaKeyIds;
    mediaKeyIds.buffer = mediaKeyId;
    mediaKeyIds.bufferLen = keyIdLen;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    EXPECT_EQ(OfflineMediaKeyStatus, 1);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *releaseRequest = nullptr;
    int32_t releaseRequestLen;
    DRM_Uint8Buffer mediaKeyIdToRelease;
    mediaKeyIdToRelease.buffer = mediaKeyId;
    mediaKeyIdToRelease.bufferLen = keyIdLen;

    errNo = OH_MediaKeySession_GenerateOfflineReleaseRequest(mediaKeySession, &mediaKeyIdToRelease, &releaseRequest,
        &releaseRequestLen);
    free(releaseRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    DRM_Uint8Buffer keyReleaseResponse;
    keyReleaseResponse.buffer = (unsigned char *)TESTKEYRELEASERESPONSE;
    keyReleaseResponse.bufferLen = sizeof(TESTKEYRELEASERESPONSE);
    errNo =
        OH_MediaKeySession_ProcessOfflineReleaseResponse(mediaKeySession, &mediaKeyIdToRelease, &keyReleaseResponse);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_ContentProtectionLevel sessionContentProtectionLevel = CONTENT_PROTECTION_LEVEL_UNKNOWN;
    errNo = OH_MediaKeySession_GetContentProtectionLevel(mediaKeySession, &sessionContentProtectionLevel);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySession_GetContentProtectionLevel(nullptr, &sessionContentProtectionLevel);
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySession_GetContentProtectionLevel(mediaKeySession, nullptr);
    EXPECT_NE(errNo, DRM_ERR_OK);
}
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GetContentProtectionLevelNormal_064, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
        free(request);}
    if (defaultUrl) {
        free(defaultUrl);}
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    // mediakeysession
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    DRM_CharBufferPair sample[1];
    filldata(&info, sample, testData);
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    filltest3(mediaKeySystem, mediaKeySession);
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_NE(errNo, DRM_ERR_OK);
}


#define DRM_SAMPLE_CHECK_AND_RETURN_RET_LOG(cond, ret, fmt, ...) \
    do {                                                         \
        if ((cond)) {                                            \
            printf(fmt, ##__VA_ARGS__);                          \
            return ret;                                          \
        }                                                        \
    } while (0)
#define DRM_SAMPLE_INFO_LOG(fmt, ...) fprintf(stdout, "[INFO] " fmt "\n", ##__VA_ARGS__)
#define DRM_SAMPLE_ERROR_LOG(fmt, ...) fprintf(stdout, "[ERROR] " fmt "\n", ##__VA_ARGS__)

Drm_ErrCode TestSystemEventCallBack(DRM_ListenerType eventType, unsigned char *info,
    int32_t infoLen, char *extra)
{
    DRM_SAMPLE_INFO_LOG("TestSystemEventCallBack ok");
    return DRM_ERR_OK;
}

Drm_ErrCode TestSessoinEventCallBack(DRM_ListenerType eventType, unsigned char *info,
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


HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_GetContentProtectionLevelNormal_065, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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


    // test 3: system callback
    errNo = OH_MediaKeySystem_SetMediaKeySystemCallback(mediaKeySystem, &TestSystemEventCallBack);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    MediaKeySession_Callback sessionCallback = { &TestSessoinEventCallBack, &TestSessoinKeyChangeCallBack };
    errNo = OH_MediaKeySession_SetMediaKeySessionCallback(mediaKeySession, &sessionCallback);
    EXPECT_EQ(errNo, DRM_ERR_OK);


    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_DecryptModuleNormal_066, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0, defaultUrlLen = 0;
    char *defaultUrl = nullptr;
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
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    DRM_CharBufferPair sample[1];
    filldata(&info, sample, testData);
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_DecryptModuleNormal_067, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    int32_t releaseRequestLen;
    unsigned char *releaseRequest = (unsigned char *)&releaseRequestLen;
    DRM_Uint8Buffer mediaKeyIdToRelease;
    mediaKeyIdToRelease.buffer = nullptr;
    mediaKeyIdToRelease.bufferLen = 11;
    MediaKeySession *mediaKeySession = (MediaKeySession *)&mediaKeyIdToRelease;
    errNo = OH_MediaKeySession_GenerateOfflineReleaseRequest(mediaKeySession, &mediaKeyIdToRelease, &releaseRequest,
        &releaseRequestLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    mediaKeyIdToRelease.buffer = (unsigned char *)&releaseRequestLen;
    mediaKeyIdToRelease.bufferLen = 0;
    errNo = OH_MediaKeySession_GenerateOfflineReleaseRequest(mediaKeySession, &mediaKeyIdToRelease, &releaseRequest,
        &releaseRequestLen);
    EXPECT_NE(errNo, DRM_ERR_OK);

    unsigned char testKeyReleaseResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer keyReleaseResponse;
    keyReleaseResponse.buffer = testKeyReleaseResponse;
    keyReleaseResponse.bufferLen = sizeof(testKeyReleaseResponse);
    mediaKeyIdToRelease.buffer = (unsigned char *)&releaseRequestLen;
    mediaKeyIdToRelease.bufferLen = 0;
    errNo =
        OH_MediaKeySession_ProcessOfflineReleaseResponse(mediaKeySession, &mediaKeyIdToRelease, &keyReleaseResponse);
    EXPECT_NE(errNo, DRM_ERR_OK);
    mediaKeyIdToRelease.buffer = nullptr;
    mediaKeyIdToRelease.bufferLen = 11;
    errNo =
        OH_MediaKeySession_ProcessOfflineReleaseResponse(mediaKeySession, &mediaKeyIdToRelease, &keyReleaseResponse);
    EXPECT_NE(errNo, DRM_ERR_OK);
    mediaKeyIdToRelease.buffer = (unsigned char *)&releaseRequestLen;
    mediaKeyIdToRelease.bufferLen = 11;
    keyReleaseResponse.buffer = nullptr;
    errNo =
        OH_MediaKeySession_ProcessOfflineReleaseResponse(mediaKeySession, &mediaKeyIdToRelease, &keyReleaseResponse);
    EXPECT_NE(errNo, DRM_ERR_OK);
    keyReleaseResponse.buffer = testKeyReleaseResponse;
    keyReleaseResponse.bufferLen = 0;
    errNo =
        OH_MediaKeySession_ProcessOfflineReleaseResponse(mediaKeySession, &mediaKeyIdToRelease, &keyReleaseResponse);
    EXPECT_NE(errNo, DRM_ERR_OK);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_RestoreOfflineMediaKeysAbNormal_068, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    DRM_Uint8Buffer mediaKeyIdToRelease;
    mediaKeyIdToRelease.buffer = (unsigned char *)&mediaKeyIdToRelease;
    mediaKeyIdToRelease.bufferLen = 0;
    MediaKeySession *mediaKeySession = (MediaKeySession *)&mediaKeyIdToRelease;
    errNo = OH_MediaKeySession_RestoreOfflineMediaKeys(mediaKeySession, &mediaKeyIdToRelease);
    EXPECT_NE(errNo, DRM_ERR_OK);
    mediaKeyIdToRelease.buffer = nullptr;
    mediaKeyIdToRelease.bufferLen = 11;
    errNo = OH_MediaKeySession_RestoreOfflineMediaKeys(mediaKeySession, &mediaKeyIdToRelease);
    EXPECT_NE(errNo, DRM_ERR_OK);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest__SetMediaKeySessionCallbackNormal_069, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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


    // test 3: system callback
    errNo = OH_MediaKeySystem_SetMediaKeySystemCallback(nullptr, &TestSystemEventCallBack);
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_SetMediaKeySystemCallback(mediaKeySystem, nullptr);
    EXPECT_NE(errNo, DRM_ERR_OK);

    MediaKeySession_Callback sessionCallback = { &TestSessoinEventCallBack, &TestSessoinKeyChangeCallBack };
    errNo = OH_MediaKeySession_SetMediaKeySessionCallback(mediaKeySession, nullptr);
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySession_SetMediaKeySessionCallback(nullptr, &sessionCallback);
    EXPECT_NE(errNo, DRM_ERR_OK);


    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest__CreateMediaKeySessionAbNormal_070, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create(nullptr, &mediaKeySystem);
    EXPECT_EQ(mediaKeySystem, nullptr);

    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", nullptr);
    EXPECT_EQ(mediaKeySystem, nullptr);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(nullptr, &contentProtectionLevel, &mediaKeySession);
    EXPECT_EQ(mediaKeySession, nullptr);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, nullptr, &mediaKeySession);
    EXPECT_EQ(mediaKeySession, nullptr);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, nullptr);
    EXPECT_EQ(mediaKeySession, nullptr);

    DRM_Uint8Buffer mediaKeyIdToClear;
    mediaKeyIdToClear.buffer = (unsigned char *)&mediaKeyIdToClear;
    mediaKeyIdToClear.bufferLen = 0;
    errNo = OH_MediaKeySystem_ClearOfflineMediaKeys(mediaKeySystem, &mediaKeyIdToClear);
    EXPECT_NE(errNo, DRM_ERR_OK);
    mediaKeyIdToClear.buffer = nullptr;
    mediaKeyIdToClear.bufferLen = 11;
    errNo = OH_MediaKeySystem_ClearOfflineMediaKeys(mediaKeySystem, &mediaKeyIdToClear);
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySession_Destroy(nullptr);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(nullptr);
    EXPECT_NE(errNo, DRM_ERR_OK);
}


HWTEST_F(DrmFrameworkUnitTest, Drm_unittest__CreateMediaKeySessionAbNormal_071, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    free(request);
    free(defaultUrl);
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_SetConfigurationString(mediaKeySystem, "testConfigurationString", "gezhegezhegezhe");
    EXPECT_NE(errNo, DRM_ERR_OK);

    char *value = nullptr;
    int32_t valueLen = 0;
    errNo = OH_MediaKeySystem_GetConfigurationString(mediaKeySystem, "testConfigurationString", &value, &valueLen);
    EXPECT_NE(errNo, DRM_ERR_OK);

    unsigned char testArray[5] = {1, 2, 3, 4, 5};
    DRM_Uint8Buffer testArrayData;
    testArrayData.buffer = testArray;
    testArrayData.bufferLen = sizeof(testArray);
    errNo = OH_MediaKeySystem_SetConfigurationByteArray(mediaKeySystem, "testConfigurationByteArray", &testArrayData);
    EXPECT_NE(errNo, DRM_ERR_OK);

    unsigned char *configByteArray = nullptr;
    int32_t byteArrayLen = 0;
    errNo = OH_MediaKeySystem_GetConfigurationByteArray(mediaKeySystem, "testConfigurationByteArray", &configByteArray,
        &byteArrayLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    DRM_Statistics *statistics;
    errNo = OH_MediaKeySystem_GetStatistics(mediaKeySystem, &statistics);
    EXPECT_NE(errNo, DRM_ERR_OK);

    contentProtectionLevel = CONTENT_PROTECTION_LEVEL_UNKNOWN;
    errNo = OH_MediaKeySystem_GetMaxContentProtectionLevel(mediaKeySystem, &contentProtectionLevel);

    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest__CreateMediaKeySessionAbNormal_072, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    unsigned char *request = nullptr;
    int32_t requestLen = 0, defaultUrlLen = 0;
    char *defaultUrl = nullptr;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);}
    if (defaultUrl) {
        free(defaultUrl);}
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    DRM_CertificateStatus certStatus = CERT_STATUS_INVALID;
    errNo = OH_MediaKeySystem_GetCertificateStatus(mediaKeySystem, &certStatus);
    errNo = OH_MediaKeySystem_SetMediaKeySystemCallback(mediaKeySystem, &TestSystemEventCallBack);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    contentProtectionLevel = CONTENT_PROTECTION_LEVEL_UNKNOWN;
    MediaKeySession *OH_MediaKeySession2 = nullptr;
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &OH_MediaKeySession2);
    EXPECT_EQ(OH_MediaKeySession2, nullptr);
    EXPECT_NE(errNo, DRM_ERR_OK);
    DRM_MediakeyIdArray *mediaKeyIdss = nullptr;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyIds(mediaKeySystem, &mediaKeyIdss);
    DRM_OfflineMediaKeyStatus OfflineMediaKeyStatus = OFFLINE_MEDIA_KEY_STATUS_UNKNOWN;
    DRM_Uint8Buffer mediaKeyIds, mediaKeyIdToClear;
    mediaKeyIds.buffer = (unsigned char *)&OfflineMediaKeyStatus;
    mediaKeyIds.bufferLen = 1;
    errNo = OH_MediaKeySystem_GetOfflineMediaKeyStatus(mediaKeySystem, &mediaKeyIds, &OfflineMediaKeyStatus);
    mediaKeyIdToClear.buffer = (unsigned char *)&OfflineMediaKeyStatus;
    mediaKeyIdToClear.bufferLen = 1;
    errNo = OH_MediaKeySystem_ClearOfflineMediaKeys(mediaKeySystem, &mediaKeyIdToClear);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest__CreateMediaKeySessionAbNormal_073, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    info.type = MEDIA_KEY_TYPE_ONLINE;
    info.mimeType.buffer = (char *)"video/mp4";
    info.mimeType.bufferLen = sizeof("video/mp4");
    info.data.buffer = testData;
    info.data.bufferLen = sizeof(testData);
    info.optionsCount = 1;
    DRM_CharBufferPair sample[1];
    sample[0].name.buffer = (char *)"optionalDataName";
    sample[0].name.bufferLen = sizeof("optionalDataName");
    sample[0].value.buffer = (char *)"optionalDataValue";
    sample[0].value.bufferLen = sizeof("optionalDataValue");
    info.optionsData = sample;
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest__CreateMediaKeySessionAbNormal_074, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest__CreateMediaKeySessionAbNormal_075, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    DRM_MediaKeyDescription *mediaKeyDescription = nullptr;
    errNo = OH_MediaKeySession_CheckMediaKeyStatus(mediaKeySession, &mediaKeyDescription);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySession_ClearMediaKeys(mediaKeySession);
    EXPECT_NE(errNo, DRM_ERR_OK);
    unsigned char *releaseRequest = nullptr;
    int32_t releaseRequestLen;
    DRM_Uint8Buffer mediaKeyIdToRelease;
    mediaKeyIdToRelease.buffer = (unsigned char *)"com.clearplay.drm";
    mediaKeyIdToRelease.bufferLen = 11;
    errNo = OH_MediaKeySession_GenerateOfflineReleaseRequest(mediaKeySession, &mediaKeyIdToRelease, &releaseRequest,
        &releaseRequestLen);
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest__CreateMediaKeySessionAbNormal_076, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
        free(request);}
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
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char testKeyReleaseResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer keyReleaseResponse;
    keyReleaseResponse.buffer = testKeyReleaseResponse;
    keyReleaseResponse.bufferLen = sizeof(testKeyReleaseResponse);
    DRM_Uint8Buffer mediaKeyIdToRelease;
    mediaKeyIdToRelease.buffer = (unsigned char *)"com.clearplay.drm";
    mediaKeyIdToRelease.bufferLen = 11;
    errNo =
        OH_MediaKeySession_ProcessOfflineReleaseResponse(mediaKeySession, &mediaKeyIdToRelease, &keyReleaseResponse);
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySession_RestoreOfflineMediaKeys(mediaKeySession, &mediaKeyIdToRelease);
    EXPECT_NE(errNo, DRM_ERR_OK);

    DRM_ContentProtectionLevel sessionContentProtectionLevel = CONTENT_PROTECTION_LEVEL_UNKNOWN;
    errNo = OH_MediaKeySession_GetContentProtectionLevel(mediaKeySession, &sessionContentProtectionLevel);
    EXPECT_NE(errNo, DRM_ERR_OK);

    bool requireSecureDecoder;
    errNo = OH_MediaKeySession_RequireSecureDecoderModule(mediaKeySession, "video/mp4", &requireSecureDecoder);
    EXPECT_NE(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest__CreateMediaKeySessionAbNormal_077, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);

    errNo = OH_MediaKeySystem_SetMediaKeySystemCallback(mediaKeySystem, &TestSystemEventCallBack);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr, *mediaKeyId = nullptr;
    int32_t requestLen = 0, defaultUrlLen = 0;
    char *defaultUrl = nullptr;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);}
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

    MediaKeySession_Callback sessionCallback = { &TestSessoinEventCallBack, &TestSessoinKeyChangeCallBack };
    errNo = OH_MediaKeySession_SetMediaKeySessionCallback(mediaKeySession, &sessionCallback);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    DRM_CharBufferPair sample[1];
    filldata(&info, sample, testData);
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    free(mediaKeyId);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest__CreateMediaKeySessionAbNormal_078, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    errNo = OH_MediaKeySystem_SetMediaKeySystemCallback(mediaKeySystem, &TestSystemEventCallBack);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char *request = nullptr;
    int32_t requestLen = 0;
    char *defaultUrl = nullptr;
    int32_t defaultUrlLen = 0;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);}
    if (defaultUrl) {
        free(defaultUrl);}
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    MediaKeySession_Callback sessionCallback = { &TestSessoinEventCallBack, &TestSessoinKeyChangeCallBack };
    errNo = OH_MediaKeySession_SetMediaKeySessionCallback(mediaKeySession, &sessionCallback);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    // mediakeysession
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    DRM_CharBufferPair sample[1];
    filldata(&info, sample, testData);
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[2] = {0x07, 0x22};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest__CreateMediaKeySessionAbNormal_079, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *request = nullptr;
    int32_t requestLen = 0, defaultUrlLen = 0;
    char *defaultUrl = nullptr;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    free(request);
    free(defaultUrl);
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    // mediakeysession
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    DRM_CharBufferPair sample[1];
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    filldata(&info, sample, testData);
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0, releaseRequestLen;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *releaseRequest = nullptr;
    DRM_Uint8Buffer mediaKeyIdToRelease;
    mediaKeyIdToRelease.buffer = (unsigned char *)"mediaKeyId";
    mediaKeyIdToRelease.bufferLen = 2;
    errNo = OH_MediaKeySession_GenerateOfflineReleaseRequest(mediaKeySession, &mediaKeyIdToRelease, &releaseRequest,
        &releaseRequestLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest__CreateMediaKeySessionAbNormal_080, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    unsigned char *request = nullptr, *mediaKeyId = nullptr, *releaseRequest = nullptr;
    int32_t requestLen = 0, defaultUrlLen = 0;
    char *defaultUrl = nullptr;
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    if (request) {
        free(request);}
    if (defaultUrl) {
        free(defaultUrl);}
    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    // mediakeysession
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    DRM_CharBufferPair sample[1];
    filldata(&info, sample, testData);
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response, mediaKeyIdToRelease, keyReleaseResponse;
    int32_t keyIdLen = 0, releaseRequestLen;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    mediaKeyIdToRelease.buffer = mediaKeyId;
    mediaKeyIdToRelease.bufferLen = keyIdLen;
    errNo = OH_MediaKeySession_GenerateOfflineReleaseRequest(mediaKeySession, &mediaKeyIdToRelease, &releaseRequest,
        &releaseRequestLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char testKeyReleaseResponse[2] = {0x33, 0x22};
    keyReleaseResponse.buffer = testKeyReleaseResponse;
    keyReleaseResponse.bufferLen = sizeof(testKeyReleaseResponse);
    errNo =
        OH_MediaKeySession_ProcessOfflineReleaseResponse(mediaKeySession, &mediaKeyIdToRelease, &keyReleaseResponse);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
}

static void filltest(MediaKeySystem *mediaKeySystem, MediaKeySession *mediaKeySession)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    unsigned char *mediaKeyId = nullptr;
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = (unsigned char *)TESTKEYSESSIONRESPONSE;
    response.bufferLen = sizeof(TESTKEYSESSIONRESPONSE);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    unsigned char *releaseRequest = nullptr;
    int32_t releaseRequestLen;
    DRM_Uint8Buffer mediaKeyIdToRelease;
    mediaKeyIdToRelease.buffer = mediaKeyId;
    mediaKeyIdToRelease.bufferLen = keyIdLen;
    errNo = OH_MediaKeySession_GenerateOfflineReleaseRequest(mediaKeySession, &mediaKeyIdToRelease, &releaseRequest,
        &releaseRequestLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    DRM_Uint8Buffer keyReleaseResponse;
    keyReleaseResponse.buffer = (unsigned char *)TESTKEYRELEASERESPONSE;
    keyReleaseResponse.bufferLen = sizeof(TESTKEYRELEASERESPONSE);
    errNo =
        OH_MediaKeySession_ProcessOfflineReleaseResponse(mediaKeySession, &mediaKeyIdToRelease, &keyReleaseResponse);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    mediaKeyIdToRelease.buffer = (unsigned char *)"mediaKeyId";
    mediaKeyIdToRelease.bufferLen = 1;
    errNo = OH_MediaKeySession_RestoreOfflineMediaKeys(mediaKeySession, &mediaKeyIdToRelease);
    EXPECT_NE(errNo, DRM_ERR_OK);
}
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest__CreateMediaKeySessionAbNormal_081, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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

    errNo = OH_MediaKeySession_ClearMediaKeys(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    // mediakeysession
    DRM_MediaKeyRequest *mediaKeyRequest = nullptr;
    DRM_MediaKeyRequestInfo info;
    unsigned char testData[139] = {0x00, 0x00, 0x00, 0x8B, 0x70, 0x73, 0x73, 0x68, 0x00, 0x00, 0x00, 0x00, 0x3D, 0x5E, 0x6D, 0x35, 0x9B, 0x9A, 0x41, 0xE8, 0xB8, 0x43, 0xDD, 0x3C, 0x6E, 0x72, 0xC4, 0x2C, 0x00, 0x00, 0x00, 0x6B, 0x7B, 0x22, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6F, 0x6E, 0x22, 0x3A, 0x22, 0x56, 0x31, 0x2E, 0x30, 0x22, 0x2C, 0x22, 0x63, 0x6F, 0x6E, 0x74, 0x65, 0x6E, 0x74, 0x49, 0x44, 0x22, 0x3A, 0x22, 0x64, 0x48, 0x4D, 0x74, 0x4D, 0x6A, 0x59, 0x30, 0x4C, 0x54, 0x45, 0x77, 0x4F, 0x44, 0x41, 0x74, 0x59, 0x57, 0x56, 0x7A, 0x22, 0x2C, 0x22, 0x6B, 0x69, 0x64, 0x73, 0x22, 0x3A, 0x5B, 0x22, 0x47, 0x2B, 0x45, 0x6B, 0x2F, 0x2B, 0x58, 0x6D, 0x55, 0x6B, 0x70, 0x42, 0x48, 0x51, 0x67, 0x58, 0x59, 0x57, 0x51, 0x51, 0x49, 0x67, 0x3D, 0x3D, 0x22, 0x5D, 0x2C, 0x22, 0x65, 0x6E, 0x73, 0x63, 0x68, 0x65, 0x6D, 0x61, 0x22, 0x3A, 0x22, 0x63, 0x62, 0x63, 0x31, 0x22, 0x7D};
    DRM_CharBufferPair sample[1];
    filldata(&info, sample, testData);
    errNo = OH_MediaKeySession_GenerateMediaKeyRequest(mediaKeySession, &info, &mediaKeyRequest);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    filltest(mediaKeySystem, mediaKeySession);

    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    sleep(1);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest__CreateMediaKeySessionAbNormal_082, TestSize.Level0)
{
    MediaKeySystem *mediaKeySystem = nullptr;
    Drm_ErrCode errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
    EXPECT_NE(mediaKeySystem, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);


    MediaKeySystemObject *systemObject = reinterpret_cast<MediaKeySystemObject *>(mediaKeySystem);
    IMediaKeySessionService::ContentProtectionLevel securityLevel = IMediaKeySessionService::SECURITY_LEVEL_SW_CRYPTO;
    OHOS::sptr<MediaKeySessionImpl> keySessionImpl = nullptr;
    systemObject->systemImpl_->~MediaKeySystemImpl();
    systemObject->systemImpl_->CreateMediaKeySession(securityLevel, &keySessionImpl);
    keySessionImpl = nullptr;

    MediaKeySystemCallback *allback = new MediaKeySystemCallback();
    allback->~MediaKeySystemCallback();

    OHOS::sptr<MediaKeySystemFactoryImpl> fatory = MediaKeySystemFactoryImpl::GetInstance();
    fatory->Init();

    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_NE(errNo, DRM_ERR_OK);

    MediaKeySystem *mediaKeySystem2 = nullptr;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem2);
    EXPECT_NE(mediaKeySystem2, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem2);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest__CreateMediaKeySessionAbNormal_083, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
        free(request);}
    if (defaultUrl) {
        free(defaultUrl);}

    unsigned char KeySystemResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    size_t KeySystemResponseLen = sizeof(KeySystemResponse);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(mediaKeySession, nullptr);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    MediaKeySession_Callback sessionCallback = { &TestSessoinEventCallBack, &TestSessoinKeyChangeCallBack };
    errNo = OH_MediaKeySession_SetMediaKeySessionCallback(mediaKeySession, &sessionCallback);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    unsigned char *mediaKeyId = nullptr;
    unsigned char testKeySessionResponse[50] = {0x31, 0x64, 0x6E, 0x5A, 0x32, 0x4E, 0x57, 0x74, 0x76, 0x4D, 0x47, 0x34, 0x34, 0x4E, 0x6A, 0x42, 0x30, 0x4D, 0x32, 0x77, 0x33, 0x4E, 0x67, 0x3D, 0x3D, 0x3A, 0x59, 0x7A, 0x56, 0x78, 0x63, 0x48, 0x64, 0x70, 0x61, 0x6D, 0x30, 0x34, 0x59, 0x57, 0x45, 0x34, 0x5A, 0x48, 0x6B, 0x79, 0x4D, 0x67, 0x3D, 0x3D};
    DRM_Uint8Buffer response;
    int32_t keyIdLen = 0;
    response.buffer = testKeySessionResponse;
    response.bufferLen = sizeof(testKeySessionResponse);
    errNo = OH_MediaKeySession_ProcessMediaKeyResponse(mediaKeySession, &response, &mediaKeyId, &keyIdLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(mediaKeySession);

    MediaKeySessionServiceCallback *Object = new MediaKeySessionServiceCallback(sessionObject->sessionImpl_);

    std::map<std::vector<uint8_t>, MediaKeySessionKeyStatus> statusTable = { { { 0x01 },
        MEDIA_KEY_SESSION_KEY_STATUS_USABLE } };

    bool hasNewGoodLicense = false;

    Object->SendEventKeyChanged(statusTable, hasNewGoodLicense);

    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}
static void killclearplay(MediaKeySystem *mediaKeySystem, MediaKeySession *mediaKeySession)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    std::vector<uint8_t> licenseIdVec = {0x01};
    std::vector<uint8_t> ReleaseRequest= {0x01};
    IMediaKeySessionService::ContentProtectionLevel securityLevel;
    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(mediaKeySession);
    system("killall -9 clearplay_host");
    uint32_t result = sessionObject->sessionImpl_->GenerateOfflineReleaseRequest(licenseIdVec, ReleaseRequest);
    EXPECT_NE(result, DRM_ERR_OK);
    result = sessionObject->sessionImpl_->ProcessOfflineReleaseResponse(licenseIdVec, ReleaseRequest);
    EXPECT_NE(result, DRM_ERR_OK);
    result = sessionObject->sessionImpl_->GetContentProtectionLevel(&securityLevel);
    EXPECT_NE(result, DRM_ERR_OK);
    std::map<std::string, std::string> licenseStatus2;
    result = sessionObject->sessionImpl_->CheckMediaKeyStatus(licenseStatus2);
    EXPECT_NE(result, DRM_ERR_OK);
    result = sessionObject->sessionImpl_->RestoreOfflineMediaKeys(licenseIdVec);
    EXPECT_NE(result, DRM_ERR_OK);
    result = sessionObject->sessionImpl_->ClearMediaKeys();
    EXPECT_NE(result, DRM_ERR_OK);
    std::string mimeType;
    bool status;
    result = sessionObject->sessionImpl_->RequireSecureDecoderModule(mimeType, &status);
    EXPECT_NE(result, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_SetMediaKeySystemCallback(mediaKeySystem, &TestSystemEventCallBack);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    MediaKeySession_Callback sessionCallback = { &TestSessoinEventCallBack, &TestSessoinKeyChangeCallBack };
    errNo = OH_MediaKeySession_SetMediaKeySessionCallback(mediaKeySession, &sessionCallback);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

static void killclearplay2(MediaKeySystem *mediaKeySystem, MediaKeySession *mediaKeySession)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    bool supported = OH_MediaKeySystem_IsSupported("com.clearplay.drm");
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported2("com.clearplay.drm", "video/mp4");
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported3("com.clearplay.drm", "video/mp4", CONTENT_PROTECTION_LEVEL_HW_CRYPTO);
    EXPECT_EQ(supported, false);
    errNo = OH_MediaKeySystem_SetConfigurationString(mediaKeySystem, "testConfigurationString",
    "testConfigurationString");
    EXPECT_NE(errNo, DRM_ERR_OK);
    char *value = nullptr;
    int32_t valueLen = 0;
    errNo = OH_MediaKeySystem_GetConfigurationString(mediaKeySystem, "testConfigurationString", &value, &valueLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    unsigned char testArray[5] = {1, 2, 3, 4, 5};
    DRM_Uint8Buffer testArrayData;
    testArrayData.buffer = testArray;
    testArrayData.bufferLen = sizeof(testArray);
    errNo = OH_MediaKeySystem_SetConfigurationByteArray(mediaKeySystem, "testConfigurationByteArray", &testArrayData);
    EXPECT_NE(errNo, DRM_ERR_OK);
    unsigned char *configByteArray = nullptr;
    int32_t byteArrayLen = 0;
    errNo = OH_MediaKeySystem_GetConfigurationByteArray(mediaKeySystem, "testConfigurationByteArray", &configByteArray,
        &byteArrayLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    DRM_Statistics *statistics;
    errNo = OH_MediaKeySystem_GetStatistics(mediaKeySystem, &statistics);
    EXPECT_NE(errNo, DRM_ERR_OK);
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_UNKNOWN;
    errNo = OH_MediaKeySystem_GetMaxContentProtectionLevel(mediaKeySystem, &contentProtectionLevel);
    EXPECT_NE(errNo, DRM_ERR_OK);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest__CreateMediaKeySessionAbNormal1, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    killclearplay(mediaKeySystem, mediaKeySession);
    killclearplay2(mediaKeySystem, mediaKeySession);
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    DRM_CertificateStatus certStatus = CERT_STATUS_INVALID;
    errNo = OH_MediaKeySystem_GetCertificateStatus(mediaKeySystem, &certStatus);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_EQ(errNo, DRM_ERR_OK);
    sleep(5);
}

static void killDrm_Service1(MediaKeySystem *mediaKeySystem, MediaKeySession *mediaKeySession)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    std::vector<uint8_t> licenseIdVec = {0x01};
    std::vector<uint8_t> ReleaseRequest = {0x01};
    IMediaKeySessionService::ContentProtectionLevel securityLevel;
    MediaKeySessionObject *sessionObject = reinterpret_cast<MediaKeySessionObject *>(mediaKeySession);
    system("killall -9 drm_service");
    uint32_t result = sessionObject->sessionImpl_->GenerateOfflineReleaseRequest(licenseIdVec, ReleaseRequest);
    EXPECT_NE(result, DRM_ERR_OK);

    result = sessionObject->sessionImpl_->ProcessOfflineReleaseResponse(licenseIdVec, ReleaseRequest);
    EXPECT_NE(result, DRM_ERR_OK);

    result = sessionObject->sessionImpl_->GetContentProtectionLevel(&securityLevel);
    EXPECT_NE(result, DRM_ERR_OK);

    std::map<std::string, std::string> licenseStatus2;
    result = sessionObject->sessionImpl_->CheckMediaKeyStatus(licenseStatus2);
    EXPECT_NE(result, DRM_ERR_OK);

    result = sessionObject->sessionImpl_->RestoreOfflineMediaKeys(licenseIdVec);
    EXPECT_NE(result, DRM_ERR_OK);

    result = sessionObject->sessionImpl_->ClearMediaKeys();
    EXPECT_NE(result, DRM_ERR_OK);

    std::string mimeType;
    bool status;
    result = sessionObject->sessionImpl_->RequireSecureDecoderModule(mimeType, &status);
    EXPECT_NE(result, DRM_ERR_OK);

    errNo = OH_MediaKeySystem_SetMediaKeySystemCallback(mediaKeySystem, &TestSystemEventCallBack);
    EXPECT_EQ(errNo, DRM_ERR_OK);

    MediaKeySession_Callback sessionCallback = { &TestSessoinEventCallBack, &TestSessoinKeyChangeCallBack };
    errNo = OH_MediaKeySession_SetMediaKeySessionCallback(mediaKeySession, &sessionCallback);
    EXPECT_EQ(errNo, DRM_ERR_OK);
}

static void killDrm_Service2(MediaKeySystem *mediaKeySystem, MediaKeySession *mediaKeySession)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    bool supported = OH_MediaKeySystem_IsSupported("com.clearplay.drm");
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported2("com.clearplay.drm", "video/mp4");
    EXPECT_EQ(supported, false);
    supported = OH_MediaKeySystem_IsSupported3("com.clearplay.drm", "video/mp4", CONTENT_PROTECTION_LEVEL_HW_CRYPTO);
    EXPECT_EQ(supported, false);
    errNo = OH_MediaKeySystem_SetConfigurationString(mediaKeySystem, "testConfigurationString", "gezhegezhegezhe");
    EXPECT_NE(errNo, DRM_ERR_OK);
    char *value = nullptr;
    int32_t valueLen = 0;
    errNo = OH_MediaKeySystem_GetConfigurationString(mediaKeySystem, "testConfigurationString", &value, &valueLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    unsigned char testArray[5] = {1, 2, 3, 4, 5};
    DRM_Uint8Buffer testArrayData;
    testArrayData.buffer = testArray;
    testArrayData.bufferLen = sizeof(testArray);
    errNo = OH_MediaKeySystem_SetConfigurationByteArray(mediaKeySystem, "testConfigurationByteArray", &testArrayData);
    EXPECT_NE(errNo, DRM_ERR_OK);
    unsigned char *configByteArray = nullptr;
    int32_t byteArrayLen = 0;
    errNo = OH_MediaKeySystem_GetConfigurationByteArray(mediaKeySystem, "testConfigurationByteArray", &configByteArray,
        &byteArrayLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    DRM_Statistics *statistics;
    errNo = OH_MediaKeySystem_GetStatistics(mediaKeySystem, &statistics);
    EXPECT_NE(errNo, DRM_ERR_OK);
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_UNKNOWN;
    errNo = OH_MediaKeySystem_GetMaxContentProtectionLevel(mediaKeySystem, &contentProtectionLevel);
    EXPECT_NE(errNo, DRM_ERR_OK);
}
HWTEST_F(DrmFrameworkUnitTest, Drm_unittest__CreateMediaKeySessionAbNormal, TestSize.Level0)
{
    Drm_ErrCode errNo = DRM_ERR_UNKNOWN;
    MediaKeySystem *mediaKeySystem = nullptr;
    MediaKeySession *mediaKeySession = nullptr;
    DRM_ContentProtectionLevel contentProtectionLevel = CONTENT_PROTECTION_LEVEL_SW_CRYPTO;
    errNo = OH_MediaKeySystem_Create("com.clearplay.drm", &mediaKeySystem);
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
    killDrm_Service1(mediaKeySystem, mediaKeySession);
    killDrm_Service2(mediaKeySystem, mediaKeySession);
    errNo =
        OH_MediaKeySystem_GenerateKeySystemRequest(mediaKeySystem, &request, &requestLen, &defaultUrl, &defaultUrlLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_ProcessKeySystemResponse(mediaKeySystem, KeySystemResponse, KeySystemResponseLen);
    EXPECT_NE(errNo, DRM_ERR_OK);
    DRM_CertificateStatus certStatus = CERT_STATUS_INVALID;
    errNo = OH_MediaKeySystem_GetCertificateStatus(mediaKeySystem, &certStatus);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_CreateMediaKeySession(mediaKeySystem, &contentProtectionLevel, &mediaKeySession);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySession_Destroy(mediaKeySession);
    EXPECT_NE(errNo, DRM_ERR_OK);
    errNo = OH_MediaKeySystem_Destroy(mediaKeySystem);
    EXPECT_NE(errNo, DRM_ERR_OK);
    sleep(5);
}
} // DrmStandard
} // OHOS
