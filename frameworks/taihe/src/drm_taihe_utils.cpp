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

#include "drm_taihe_utils.h"

namespace ANI {
namespace DrmStandard {

string DrmTaiheUtils::ToTaiheString(const std::string &src)
{
    return string(src);
}

void DrmTaiheUtils::ThrowExceptionError(const std::string errMsg)
{
    DRM_ERR_LOG("errMsg: %{public}s", errMsg.c_str());
    taihe::set_error(errMsg);
}

template <typename EnumType>
bool DrmTaiheUtils::GetEnumKeyByValue(int32_t value, typename EnumType::key_t &key)
{
    for (size_t index = 0; index < std::size(EnumType::table); ++index) {
        if (EnumType::table[index] == value) {
            key = static_cast<typename EnumType::key_t>(index);
            return true;
        }
    }
    return false;
}

template
bool DrmTaiheUtils::GetEnumKeyByValue<ContentProtectionLevel>(int32_t value,
    typename ContentProtectionLevel::key_t &key);
template
bool DrmTaiheUtils::GetEnumKeyByValue<CertificateStatus>(int32_t value,
    typename CertificateStatus::key_t &key);
template
bool DrmTaiheUtils::GetEnumKeyByValue<OfflineMediaKeyStatus>(int32_t value,
    typename OfflineMediaKeyStatus::key_t &key);
} // namespace DrmStandard
} // namespace ANI