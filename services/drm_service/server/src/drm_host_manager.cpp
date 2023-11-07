/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#include "drm_host_manager.h"
#include "hdf_device_class.h"
#include "iremote_broker.h"
#include "iservmgr_hdi.h"
#include "drm_log.h"
#include "drm_error_code.h"
#include "drm_napi_utils.h"
#include "servmgr_hdi.h"

namespace OHOS {
namespace DrmStandard {
using OHOS::HDI::ServiceManager::V1_0::IServiceManager;
void DrmHostManager::DrmHostDeathRecipient::OnRemoteDied(const wptr<IRemoteObject> &remote)
{
    DRM_DEBUG_LOG("Remote died, do clean works.");
}

DrmHostManager::DrmHostManager(StatusCallback* statusCallback)
{
}

DrmHostManager::~DrmHostManager() {
}

int32_t DrmHostManager::Init(void)
{
    DRM_INFO_LOG("DrmHostManager::Init enter.");
    DRM_INFO_LOG("DrmHostManager::Init exit.");
    return DRM_OK;
}

void DrmHostManager::DeInit(void)
{
    DRM_ERR_LOG("DrmHostManager::DeInit");
}

void DrmHostManager::OnReceive(const HDI::ServiceManager::V1_0::ServiceStatus& status)
{
}

int32_t DrmHostManager::GetSevices(std::string &uuid, bool *isSurpported)
{
    DRM_INFO_LOG("DrmHostManager::GetSevices enter, uuid:%{public}s.", uuid.c_str());
    DRM_INFO_LOG("DrmHostManager::GetSevices exit, uuid:%{public}s.", uuid.c_str());
    return DRM_OK;
}

int32_t DrmHostManager::IsMediaKeySystemSupported(std::string &uuid, bool *isSurpported)
{
    DRM_INFO_LOG("DrmHostManager::IsMediaKeySystemSupported one parameters enter, uuid:%{public}s.", uuid.c_str());
    DRM_INFO_LOG("DrmHostManager::IsMediaKeySystemSupported one parameters exit, isSurpported:%{public}d.", *isSurpported);
    return DRM_OK;
}

int32_t DrmHostManager::IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType, bool *isSurpported)
{
    DRM_INFO_LOG("DrmHostManager::IsMediaKeySystemSupported two parameters enter.");
    DRM_INFO_LOG("DrmHostManager::IsMediaKeySystemSupported two parameters exit.");
    return DRM_OK;
}

int32_t DrmHostManager::IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType, int32_t securityLevel, bool *isSurpported)
{
    DRM_INFO_LOG("IsMediaKeySystemSupported three parameters enter");
    DRM_INFO_LOG("IsMediaKeySystemSupported three parameters exit, isSurpported:%{public}d.", *isSurpported);
    return DRM_OK;
}

int32_t DrmHostManager::CreateMediaKeySystem(std::string &uuid)
{
    DRM_INFO_LOG("DrmHostManager::CreateMediaKeySystem enter.");
    DRM_INFO_LOG("DrmHostManager::CreateMediaKeySystem exit.");
    return DRM_OK;
}
} // namespace DrmStandard
} // namespace OHOS