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
#ifndef AWSIOTTHINGSHADOWGATEIMPL_H
#define AWSIOTTHINGSHADOWGATEIMPL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "AWSIoT.h"
#include "ConnectionManagement.h"
#include "AWSIoTThingShadowDocumentDefinition.h"
#include "DemoFirmwareUpdateInfo.h"
#include "CommonServices.h"
#include "JSON.h"
#include "FOTAUpdate.h"
#include "FotaHub.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_JSON_BUFFER_LENGTH 500

enum awsThingShadowEngine__states {
  awsThingShadowEngine_idle__state = 0,
  awsThingShadowEngine_connectedToNetwork__state,
  awsThingShadowEngine_connectingToThingShadow__state,
  awsThingShadowEngine_updatingThingShadowState__state,
  awsThingShadowEngine_waitingForThingShadowDelta__state,
  awsThingShadowEngine_acknowledgingThingShadowDelta__state,
  awsThingShadowEngine_enteringFirmwareUpdateCycle_firmwareUpdateInProgress__state,
  awsThingShadowEngine_enteringFirmwareUpdateCycle_disconnectedFromThingShadow__state,
  awsThingShadowEngine_enteringFirmwareUpdateCycle_disconnectingFromThingShadow__state
};
typedef enum awsThingShadowEngine__states awsThingShadowEngine__states_t;

struct awsThingShadowEngine__data {
  awsThingShadowEngine__states_t __currentState;
};
typedef struct awsThingShadowEngine__data awsThingShadowEngine__data_t;

struct AWSIoTThingShadowGateImpl__cdata {
  /* 
   * fields
   */
  AWSIoTConnectionParameters_t *connectionParameters__field;
  /* 
   * fields
   */
  const void* awsIoTThingShadowSession__field;
  /* 
   * fields
   */
  bool updateAvailable__field;
  /* 
   * fields
   */
  awsThingShadowEngine__data_t thingShadowEngine__field;
  /* 
   * required ports
   */
  IDemoFirmwareUpdateVersionInfo__idata_t *firmwareUpdateVersionInfo__port;
  /* 
   * Req port ops
   */
  IJSONDocumentIOHandler__idata_t *jsonDocumentIOHandler__ops;
  /* 
   * Req port ops
   */
  IAWSIoTThingShadow__idata_t *thingShadow__ops;
  /* 
   * Req port ops
   */
  IJSONParser__idata_t *jsonParser__ops;
  /* 
   * Req port ops
   */
  IJSONSerializer__idata_t *jsonSerializer__ops;
};
typedef struct AWSIoTThingShadowGateImpl__cdata AWSIoTThingShadowGateImpl__cdata_t;

enum awsThingShadowEngine__inevents {
  awsThingShadowEngine_epsilon__event,
  awsThingShadowEngine_connectionStatusChanged__event,
  awsThingShadowEngine_connectedToThingShadow__event,
  awsThingShadowEngine_disconnectedFromThingShadow__event,
  awsThingShadowEngine_thingShadowStatus__event,
  awsThingShadowEngine_thingShadowDelta__event,
  awsThingShadowEngine_fotaUpdateStatus__event
};
typedef enum awsThingShadowEngine__inevents awsThingShadowEngine__inevents_t;

void AWSIoTThingShadowGateImpl_init(void *___id);

void AWSIoTThingShadowGateImpl_awsThingShadowEngine__init(awsThingShadowEngine__data_t *instance, void *___id);

bool AWSIoTThingShadowGateImpl_awsThingShadowEngine__execute(awsThingShadowEngine__data_t *instance, awsThingShadowEngine__inevents_t event, void *arguments[], void *___id);

void AWSIoTThingShadowGateImpl_thingShadowHandler_connectionError(const void* hSession, AWSIoTError_t error, void *___id);

void AWSIoTThingShadowGateImpl_runnable_run(void *___id);

bool AWSIoTThingShadowGateImpl_updateThingShadowState(void *___id);

bool AWSIoTThingShadowGateImpl_handleThingShadowDelta(char const *jsonString, size_t const jsonStringLength, void *___id);

void AWSIoTThingShadowGateImpl_evaluateUpdateInfo(void *___id);

void AWSIoTThingShadowGateImpl_connectionMonitorHandler_statusChanged(ConnectionStatus_t status, void *___id);

void AWSIoTThingShadowGateImpl_thingShadowHandler_connected(const void* hSession, void *___id);

void AWSIoTThingShadowGateImpl_thingShadowHandler_disconnected(const void* hSession, void *___id);

void AWSIoTThingShadowGateImpl_thingShadowHandler_status(const void* hSession, char const *thingName, AWSIoTThingShadowAction_t action, AWSIoTThingShadowAckStatus_t status, char *jsonString, size_t jsonStringLength, void *___id);

void AWSIoTThingShadowGateImpl_thingShadowHandler_delta(const void* hSession, char const *thingName, char *jsonString, size_t jsonStringLength, void *___id);

void AWSIoTThingShadowGateImpl_fotaUpdateStatusHandler_updateStatus(FOTAUpdateStatus_t status, void *___id);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
