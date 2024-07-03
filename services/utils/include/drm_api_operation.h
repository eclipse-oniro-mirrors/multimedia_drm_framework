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
    int config_timeout = 0;
    int config_row = 0;
};

struct ApiEvent {
    std::string domain;
    std::string name;
    bool isRealTime = false;
};

struct ApiEventConfig {
    ApiEvent event1;
    ApiEvent event2;
    ApiEvent event3;
};

class ConfigParser {
public:
    static int64_t AddProcessor();
    static void WriteEndEvent(const int result, const int errCode, std::string apiName);
    ConfigParser(const ConfigParser&) = delete;
    ConfigParser& operator=(const ConfigParser&) = delete;

private:
    ConfigParser() {}
    ~ConfigParser() {}
    static bool LoadConfigurationFile(const std::string &configFile);
    static void GetConfigurationParams(ApiReportConfig &reportConfig, ApiEventConfig &eventConfig);
    static std::string Trim(const std::string &str);
    static std::pair<std::string, std::string> ParseKeyValue(const std::string &line);
    static bool TryParseInt(const std::string& str, int& out);
    static void ParseReportConfig(std::istringstream &stream, ApiReportConfig &reportConfig);
    static void ParseEvent(std::istringstream &stream, ApiEvent &event);
    static void ParseEventConfig(std::istringstream &stream, ApiEventConfig &eventConfig);
    static void ParseApiOperationManagement(std::istringstream &stream, ApiReportConfig &reportConfig,
        ApiEventConfig &eventConfig);
    static std::string g_fileContent;
    static std::mutex g_apiOperationMutex;
    static int64_t g_processorId;
};
}  // namespace DrmStandard
}  // namespace OHOS
#endif  // DRM_API_OPERATION_H