#ifndef OHOS_DRM_DRM_HOST_MANAGER_H
#define OHOS_DRM_DRM_HOST_MANAGER_H
#include <refbase.h>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
// #include "drm_interface_proxy.h"
// #include "drm_impl.h"
// #include "drm_types.h"
#include "i_mediakeysystem_service.h"
#include "iservstat_listener_hdi.h"

namespace OHOS{
namespace DrmStandard{

class DrmHostManager : public virtual RefBase, public HDI::ServiceManager::V1_0::ServStatListenerStub {
public:
    class StatusCallback{
    public:
        virtual ~StatusCallback() = default;
    };
    class DrmHostDeathRecipient : public IRemoteObject::DeathRecipient {
    public:
        explicit DrmHostDeathRecipient() {};
        virtual ~DrmHostDeathRecipient() = default;
        void OnRemoteDied(const wptr<IRemoteObject> &remote) override;
    };

    explicit DrmHostManager(StatusCallback* statusCallback);
    ~DrmHostManager() override;

    int32_t Init(void);
    void DeInit(void);
    void OnReceive(const HDI::ServiceManager::V1_0::ServiceStatus& status) override;
    int32_t IsMediaKeySystemSupported(std::string &uuid, bool *isSurpported);
    int32_t IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType, bool *isSurpported);
    int32_t IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType, int32_t securityLevel, bool *isSurpported);
    int32_t CreateMediaKeySystem(std::string &uuid);
private:
    int32_t GetSevices(std::string &uuid, bool *isSurpported);
};

} // DrmStandard
} // OHOS


#endif //OHOS_DRM_DRM_HOST_MANAGER_H