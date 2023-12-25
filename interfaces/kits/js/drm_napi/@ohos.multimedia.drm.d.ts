/*
* Copyright (C) 2023-2024 Huawei Device Co., Ltd.
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

import {ErrorCallback, AsyncCallback, Callback} from './basic';
import { Context } from './app/context';


declare namespace drm {
  /**
   * Find a MediaKeySystem name.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   * @param uuid Used to point a Digital Right Managements solution.
   * @returns return the MediaKeySystem name.
   */
  function findMediaKeySystem(uuid: string): string;
  /**
   * Creates a Mediakeysystem instance.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   * @param uuid Used to point a Digital Right Managements solution.
   * @param callback Callback used to return the Mediakeysystem instance.
   */
  function createMediaKeySystem(uuid: string, callback: Callback<MediaKeySystem>): void;

  /**
   * Creates a MediaKeySystem instance.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   * @param uuid Used to point a Digital Right Managements solution.
   * @returns return the MediaKeySystem instance.
   */
  function createMediaKeySystem(uuid: string): MediaKeySystem;

  /**
   * Judge whether a system that specifies UUID, mimetype and security level is supported.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   * @param uuid Used to point a Digital Right Managements solution.
   * @param mimeType Used to specifies the meida type.
   * @param level Used to specifies the SecurityLevel.
   * @returns Whether these conditions will be met.
   */
  function isMediaKeySystemSupported(uuid: string, mimeType: string, level: SecurityLevel): boolean;

  /**
   * Judge whether a system that specifies UUID, mimetype and security level is supported.
   * @param uuid Used to point a Digital Right Managements solution.
   * @param mimeType Used to specifies the meida type.
   * @returns Whether these conditions will be met.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   */
  function isMediaKeySystemSupported(uuid: string, mimeType: string): boolean;

  /**
   * Judge whether a system that specifies UUID, mimetype and security level is supported.
   * @param uuid Used to point a Digital Right Managements solution.
   * @returns Whether these conditions will be met.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   */
  function isMediaKeySystemSupported(uuid: string): boolean;

  /**
   * Manages and record key sessions. Before calling an MediaKeySystem method,
   * we must use getMediaKeySystem to get a MediaKeySystem instance, then we can call functions.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   */
  interface MediaKeySystem {

    /**
     * Get the specified configuration.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param configName Used to specify the config name.
     * @returns The result.
     */
    getConfigurationString(configName: string): string;

    /**
     * Set the specified configuration.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param configName Used to specify the config name.
     * @param value The value to be set.
     */
    setConfigurationString(configName: string, value: string): void;

    /**
     * Get the specified configuration.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param configName Used to specify the config name.
     * @returns The result.
     */
    getConfigurationByteArray(configName: string): Uint8Array;

    /**
     * Set the specified configuration.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param configName Used to specify the config name.
     * @param value The value to be set.
     */
    setConfigurationByteArray(configName: string, value: Uint8Array): void;

    /**
     * Get performance statistics information.That includes currentSessionNum, version, decryptNumber,
     * and errorDecryptNumber.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @returns A map that includes perfaormence index and corresponding statistics.
     */
    getMetrics(): MetricKeyValue[];

    /**
     * Get security level.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @returns return SecurityLevel.
     */
    getMaxSecurityLevel(): SecurityLevel;

    /**
     * Generate the media key system provision request.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @returns return a ProvisionRequest
     */
    generateKeySystemRequest(): ProvisionRequest;

    /**
     * Process the response corresponding the key system request obtained by the application.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param response Response corresponding to the request.
     */
    processKeySystemResponse(response: Uint8Array): void;

    /**
     * Get certificate status of DRM system.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @returns Certificate Status.
     */
    getCertificateStatus():CertificateStatus;

    /**
     * Register or unregister listens for drm events.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param type Type of the drm event to listen for.
     * @param callback Callback used to listen for the key system required event.
     */
    on(type: 'provisionRequired', callback: Callback<EventInfo>): void;
    off(type: 'provisionRequired'): void;

    /**
     * Register or unregister listens for drm events.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param type Type of the drm event to listen for.
     * @param callback Callback used to listen for Session lost event.
     */
    on(type: 'sessionLost', callback: Callback<EventInfo>): void;
    off(type: 'sessionLost'): void;

    /**
     * Create a key session instance with level.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param level Used to specify the Security level.
     * @returns return a MediaKeySession instance.
     */
    createMediaKeySession(level: SecurityLevel): MediaKeySession;

    /**
     * Create a key session instance.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @returns return a MediaKeySession instance.
     */
    createMediaKeySession(): MediaKeySession;

    /**
     * Get the list of offline LicenseIds.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @returns The list of offline LicenseIds.
     */
    getOfflineLicenseIds(): Uint8Array[];

    /**
     * Get offline license status corresponding to the licenseId.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param licenseId The licenseId specifies license.
     * @returns OfflineKeyState.
     */
    getOfflineLicenseStatus(licenseId: Uint8Array): OfflineLicenseStatus;

    /**
     * Remove license corresponding to the licenseId.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param licenseId The licenseId specifies which license should be remove.
     */
    removeOfflineLicense(licenseId: Uint8Array): void;
    /**
     * Release the resource before the mediakeysystem gonna be unused.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     */
    destroy(): void;
  }

    /**
     * Provide functions and keep a decrypt module. Before calling an MediaKeySession method,
     * we must use MediaKeySystem's createMediaKeySession to get a key session instance.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     */
  interface MediaKeySession {

    /**
     * Generate the license request.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param mimeType Media type.
     * @param initData pssh after base64.
     * @param licenseType Offline or online.
     */
    generateLicenseRequest(mimeType: string, initData: Uint8Array, licenseType: number,
        optionalData: OptionalData[]): LicenseRequest;

    /**
     * Process the response corresponding the license request obtained by the application.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param response The response.
     * @returns Return licenseId.
     */
    processLicenseResponse(response: Uint8Array): Uint8Array;

    /**
     * Check the license status
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @returns License status pair.
     */
    checkLicenseStatus(): LicenseStatus[];

    /**
     * Remove license.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     */
    removeLicense(): void;

    /**
     * Generate offline license request.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param licenseId The licenseId specifies which media content's license request should be generated.
     * @returns Return the license request.
     */
    generateOfflineReleaseRequest(licenseId: Uint8Array): Uint8Array;

    /**
     * Process offline license response.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param licenseId The licenseId specifies which media content's license it is
     * @param response The offline license.
     */
    processOfflineReleaseResponse(licenseId: Uint8Array, response: Uint8Array): void;

    /**
     * Restore offline license response.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param licenseId The licenseId specifies which license should be restore.
     */
    restoreOfflineLicense(licenseId: Uint8Array): void;

    /**
     * Get security level.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @returns return SecurityLevel.
     */
    getSecurityLevel(): SecurityLevel;

    /**
     * When the decrypt content need a secure decoder, return true, otherwise return false.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param mimeType The media content type.
     * @returns Whether secure decoder is needed.
     */
    requireSecureDecoderModule(mimeType: string): boolean;

    /**
     * Register or unregister listens for drm events.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param type Type of the drm event to listen for.
     * @param callback Callback used to listen for the key required event.
     */
    on(type: 'keyNeeded', callback: Callback<EventInfo>): void;
    off(type: 'keyNeeded'): void;

    /**
     * Register or unregister listens for drm events.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param type Type of the drm event to listen for.
     * @param callback Callback used to listen for the key required event.
     */
    on(type: 'keyExpired', callback: Callback<EventInfo>): void;
    off(type: 'keyExpired'): void;

    /**
     * Register or unregister listens for drm events.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param type Type of the drm event to listen for.
     * @param callback Callback used to listen for the vendor defined event.
     */
    on(type: 'vendorDefined', callback: Callback<EventInfo>): void;
    off(type: 'vendorDefined'): void;

    /**
     * Register or unregister listens for drm events.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param type Type of the drm event to listen for.
     * @param callback Callback used to listen for expiration update event.
     */
    on(type: 'expirationUpdated', callback: Callback<EventInfo>): void;
    off(type: 'expirationUpdated'): void;

    /**
     * Register or unregister listens for drm events.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param type Type of the drm event to listen for.
     * @param callback Callback used to listen for keys change event.
     */
    on(type: 'keyChanged', callback: Callback<KeysInfo[]>): void;
    off(type: 'keyChanged'): void;

    /**
     * Release the resource before the session gonna be unused.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     */
    destroy(): void;

  }

    /**
     * Enumerates which properties we can get.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     */
  enum PreDefinedConfigName {
    CONFIG_DEVICE_VENDOR = "vendor",
    CONFIG_DEVICE_VERSION = "version",
    CONFIG_DEVICE_DESCRIPTION = "description",
    CONFIG_DEVICE_ALGORITHMS = "algorithms",
    CONFIG_DEVICE_UNIQUE_ID = "deviceUniqueId",
    CONFIG_SESSION_MAX = "maxSessionNum",
    CONFIG_SESSION_CURRENT = "currentSessionNum",
    CONFIG_OUTPUT_HDCP_MAX = "maxHDCPLevel",
    CONFIG_OUTPUT_HDCP_CURRENT = "currentHDCPLevel",
    CONFIG_OUTPUT_ADCP_MAX = "maxADCPLevel",
    CONFIG_OUTPUT_ADCP_CURRENT = "currentADCPLevel",
  }

  /**
   * Enumerates event types of listener.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   */
  enum ListenerType {
    LISTENER_DRM_EVENT = 200,
    LISTENER_PROVISION_REQUIRED = 201,
    LISTENER_KEY_NEEDED = 202,
    LISTENER_KEY_EXPIRED = 203,
    LISTENER_VENDOR_DEFINED = 204,
    LISTENER_KEYSESSION_RECLAIMED = 205,
    LISTENER_EXPIRATION_UPDATE = 206,
    LISTENER_KEY_CHANGE = 207,
    LISTENER_KEYSESSION_LOST = 208,
  }

  /**
   * Enumerates hdcp level.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   */
  enum HdcpLevel {
    HDCP_UNKNOWN,
    HDCP_NONE,
    HDCP_V1,
    HDCP_V2,
    HDCP_V2_1, 
    HDCP_V2_2,
    HDCP_V2_3,
    HDCP_NO_OUTPUT = 0x7fff,
  }

  /**
   * Enumerates adcp level.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   */
  enum AdcpLevel {
    ADCP_UNKNOWN = 0,
    ADCP_V1_L1,
    ADCP_V1_L2,
    ADCP_V1_L3,
    ADCP_NO_OUTPUT = 0x7fff,
  }

  /**
   * Enumerates license type.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   */
  enum LicenseType {
    LICENSE_TYPE_ONLINE = 0,
    LICENSE_TYPE_OFFLINE,
  }

  /**
   * Enumerates offline license status.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   */
  enum OfflineLicenseStatus {
    OFFLINELICENSE_STATUS_UNKNOWN = 0,
    OFFLINELICENSE_STATUS_USABLE = 1,
    OFFLINELICENSE_STATUS_INACTIVE = 2,
  }

  /**
   * Enumerates certificate status.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   */
  enum CertificateStatus {
    CERT_STATUS_PROVISIONED = 0,
    CERT_STATUS_NOT_PROVISIONED,
    CERT_STATUS_EXPIRED,
    CERT_STATUS_INVALID,
    CERT_STATUS_GET_FAILED,
  }

  /**
   * Enumerates license request types.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   */
  enum RequestType {
    REQUEST_TYPE_UNKNOWN = 0,
    REQUEST_TYPE_INITIAL = 1,
    REQUEST_TYPE_RENEWAL = 2,
    REQUEST_TYPE_RELEASE = 3,
    REQUEST_TYPE_NONE = 4,
    REQUEST_TYPE_UPDATE = 5,
  }

  /**
   * Enumerates security level.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   */
  enum SecurityLevel {
    SECURITY_LEVEL_UNKNOWN = 0,
    SECURITY_LEVEL_SW_CRYPTO = 1,
    SECURITY_LEVEL_SW_DECODE = 2,
    SECURITY_LEVEL_HW_CRYPTO = 3,
    SECURITY_LEVEL_HW_DECODE = 4,
    SECURITY_LEVEL_HW_ALL = 5,
    SECURITY_LEVEL_MAX = 6,
  }

  /**
   * Provides the drm provision request definations.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   */
  interface ProvisionRequest {
    mData: Uint8Array;
    mDefaultURL: string;
  }

  /**
   * Provides the drm license request info optional data.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   */
  interface OptionalData {
    name: string;
    value: string;
  }

    /**
   * Provides the drm license request definations.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   */
    interface LicenseRequest {
    licnseRequestType: RequestType;
    mData: Uint8Array;
    mDefaultURL: string;
  }

  /**
   * Used to indicates the event info.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   */
  interface EventInfo {
    name: string;
    value: string;
  }

  /**
   * Used to indicates the metrics info.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   */
  interface MetricKeyValue {
    name: string;
    value: string;
  }

  /**
   * Used to indicates the license status.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   */
  interface LicenseStatus {
    name: string;
    value: string;
  }

  /**
   * Used to indicates the license status with a key and its value.
   * @since 8
   * @syscap SystemCapability.Multimedia.Drm.Core
   */
  interface KeysInfo {
    licenseId: Uint8Array;
    value: string;
  }
}

export default drm;