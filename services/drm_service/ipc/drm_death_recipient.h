#ifndef OHOS_DRM_DRM_DEATH_RECIPIENT_H
#define OHOS_DRM_DRM_DEATH_RECIPIENT_H

#include "iremote_object.h"
#include "nocopyable.h"

namespace OHOS {
namespace DrmStandard {

class DrmDeathRecipient : public IRemoteObject::DeathRecipient, public NoCopyable {
public:
    explicit DrmDeathRecipient(pid_t pid) : pid_(pid) {}
    virtual ~DrmDeathRecipient() = default;

    void OnRemoteDied(const wptr<IRemoteObject> &remote) override
    {
        (void) remote;
        if (diedCb_ != nullptr) {
            diedCb_(pid_);
        }
    }

    using NotifyCbFunc = std::function<void(pid_t)>;
    
    void SetNotifyCb(NotifyCbFunc func)
    {
        diedCb_ = func;
    }

private:
    pid_t pid_ = 0;
    NotifyCbFunc diedCb_ = nullptr;
};

} // DrmStandard   
} // OHOS

#endif