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

#ifndef OHOS_DRM_KeySessionCallbackNapi_H_
#define OHOS_DRM_KeySessionCallbackNapi_H_

#include "key_session_impl.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "common_napi.h"

namespace OHOS {
namespace DrmStandard {

class KeySessionCallbackNapi : public KeySessionImplCallback {
public:
    explicit KeySessionCallbackNapi();
    virtual ~KeySessionCallbackNapi();
    void SetCallbackReference(const std::string eventType, sptr<CallBackPair> callbackPair);
    void ClearCallbackReference(const std::string eventType);
    void OnKeySessionKeyExpired(const std::string eventType, const KeyStatus status) override;
    void OnKeySessionReclaimed(const std::string eventType, const SessionStatus status) override;

private:
    std::mutex mutex_;
    std::map<std::string, sptr<CallBackPair>> callbackMap_;
};

} // namespace DrmStandard
} // namespace OHOS


#endif