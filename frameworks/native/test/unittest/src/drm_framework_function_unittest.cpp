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
} // DrmStandard
} // OHOS