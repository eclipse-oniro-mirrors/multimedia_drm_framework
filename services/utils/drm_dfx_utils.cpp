/*
* Copyright (c) 2024 Huawei Device Co., Ltd.
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

#include <cstdint>
#include <iomanip>
#include <sstream>
#include "drm_dfx_utils.h"
#include "drm_log.h"
#include "iservice_registry.h"
#include "bundle_mgr_interface.h"
#include "bundle_mgr_proxy.h"
#include "system_ability_definition.h"

namespace OHOS {
namespace DrmStandard {
std::string __attribute__((visibility("default"))) GetClientBundleName(int32_t uid)
{
    std::string bundleName = "";
    auto samgr = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (samgr == nullptr) {
        DRM_ERR_LOG("Get ability manager failed");
        return bundleName;
    }

    sptr<IRemoteObject> object = samgr->GetSystemAbility(BUNDLE_MGR_SERVICE_SYS_ABILITY_ID);
    if (object == nullptr) {
        DRM_ERR_LOG("object is NULL.");
        return bundleName;
    }

    sptr<OHOS::AppExecFwk::IBundleMgr> bms = iface_cast<OHOS::AppExecFwk::IBundleMgr>(object);
    if (bms == nullptr) {
        DRM_ERR_LOG("bundle manager service is NULL.");
        return bundleName;
    }

    auto result = bms->GetNameForUid(uid, bundleName);
    if (result != ERR_OK) {
        DRM_ERR_LOG("GetBundleNameForUid fail");
        return "";
    }

    DRM_INFO_LOG("BundleName:%{public}s", bundleName.c_str());

    return bundleName;
}

uint8_t __attribute__((visibility("default"))) GetClientBundleInfoTargetVersion(const std::string &bundleName,
    int32_t uid)
{
    if (bundleName.empty()) {
        DRM_ERR_LOG("bundleName is empty.");
        return 0;
    }
    auto samgr = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (samgr == nullptr) {
        DRM_ERR_LOG("Get ability manager failed");
        return 0;
    }

    sptr<IRemoteObject> object = samgr->GetSystemAbility(BUNDLE_MGR_SERVICE_SYS_ABILITY_ID);
    if (object == nullptr) {
        DRM_ERR_LOG("object is NULL.");
        return 0;
    }

    sptr<OHOS::AppExecFwk::IBundleMgr> bms = iface_cast<OHOS::AppExecFwk::IBundleMgr>(object);
    if (bms == nullptr) {
        DRM_ERR_LOG("bundle manager service is NULL.");
        return 0;
    }

    auto flags = (
        static_cast<int32_t>(AppExecFwk::GetBundleInfoFlag::GET_BUNDLE_INFO_WITH_ABILITY) |
        static_cast<int32_t>(AppExecFwk::GetBundleInfoFlag::GET_BUNDLE_INFO_WITH_APPLICATION) |
        static_cast<int32_t>(AppExecFwk::GetBundleInfoFlag::GET_BUNDLE_INFO_WITH_HAP_MODULE));

    AppExecFwk::BundleInfo bundleInfo;
    auto result = bms->GetBundleInfoV9(bundleName, flags, bundleInfo, uid / AppExecFwk::Constants::BASE_USER_RANGE);
    if (result != ERR_OK) {
        DRM_ERR_LOG("GetBundleInfoV9 err:%{public}d, bundleName: %{public}s", result, bundleName.c_str());
        return 0;
    }
    uint8_t targetVersion = static_cast<uint8_t>(bundleInfo.targetVersion % 100); // % 100 to get the real version

    DRM_INFO_LOG("targetVersion: %{public}u", targetVersion);

    return targetVersion;
}

std::string __attribute__((visibility("default"))) CastToHexString(std::vector<uint8_t> binaryData)
{
    std::string hexString;
    for (uint8_t binary : binaryData) {
        std::stringstream stream;
        stream << std::hex << std::setw(minimumDigit) << std::setfill('0') << static_cast<int>(binary);
        hexString += stream.str();
    }
    return hexString;
}

uint32_t __attribute__((visibility("default"))) CalculateTimeDiff(std::chrono::system_clock::time_point timeBefore,
    std::chrono::system_clock::time_point timeAfter)
{
    auto duration = timeAfter - timeBefore;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    return static_cast<uint32_t>(milliseconds.count());
}
}  // namespace DrmStandard
}  // namespace OHOS