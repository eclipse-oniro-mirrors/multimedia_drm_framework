/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#ifndef DRM_NET_OBSERVER_H
#define DRM_NET_OBSERVER_H

#include <mutex>
#include <future>
#include <atomic>
#include "refbase.h"
#include "net_all_capabilities.h"
#include "net_conn_callback_stub.h"
#include "net_specifier.h"
#include "drm_host_manager.h"

namespace OHOS {
namespace DrmStandard {

class DrmNetObserver : public OHOS::NetManagerStandard::NetConnCallbackStub,
                       public virtual OHOS::RefBase {
public:
    DrmNetObserver() = default;
    ~DrmNetObserver();

    void StartObserver();
    int32_t StopObserver();
    int32_t NetCapabilitiesChange(
        sptr<OHOS::NetManagerStandard::NetHandle> &netHandle,
        const sptr<OHOS::NetManagerStandard::NetAllCapabilities> &netAllCap) final;
    int32_t SetDrmHostManager(const sptr<DrmHostManager> &drmHostManager);

private:
    int32_t HandleNetAllCap(const OHOS::NetManagerStandard::NetAllCapabilities &netAllCap);

    sptr<DrmHostManager> drmHostManager_;
    sptr<OHOS::NetManagerStandard::INetConnCallback> netCallback_;
    std::mutex netCallbackMutex_;
    std::future<void> startFuture_;
    std::atomic<bool> stopRequested_{false};
};

} // namespace DrmStandard
} // namespace OHOS

#endif // DRM_NET_OBSERVER_H