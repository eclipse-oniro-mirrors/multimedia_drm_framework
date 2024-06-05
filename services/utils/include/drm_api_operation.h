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

#ifndef DRM_API_OPERATION_H
#define DRM_API_OPERATION_H
#include <string>

namespace OHOS {
namespace DrmStandard {
    #define DRM_API_OPERATION_CONFIG_PATH "/etc/drm/drm_api_operation.cfg"
    struct ApiReportConfig {
        std::string config_name;
        std::string config_appId;
        std::string config_routeInfo;
        int config_timeout;
        int config_row;
    };

    struct ApiEventConfig {
        std::string domain;
        std::string name;
        bool isRealTime;
    };

    std::string trim(const std::string& str);
    std::pair<std::string, std::string> parseKeyValue(const std::string& line);
    void parseReportConfig(std::ifstream& file, ApiReportConfig& reportConfig);
    void parseEvent(std::ifstream& file, ApiEventConfig& eventConfig);
    void parseEventConfig(std::ifstream& file, ApiEventConfig& event);
    void parseApiOperationManagement(std::ifstream& file, ApiReportConfig& reportConfig, ApiEventConfig& event);
    int32_t GetConfigurationParame(const std::string &configFile, ApiReportConfig &reportConfig, ApiEventConfig &event);
    int64_t AddProcessor();
    void WriteEndEvent(const int result, const int errCode, std::string apiName);
}  // namespace DrmStandard
}  // namespace OHOS
#endif  // DRM_API_OPERATION_H