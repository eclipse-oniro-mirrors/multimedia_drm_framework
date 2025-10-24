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

#include "drm_net_observer.h"
#include "drm_log.h"
#include "drm_error_code.h"
#include "net_conn_client.h"
#include "net_conn_constants.h"
#include <unistd.h>
#include <future>
#include <refbase.h>

namespace OHOS {
namespace DrmStandard {

using namespace OHOS::NetManagerStandard;

static constexpr int32_t RETRY_MAX_TIMES = 10;
static constexpr int32_t RETRY_INTERVAL_S = 1;

DrmNetObserver::~DrmNetObserver() {
    DRM_CHECK_AND_RETURN_RET_LOG(startFuture_.valid(), , "startFuture_ not valid");
    stopRequested_ = true;
    StopObserver();
    startFuture_.wait();
}

void DrmNetObserver::StartObserver() {
    DRM_INFO_LOG("DRM NetObserver Start");
    stopRequested_ = false;
    startFuture_ = std::async(std::launch::async, [self = sptr<DrmNetObserver>(this)]() {
        NetSpecifier netSpecifier;
        NetAllCapabilities netAllCapabilities;
        netAllCapabilities.netCaps_.insert(NetCap::NET_CAPABILITY_INTERNET);
        netSpecifier.ident_ = "";
        netSpecifier.netCapabilities_ = netAllCapabilities;
        sptr<NetSpecifier> specifier = new NetSpecifier(netSpecifier);

        int32_t retryCount = 0;
        int32_t ret = NetConnResultCode::NET_CONN_SUCCESS;

        do {
            DRM_CHECK_AND_RETURN_RET_LOG(!self->stopRequested_, , "early exit");
            ret = NetConnClient::GetInstance().RegisterNetConnCallback(specifier, self, 0);
            if (ret == NetConnResultCode::NET_CONN_SUCCESS) {
                DRM_INFO_LOG("DRM NetObserver Start Success");
                std::lock_guard<std::mutex> lock(self->netCallbackMutex_);
                self->netCallback_ = self;
                return;
            }
            ++retryCount;
            DRM_WARNING_LOG("DRM Start Observer Retry %d, ret = %d", retryCount, ret);
            sleep(RETRY_INTERVAL_S);
        } while (retryCount < RETRY_MAX_TIMES);

        DRM_ERR_LOG("DRM Start Observer Failed after %d retries", retryCount);
    });
}

int32_t DrmNetObserver::StopObserver() {
    sptr<INetConnCallback> callbackCopy;
    {
        std::lock_guard<std::mutex> lock(netCallbackMutex_);
        callbackCopy = netCallback_;
        netCallback_ = nullptr;
    }
    DRM_CHECK_AND_RETURN_RET_LOG(callbackCopy, DRM_INNER_ERR_UNKNOWN, "no registered callback");

    int32_t ret = NetConnClient::GetInstance().UnregisterNetConnCallback(callbackCopy);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == NetConnResultCode::NET_CONN_SUCCESS, DRM_INNER_ERR_UNKNOWN, "Unregister Error, ret=%d", ret);
    DRM_INFO_LOG("Unregister Success");
    return DRM_INNER_ERR_OK;
}

int32_t DrmNetObserver::NetCapabilitiesChange(sptr<NetHandle>& netHandle,
                                              const sptr<NetAllCapabilities>& netAllCap) {
    (void)netHandle;
    DRM_INFO_LOG("NetCapabilitiesChange");
    DRM_CHECK_AND_RETURN_RET_LOG(netAllCap, DRM_INNER_ERR_INVALID_VAL, "NetAllCap is nullptr");
    return HandleNetAllCap(*netAllCap);
}

int32_t DrmNetObserver::HandleNetAllCap(const NetAllCapabilities& netAllCap) {
    DRM_CHECK_AND_RETURN_RET_LOG(m_drmHostManager, DRM_INNER_ERR_INVALID_VAL, "drmHostManager is nullptr");

    bool hasInternet = netAllCap.netCaps_.count(NetCap::NET_CAPABILITY_INTERNET) &&
                       netAllCap.netCaps_.count(NetCap::NET_CAPABILITY_VALIDATED);
    bool isChecking  = netAllCap.netCaps_.count(NetCap::NET_CAPABILITY_CHECKING_CONNECTIVITY);

    if (hasInternet && !isChecking) {
        DRM_INFO_LOG("DrmNetwork OK, trigger CertDownload");
        m_drmHostManager->SetIsNetWork(true);
        m_drmHostManager->condVar.notify_one();

        int32_t ret = StopObserver();
        DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK, DRM_INNER_ERR_UNKNOWN, "StopObserver Error");
        DRM_INFO_LOG("DrmNetwork OK, CertDownload End");
        return DRM_INNER_ERR_OK;
    }

    DRM_INFO_LOG("Network not ready or still checking");
    return DRM_INNER_ERR_UNKNOWN;
}

int32_t DrmNetObserver::SetDrmHostManager(const sptr<DrmHostManager>& drmHostManager) {
    DRM_CHECK_AND_RETURN_RET_LOG(drmHostManager, DRM_INNER_ERR_INVALID_VAL, "drmHostManager is nullptr");
    m_drmHostManager = drmHostManager;
    return DRM_INNER_ERR_OK;
}

} // namespace DrmStandard
} // namespace OHOS