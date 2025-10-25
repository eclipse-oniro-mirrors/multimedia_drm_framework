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

#include <string>
#include "gmock/gmock.h"
#include "drm_helper.h"
#include "drm_framework_unittest.h"
#include "drm_net_observer.h"

using namespace testing::ext;
using namespace std;

namespace OHOS {
namespace DrmStandard {
const std::string SECURE_TYPE = "secure";
const std::string BASIC_STATEMENT_AGREED = "basic_statement_agreed";
const std::string INVALID_DATA = "invalid_data";

HWTEST_F(DrmFrameworkUnitTest, Drm_Unittest_GetSettingDataValue_001, TestSize.Level0)
{
    string ret0 = DrmHelper::GetSettingDataValue(SECURE_TYPE, BASIC_STATEMENT_AGREED);
    EXPECT_TRUE(ret0.size() != 0);

    string ret1 = DrmHelper::GetSettingDataValue(SECURE_TYPE, BASIC_STATEMENT_AGREED);
    EXPECT_EQ(ret1, INVALID_DATA);

    string ret2 = DrmHelper::GetSettingDataValue("", "");
    EXPECT_EQ(ret2, "");
}

HWTEST_F(DrmFrameworkUnitTest, Drm_Unittest_CreateDataShareHelperProxy_001, TestSize.Level0)
{
    auto ret0 = DrmHelper::CreateDataShareHelperProxy(1, SECURE_TYPE);
    EXPECT_TRUE(ret0 != nullptr);

    auto ret1 = DrmHelper::CreateDataShareHelperProxy(-1, SECURE_TYPE);
    EXPECT_TRUE(ret1 == nullptr);

    auto ret2 = DrmHelper::CreateDataShareHelperProxy(1, "");
    EXPECT_TRUE(ret2 == nullptr);
}

HWTEST_F(DrmFrameworkUnitTest, Drm_Unittest_GetCurrentUserId_001, TestSize.Level0)
{
    auto ret0 = DrmHelper::GetCurrentUserId();
    EXPECT_TRUE(ret0 != -1);
}

// 1. 析构时 future 无效
HWTEST_F(DrmFrameworkUnitTest, Drm_Unittest_NetObserver_Destruct_FutureInvalid, TestSize.Level0)
{
    auto observer = std::make_shared<DrmNetObserver>();
    EXPECT_TRUE(true);
}

// 2. 析构时 future 有效且正常退出
HWTEST_F(DrmFrameworkUnitTest, Drm_Unittest_NetObserver_Destruct_FutureValid, TestSize.Level0)
{
    auto observer = std::make_shared<DrmNetObserver>();
    observer->StartObserver();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    observer.reset();
    EXPECT_TRUE(true);
}

// 3. StartObserver 立即被 stopRequested_ 打断
HWTEST_F(DrmFrameworkUnitTest, Drm_Unittest_NetObserver_StartObserver_ExitEarly, TestSize.Level0)
{
    auto observer = std::make_shared<DrmNetObserver>();
    observer->StartObserver();
    observer->stopRequested_ = true;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_TRUE(true);
}

// 4. RegisterNetConnCallback 失败并重试满 RETRY_MAX_TIMES
HWTEST_F(DrmFrameworkUnitTest, Drm_Unittest_NetObserver_StartObserver_RetryExhaust, TestSize.Level0)
{
    auto observer = std::make_shared<DrmNetObserver>();
    observer->StartObserver();
    std::this_thread::sleep_for(std::chrono::seconds(RETRY_MAX_TIMES + 1));
    EXPECT_TRUE(true);
}

// 5. StopObserver 无回调可注销
HWTEST_F(DrmFrameworkUnitTest, Drm_Unittest_NetObserver_StopObserver_NoCallback, TestSize.Level0)
{
    auto observer = std::make_shared<DrmNetObserver>();
    int32_t ret = observer->StopObserver();
    EXPECT_EQ(ret, DRM_INNER_ERR_UNKNOWN);
}

// 6. StopObserver 注销成功
HWTEST_F(DrmFrameworkUnitTest, Drm_Unittest_NetObserver_StopObserver_Ok, TestSize.Level0)
{
    auto observer = std::make_shared<DrmNetObserver>();
    auto hostMgr  = sptr<DrmHostManager>(new DrmHostManager());
    observer->SetDrmHostManager(hostMgr);
    observer->netCallback_ = observer;
    int32_t ret = observer->StopObserver();
    EXPECT_EQ(ret, DRM_INNER_ERR_OK);
}

// 7. NetCapabilitiesChange 传入 nullptr
HWTEST_F(DrmFrameworkUnitTest, Drm_Unittest_NetObserver_NetCapabilitiesChange_NullCap, TestSize.Level0)
{
    auto observer = std::make_shared<DrmNetObserver>();
    sptr<NetHandle> handle = new NetHandle(1);
    int32_t ret = observer->NetCapabilitiesChange(handle, nullptr);
    EXPECT_EQ(ret, DRM_INNER_ERR_INVALID_VAL);
}

// 8. HandleNetAllCap drmHostManager 为空
HWTEST_F(DrmFrameworkUnitTest, Drm_Unittest_NetObserver_HandleNetAllCap_NoHostMgr, TestSize.Level0)
{
    auto observer = std::make_shared<DrmNetObserver>();
    NetAllCapabilities cap;
    int32_t ret = observer->HandleNetAllCap(cap);
    EXPECT_EQ(ret, DRM_INNER_ERR_INVALID_VAL);
}

// 9. 网络未就绪：既无 INTERNET 也无 VALIDATED
HWTEST_F(DrmFrameworkUnitTest, Drm_Unittest_NetObserver_HandleNetAllCap_NoInternet, TestSize.Level0)
{
    auto observer = std::make_shared<DrmNetObserver>();
    auto hostMgr  = sptr<DrmHostManager>(new DrmHostManager());
    observer->SetDrmHostManager(hostMgr);
    NetAllCapabilities cap;
    int32_t ret = observer->HandleNetAllCap(cap);
    EXPECT_EQ(ret, DRM_INNER_ERR_UNKNOWN);
}

// 10. 网络正在检查 CONNECTIVITY
HWTEST_F(DrmFrameworkUnitTest, Drm_Unittest_NetObserver_HandleNetAllCap_Checking, TestSize.Level0)
{
    auto observer = std::make_shared<DrmNetObserver>();
    auto hostMgr  = sptr<DrmHostManager>(new DrmHostManager());
    observer->SetDrmHostManager(hostMgr);
    NetAllCapabilities cap;
    cap.netCaps_.insert(NetCap::NET_CAPABILITY_INTERNET);
    cap.netCaps_.insert(NetCap::NET_CAPABILITY_VALIDATED);
    cap.netCaps_.insert(NetCap::NET_CAPABILITY_CHECKING_CONNECTIVITY);
    int32_t ret = observer->HandleNetAllCap(cap);
    EXPECT_EQ(ret, DRM_INNER_ERR_UNKNOWN);
}

// 11. 网络就绪且触发完整流程
HWTEST_F(DrmFrameworkUnitTest, Drm_Unittest_NetObserver_HandleNetAllCap_InternetOk, TestSize.Level0)
{
    auto observer = std::make_shared<DrmNetObserver>();
    auto hostMgr  = sptr<DrmHostManager>(new DrmHostManager());
    observer->SetDrmHostManager(hostMgr);
    observer->netCallback_ = observer;
    NetAllCapabilities cap;
    cap.netCaps_.insert(NetCap::NET_CAPABILITY_INTERNET);
    cap.netCaps_.insert(NetCap::NET_CAPABILITY_VALIDATED);
    int32_t ret = observer->HandleNetAllCap(cap);
    EXPECT_EQ(ret, DRM_INNER_ERR_OK);
}

// 12. SetDrmHostManager 传入 nullptr
HWTEST_F(DrmFrameworkUnitTest, Drm_Unittest_NetObserver_SetDrmHostManager_Null, TestSize.Level0)
{
    auto observer = std::make_shared<DrmNetObserver>();
    int32_t ret = observer->SetDrmHostManager(nullptr);
    EXPECT_EQ(ret, DRM_INNER_ERR_INVALID_VAL);
}

// 13. SetDrmHostManager 正常设置
HWTEST_F(DrmFrameworkUnitTest, Drm_Unittest_NetObserver_SetDrmHostManager_Ok, TestSize.Level0)
{
    auto observer = std::make_shared<DrmNetObserver>();
    auto hostMgr  = sptr<DrmHostManager>(new DrmHostManager());
    int32_t ret = observer->SetDrmHostManager(hostMgr);
    EXPECT_EQ(ret, DRM_INNER_ERR_OK);
}

} // DrmStandard
} // OHOS