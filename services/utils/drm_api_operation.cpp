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

#include <fstream>
#include <sys/file.h>
#include <fcntl.h>
#include <unistd.h>
#include "drm_api_operation.h"
#include "app_event.h"
#include "app_event_processor_mgr.h"
#include "drm_dfx_utils.h"
#include "drm_log.h"

namespace OHOS {
namespace DrmStandard {
static int64_t processorId = -1;
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

std::pair<std::string, std::string> parseKeyValue(const std::string& line) {
    size_t colonPos = line.find(':');
    if (colonPos != std::string::npos) {
        std::string key = trim(line.substr(0, colonPos));
        std::string value = trim(line.substr(colonPos + 1));
        if (!value.empty() && value.front() == '"' && value.back() == '"') {
            value = value.substr(1, value.size() - 2);
        }
        return std::make_pair(key, value);
    }
    return std::make_pair("", "");
}

void parseReportConfig(std::ifstream& file, ApiReportConfig& reportConfig) {
    std::string line;
    while (getline(file, line)) {
        line = trim(line);
        if (line == "},") {
            break;
        }
        std::pair<std::string, std::string> keyValue = parseKeyValue(line);
        std::string key = keyValue.first;
        std::string value = keyValue.second;

        if (!key.empty() && !value.empty()) {
            if (key == "\"config_name\"") {
                reportConfig.config_name = value;
            } else if (key == "\"config_appId\"") {
                reportConfig.config_appId = value;
            } else if (key == "\"config_routeInfo\"") {
                reportConfig.config_routeInfo = value;
            } else if (key == "\"config_TriggerCond.timeout\"") {
                value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
                reportConfig.config_timeout = std::stoi(value);
            } else if (key == "\"config_TriggerCond.row\"") {
                value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
                reportConfig.config_row = std::stoi(value);
            }
        }
    }
}

void parseEvent(std::ifstream& file, ApiEventConfig& eventConfig) {
    std::string line;
    while (getline(file, line)) {
        line = trim(line);
        if (line == "},") {
            break;
        }
        std::pair<std::string, std::string> keyValue = parseKeyValue(line);
        std::string key = keyValue.first;
        std::string value = keyValue.second;

        if (!key.empty() && !value.empty()) {
            if (key == "\"domain\"") {
                eventConfig.domain = value;
            } else if (key == "\"name\"") {
                eventConfig.name = value;
            } else if (key == "\"isRealTime\"") {
                eventConfig.isRealTime = (value == "true");
            }
        }
    }
}

void parseEventConfig(std::ifstream& file, ApiEventConfig& event) {
    std::string line;
    while (getline(file, line)) {
        line = trim(line);
        if (line == "\"event\": {") {
            parseEvent(file, event);
        }
    }
}

void parseApiOperationManagement(std::ifstream& file, ApiReportConfig& reportConfig, ApiEventConfig& event) {
    std::string line;
    bool inApiOperationManagement = false;
    while (getline(file, line)) {
        line = trim(line);
        if (line == "\"api_operation_management\": {") {
            inApiOperationManagement = true;
        } else if (inApiOperationManagement) {
            if (line == "}") {
                break;
            } else if (line == "\"report_config\": {") {
                parseReportConfig(file, reportConfig);
            } else if (line == "\"event_config\": {") {
                parseEventConfig(file, event);
            }
        }
    }
}

int32_t GetConfigurationParame(const std::string &configFile, ApiReportConfig &reportConfig, ApiEventConfig &event) {
    int fd = open(configFile.c_str(), O_RDONLY);
    if (fd == -1) {
        perror("Unable to open file");
        return 1;
    }

    std::ifstream file(configFile);
    if (!file.is_open()) {
        perror("Unable to open file");
        close(fd);
        return 1;
    }

    parseApiOperationManagement(file, reportConfig, event);

    file.close();
    close(fd);
    return 0;
}

int64_t AddProcessor() {
    ApiReportConfig reportConfig;
    ApiEventConfig apiEvent;
    GetConfigurationParame(DRM_API_OPERATION_CONFIG_PATH, reportConfig, apiEvent);
    HiviewDFX::HiAppEvent::ReportConfig config;
    config.name = reportConfig.config_name;
    config.appId = reportConfig.config_appId;
    config.routeInfo = reportConfig.config_routeInfo;
    config.triggerCond.timeout = reportConfig.config_timeout;
    config.triggerCond.row = reportConfig.config_row;
    config.eventConfigs.clear();
    HiviewDFX::HiAppEvent::EventConfig event;
    event.domain = apiEvent.domain;
    event.name = apiEvent.name;
    event.isRealTime = apiEvent.isRealTime;
    config.eventConfigs.push_back(event);
    if (processorId != -1) {
        return processorId;
    }
    processorId = HiviewDFX::HiAppEvent::AppEventProcessorMgr::AddProcessor(config);
    return processorId;
}

void WriteEndEvent(const int result, const int errCode, std::string apiName) {
    AddProcessor();
    std::string transId = std::string("traceId_") + std::to_string(std::rand());
    int64_t beginTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now())
        .time_since_epoch().count();
    HiviewDFX::HiAppEvent::Event event("api_diagnostic", "api_exec_end", OHOS::HiviewDFX::HiAppEvent::BEHAVIOR);
    event.AddParam("trans_id", transId);
    event.AddParam("api_name", apiName);
    event.AddParam("sdk_name", std::string("DrmKit"));
    event.AddParam("begin_time", beginTime);
    event.AddParam("end_time", time(NULL));
    event.AddParam("result", result);
    event.AddParam("error_code", errCode);
    HiviewDFX::HiAppEvent::Write(event);
}
}  // namespace DrmStandard
}  // namespace OHOS