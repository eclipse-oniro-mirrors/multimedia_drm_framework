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

#ifndef DRM_LISTENER_PROXY_H
#define DRM_LISTENER_PROXY_H

#include "i_drm_listener.h"
#include "nocopyable.h"

namespace OHOS {
namespace DrmStandard {
class DrmListenerProxy : public IRemoteProxy<IDrmListener>, public NoCopyable {
public:
    explicit DrmListenerProxy(const sptr<IRemoteObject> &impl);
    virtual ~DrmListenerProxy();
private:
    static inline BrokerDelegator<DrmListenerProxy> delegator_;
};
} // namespace DrmStandard
} // namespace OHOS
#endif // DRM_LISTENER_PROXY_H