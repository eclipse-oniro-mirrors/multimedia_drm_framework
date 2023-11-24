/*
 * Copyright (c) 2023  Huawei Device Co., Ltd.
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

#ifndef OHOS_DRM_DRM_HOST_MANAGER_H
#define OHOS_DRM_DRM_HOST_MANAGER_H

#include <refbase.h>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "i_mediakeysystem_service.h"
#include "iservstat_listener_hdi.h"
#include "media_key_session_proxy.h"
#include "media_decrypt_module_proxy.h"
#include "media_key_system_factory_proxy.h"
#include "media_key_system_proxy.h"

namespace OHOS {
namespace DrmStandard {
using namespace OHOS::HDI::Drm::V1_0;
using namespace OHOS::HDI;

class DrmHostManager : public virtual RefBase, public HDI::ServiceManager::V1_0::ServStatListenerStub {
public:
    class StatusCallback {
    public:
        virtual ~StatusCallback() = default;
    };
    class DrmHostDeathRecipient : public IRemoteObject::DeathRecipient {
    public:
        explicit DrmHostDeathRecipient() {};
        virtual ~DrmHostDeathRecipient() = default;
        void OnRemoteDied(const wptr<IRemoteObject> &remote) override;
    };

    explicit DrmHostManager(StatusCallback *statusCallback);
    ~DrmHostManager() override;

    int32_t Init(void);
    void DeInit(void);
    void OnReceive(const HDI::ServiceManager::V1_0::ServiceStatus &status) override;
    int32_t IsMediaKeySystemSupported(std::string &uuid, bool *isSurpported);
    int32_t IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType, bool *isSurpported);
    int32_t IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType, int32_t securityLevel,
        bool *isSurpported);
    int32_t CreateMediaKeySystem(std::string &uuid, sptr<IMediaKeySystem> &hdiMediaKeySystem);

private:
    int32_t GetSevices(std::string &uuid, bool *isSurpported);
    int32_t OEMCertifaicateServiceManage();
    std::mutex mutex_;
    StatusCallback *statusCallback_;
    std::string service_name_ = "drm_interface_service";
    sptr<IMediaKeySystemFactory> drmHostServieProxy_;
    sptr<IMediaKeySystem> hdiMediaKeySystem;
};
} // DrmStandard
} // OHOS


#endif // OHOS_DRM_DRM_HOST_MANAGER_H