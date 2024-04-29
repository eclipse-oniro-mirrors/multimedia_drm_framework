/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

#ifndef DRM_DFX_H
#define DRM_DFX_H

#ifdef ENABLE_DRM_SYSEVENT_CONTROL

#include <list>
#include <string>
#include <refbase.h>
#include "nocopyable.h"
#include "meta/meta.h"
#include "hisysevent.h"
#include "hitrace/tracechain.h"
#include <chrono>
#endif

namespace OHOS {
namespace DrmStandard {
using namespace OHOS::HiviewDFX;

#define EXPORT __attribute__ ((visibility ("default")))
#ifdef ENABLE_DRM_SYSEVENT_CONTROL


static std::chrono::system_clock::time_point currentTime_ = std::chrono::system_clock::now();
static std::map<uint64_t, int32_t> idMap_;
static std::map<int32_t, std::list<std::pair<uint64_t, std::shared_ptr<Media::Meta>>>> mediaInfoMap;
static std::list<u_int64_t> invalidInstanceIdList_;
static std::mutex mux_;

class DrmEvent {
public:
    static DrmEvent& GetInstance();

    template<typename... Types>
    static int HiSysWriteFault(const std::string& eventName, Types... keyValues)
    {
        return HiSysEventWrite(OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MEDIA, eventName,
            OHOS::HiviewDFX::HiSysEvent::EventType::FAULT, keyValues...);
    }

    template<typename... Types>
    static int HiSysWriteBehavior(const std::string& eventName, Types... keyValues)
    {
        return HiSysEventWrite(OHOS::HiviewDFX::HiSysEvent::Domain::MULTI_MEDIA, eventName,
            OHOS::HiviewDFX::HiSysEvent::EventType::BEHAVIOR, keyValues...);
    }
};
#endif

#ifdef ENABLE_DRM_SYSEVENT_CONTROL
#define HISYSEVENT_FAULT(eventName, ...) ((void)DrmEvent::HiSysWriteFault(eventName, __VA_ARGS__))
#define HISYSEVENT_BEHAVIOR(eventName, ...) ((void)DrmEvent::HiSysWriteBehavior(eventName, __VA_ARGS__))

#else
#define HISYSEVENT_FAULT(...)
#define HISYSEVENT_BEHAVIOR(...)
#endif

} // namespace DrmStandard
} // namespace OHOS
#endif // DRM_DFX_H