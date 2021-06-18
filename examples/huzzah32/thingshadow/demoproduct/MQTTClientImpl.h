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
#ifndef MQTTCLIENTIMPL_H
#define MQTTCLIENTIMPL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "MQTT.h"
#include "Socket.h"
#include "SocketDefinitions.h"
#include "Timer.h"
#include "MQTTPacketExtensions.h"
#include "DatagramHelper.h"
#include <MQTTPacket.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_MQTT_SESSIONS_COUNT 3

#define MQTT_PING_RESPONSE_TIMEOUT 5

struct MQTTSessionData {
  Datagram_t *pConnDatagram;
  Datagram_t *pReceivedDatagram;
  bool busy;
  bool connected;
  void *pUserData;
  IMQTTClientHandler__idata_t *mqttClientHandler;
  const void* hSocketSession;
  uint8_t nextPacketID;
  uint16_t keepAliveInterval;
  float keepAliveIntervalScale;
  uint16_t keepAliveTimeoutInterval;
};
typedef struct MQTTSessionData MQTTSessionData_t;

struct MQTTClientImpl__cdata {
  /* 
   * fields
   */
  uint16_t maxSubscribeMessageLength__field;
  /* 
   * fields
   */
  MQTTSessionData_t mqttSessions__field[MAX_MQTT_SESSIONS_COUNT];
  /* 
   * Req port ops
   */
  IClientSocket__idata_t *socket__ops;
  /* 
   * Req port ops
   */
  IMQTTClientHandler__idata_t *mqttClientHandler__ops;
  /* 
   * Req port ops
   */
  ITimer__idata_t *keepAliveTimer__ops;
  /* 
   * Req port ops
   */
  ITimer__idata_t *keepAliveTimeoutTimer__ops;
};
typedef struct MQTTClientImpl__cdata MQTTClientImpl__cdata_t;

MQTTError_t socketErrorToMQTTError(SocketError_t socketError);

MQTTSessionData_t *MQTTClientImpl_newMQTTSessionData(void *___id);

bool MQTTClientImpl_isMQTTSessionDataDeleted(MQTTSessionData_t *pSessionData, void *___id);

void MQTTClientImpl_deleteMQTTSessionData(MQTTSessionData_t *pSessionData, void *___id);

const void* MQTTClientImpl_mqttClient_connect(MQTTConnectionParameters_t *pConnParams, void *pUserData, void *___id);

bool MQTTClientImpl_mqttClient_isConnected(const void* hSession, void *___id);

bool MQTTClientImpl_mqttClient_subscribe(const void* hSession, char *topicList[], MQTTQoS_t qosList[], uint64_t const  topicCount, uint16_t subMsgId, bool duplicatedMsg, void *___id);

Datagram_t *MQTTClientImpl_mqttClient_newPublishMessage(const void* hSession, size_t payloadLength, void *___id);

bool MQTTClientImpl_mqttClient_sendPublishMessage(const void* hSession, char *topicName, Datagram_t *pMessage, MQTTPublishParameters_t *pParams, void *___id);

void MQTTClientImpl_mqttClient_disconnect(const void* hSession, void *___id);

void *MQTTClientImpl_mqttClient_getUserData(const void* hSession, void *___id);

void MQTTClientImpl_clientSocketHandler_connected(const void* hSession, void *___id);

void MQTTClientImpl_clientSocketHandler_datagramSent(const void* hSession, void *___id);

void MQTTClientImpl_clientSocketHandler_datagramReceived(const void* hSession, Datagram_t *pDatagram, void *___id);

void MQTTClientImpl_clientSocketHandler_disconnected(const void* hSession, void *___id);

void MQTTClientImpl_clientSocketHandler_connectionError(const void* hSession, SocketError_t error, void *___id);

void MQTTClientImpl_mqttClient_deleteDatagram(const void* hSession, Datagram_t *pDatagram, void *___id);

Datagram_t *MQTTClientImpl_mqttClient_newDatagram(const void* hSession, size_t payloadLength, DatagramType_t type, void *___id);

void MQTTClientImpl_keepAliveTimerHandler_expired(void *___id);

void MQTTClientImpl_keepAliveTimeoutHandler_expired(void *___id);

void MQTTClientImpl_handleReceivedConnAckPacket(MQTTSessionData_t *pSessionData, void *___id);

void MQTTClientImpl_handleReceivedSubAckPacket(MQTTSessionData_t *pSessionData, void *___id);

void MQTTClientImpl_handleReceivedPublishPacket(MQTTSessionData_t *pSessionData, void *___id);

void MQTTClientImpl_handleReceivedPubAckPacket(MQTTSessionData_t *pSessionData, void *___id);

void MQTTClientImpl_cleanupReceivedDatagram(MQTTSessionData_t *pSessionData, void *___id);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
