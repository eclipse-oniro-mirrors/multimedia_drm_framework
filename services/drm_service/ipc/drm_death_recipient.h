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

#ifndef OHOS_DRM_DRM_DEATH_RECIPIENT_H
#define OHOS_DRM_DRM_DEATH_RECIPIENT_H

#include "iremote_object.h"
#include "nocopyable.h"
#include "drm_log.h"

namespace OHOS {
namespace DrmStandard {
class DrmDeathRecipient : public IRemoteObject::DeathRecipient, public NoCopyable {
public:
    explicit DrmDeathRecipient(pid_t pid) : pid_(pid) {}
    virtual ~DrmDeathRecipient() = default;

    void OnRemoteDied(const wptr<IRemoteObject> &remote) override
    {
        (void)remote;
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