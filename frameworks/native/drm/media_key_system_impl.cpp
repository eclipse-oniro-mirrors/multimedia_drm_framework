/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include "media_key_system_impl.h"
#include "i_mediakeysystem_service.h"

namespace OHOS {
namespace DrmStandard {
MediaKeySystemImpl::MediaKeySystemImpl(sptr<IMediaKeySystemService> &mediaKeysystem)
{
    DRM_DEBUG_LOG("MediaKeySystemImpl:0x%{public}06" PRIXPTR "MediaKeySystemImpl Instances create",
        FAKE_POINTER(this));
    serviceProxy_ = mediaKeysystem;
}

MediaKeySystemImpl::~MediaKeySystemImpl()
{
    serviceProxy_ = nullptr;
}

int32_t MediaKeySystemImpl::Release()
{
    DRM_INFO_LOG("MediaKeySystemImpl::Release enter.");
    int errCode = DRM_UNKNOWN_ERROR;
    if (serviceProxy_ != nullptr) {
        errCode = serviceProxy_->Release();
        if (errCode != DRM_OK) {
            DRM_ERR_LOG("Failed to Release keySystem!, %{public}d", errCode);
            return errCode;
        }
        serviceProxy_ = nullptr;
    } else {
        DRM_ERR_LOG("serviceProxy_ == nullptr");
        return -1;
    }
    DRM_INFO_LOG("MediaKeySystemImpl::Release exit.");
    return DRM_OK;
}

int MediaKeySystemImpl::GenerateKeySystemRequest(IMediaKeySystemService::RequestType type,
    std::vector<uint8_t> &request, std::string &defaultUrl)
{
    DRM_INFO_LOG("MediaKeySystemImpl::GenerateKeySystemRequest enter. type:%{public}d.", type);
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl::GenerateKeySystemRequest serviceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = serviceProxy_->GenerateKeySystemRequest(type, request, defaultUrl);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemImpl::GenerateKeySystemRequest failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySystemImpl::GenerateKeySystemRequest exit.");
    return DRM_OK;
}

int MediaKeySystemImpl::ProcessKeySystemResponse(IMediaKeySystemService::RequestType type,
    const std::vector<uint8_t> &response)
{
    DRM_INFO_LOG("MediaKeySystemImpl::ProcessKeySystemResponse enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl::ProcessKeySystemResponse serviceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = serviceProxy_->ProcessKeySystemResponse(type, response);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemImpl::ProcessKeySystemResponse failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySystemImpl::ProcessKeySystemResponse exit.");
    return DRM_OK;
}

int MediaKeySystemImpl::SetConfiguration(IMediaKeySystemService::ConfigType type, std::string &propertyName,
    std::string &value)
{
    DRM_INFO_LOG("SetConfiguration enter, configType:%{public}d, propertyName:%{public}s, value:%{public}s.",
        (int)type, propertyName.c_str(), value.c_str());
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl::SetConfiguration serviceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = serviceProxy_->SetConfiguration(type, propertyName, value);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemImpl::SetConfiguration failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("SetConfiguration exit.");
    return DRM_OK;
}

int MediaKeySystemImpl::GetConfiguration(IMediaKeySystemService::ConfigType configType, std::string &propertyName,
    std::string &value)
{
    DRM_INFO_LOG("MediaKeySystemImpl::GetConfiguration enter, configType:%{public}d, propertyName:%{public}s.",
        (int)configType, propertyName.c_str());
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl::GetConfiguration serviceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = serviceProxy_->GetConfiguration(configType, propertyName, value);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemImpl::GetConfiguration failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }

    DRM_INFO_LOG("MediaKeySystemImpl::GetConfiguration exit, value:%{public}s.", value.c_str());
    return DRM_OK;
}

int MediaKeySystemImpl::CreateKeySession(IKeySessionService::SecurityLevel securityLevel,
    sptr<KeySessionImpl> *keySessionImpl)
{
    DRM_INFO_LOG("MediaKeySystemImpl::CreateKeySession enter.");
    sptr<IKeySessionService> keySessionProxy = nullptr;
    sptr<KeySessionImpl> localKeySessionImpl = nullptr;
    int retCode = DRM_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl:: serviceProxy_ == nullptr");
        return DRM_SERVICE_ERROR;
    }

    retCode = serviceProxy_->CreateKeySession(securityLevel, keySessionProxy);
    if (retCode == DRM_OK) {
        if (keySessionProxy != nullptr) {
            localKeySessionImpl = new(std::nothrow) KeySessionImpl(keySessionProxy);
            if (localKeySessionImpl == nullptr) {
                DRM_ERR_LOG("Failed to new KeySessionImpl");
                return DRM_ALLOC_ERROR;
            }
        } else {
            DRM_ERR_LOG("Failed to CreateKeySessionImpl with session is null");
            return DRM_UNKNOWN_ERROR;
        }
    } else {
        DRM_ERR_LOG("Failed to get session object from mediakeysystem service!, %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    *keySessionImpl = localKeySessionImpl;
    DRM_INFO_LOG("MediaKeySystemImpl::CreateKeySession exit.");
    return DRM_OK;
}

int32_t MediaKeySystemImpl::GetMetric(std::vector<IMediaKeySystemService::KeyValue> &infoMap)
{
    DRM_INFO_LOG("MediaKeySystemImpl::GetMetric enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl::GetMetric serviceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = serviceProxy_->GetMetric(infoMap);
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemImpl::GetMetric failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySystemImpl::GetMetric exit.");
    return DRM_OK;
}

int MediaKeySystemImpl::GetSecurityLevel(IKeySessionService::SecurityLevel *securityLevel)
{
    DRM_INFO_LOG("MediaKeySystemImpl::GetSecurityLevel enter.");
    std::lock_guard<std::mutex> lock(mutex_);
    int32_t retCode = DRM_OK;

    if (serviceProxy_ == nullptr) {
        DRM_ERR_LOG("MediaKeySystemImpl::GetSecurityLevel serviceProxy_ is null");
        return DRM_SERVICE_ERROR;
    }
    retCode = serviceProxy_->GetSecurityLevel((IKeySessionService::SecurityLevel *)securityLevel);
    DRM_ERR_LOG("MediaKeySystemImpl::GetSecurityLevel 277");
    if (retCode != DRM_OK) {
        DRM_ERR_LOG("MediaKeySystemImpl::GetSecurityLevel failed, retCode: %{public}d", retCode);
        return DRM_SERVICE_ERROR;
    }
    DRM_INFO_LOG("MediaKeySystemImpl::GetSecurityLevel exit.");
    return DRM_OK;
}
} // namespace DrmStandard
} // namespace OHOS