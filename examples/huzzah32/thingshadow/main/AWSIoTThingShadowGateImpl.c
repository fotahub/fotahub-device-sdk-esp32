/*
 *  Copyright (C) 2022 FotaHub Inc. All rights reserved.
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
#include "AWSIoTThingShadowGateImpl.h"

#include "DemoFOTAUpdateWorkflow.h"
#include "AWSIoTMQTTThingShadowHelper.h"
#include "BinHexConverter.h"
#include "DemoProductInfo.h"
#include <stdlib.h>
#include <string.h>

void AWSIoTThingShadowGateImpl_init(void *___id)
{
  AWSIoTThingShadowGateImpl__cdata_t *___cid = ((AWSIoTThingShadowGateImpl__cdata_t *) ___id);
  AWSIoTThingShadowGateImpl_awsThingShadowEngine__init(&___cid->thingShadowEngine__field, ___cid);
}

void AWSIoTThingShadowGateImpl_awsThingShadowEngine__init(awsThingShadowEngine__data_t *instance, void *___id)
{
  instance->__currentState = awsThingShadowEngine_idle__state;
}

bool AWSIoTThingShadowGateImpl_awsThingShadowEngine__execute(awsThingShadowEngine__data_t *instance, awsThingShadowEngine__inevents_t event, void *arguments[], void *___id)
{
  AWSIoTThingShadowGateImpl__cdata_t *___cid = ((AWSIoTThingShadowGateImpl__cdata_t *) ___id);
  switch (instance->__currentState)
  {
    case awsThingShadowEngine_idle__state:
    {
      switch (event)
      {
        case awsThingShadowEngine_connectionStatusChanged__event:
        {
          if ((*((ConnectionStatus_t *)((arguments[0])))) >= CONNECTION_STATUS_CONNECTIVITY_AVAILABLE) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = awsThingShadowEngine_connectedToNetwork__state;
          }
          break;
        }
        case awsThingShadowEngine_fotaUpdateStatus__event:
        {
          /* 
           * transition actions
           */
          updateStatus.value = (*((FOTAUpdateStatus_t *)((arguments[0]))));
          
          /* 
           * enter target state
           */
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    case awsThingShadowEngine_connectedToNetwork__state:
    {
      switch (event)
      {
        case awsThingShadowEngine_epsilon__event:
        {
          /* 
           * transition actions
           */
          printf("Connecting to AWS IoT Thing Shadow\r\n");
          ___cid->awsIoTThingShadowSession__field = (*___cid->thingShadow__ops->connect)(___cid->connectionParameters__field, NULL, ___cid->thingShadow__ops->__instance);
          
          /* 
           * enter target state
           */
          instance->__currentState = awsThingShadowEngine_connectingToThingShadow__state;
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    case awsThingShadowEngine_connectingToThingShadow__state:
    {
      switch (event)
      {
        case awsThingShadowEngine_connectedToThingShadow__event:
        {
          /* 
           * transition actions
           */
          bool __transitionResult = true;
          
          printf("Connected to AWS IoT Thing Shadow\r\n");
          ___cid->awsIoTThingShadowSession__field = (*((void const **)((arguments[0]))));
          if (!AWSIoTThingShadowGateImpl_updateThingShadowState(___cid)) 
          {
            __transitionResult = false;
          }
          
          if (instance->__currentState == awsThingShadowEngine_connectingToThingShadow__state) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = awsThingShadowEngine_updatingThingShadowState__state;
          }
          
          if (!__transitionResult) 
          {
            return false;
          }
          break;
        }
        case awsThingShadowEngine_connectionStatusChanged__event:
        {
          if ((*((ConnectionStatus_t *)((arguments[0])))) < CONNECTION_STATUS_CONNECTIVITY_AVAILABLE) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = awsThingShadowEngine_idle__state;
          }
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    case awsThingShadowEngine_updatingThingShadowState__state:
    {
      switch (event)
      {
        case awsThingShadowEngine_connectionStatusChanged__event:
        {
          if ((*((ConnectionStatus_t *)((arguments[0])))) < CONNECTION_STATUS_CONNECTIVITY_AVAILABLE) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = awsThingShadowEngine_idle__state;
          }
          break;
        }
        case awsThingShadowEngine_disconnectedFromThingShadow__event:
        {
          /* 
           * enter target state
           */
          instance->__currentState = awsThingShadowEngine_connectedToNetwork__state;
          break;
        }
        case awsThingShadowEngine_thingShadowStatus__event:
        {
          if ((*((IoTResponseStatus_t *)((arguments[3])))) == IOT_RESPONSE_STATUS_ACCEPTED) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = awsThingShadowEngine_waitingForThingShadowDelta__state;
            break;
          }
          if ((*((IoTResponseStatus_t *)((arguments[3])))) != IOT_RESPONSE_STATUS_ACCEPTED) 
          {
            /* 
             * transition actions
             */
            bool __transitionResult = true;
            
            if (!AWSIoTThingShadowGateImpl_updateThingShadowState(___cid)) 
            {
              __transitionResult = false;
            }
            
            if (instance->__currentState == awsThingShadowEngine_updatingThingShadowState__state) 
            {
              /* 
               * enter target state
               */
            }
            
            if (!__transitionResult) 
            {
              return false;
            }
          }
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    case awsThingShadowEngine_waitingForThingShadowDelta__state:
    {
      switch (event)
      {
        case awsThingShadowEngine_connectionStatusChanged__event:
        {
          if ((*((ConnectionStatus_t *)((arguments[0])))) < CONNECTION_STATUS_CONNECTIVITY_AVAILABLE) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = awsThingShadowEngine_idle__state;
          }
          break;
        }
        case awsThingShadowEngine_disconnectedFromThingShadow__event:
        {
          /* 
           * enter target state
           */
          instance->__currentState = awsThingShadowEngine_connectedToNetwork__state;
          break;
        }
        case awsThingShadowEngine_thingShadowDelta__event:
        {
          /* 
           * transition actions
           */
          bool __transitionResult = true;
          awsThingShadowEngine__states_t __targetState = awsThingShadowEngine_acknowledgingThingShadowDelta__state;
          
          if (!AWSIoTThingShadowGateImpl_handleThingShadowDelta((*((char **)((arguments[2])))), (*((size_t *)((arguments[3])))), ___cid)) 
          {
            __targetState = awsThingShadowEngine_waitingForThingShadowDelta__state;
            __transitionResult = false;
          }
          
          if (instance->__currentState == awsThingShadowEngine_waitingForThingShadowDelta__state) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = __targetState;
          }
          
          if (!__transitionResult) 
          {
            return false;
          }
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    case awsThingShadowEngine_acknowledgingThingShadowDelta__state:
    {
      switch (event)
      {
        case awsThingShadowEngine_connectionStatusChanged__event:
        {
          if ((*((ConnectionStatus_t *)((arguments[0])))) < CONNECTION_STATUS_CONNECTIVITY_AVAILABLE) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = awsThingShadowEngine_idle__state;
          }
          break;
        }
        case awsThingShadowEngine_disconnectedFromThingShadow__event:
        {
          /* 
           * enter target state
           */
          instance->__currentState = awsThingShadowEngine_connectedToNetwork__state;
          break;
        }
        case awsThingShadowEngine_thingShadowStatus__event:
        {
          if (!___cid->updateAvailable__field) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = awsThingShadowEngine_waitingForThingShadowDelta__state;
            break;
          }
          if (___cid->updateAvailable__field) 
          {
            /* 
             * transition actions
             */
            printf("Disconnecting from AWS IoT Thing Shadow\r\n");
            (*___cid->thingShadow__ops->disconnect)(___cid->awsIoTThingShadowSession__field, ___cid->thingShadow__ops->__instance);
            
            if (instance->__currentState == awsThingShadowEngine_acknowledgingThingShadowDelta__state) 
            {
              /* 
               * enter target state
               */
              instance->__currentState = awsThingShadowEngine_enteringFirmwareUpdateCycle_disconnectingFromThingShadow__state;
            }
          }
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    case awsThingShadowEngine_enteringFirmwareUpdateCycle_firmwareUpdateInProgress__state:
    {
      switch (event)
      {
        case awsThingShadowEngine_connectionStatusChanged__event:
        {
          if ((*((ConnectionStatus_t *)((arguments[0])))) < CONNECTION_STATUS_CONNECTIVITY_AVAILABLE) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = awsThingShadowEngine_idle__state;
          }
          break;
        }
        case awsThingShadowEngine_fotaUpdateStatus__event:
        {
          if ((*((FOTAUpdateStatus_t *)((arguments[0])))) != FOTA_UPDATE_STATUS_DOWNLOAD_SUCCEEDED) 
          {
            /* 
             * transition actions
             */
            updateStatus.value = (*((FOTAUpdateStatus_t *)((arguments[0]))));
            
            /* 
             * enter target state
             */
            instance->__currentState = awsThingShadowEngine_connectedToNetwork__state;
          }
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    case awsThingShadowEngine_enteringFirmwareUpdateCycle_disconnectedFromThingShadow__state:
    {
      switch (event)
      {
        case awsThingShadowEngine_connectionStatusChanged__event:
        {
          if ((*((ConnectionStatus_t *)((arguments[0])))) < CONNECTION_STATUS_CONNECTIVITY_AVAILABLE) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = awsThingShadowEngine_idle__state;
          }
          break;
        }
        case awsThingShadowEngine_epsilon__event:
        {
          /* 
           * transition actions
           */
          fotaUpdateWorkflow_onFirmwareUpdateVersionChanged(___cid->firmwareUpdateVersionInfo__port->updateVersion);
          
          if (instance->__currentState == awsThingShadowEngine_enteringFirmwareUpdateCycle_disconnectedFromThingShadow__state) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = awsThingShadowEngine_enteringFirmwareUpdateCycle_firmwareUpdateInProgress__state;
          }
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    case awsThingShadowEngine_enteringFirmwareUpdateCycle_disconnectingFromThingShadow__state:
    {
      switch (event)
      {
        case awsThingShadowEngine_connectionStatusChanged__event:
        {
          if ((*((ConnectionStatus_t *)((arguments[0])))) < CONNECTION_STATUS_CONNECTIVITY_AVAILABLE) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = awsThingShadowEngine_idle__state;
          }
          break;
        }
        case awsThingShadowEngine_disconnectedFromThingShadow__event:
        {
          /* 
           * transition actions
           */
          ___cid->awsIoTThingShadowSession__field = NULL;
          
          /* 
           * enter target state
           */
          instance->__currentState = awsThingShadowEngine_enteringFirmwareUpdateCycle_disconnectedFromThingShadow__state;
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    default: {
      break;
    }
  }
  return true;
}

void AWSIoTThingShadowGateImpl_thingShadowHandler_connectionError(void const* hSession, IoTError_t error, void *___id)
{
  /* 
   * Do nothing
   */
}

void AWSIoTThingShadowGateImpl_runnable_run(void *___id)
{
  AWSIoTThingShadowGateImpl__cdata_t *___cid = ((AWSIoTThingShadowGateImpl__cdata_t *) ___id);
  AWSIoTThingShadowGateImpl_awsThingShadowEngine__execute(&___cid->thingShadowEngine__field, awsThingShadowEngine_epsilon__event, NULL, ___cid);
}

bool AWSIoTThingShadowGateImpl_updateThingShadowState(void *___id)
{
  AWSIoTThingShadowGateImpl__cdata_t *___cid = ((AWSIoTThingShadowGateImpl__cdata_t *) ___id);
  /* 
   * Only update updateCurrentVersion thing shadow attribute
   */
  (*___cid->jsonParser__ops->resetFlags)(((struct jsontree_value *)(&requestStateDoc)), ___cid->jsonParser__ops->__instance);
  
  requestStateDoc.modified = true;
  stateObj.modified = true;
  attributeUpdateStateObj.modified = true;
  updateCurrentVersion.modified = true;
  updateStatus.modified = true;
  
  char jsonDocument[MAX_JSON_BUFFER_LENGTH + 1] = { 0 };
  if (!(*___cid->jsonSerializer__ops->serialize)(((struct jsontree_value *)(&requestStateDoc)), MODE_MODIFIED_ATTRIBUTES, ___cid->jsonDocumentIOHandler__ops, NULL, jsonDocument, sizeof(jsonDocument), ___cid->jsonSerializer__ops->__instance)) 
  {
    return false;
  }
  
  (*___cid->thingShadow__ops->report)(___cid->awsIoTThingShadowSession__field, jsonDocument, ___cid->thingShadow__ops->__instance);
  
  return true;
}

bool AWSIoTThingShadowGateImpl_handleThingShadowDelta(char const *jsonString, size_t const jsonStringLength, void *___id)
{
  AWSIoTThingShadowGateImpl__cdata_t *___cid = ((AWSIoTThingShadowGateImpl__cdata_t *) ___id);
  /* 
   * Process thing shadow delta
   */
  (*___cid->jsonParser__ops->resetFlags)(((struct jsontree_value *)(&deltaStateDoc)), ___cid->jsonParser__ops->__instance);
  if (!(*___cid->jsonParser__ops->parse)(((struct jsontree_value *)(&deltaStateDoc)), ___cid->jsonDocumentIOHandler__ops, NULL, ((char *) jsonString), jsonStringLength, ___cid->jsonParser__ops->__instance)) 
  {
    return false;
  }
  AWSIoTThingShadowGateImpl_evaluateUpdateInfo(___cid);
  
  /* 
   * Update all thing shadow attributes
   */
  char jsonDocument[MAX_JSON_BUFFER_LENGTH + 1] = { 0 };
  (*___cid->jsonParser__ops->resetFlags)(((struct jsontree_value *)(&requestStateDoc)), ___cid->jsonParser__ops->__instance);
  void const* hDocument = (*___cid->jsonSerializer__ops->beginDocument)(((struct jsontree_value *)(&requestStateDoc)), NULL, NULL, jsonDocument, sizeof(jsonDocument), ___cid->jsonSerializer__ops->__instance);
  if (hDocument == NULL) 
  {
    return false;
  }
  bool result = (*___cid->jsonSerializer__ops->addValue)(hDocument, AWS_IOT_THING_SHADOW_JSON_DESIRED_PATH, MODE_ALL_ATTRIBUTES, ___cid->jsonSerializer__ops->__instance);
  result = result && (*___cid->jsonSerializer__ops->endDocument)(hDocument, ___cid->jsonSerializer__ops->__instance);
  if (!result) 
  {
    return false;
  }
  (*___cid->thingShadow__ops->report)(___cid->awsIoTThingShadowSession__field, jsonDocument, ___cid->thingShadow__ops->__instance);
  
  return true;
}

void AWSIoTThingShadowGateImpl_evaluateUpdateInfo(void *___id)
{
  AWSIoTThingShadowGateImpl__cdata_t *___cid = ((AWSIoTThingShadowGateImpl__cdata_t *) ___id);
  ___cid->updateAvailable__field = false;
  if (strlen(___cid->firmwareUpdateVersionInfo__port->updateVersion) > 0) 
  {
    if (strcmp(___cid->firmwareUpdateVersionInfo__port->updateVersion, DEMO_PRODUCT_FIRMWARE_VERSION) != 0) 
    {
      printf("Firmware update request towards version %s received\r\n", ___cid->firmwareUpdateVersionInfo__port->updateVersion);
      ___cid->updateAvailable__field = true;
    }
    else
    {
      printf("Ignoring FOTA update request to version %s as this version is already running\r\n", ___cid->firmwareUpdateVersionInfo__port->updateVersion);
    }
  }
}

void AWSIoTThingShadowGateImpl_connectionMonitorHandler_statusChanged(ConnectionStatus_t status, void *___id)
{
  AWSIoTThingShadowGateImpl__cdata_t *___cid = ((AWSIoTThingShadowGateImpl__cdata_t *) ___id);
  ConnectionStatus_t ___awsThingShadowEngine_connectionStatusChanged_status__arg = status;
  void *___awsThingShadowEngine_connectionStatusChanged__args[1] = { &___awsThingShadowEngine_connectionStatusChanged_status__arg };
  AWSIoTThingShadowGateImpl_awsThingShadowEngine__execute(&___cid->thingShadowEngine__field, awsThingShadowEngine_connectionStatusChanged__event, ___awsThingShadowEngine_connectionStatusChanged__args, ___cid);
}

void AWSIoTThingShadowGateImpl_thingShadowHandler_connected(void const* hSession, void *___id)
{
  AWSIoTThingShadowGateImpl__cdata_t *___cid = ((AWSIoTThingShadowGateImpl__cdata_t *) ___id);
  void const* ___awsThingShadowEngine_connectedToThingShadow_session__arg = hSession;
  void *___awsThingShadowEngine_connectedToThingShadow__args[1] = { &___awsThingShadowEngine_connectedToThingShadow_session__arg };
  AWSIoTThingShadowGateImpl_awsThingShadowEngine__execute(&___cid->thingShadowEngine__field, awsThingShadowEngine_connectedToThingShadow__event, ___awsThingShadowEngine_connectedToThingShadow__args, ___cid);
}

void AWSIoTThingShadowGateImpl_thingShadowHandler_disconnected(void const* hSession, void *___id)
{
  AWSIoTThingShadowGateImpl__cdata_t *___cid = ((AWSIoTThingShadowGateImpl__cdata_t *) ___id);
  void const* ___awsThingShadowEngine_disconnectedFromThingShadow_session__arg = hSession;
  void *___awsThingShadowEngine_disconnectedFromThingShadow__args[1] = { &___awsThingShadowEngine_disconnectedFromThingShadow_session__arg };
  AWSIoTThingShadowGateImpl_awsThingShadowEngine__execute(&___cid->thingShadowEngine__field, awsThingShadowEngine_disconnectedFromThingShadow__event, ___awsThingShadowEngine_disconnectedFromThingShadow__args, ___cid);
}

void AWSIoTThingShadowGateImpl_thingShadowHandler_status(void const* hSession, char const *thingName, IoTAction_t action, IoTResponseStatus_t status, char *jsonString, size_t jsonStringLength, void *___id)
{
  AWSIoTThingShadowGateImpl__cdata_t *___cid = ((AWSIoTThingShadowGateImpl__cdata_t *) ___id);
  void const* ___awsThingShadowEngine_thingShadowStatus_session__arg = hSession;
  char const *___awsThingShadowEngine_thingShadowStatus_thingName__arg = thingName;
  IoTAction_t ___awsThingShadowEngine_thingShadowStatus_action__arg = action;
  IoTResponseStatus_t ___awsThingShadowEngine_thingShadowStatus_status__arg = status;
  char *___awsThingShadowEngine_thingShadowStatus_jsonString__arg = jsonString;
  size_t ___awsThingShadowEngine_thingShadowStatus_jsonStringLength__arg = jsonStringLength;
  void *___awsThingShadowEngine_thingShadowStatus__args[6] = { &___awsThingShadowEngine_thingShadowStatus_session__arg, &___awsThingShadowEngine_thingShadowStatus_thingName__arg, &___awsThingShadowEngine_thingShadowStatus_action__arg, &___awsThingShadowEngine_thingShadowStatus_status__arg, &___awsThingShadowEngine_thingShadowStatus_jsonString__arg, &___awsThingShadowEngine_thingShadowStatus_jsonStringLength__arg };
  AWSIoTThingShadowGateImpl_awsThingShadowEngine__execute(&___cid->thingShadowEngine__field, awsThingShadowEngine_thingShadowStatus__event, ___awsThingShadowEngine_thingShadowStatus__args, ___cid);
}

void AWSIoTThingShadowGateImpl_thingShadowHandler_desired(void const* hSession, char const *thingName, char *jsonString, size_t jsonStringLength, void *___id)
{
  AWSIoTThingShadowGateImpl__cdata_t *___cid = ((AWSIoTThingShadowGateImpl__cdata_t *) ___id);
  void const* ___awsThingShadowEngine_thingShadowDelta_session__arg = hSession;
  char const *___awsThingShadowEngine_thingShadowDelta_thingName__arg = thingName;
  char *___awsThingShadowEngine_thingShadowDelta_jsonString__arg = jsonString;
  size_t ___awsThingShadowEngine_thingShadowDelta_jsonStringLength__arg = jsonStringLength;
  void *___awsThingShadowEngine_thingShadowDelta__args[4] = { &___awsThingShadowEngine_thingShadowDelta_session__arg, &___awsThingShadowEngine_thingShadowDelta_thingName__arg, &___awsThingShadowEngine_thingShadowDelta_jsonString__arg, &___awsThingShadowEngine_thingShadowDelta_jsonStringLength__arg };
  AWSIoTThingShadowGateImpl_awsThingShadowEngine__execute(&___cid->thingShadowEngine__field, awsThingShadowEngine_thingShadowDelta__event, ___awsThingShadowEngine_thingShadowDelta__args, ___cid);
}

void AWSIoTThingShadowGateImpl_fotaUpdateStatusHandler_updateStatus(FOTAUpdateStatus_t status, void *___id)
{
  AWSIoTThingShadowGateImpl__cdata_t *___cid = ((AWSIoTThingShadowGateImpl__cdata_t *) ___id);
  FOTAUpdateStatus_t ___awsThingShadowEngine_fotaUpdateStatus_status__arg = status;
  void *___awsThingShadowEngine_fotaUpdateStatus__args[1] = { &___awsThingShadowEngine_fotaUpdateStatus_status__arg };
  AWSIoTThingShadowGateImpl_awsThingShadowEngine__execute(&___cid->thingShadowEngine__field, awsThingShadowEngine_fotaUpdateStatus__event, ___awsThingShadowEngine_fotaUpdateStatus__args, ___cid);
}
