/*
 *  Copyright (C) 2020-2021 FotaHub Inc. All rights reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of the FotaHub(R) Device SDK program (https://fotahub.com)
 */
#ifndef FOTAHUB_H
#define FOTAHUB_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "FOTAUpdateDefinitions.h"
#include "ConnectionManagementDefinitions.h"

#ifdef __cplusplus
extern "C" {
#endif

enum NetworkAdapterStatus {
  NETWORK_ADAPTER_STATUS_DISCONNECTED,
  NETWORK_ADAPTER_STATUS_ERROR = 1,
  NETWORK_ADAPTER_STATUS_CONNECTED = 2
};

void fotahub_init(char const *productId, char const *productName, FOTAUpdateClientConfig_t const *pUpdateClientConfig);

void fotahub_run(void);

void fotahub_notifyNetworkStatusChange(enum NetworkAdapterStatus status);

bool fotahub_downloadFirmwareUpdate(FirmwareUpdateInfo_t *pUpdateInfo);

bool fotahub_activateFirmwareUpdate(FirmwareUpdateInfo_t *pUpdateInfo);

bool fotahub_confirmFirmwareUpdate(FirmwareUpdateInfo_t *pUpdateInfo);

bool fotahub_revertFirmwareUpdate(FirmwareUpdateInfo_t *pUpdateInfo);

extern void fotahub_onConnectionStatusChanged(ConnectionStatus_t status);

extern void fotahub_onFirmwareUpdateStatusChanged(FOTAUpdateStatus_t status);

extern void fotahub_onProvideFirmwareUpdateVerificationInfo(FirmwareUpdateInfo_t const *pUpdateInfo, FOTAUpdateVerificationAlgorithm_t *pVerificationAlgorithm, uint8_t const  **ppVerificationData);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
