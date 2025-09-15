/*
 * Copyright (C) 2025 Huawei Device Co., Ltd.
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

#include "drm_helper.h"
#include "drm_log.h"
#include "parameters.h"
#include "iservice_registry.h"
#include "os_account_manager.h"
#include "uri.h"

namespace OHOS {
namespace DrmStandard {
const int32_t MEDIA_KEY_SYSTEM_SERVICE_ID = 3012;
const std::string SECURE_TYPE = "secure";
const std::string KEY_WORD = "KEYWORD";
const std::string SETTING_USER_SECURE_URI_PROXY =
    "datashare:///com.ohos.settingsdata/entry/settingsdata/USER_SETTINGSDATA_SECURE_";
const std::string SETTINGS_DATA_EXT_URI = "datashare:///com.ohos.settingsdata.DataAbility";
const std::string SETTING_USER_SECURE_URI_PROXY_SUFFIX = "?Proxy=true&key=";

std::string DrmHelper::GetDeviceType()
{
    std::string deviceType = OHOS::system::GetParameter("const.product.devicetype", "");
    if (deviceType.empty()) {
        deviceType = OHOS::system::GetParameter("const.build.characteristics", "");
        DRM_WARNING_LOG("DrmHelper GetParameter characteristics is:%{public}s", deviceType.c_str());
    }
    return deviceType;
}

std::string DrmHelper::GetSettingDataValue(const std::string &tableType, const std::string &key)
{
    std::string value;
    int32_t currentuserId = DrmHelper::GetCurrentUserId();
    if (currentuserId < 0) {
        DRM_ERR_LOG("DrmHelper currentuserId is invalid");
        return "";
    }
    auto dataShareHelper = DrmHelper::CreateDataShareHelperProxy(currentuserId, tableType);
    if (dataShareHelper == nullptr) {
        DRM_ERR_LOG("DrmHelper dataShareHelper return nullptr");
        return "";
    }

    std::string SettingSystemUri = SETTING_USER_SECURE_URI_PROXY +
        std::to_string(currentuserId) + SETTING_USER_SECURE_URI_PROXY_SUFFIX + key;
    OHOS::Uri uri(SettingSystemUri);

    DataShare::DataSharePredicates predicates;
    std::vector<std::string> columns;
    predicates.EqualTo(KEY_WORD, key);
    auto result = dataShareHelper->Query(uri, predicates, columns);
    if (result == nullptr) {
        DRM_WARNING_LOG("DrmHelper query error, result is null");
        dataShareHelper->Release();
        return "";
    }

    if (result->GoToFirstRow() != DataShare::E_OK) {
        DRM_WARNING_LOG("DrmHelper query error, go to first row error");
        result->Close();
        dataShareHelper->Release();
        return "";
    }

    int columnIndex{0};
    result->GetColumnIndex("VALUE", columnIndex);
    result->GetString(columnIndex, value);
    result->Close();
    dataShareHelper->Release();
    DRM_INFO_LOG("DrmHelper query success, value: %{public}s", value.c_str());
    return value;
}

std::shared_ptr<DataShare::DataShareHelper> DrmHelper::CreateDataShareHelperProxy(int32_t userId, std::string tableType)
{
    auto saManager = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (saManager == nullptr) {
        DRM_ERR_LOG("DrmHelper saManager return nullptr");
        return nullptr;
    }
    auto remoteObj = saManager->GetSystemAbility(MEDIA_KEY_SYSTEM_SERVICE_ID);
    if (remoteObj == nullptr) {
        DRM_ERR_LOG("DrmHelper saManager->GetSystemAbility return nullptr");
        return nullptr;
    }

    std::shared_ptr<DataShare::DataShareHelper> helper = nullptr;
    std::string SettingSystemUri = "";

    if (userId > 0 && tableType == SECURE_TYPE) {
        SettingSystemUri = SETTING_USER_SECURE_URI_PROXY + std::to_string(userId) + "?Proxy=true";
        helper = DataShare::DataShareHelper::Creator(remoteObj, SettingSystemUri, SETTINGS_DATA_EXT_URI);
    }

    if (helper == nullptr) {
        DRM_WARNING_LOG("DrmHelper helper is nullptr, uri=%{public}s", SettingSystemUri.c_str());
        return nullptr;
    }
    return helper;
}

int32_t DrmHelper::GetCurrentUserId()
{
    std::vector<int32_t> ids;
    int32_t currentuserId = -1;
    ErrCode result;
    int32_t retry = 5;
    while (retry--) {
        result = AccountSA::OsAccountManager::QueryActiveOsAccountIds(ids);
        if (result == ERR_OK && !ids.empty()) {
            currentuserId = ids[0];
            DRM_DEBUG_LOG("DrmHelper current userId is :%{public}d", currentuserId);
            break;
        }
        sleep(1);
    }
    if (result != ERR_OK || ids.empty()) {
        DRM_WARNING_LOG("DrmHelper current userId is empty");
    }
    return currentuserId;
}
} // namespace DrmStandard
} // namespace OHOS