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

#include "key_session_service.h"
#include "drm_log.h"
#include "ipc_skeleton.h"

namespace OHOS {
namespace DrmStandard {
static std::mutex sessionMutex_;
KeySessionService::KeySessionService()
{
    DRM_DEBUG_LOG("KeySessionService::KeySessionService.");
}

KeySessionService::~KeySessionService()
{
    DRM_INFO_LOG("KeySessionService::~KeySessionService enter.");
    DRM_INFO_LOG("KeySessionService::~KeySessionService exit.");
}

int32_t KeySessionService::CloseKeySessionServiceByCallback()
{
    DRM_INFO_LOG("KeySessionService::CloseKeySessionServiceByCallback enter.");
    DRM_INFO_LOG("KeySessionService::CloseKeySessionServiceByCallback exit.");
    return DRM_OK;
}

int32_t KeySessionService::Release()
{
    DRM_INFO_LOG("KeySessionService::Release enter.");
    DRM_INFO_LOG("KeySessionService::Release exit.");
    return DRM_OK;
}

int32_t KeySessionService::SetKeySessionServiceOperatorsCallback(wptr<IKeySessionServiceOperatorsCallback> callback)
{
    DRM_INFO_LOG("KeySessionService::SetKeySessionServiceOperatorsCallback enter.");
    DRM_INFO_LOG("KeySessionService::SetKeySessionServiceOperatorsCallback exit.");
    return DRM_OK;
}

int32_t KeySessionService::GenerateLicenseRequest(IKeySessionService::DrmInfo &drmInfo,
    IKeySessionService::LicenseInfo &licenseInfo)
{
    DRM_INFO_LOG("KeySessionService::GenerateLicenseRequest enter.");
    DRM_INFO_LOG("KeySessionService::GenerateLicenseRequest exit.");
    return DRM_OK;
}

int32_t KeySessionService::ProcessLicenseResponse(std::vector<uint8_t> &keyId, std::vector<uint8_t> &licenseResponse)
{
    DRM_INFO_LOG("KeySessionService::ProcessLicenseResponse enter.");
    DRM_INFO_LOG("bKeySessionService::ProcessLicenseResponse exit.");
    return DRM_OK;
}

int32_t KeySessionService::GenerateOfflineReleaseRequest(std::vector<uint8_t> &keyId,
    std::vector<uint8_t> &releaseRequest)
{
    DRM_INFO_LOG("KeySessionService::GenerateOfflineReleaseRequest enter.");
    DRM_INFO_LOG("KeySessionService::GenerateOfflineReleaseRequest exit.");
    return DRM_OK;
}

int32_t KeySessionService::ProcessOfflineReleaseResponse(std::vector<uint8_t> &keyId,
    std::vector<uint8_t> &releaseResponse)
{
    DRM_INFO_LOG("KeySessionService::ProcessOfflineReleaseResponse enter.");
    DRM_INFO_LOG("KeySessionService::ProcessOfflineReleaseResponse exit.");
    return DRM_OK;
}

int32_t KeySessionService::CheckLicenseStatus(std::vector<IKeySessionService::KeyValue> &infoMap)
{
    DRM_INFO_LOG("KeySessionService::CheckLicenseStatus enter.");
    DRM_INFO_LOG("KeySessionService::CheckLicenseStatus exit.");
    return DRM_OK;
}

int32_t KeySessionService::RestoreOfflineKeys(std::vector<uint8_t> &keyId)
{
    DRM_INFO_LOG("KeySessionService::RestoreOfflineKeys enter.");
    DRM_INFO_LOG("KeySessionService::RestoreOfflineKeys exit.");
    return DRM_OK;
}

int32_t KeySessionService::RemoveOfflineKeys(std::vector<uint8_t> &keyId)
{
    DRM_INFO_LOG("KeySessionService::RemoveOfflineKeys enter.");
    DRM_INFO_LOG("KeySessionService::RemoveOfflineKeys exit.");
    return DRM_OK;
}

int32_t KeySessionService::GetOfflineKeyIds(std::vector<std::vector<uint8_t>> &keyIds)
{
    DRM_INFO_LOG("KeySessionService::GetOfflineKeyIds enter.");
    DRM_INFO_LOG("KeySessionService::GetOfflineKeyIds exit.");
    return DRM_OK;
}

int32_t KeySessionService::RemoveLicenses()
{
    DRM_INFO_LOG("KeySessionService::RemoveLicenses enter.");
    DRM_INFO_LOG("KeySessionService::RemoveLicenses exit.");
    return DRM_OK;
}

int32_t KeySessionService::CreateMediaDecryptModule(sptr<IMediaDecryptModuleService> &decryptModule)
{
    DRM_INFO_LOG("KeySessionService::CreateMediaDecryptModule enter.");
    DRM_INFO_LOG("KeySessionService::CreateMediaDecryptModule exit.");
    return DRM_OK;
}

int32_t KeySessionService::GetOfflineKeyState(std::vector<uint8_t> &keyId, IKeySessionService::OfflineKeyState &state)
{
    DRM_INFO_LOG("KeySessionService::GetOfflineKeyState enter.");
    DRM_ERR_LOG("KeySessionService::GetOfflineKeyState exit.");
    return DRM_OK;
}

int32_t KeySessionService::SetKeySessionServiceCallback(sptr<IKeySessionServiceCallback>& callback)
{
    DRM_INFO_LOG("KeySessionService::SetKeySessionServiceCallback enter.");
    DRM_INFO_LOG("KeySessionService::SetKeySessionServiceCallback exit.");
    return DRM_OK;
}

void KeySessionService::OnKeySessionKeyExpiredStatus(const KeyStatus status)
{
    DRM_INFO_LOG("KeySessionService::OnKeySessionKeyExpiredStatus status %{public}d.", status);
}

void KeySessionService::OnKeySessionReclaimed(const SessionStatus status)
{
    DRM_INFO_LOG("KeySessionService::OnKeySessionReclaimed status %{public}d.", status);
}
} // DrmStandard 
} // OHOS