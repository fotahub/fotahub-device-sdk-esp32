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
#ifndef ABSTRACTAWSIOTMQTTTHINGSHADOWIMPL_H
#define ABSTRACTAWSIOTMQTTTHINGSHADOWIMPL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "Timer.h"
#include "IoT.h"
#include "MQTT.h"
#include "AWSIoTDefinitions.h"
#include "AWSCredentialsDefinitions.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_AWS_IOT_MQTT_REQUEST_COUNT 2

struct AWSIoTSessionData {
  bool busy;
  const void* hDownstreamSession;
  void *pUserData;
  Datagram_t *pPublishMessage;
  IoTAction_t action;
  IoTConnectionParameters_t *pConnectionParams;
};
typedef struct AWSIoTSessionData AWSIoTSessionData_t;

struct AbstractAWSIoTMQTTThingShadowImpl__cdata {
  /* 
   * fields
   */
  AWSIoTSessionData_t awsIoTRequests__field[MAX_AWS_IOT_MQTT_REQUEST_COUNT];
  /* 
   * Req port ops
   */
  IIoTClientHandler__idata_t *thingShadowHandlers__ops[MAX_AWS_IOT_THING_SHADOW_HANDLER_COUNT];
  /* 
   * Req port ops
   */
  IMQTTClient__idata_t *mqttClient__ops;
  /* 
   * Req port ops
   */
  ITimer__idata_t *timeoutTimer__ops;
  /* 
   * Abstract runnables
   */
  const void* ((*(thingShadow_connect__abstract))(IoTConnectionParameters_t *,void *,void *));
};
typedef struct AbstractAWSIoTMQTTThingShadowImpl__cdata AbstractAWSIoTMQTTThingShadowImpl__cdata_t;

AWSIoTSessionData_t *AbstractAWSIoTMQTTThingShadowImpl_newAWSIoTData(void *___id);

bool AbstractAWSIoTMQTTThingShadowImpl_isAWSIoTDataDeleted(AWSIoTSessionData_t *pSessionData, void *___id);

void AbstractAWSIoTMQTTThingShadowImpl_deleteAWSIoTData(AWSIoTSessionData_t *pSessionData, void *___id);

bool AbstractAWSIoTMQTTThingShadowImpl_thingShadow_report(const void* hSession, char *jsonString, void *___id);

bool AbstractAWSIoTMQTTThingShadowImpl_thingShadow_get(const void* hSession, void *___id);

bool AbstractAWSIoTMQTTThingShadowImpl_thingShadow_isConnected(const void* hSession, void *___id);

void AbstractAWSIoTMQTTThingShadowImpl_thingShadow_disconnect(const void* hSession, void *___id);

void *AbstractAWSIoTMQTTThingShadowImpl_thingShadow_getUserData(const void* hSession, void *___id);

void AbstractAWSIoTMQTTThingShadowImpl_mqttClientHandler_connected(const void* hSession, void *___id);

void AbstractAWSIoTMQTTThingShadowImpl_mqttClientHandler_subscribed(const void* hSession, uint16_t subMsgId, MQTTQoS_t grantedQoS[], void *___id);

void AbstractAWSIoTMQTTThingShadowImpl_mqttClientHandler_published(const void* hSession, void *___id);

void AbstractAWSIoTMQTTThingShadowImpl_mqttClientHandler_publishMessageReceived(const void* hSession, char *topicName, size_t topicNameLen, Datagram_t *pDatagram, void *___id);

void AbstractAWSIoTMQTTThingShadowImpl_mqttClientHandler_disconnected(const void* hSession, void *___id);

void AbstractAWSIoTMQTTThingShadowImpl_mqttClientHandler_connectionError(const void* hSession, MQTTError_t error, void *___id);

void AbstractAWSIoTMQTTThingShadowImpl_timeoutHandler_expired(void *___id);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
