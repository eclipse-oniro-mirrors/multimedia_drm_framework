/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
#include <string>
#include "cstdio"
#include "cstdlib"
#include "drm_dfx.h"
#include "drm_dfx_utils.h"
#include <securec.h>
#include "native_drm_common.h"
#include "native_drm_err.h"
#include "gmock/gmock.h"
#include "native_drm_base.h"
#include "native_drm_object.h"
#include "native_mediakeysession.h"
#include "native_mediakeysystem.h"
#include "drm_framework_unittest.h"
#include "http.h"

#define DRM_SAMPLE_CHECK_AND_RETURN_RET_LOG(cond, ret, fmt, ...) \
    do {                                                         \
        if (!(cond)) {                                            \
            printf(fmt, ##__VA_ARGS__);                          \
            return ret;                                          \
        }                                                        \
    } while (0)

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
#define LICENSE_URL "http://192.168.50.59:9528/getLicense"
#define PROVISION_URL "http://192.168.50.59:9528/getProvision"


using namespace testing::ext;
using namespace std;


namespace OHOS {
namespace DrmStandard {

HWTEST_F(DrmFrameworkUnitTest, Drm_Unittest_GetMediaKeySystems, TestSize.Level0)
{
    uint32_t count = 10;
    DRM_MediaKeySystemDescription infos[10];
    memset_s(infos, sizeof(infos), 0, sizeof(infos));
    Drm_ErrCode errNo = OH_MediaKeySystem_GetMediaKeySystems(infos, &count);
    EXPECT_EQ(errNo, 0);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_Unittest_GetMediaKeySystemsAbnorm, TestSize.Level0)
{
    uint32_t count = 10;
    Drm_ErrCode errNo = OH_MediaKeySystem_GetMediaKeySystems(nullptr, &count);
    EXPECT_NE(errNo, 0);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_Unittest_GetMediaKeySystemsAbnorm1, TestSize.Level0)
{
    DRM_MediaKeySystemDescription infos[10];
    memset_s(infos, sizeof(infos), 0, sizeof(infos));
    Drm_ErrCode errNo = OH_MediaKeySystem_GetMediaKeySystems(infos, nullptr);
    EXPECT_NE(errNo, 0);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_Unittest_GetMediaKeySystemsAbnorm2, TestSize.Level0)
{
    uint32_t count = 0;
    DRM_MediaKeySystemDescription infos[10];
    memset_s(infos, sizeof(infos), 0, sizeof(infos));
    Drm_ErrCode errNo = OH_MediaKeySystem_GetMediaKeySystems(infos, &count);
    EXPECT_NE(errNo, 0);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_ReportFaultEvent, TestSize.Level0)
{
    uint32_t errorCode = 0;
    std::string errorMesg = "GenerateMediaKeyRequest failed";
    std::string extraMesg = "";
    Drm_ErrCode errNo = (Drm_ErrCode)ReportFaultEvent(errorCode, errorMesg, extraMesg);
    EXPECT_EQ(errNo, 0);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_ReportDecryptionFaultEvent, TestSize.Level0)
{
    uint32_t errorCode = 0;
    std::string errorMesg = "Decryption failed";
    std::string decryptAlgo = "decryptAlgo";
    std::string decryptKeyid = "decryptKeyid";
    std::string decryptIv = "decryptIv";
    Drm_ErrCode errNo = (Drm_ErrCode)ReportDecryptionFaultEvent(errorCode, errorMesg, decryptAlgo, decryptKeyid,
        decryptIv);
    EXPECT_EQ(errNo, 0);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_ReportServiceBehaviorEvent, TestSize.Level0)
{
    std::string serviceName = "DRM_OEM_SERVICE";
    std::string action = "start";
    Drm_ErrCode errNo = (Drm_ErrCode)ReportServiceBehaviorEvent(serviceName, action);
    EXPECT_EQ(errNo, 0);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_ReportLicenseBehaviorEvent, TestSize.Level0)
{
    std::string mediaKeyType = "keytype";
    struct DownLoadInfo downLoadInfo = InitDownLoadInfo(10, "success", 1, "success");
    struct StatisticsInfo statisticsInfo = {
        "123123",
        "clearplay",
        "vendor",
        "clearplay_v1",
        "bundleName",
        0,
    };
    Drm_ErrCode errNo = (Drm_ErrCode)ReportLicenseBehaviorEvent(statisticsInfo, mediaKeyType, downLoadInfo);
    EXPECT_EQ(errNo, 0);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_ReportCertificateBehaviorEvent, TestSize.Level0)
{
    std::string mediaKeyType = "keytype";
    uint32_t callServerTime = 1;
    uint32_t serverCostDuration = 19;
    std::string serverResult = "serverResult";
    struct DownLoadInfo downLoadInfo = InitDownLoadInfo(10, "success", 1, "success");
    struct StatisticsInfo statisticsInfo = {
        "123123",
        "clearplay",
        "vendor",
        "clearplay_v1",
        "bundleName",
        0,
    };
    Drm_ErrCode errNo = (Drm_ErrCode)ReportCertificateBehaviorEvent(statisticsInfo, downLoadInfo, callServerTime,
        serverCostDuration,
        serverResult);
    EXPECT_EQ(errNo, 0);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteServiceEvent, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmServiveInfo info;
    info.module = "drm_core";
    info.currentTime = 1024;
    info.serviceName = "DrmKeyService";
    info.action = "Init";
    info.memoryUsage = 2048;
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteServiceEvent(
        "DRM_SERVICE_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::BEHAVIOR,
        info));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteServiceEventAbNormal_001, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmServiveInfo info;
    info.module = "drm_core";
    info.currentTime = 1024;
    info.serviceName = "DrmKeyService";
    info.action = "Init";
    info.memoryUsage = 2048;
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteServiceEvent(
        "",
        OHOS::HiviewDFX::HiSysEvent::EventType::BEHAVIOR,
        info));
    EXPECT_NE(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteServiceEventAbNormal_002, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmServiveInfo info;
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteServiceEvent(
        "DRM_SERVICE_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::BEHAVIOR,
        info));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteServiceEvent_003, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmServiveInfo info;
    info.module = "drm_core";
    info.currentTime = 1024;
    info.serviceName = "DrmKeyService";
    info.action = "Init";
    info.memoryUsage = 2048;
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteServiceEvent(
        "DRM_SERVICE_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        info));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteLicenseEvent, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmLicenseInfo license;
    license.module = "VideoModule";
    license.currentTime = 1697730000;
    license.appName = "MyApp";
    license.instanceId = "instance_001";
    license.drmName = "ClearPlayDRM";
    license.drmUuid = "com.clearplay.drm";
    license.clientVersion = "v1.2.3";
    license.licenseType = "Trial";
    license.generationDuration = 500;
    license.generationResult = "Success";
    license.processDuration = 300;
    license.processResult = "Processed";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteLicenseEvent(
        "DRM_LICENSE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::BEHAVIOR,
        license));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteLicenseEvent_001, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmLicenseInfo license;
    license.module = "VideoModule";
    license.currentTime = 1697730000;
    license.appName = "MyApp";
    license.instanceId = "instance_001";
    license.drmName = "ClearPlayDRM";
    license.drmUuid = "com.clearplay.drm";
    license.clientVersion = "v1.2.3";
    license.licenseType = "Trial";
    license.generationDuration = 500;
    license.generationResult = "Success";
    license.processDuration = 300;
    license.processResult = "Processed";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteLicenseEvent(
        "",
        OHOS::HiviewDFX::HiSysEvent::EventType::BEHAVIOR,
        license));
    EXPECT_NE(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteLicenseEvent_002, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmLicenseInfo license;
    license.module = "VideoModule";
    license.currentTime = 1697730000;
    license.appName = "MyApp";
    license.instanceId = "instance_001";
    license.drmName = "ClearPlayDRM";
    license.drmUuid = "com.clearplay.drm";
    license.clientVersion = "v1.2.3";
    license.licenseType = "Trial";
    license.generationDuration = 500;
    license.generationResult = "Success";
    license.processDuration = 300;
    license.processResult = "Processed";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteLicenseEvent(
        "DRM_LICENSE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        license));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteLicenseEvent_003, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmLicenseInfo license;
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteLicenseEvent(
        "DRM_LICENSE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        license));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "com.clearplay.drm";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 500;
    cert.generationResult = "Success";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::BEHAVIOR,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_001, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "com.clearplay.drm";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 500;
    cert.generationResult = "Success";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "",
        OHOS::HiviewDFX::HiSysEvent::EventType::BEHAVIOR,
        cert));
    EXPECT_NE(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_002, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "com.clearplay.drm";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 500;
    cert.generationResult = "Success";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_003, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::BEHAVIOR,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_004, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "com.clearplay.drm";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 500;
    cert.generationResult = "Success";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_005, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 0;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "com.clearplay.drm";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 500;
    cert.generationResult = "Success";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_006, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "com.clearplay.drm";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 500;
    cert.generationResult = "Success";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 0;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_007, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "com.clearplay.drm";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 500;
    cert.generationResult = "Success";
    cert.processDuration = 300;
    cert.processResult = "";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_008, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 500;
    cert.generationResult = "Success";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_009, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 500;
    cert.generationResult = "Success";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_010, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "";
    cert.generationDuration = 500;
    cert.generationResult = "Success";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_011, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "Success";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_012, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "Success";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "Servernot";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_013, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "Success";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "error";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_014, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "Success";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "test";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_015, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "Success";
    cert.processDuration = 300;
    cert.processResult = "";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_016, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_017, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_018, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_019, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_020, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_021, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_022, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_023, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_024, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_025, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_026, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "testdemo";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_027, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_028, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_029, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_030, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 123;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_031, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 123456;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_032, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_033, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "";
    cert.instanceId = "";
    cert.drmName = "";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_034, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "";
    cert.drmName = "";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "";
    cert.processDuration = 300;
    cert.processResult = "";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_035, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 0;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_036, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "";
    cert.currentTime = ;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_037, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "0x";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_037, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 0;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 0;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_038, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "";
    cert.instanceId = "";
    cert.drmName = "";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_039, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "";
    cert.currentTime = 0;
    cert.appName = "";
    cert.instanceId = "";
    cert.drmName = "";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_040, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "";
    cert.processDuration = 300;
    cert.processResult = "";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_041, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "suceess";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_042, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_043, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "";
    cert.currentTime = 1697730000;
    cert.appName = "";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_044, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Process";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_045, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "done";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_046, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "processing";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_047, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "PROCESSING";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_048, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "SUCCESS";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_049, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "VideoModule";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "SERVEROK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}

HWTEST_F(DrmFrameworkUnitTest, Drm_unittest_WriteCertificateEvent_050, TestSize.Level0)
{
    DrmEvent *drmevent =new DrmEvent();
    DrmCertificateInfo cert;
    cert.module = "video";
    cert.currentTime = 1697730000;
    cert.appName = "MyApp";
    cert.instanceId = "instance_001";
    cert.drmName = "ClearPlayDRM";
    cert.drmUuid = "";
    cert.clientVersion = "v1.2.3";
    cert.generationDuration = 123456;
    cert.generationResult = "failed";
    cert.processDuration = 300;
    cert.processResult = "Processed";
    cert.callServerTime = 1697730050;
    cert.serverCostDuration = 150;
    cert.serverResult = "ServerOK";
    Drm_ErrCode errNo = static_cast<Drm_ErrCode>(drmevent->WriteCertificateEvent(
        "DRM_CERTIFICATE_DOWNLOAD_INFO",
        OHOS::HiviewDFX::HiSysEvent::EventType::FAULT,
        cert));
    EXPECT_EQ(errNo, 0);
    delete drmevent;
}
} // DrmStandard
} // OHOS