/*
* Copyright (C) 2021 Huawei Device Co., Ltd.
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

/**
 * @name drm
 * @since 8
 */
declare namespace drm {
    /**
     * Creates a Mediakeysystem instance.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param uuid Used to point a Digital Right Managements solution.
     * @param callback Callback used to return the Mediakeysystem instance.
     */
    function getMediaKeySystem(uuid: string, callback: AsyncCallback<MediaKeySystem>): void;

    /**
     * Creates a MediaKeySystem instance.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     * @param uuid Used to point a Digital Right Managements solution.
     * @returns Promise used to return the MediaKeySystem instance.
     */
    function getMediaKeySystem(uuid: string): Promise<MediaKeySystem>;

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
     * Manages and record key sessions. Before calling an MediaKeySystem method, we must use getMediaKeySystem
     * to get a MediaKeySystem instance, then we can call functions.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     */
    interface MediaKeySystem {

        /**
         * Get the specified configuration.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @param configType Used to specify the config type.
         * @param propertyName Used to specify the property name.
         * @returns The result.
         */
        getConfiguration(configType: ConfigType, propertyName: PropertyName): string;

        /**
         * Set the specified configuration.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @param configType Used to specify the config type.
         * @param propertyName Used to specify the property name.
         * @param value The value to be set.
         */
        setConfiguration(configType: ConfigType, propertyName: PropertyName, value: string): void;

        /**
         * Generate the media key system provision request.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @param type Used to specify the request type.
         * @returns Promise used to return a DrmRequest
         */
        generateKeySystemRequest(type: RequestType): Promise<DrmRequest>;

        /**
         * Process the response corresponding the key system request obtained by the application.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @param type Used to specify the request type.
         * @param response Response corresponding to the request.
         */
        processKeySystemResponse(type: RequestType, response: Uint8Array): Promise<void>;

        /**
         * Create a key session instance.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @param level Used to specify the Security level.
         * @returns Promise used to return a KeySession instance.
         */
        createKeySession(level: SecurityLevel): Promise<KeySession>;

        /**
         * Get performance statistics information.That includes currentSessionNum, version, decryptNumber,
         * and errorDecryptNumber.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @returns A map that includes perfaormence index and corresponding statistics.
         */
        getMetric(): {[key: string]: string};

        /**
         * Get security level.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @returns Promise used to return SecurityLevel.
         */
        getSecurityLevel(): Promise<SecurityLevel>;
    }

    /**
     * Provide functions and keep a decrypt module. Before calling an KeySession method, we must use MediaKeySystem's
     * createKeySession to get a key session instance.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     */
    interface KeySession {

        /**
         * Generate the license request.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @param mimeType Media type.
         * @param initData pssh after base64.
         * @param keyType Offline or online.
         */
        generateLicenseRequest(mimeType: string, initData: Uint8Array, keyType: number): Promise<DrmRequest>;

        /**
         * Process the response corresponding the license request obtained by the application.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @param response The response.
         * @returns Return keyID.
         */
        processLicenseResponse(response: Uint8Array): Promise<string>;

        /**
         * Check the license status
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @returns License status pair.
         */
        checkLicenseStatus(): Promise<KeyValue[]>;

        /**
         * Release the resource before the session gonna be unused.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         */
        release(): void;

        /**
         * Remove license.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         */
        removeLicenses(): void;

        /**
         * Generate offline license request.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @param keyId The keyId specifies which media content's license request should be generated.
         * @returns Return the license request.
         */
        generateOfflineReleaseRequest(keyId: Uint8Array): Promise<Uint8Array>;

        /**
         * Process offline license response.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @param keyId The keyId specifies which media content's license it is
         * @param response The offline license.
         */
        processOfflineReleaseResponse(keyId: Uint8Array, response: Uint8Array): void;

        /**
         * Restore offline license response.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @param keyId The keyId specifies which license should be restore.
         */
        restoreOfflineKeys(keyId: Uint8Array): void;

        /**
         * Get the list of offline keyIds.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @returns The list of offline keyIds.
         */
        getOfflineKeyIds(): Uint8Array[];

        /**
         * Remove license corresponding to the keyId.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @param keyId The keyId specifies which license should be remove.
         */
        removeOfflineKeys(keyId: Uint8Array): void;

        /**
         * Get offline license status corresponding to the keyId.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @param keyId The keyId specifies license.
         * @returns OfflineKeyState.
         */
        getOfflineKeyState(keyId: Uint8Array): Promise<OfflineKeyState>;

        /**
         * Get decrypt module.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @returns Return the media decrypt module instance.
         */
        getDecryptModule(): Promise<MediaDecryptModule>;

        /**
         * Register or unregister listens for drm events.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @param type Type of the drm event to listen for.
         * @param callback Callback used to listen for the key system required event.
         */
        on(type: 'keySystemRequired', callback: Callback<number>): void;
        off(type: 'keySystemRequired'): void;

        /**
         * Register or unregister listens for drm events.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @param type Type of the drm event to listen for.
         * @param callback Callback used to listen for the key required event.
         */
        on(type: 'keyRequired', callback: Callback<number>): void;
        off(type: 'keyRequired'): void;

        /**
         * Register or unregister listens for drm events.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @param type Type of the drm event to listen for.
         * @param callback Callback used to listen for the key required event.
         */
        on(type: 'keyExpired', callback: Callback<number>): void;
        off(type: 'keyExpired'): void;

        /**
         * Register or unregister listens for drm events.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @param type Type of the drm event to listen for.
         * @param callback Callback used to listen for the vendor defined event.
         */
        on(type: 'vendorDefined', callback: Callback<number>): void;
        off(type: 'vendorDefined'): void;

        /**
         * Register or unregister listens for drm events.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @param type Type of the drm event to listen for.
         * @param callback Callback used to listen for keySession reclaimed event.
         */
        on(type: 'keySessionReclaimed', callback: Callback<number>): void;
        off(type: 'keySessionReclaimed'): void;
    }

    /**
     * Provide decrypt functions. Before we gonna use a decrypt module, we shoud call getDecryptModule.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     */
    interface MediaDecryptModule {

        /**
         * When the decrypt content need a secure decoder, return true, otherwise return false.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @param mimeType The media content type.
         * @returns Whether secure decoder is needed.
         */
        requireSecureDecoderModule(mimeType: string): boolean;

        /**
         * Decrypt content.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         * @param secureDecoderStatus Secure decoder status.
         * @param cryptInfo Crypt info.
         * @param srcBuffer Source buffer.
         * @param dstBuffer Destination buffer.
         */
        decryptData(secureDecoderStatus: boolean, cryptInfo: CryptInfo, srcBuffer: number,
            dstBuffer: number): Promise<void>;

        /**
         * Release the resource that a decrypt module owned. When a decrypt module will be unused,
         * we should call this function.
         * @since 8
         * @syscap SystemCapability.Multimedia.Drm.Core
         */
        release(): void;
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
        LISTENER_KEYSESSION_LOSE = 208,
    }

    /**
     * Enumerates which properties we can get.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     */
    enum PropertyName {
        PROPERTY_DEVICE_VENDOR = "vendor",
        PROPERTY_DEVICE_VERSION = "version",
        PROPERTY_DEVICE_DESCRIPTION = "description",
        PROPERTY_DEVICE_ALGORITHMS = "algorithms",
        PROPERTY_DEVICE_UNIQUE_ID = "deviceUniqueId",
        PROPERTY_SESSION_MAX = "maxSessionNum",
        PROPERTY_SESSION_CURRENT = "currentSessionNum",
        PROPERTY_OUTPUT_HDCP_MAX = "maxHDCPLevel",
        PROPERTY_OUTPUT_HDCP_CURRENT = "currentHDCPLevel",
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
     * Enumerates which configurations.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     */
    enum ConfigType {
        CONFIGTYPE_DEVICEPROPERTY = 0,
        CONFIGTYPE_KEYSESSION = 1,
        CONFIGTYPE_OUTPUTPROTECTTYPE = 2,
    }

    /**
     * Enumerates license type.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     */
    enum KeyType {
        KEYTYPE_OFFLINE = 0,
        KEYTYPE_ONLINE,
    }

    /**
     * Enumerates offline license state.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     */
    enum OfflineKeyState {
        OFFLINEKEYSTATE_UNKNOWN = 0,
        OFFLINEKEYSTATE_USABLE = 1,               
        OFFLINEKEYSTATE_INACTIVE = 2,
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
        REQUEST_TYPE_DOWNLOADCERT = 6,
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
     * Provides the drm request definations.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     */
    interface DrmRequest {
        keyRequestType: RequestType;
        mData: Uint8Array;
        mDefaultURL: string;
    }

    /**
     * Used to indicates the license status with a key and its value.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     */
    interface KeyValue {
        name: string;
        value: string;
    }

    /**
     * Used to indicates the length of clear bytes and decrypt length in a subsample.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     */
    interface SubSample {
        clearHeaderLen: number;
        payLoadLen: number;
    }

    /**
     * Decryption pattern.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     */
    interface Pattern {
        encryptBlocks: number;
        skipBlocks: number;
    }

    /**
     * Enumerates encryption Algorithm types.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     */
    enum CryptAlgorithmType {
        ALGTYPE_UNENCRYPTED = 0,
        ALGTYPE_AES_CTR = 1,
        ALGTYPE_AES_WV = 2,
        ALGTYPE_AES_CBC = 3,
        ALGTYPE_SM4_CBC = 4,
    }

    /**
     * Provides the defination of encryption info.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     */
    interface CryptInfo {
        type: CryptAlgorithmType;
        keyId: Uint8Array;
        iv: Uint8Array;
        pattern: Pattern;
        subSample: SubSample[];
    }

    /**
     * Provides the defination of decryption data buffer.
     * @since 8
     * @syscap SystemCapability.Multimedia.Drm.Core
     */
    interface DecryptDataBuffer {
        bufferLen: number;
        buffer: ArrayBuffer;
    }
}

export default drm;