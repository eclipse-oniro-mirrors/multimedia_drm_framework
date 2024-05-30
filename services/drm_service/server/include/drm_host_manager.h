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
#include <shared_mutex>
#include <string>
#include <vector>
#include <queue>
#include <condition_variable>
#include <thread>

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

enum ExtraInfo {
    OEM_CERT_PROVISIONED_DONE = 0,
    MEDIA_KEY_SYSTEM_ERROR,
    GET_OEM_CERTIFIATE_ERROE,
};

struct Message {
    enum Type {
        UnLoadOEMCertifaicateService
    };
    Type type;
    std::string name;
    ExtraInfo extraInfo;
    Message(Type t, std::string pluginName, ExtraInfo info) : type(t), name(pluginName), extraInfo(info) {}
};

#define OEM_CERTIFICATE_PATH "/system/lib64/oem_certificate_service/"
#define PLUGIN_LAZYLOAD_CONFIG_PATH "/etc/drm/drm_plugin_lazyloding.cfg"

typedef void (*MediaKeySystemCallBack)(std::string &, ExtraInfo);
typedef int32_t (*QueryMediaKeySystemNameFuncType)(std::string &);
typedef int32_t (*SetMediaKeySystemFuncType)(sptr<OHOS::HDI::Drm::V1_0::IMediaKeySystem> &);
typedef bool (*IsProvisionRequiredFuncType)();
typedef int32_t (*ThreadExitNotifyFuncType)(MediaKeySystemCallBack);
typedef int32_t (*StartThreadFuncType)();
typedef void (*StopThreadFuncType)();

class DrmHostManager : public virtual RefBase, public HDI::ServiceManager::V1_0::ServStatListenerStub {
public:
    class StatusCallback {
    public:
        virtual ~StatusCallback() = default;
    };
    class DrmHostDeathRecipient : public IRemoteObject::DeathRecipient {
    public:
        explicit DrmHostDeathRecipient();
        virtual ~DrmHostDeathRecipient();
        void OnRemoteDied(const wptr<IRemoteObject> &remote) override;
    };

    explicit DrmHostManager(StatusCallback *statusCallback);
    ~DrmHostManager() override;

    int32_t Init(void);
    void DeInit(void);
    void OnReceive(const HDI::ServiceManager::V1_0::ServiceStatus &status) override;
    int32_t IsMediaKeySystemSupported(std::string &name, bool *isSurpported);
    int32_t IsMediaKeySystemSupported(std::string &name, std::string &mimeType, bool *isSurpported);
    int32_t IsMediaKeySystemSupported(std::string &name, std::string &mimeType, int32_t securityLevel,
        bool *isSurpported);
    int32_t CreateMediaKeySystem(std::string &name, sptr<IMediaKeySystem> &hdiMediaKeySystem);
    int32_t GetMediaKeySystems(std::map<std::string, std::string> &mediaKeySystemDescription);
    int32_t GetMediaKeySystemUuid(std::string &name, std::string &uuid);
    void ReleaseMediaKeySystem(std::string &name);
private:
    static void UnLoadOEMCertifaicateService(std::string &name, ExtraInfo info);
    void StopServiceThread();
    void ProcessMessage();
    void ServiceThreadMain();
    void GetOemLibraryPath(std::vector<std::string> &libsToLoad);
    void OemCertificateManager();
    int32_t GetSevices(std::string &name, bool *isSurpported);
    void ReleaseHandleAndKeySystemMap(void *handle);
    void loadPluginInfo(const std::string& filePath);
    void ReleaseSevices(std::string &name);
    StatusCallback *statusCallback_;
    std::string service_name_ = "drm_interface_service";
    sptr<DrmHostDeathRecipient> drmHostDeathRecipient_ = nullptr;
    sptr<IMediaKeySystem> hdiMediaKeySystem;
    std::recursive_mutex handleAndKeySystemMapMutex;
    std::map<void *, sptr<IMediaKeySystem>> handleAndKeySystemMap;
    std::thread serviceThread;
    bool serviceThreadRunning = false;
    std::vector<void *> loadedLibs;
    static std::mutex queueMutex;
    static std::queue<Message> messageQueue;
    static std::condition_variable cv;
    std::mutex libMutex;
    std::map<std::string, void *> libMap;
    std::shared_mutex drmHostServieProxyMapMtx;
    std::map<std::string, sptr<IMediaKeySystemFactory>> drmHostServieProxyMap;
    std::shared_mutex lazyLoadPluginInfoMapMtx;
    std::map<std::string, std::string> lazyLoadPluginInfoMap;
    std::map<std::string, int32_t> pluginCountMap;
};
} // DrmStandard
} // OHOS


#endif // OHOS_DRM_DRM_HOST_MANAGER_H
