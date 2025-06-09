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
#include "iservmgr_hdi.h"
#include "servmgr_hdi.h"
#include "idevmgr_hdi.h"
#include "hdf_device_class.h"
#include "i_mediakeysystem_service.h"
#include "iservstat_listener_hdi.h"
#include "v1_0/media_key_system_factory_proxy.h"
#include "v1_0/media_key_system_proxy.h"

namespace OHOS {
namespace DrmStandard {
using namespace OHOS::HDI::Drm::V1_0;
using namespace OHOS::HDI;
using OHOS::HDI::DeviceManager::V1_0::IDeviceManager;
using OHOS::HDI::ServiceManager::V1_0::IServiceManager;

enum ExtraInfo {
    OEM_CERT_PROVISIONED_DONE = 0,
    MEDIA_KEY_SYSTEM_ERROR,
    GET_OEM_CERTIFICATE_ERROR,
    GET_OEM_PROVISION_REQUEST_ERROR,
    OEM_CERT_PROVISION_SERVER_ERROR,
    OEM_CERT_PROVISIONED_ERROR,
    EXIT_FOR_UNKNOWN_CAUSE,
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
        virtual void OnDrmPluginDied(std::string &name) = 0;
    };
    class DrmHostDeathRecipient : public IRemoteObject::DeathRecipient {
    public:
        explicit DrmHostDeathRecipient(const sptr<DrmHostManager>& drmHostManager,
            std::string &name);
        virtual ~DrmHostDeathRecipient();
        void OnRemoteDied(const wptr<IRemoteObject> &remote) override;
    private:
        wptr<DrmHostManager> drmHostManager_;
        std::string name_;
    };

    explicit DrmHostManager(StatusCallback *statusCallback);
    ~DrmHostManager() override;

    int32_t Init(void);
    void DeInit(void);
    void OnReceive(const HDI::ServiceManager::V1_0::ServiceStatus &status) override;
    int32_t IsMediaKeySystemSupported(std::string &name, bool *isSupported);
    int32_t IsMediaKeySystemSupported(std::string &name, std::string &mimeType, bool *isSupported);
    int32_t IsMediaKeySystemSupported(std::string &name, std::string &mimeType, int32_t securityLevel,
        bool *isSupported);
    int32_t CreateMediaKeySystem(std::string &name, sptr<IMediaKeySystem> &hdiMediaKeySystem);
    int32_t GetMediaKeySystems(std::map<std::string, std::string> &mediaKeySystemDescription);
    int32_t GetMediaKeySystemUuid(std::string &name, std::string &uuid);
    void ReleaseMediaKeySystem(sptr<IMediaKeySystem> &hdiMediaKeySystem);
    void ClearDeathService(std::string &name);
    void OnDrmPluginDied(std::string &name);
private:
    static void UnLoadOEMCertifaicateService(std::string &name, ExtraInfo info);
    int32_t InitGetMediaKeySystems();
    void StopServiceThread();
    void DelayedLazyUnLoad();
    void ProcessMessage();
    void ServiceThreadMain();
    void GetOemLibraryPath(std::vector<std::string> &libsToLoad);
    void OemCertificateManager();
    int32_t ProcessLazyLoadInfomation(std::string &name, sptr<IMediaKeySystemFactory> &drmHostServieProxy);
    int32_t ProcessLazyLoadPlugin(std::string &name, std::vector<std::string> &serviceName,
        sptr<IDeviceManager> &deviceMgr, sptr<IServiceManager> &servmgr);
    int32_t GetServices(std::string &name, bool *isSupported, sptr<IMediaKeySystemFactory> &drmHostServieProxys);
    void ReleaseHandleAndKeySystemMap(void *handle);
    std::string StringTrim(const std::string& str);
    int32_t LazyLoadPlugin(std::string &name, std::vector<std::string> &serviceName,
    sptr<IDeviceManager> &deviceMgr, sptr<IServiceManager> &servmgr);
    void parseLazyLoadService(std::ifstream& file, std::map<std::string, std::string>& lazyLoadPluginInfoMap);
    int32_t LoadPluginInfo(const std::string& filePath);
    void ReleaseSevices(sptr<IMediaKeySystemFactory> drmHostServieProxy);
    void UnloadAllServices();

    StatusCallback *statusCallback_;
    std::map<void *, sptr<IMediaKeySystem>> handleAndKeySystemMap;
    std::thread serviceThread;
    std::thread messageQueueThread;
    bool serviceThreadRunning = false;
    std::vector<void *> loadedLibs;
    std::recursive_mutex drmHostMapMutex;
    static std::mutex queueMutex;
    static std::queue<Message> messageQueue;
    static std::condition_variable cv;
    std::map<std::string, void *> pluginNameAndHandleMap;
    std::map<std::string, std::string> lazyLoadPluginInfoMap;
    std::map<std::string, int32_t> lazyLoadPluginCountMap;
    std::map<std::string, int32_t> lazyLoadPluginTimeoutMap;
    std::map<std::string, std::string> mediaKeySystemDescription_;
    std::map<sptr<IMediaKeySystem>, sptr<IMediaKeySystemFactory>> hdiMediaKeySystemAndFactoryMap;
    std::map<sptr<IMediaKeySystemFactory>, std::string> hdiMediaKeySystemFactoryAndPluginNameMap;
    std::map<sptr<IMediaKeySystemFactory>, sptr<DrmHostDeathRecipient>> drmHostDeathRecipientMap;
};
} // DrmStandard
} // OHOS

#endif // OHOS_DRM_DRM_HOST_MANAGER_H