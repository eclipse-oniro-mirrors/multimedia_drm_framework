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

#include "media_key_system_factory_impl.h"
#include "imedia_key_system_service.h"
#include "drm_error_code.h"
#include "napi_param_utils.h"
#include "iservice_registry.h"
#include "system_ability_definition.h"

namespace OHOS {
namespace DrmStandard {
using namespace OHOS::HiviewDFX;

sptr<MediaKeySystemFactoryImpl> MediaKeySystemFactoryImpl::mediaKeySystemFactoryImpl_ = nullptr;
constexpr int64_t SLEEP_TIME = 1;
constexpr int32_t RETRY_TIMES = 3;

MediaKeySystemFactoryImpl::MediaKeySystemFactoryImpl()
{
    DRM_DEBUG_LOG("0x%{public}06" PRIXPTR "MediaKeySystemFactoryImpl Instances create",
        FAKE_POINTER(this));
    traceId_ = HiTraceChain::Begin("MediaKeySystemFactory", HITRACE_FLAG_DEFAULT);
    Init();
}

MediaKeySystemFactoryImpl::~MediaKeySystemFactoryImpl()
{
    DRM_INFO_LOG("~MediaKeySystemFactoryImpl enter.");
    HiTraceChain::End(traceId_);
    deathRecipient_ = nullptr;
}

const sptr<IMediaKeySystemFactoryService> MediaKeySystemFactoryImpl::GetServiceProxy()
{
    std::lock_guard<std::mutex> lock(serviceProxyMutex_);
    if (privateServiceProxy_ != nullptr) {
        return privateServiceProxy_;
    }

    DRM_INFO_LOG("Connect media key system service.");
    sptr<IRemoteObject> object = nullptr;
    auto samgr = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    DRM_CHECK_AND_RETURN_RET_LOG(samgr != nullptr, nullptr,
        "Failed to get System ability manager!");
    object = samgr->CheckSystemAbility(MEDIA_KEY_SYSTEM_SERVICE_ID);
    if (object == nullptr) {
        object = samgr->LoadSystemAbility(MEDIA_KEY_SYSTEM_SERVICE_ID, 30); // 30: timeout
    }
    DRM_CHECK_AND_RETURN_RET_LOG(object != nullptr, nullptr,
        "the object returned by GetSystemAbility is nullptr!");
    sptr<IMediaKeySystemFactoryService> tmpProxy = iface_cast<IMediaKeySystemFactoryService>(object);
    DRM_CHECK_AND_RETURN_RET_LOG(tmpProxy != nullptr, nullptr, "cast the object to privateServiceProxy_ failed!");

    pid_t pid = 0;
    sptr<DrmDeathRecipient> deathRecipient = new (std::nothrow) DrmDeathRecipient(pid);
    DRM_CHECK_AND_RETURN_RET_LOG(deathRecipient != nullptr, nullptr,
        "failed to new DrmDeathRecipient!");
    deathRecipient->SetNotifyCb([this] (pid_t pid) {
        this->MediaKeySystemFactoryServerDied(pid);
    });
    bool result = object->AddDeathRecipient(deathRecipient);
    DRM_CHECK_AND_RETURN_RET_LOG(result, nullptr, "failed to new DrmDeathRecipient!");

    DRM_INFO_LOG("Ready to create listener object");
    sptr<DrmListenerStub> tmpListenerStub = new(std::nothrow) DrmListenerStub();
    DRM_CHECK_AND_RETURN_RET_LOG(tmpListenerStub != nullptr, nullptr, "failed to new DrmListenerStub object");

    sptr<IRemoteObject> listenerObject = tmpListenerStub->AsObject();
    DRM_CHECK_AND_RETURN_RET_LOG(listenerObject != nullptr, nullptr, "listener object is nullptr.");
    int32_t ret = tmpProxy->SetListenerObject(listenerObject);
    DRM_CHECK_AND_RETURN_RET_LOG(ret == DRM_INNER_ERR_OK, nullptr, "set listener failed.");
    privateServiceProxy_ = tmpProxy;
    deathRecipient_ = deathRecipient;
    listenerStub_ = tmpListenerStub;
    return privateServiceProxy_;
}

sptr<MediaKeySystemFactoryImpl> &MediaKeySystemFactoryImpl::GetInstance()
{
    DRM_INFO_LOG("GetInstance enter.");
    if (MediaKeySystemFactoryImpl::mediaKeySystemFactoryImpl_ == nullptr) {
        DRM_DEBUG_LOG("Initializing MediaKeySystemFactoryImpl for first time!");
        MediaKeySystemFactoryImpl::mediaKeySystemFactoryImpl_ = new (std::nothrow) MediaKeySystemFactoryImpl();
        if (MediaKeySystemFactoryImpl::mediaKeySystemFactoryImpl_ == nullptr) {
            DRM_ERR_LOG("GetInstance failed to new MediaKeySystemFactoryImpl");
        }
    }
    return MediaKeySystemFactoryImpl::mediaKeySystemFactoryImpl_;
}

void MediaKeySystemFactoryImpl::Init()
{
    DRM_INFO_LOG("Init enter.");
    HiTraceChain::SetId(traceId_);
}

void MediaKeySystemFactoryImpl::MediaKeySystemFactoryServerDied(pid_t pid)
{
    DRM_ERR_LOG("MediaKeySystemFactoryServerDied has died, pid:%{public}d!", pid);
    {
        std::lock_guard<std::mutex> lock(serviceProxyMutex_);
        if (privateServiceProxy_ != nullptr && privateServiceProxy_->AsObject() != nullptr) {
            (void)privateServiceProxy_->AsObject()->RemoveDeathRecipient(deathRecipient_);
            privateServiceProxy_ = nullptr;
        }
        listenerStub_ = nullptr;
        deathRecipient_ = nullptr;
    }

    int32_t retry = RETRY_TIMES;
    sptr<IMediaKeySystemFactoryService> serviceProxy = nullptr;
    while (retry--) {
        // Sleep and wait for 1 second;
        sleep(SLEEP_TIME);
        serviceProxy = GetServiceProxy();
        if (serviceProxy != nullptr) {
            DRM_INFO_LOG("Reconnect media key system service success!");
            break;
        }
    }

    DRM_CHECK_AND_RETURN_LOG(serviceProxy != nullptr, "failed to reconnect service!");
}

bool MediaKeySystemFactoryImpl::IsMediaKeySystemSupported(std::string &name)
{
    DRM_INFO_LOG("IsMediaKeySystemSupported enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    bool isSupported = false;
    const sptr<IMediaKeySystemFactoryService> serviceProxy = GetServiceProxy();
    DRM_CHECK_AND_RETURN_RET_LOG(serviceProxy != nullptr, isSupported, "service proxy is nullptr!");
    ret = serviceProxy->IsMediaKeySystemSupported(name, isSupported);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("IsMediaKeySystemSupported failed, ret: %{public}d", ret);
    }
    return isSupported;
}

bool MediaKeySystemFactoryImpl::IsMediaKeySystemSupported(std::string &name, std::string &mimeType)
{
    DRM_INFO_LOG("IsMediaKeySystemSupported enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    bool isSupported = false;

    const sptr<IMediaKeySystemFactoryService> serviceProxy = GetServiceProxy();
    DRM_CHECK_AND_RETURN_RET_LOG(serviceProxy != nullptr, isSupported, "service proxy is nullptr!");
    ret = serviceProxy->IsMediaKeySystemSupported(name, mimeType, isSupported);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("IsMediaKeySystemSupported failed, ret: %{public}d", ret);
    }
    return isSupported;
}

bool MediaKeySystemFactoryImpl::IsMediaKeySystemSupported(std::string &uuid, std::string &mimeType,
    ContentProtectionLevel securityLevel)
{
    DRM_INFO_LOG("IsMediaKeySystemSupported enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    bool isSupported = false;

    const sptr<IMediaKeySystemFactoryService> serviceProxy = GetServiceProxy();
    DRM_CHECK_AND_RETURN_RET_LOG(serviceProxy != nullptr, isSupported, "service proxy is nullptr!");
    ret = serviceProxy->IsMediaKeySystemSupported(uuid, mimeType, static_cast<int32_t>(securityLevel), isSupported);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("IsMediaKeySystemSupported failed, ret: %{public}d", ret);
    }
    return isSupported;
}

int32_t MediaKeySystemFactoryImpl::GetMediaKeySystems(std::map<std::string, std::string> &keySystemNames)
{
    DRM_INFO_LOG("GetMediaKeySystems enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    const sptr<IMediaKeySystemFactoryService> serviceProxy = GetServiceProxy();
    DRM_CHECK_AND_RETURN_RET_LOG(serviceProxy != nullptr, DRM_INNER_ERR_BASE, "service proxy is nullptr!");
    ret = serviceProxy->GetMediaKeySystems(keySystemNames);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GetMediaKeySystems failed, ret: %{public}d", ret);
        return ret;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySystemFactoryImpl::GetMediaKeySystemUuid(std::string &name, std::string &uuid)
{
    DRM_INFO_LOG("GetMediaKeySystemUuid enter.");
    int32_t ret = DRM_INNER_ERR_OK;
    const sptr<IMediaKeySystemFactoryService> serviceProxy = GetServiceProxy();
    DRM_CHECK_AND_RETURN_RET_LOG(serviceProxy != nullptr, DRM_INNER_ERR_BASE, "service proxy is nullptr!");
    ret = serviceProxy->GetMediaKeySystemUuid(name, uuid);
    if (ret != DRM_INNER_ERR_OK) {
        DRM_ERR_LOG("GetMediaKeySystemUuid failed, ret: %{public}d", ret);
        return ret;
    }
    return DRM_INNER_ERR_OK;
}

int32_t MediaKeySystemFactoryImpl::CreateMediaKeySystem(std::string &name, sptr<MediaKeySystemImpl> *mediaKeySystemImpl)
{
    DRM_INFO_LOG("CreateMediaKeySystem enter.");
    sptr<IMediaKeySystemService> mediaKeySystemProxy = nullptr;
    sptr<MediaKeySystemImpl> localMediaKeySystemImpl = nullptr;
    int32_t ret = DRM_INNER_ERR_OK;
    if (mediaKeySystemImpl == nullptr) {
        DRM_ERR_LOG("mediaKeySystemImpl is nullptr");
        return DRM_INNER_ERR_INVALID_VAL;
    }

    const sptr<IMediaKeySystemFactoryService> serviceProxy = GetServiceProxy();
    DRM_CHECK_AND_RETURN_RET_LOG(serviceProxy != nullptr, DRM_INNER_ERR_BASE, "service proxy is nullptr!");
    ret = serviceProxy->CreateMediaKeySystem(name, mediaKeySystemProxy);
    if (ret == DRM_INNER_ERR_OK) {
        if (mediaKeySystemProxy != nullptr) {
            localMediaKeySystemImpl = new (std::nothrow) MediaKeySystemImpl(mediaKeySystemProxy);
            if (localMediaKeySystemImpl == nullptr) {
                DRM_ERR_LOG("Failed to new MediaKeySystemImpl");
                return DRM_INNER_ERR_SERVICE_FATAL_ERROR;
            }
        } else {
            DRM_ERR_LOG("mediaKeySystemProxy is nullptr");
            return DRM_INNER_ERR_UNKNOWN;
        }
    } else if (ret == DRM_INNER_ERR_MAX_SYSTEM_NUM_REACHED) {
        DRM_ERR_LOG("The number of MediaKeySystem is greater than 64");
        return DRM_INNER_ERR_MAX_SYSTEM_NUM_REACHED;
    } else {
        DRM_ERR_LOG("Service faltal error");
        return DRM_INNER_ERR_SERVICE_FATAL_ERROR;
    }
    *mediaKeySystemImpl = localMediaKeySystemImpl;
    return DRM_INNER_ERR_OK;
}
} // namespace DrmStandard
} // namespace OHOS