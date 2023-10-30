/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

#include <iostream>
#include <string>
#include <map>
#include "napi/native_api.h"
#include "napi/native_node_api.h"

namespace OHOS {
namespace DrmStandard {

class CallBackPair : public RefBase {
public:
    CallBackPair(napi_env env, napi_ref callback) : env_(env), callback_(callback) { }
    
    ~CallBackPair()
    {
        if (env_ != nullptr && callback_ != nullptr) {
            (void)napi_delete_reference(env_, callback_);
        }
    }

    napi_env GetEnv() {
        return env_;
    }

    napi_ref GetCallback() {
        return callback_;
    }

private:
    napi_env env_;
    napi_ref callback_;
};

namespace DrmEvent {
const std::string DRM_EVENT_KEY_EXPIRED = "keyExpired";
const std::string DRM_EVENT_KEY_SESSION_RECLAIMED = "keySessionReclaimed";
const std::string DRM_EVENT_SYSTEM_PROVISION_REQUIRED = "keySystemProvisionRequired";
}

} // namespace DrmStandard
} // namespace OHOS