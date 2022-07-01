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
#include "MQTTClientImpl.h"

#include "PhysicalUnitConversions.h"
#include <string.h>

static int32_t getReceivedData(uint8_t *buf, int32_t count);

static uint8_t *pReceivedData;

MQTTError_t socketErrorToMQTTError(SocketError_t socketError)
{
  switch (socketError)
  {
    case SOCKET_ERROR_CONNECTION_REFUSED:
    {
      return MQTT_ERROR_CONNECTION_REFUSED;
    }
    case SOCKET_ERROR_CONNECTION_CLOSED:
    {
      return MQTT_ERROR_SOCKET_DISCONNECTED;
    }
    default: {
      return MQTT_ERROR_SOCKET_CONNECTION_FAILED;
    }
  }
}

static int32_t getReceivedData(uint8_t *buf, int32_t count)
{
  if (pReceivedData == NULL) 
  {
    return -1;
  }
  memcpy(buf, pReceivedData, ((size_t) count));
  pReceivedData += count;
  return count;
}

MQTTSessionData_t *MQTTClientImpl_newMQTTSessionData(void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  for ( int16_t __i = 0 ; __i < MAX_MQTT_SESSIONS_COUNT; __i++ )
  {
    if (!(___cid->mqttSessions__field[__i].busy)) 
    {
      ___cid->mqttSessions__field[__i].busy = true;
      ___cid->mqttSessions__field[__i].connected = false;
      ___cid->mqttSessions__field[__i].pConnDatagram = NULL;
      ___cid->mqttSessions__field[__i].pReceivedDatagram = NULL;
      ___cid->mqttSessions__field[__i].pUserData = NULL;
      ___cid->mqttSessions__field[__i].mqttClientHandler = ___cid->mqttClientHandler__ops;
      ___cid->mqttSessions__field[__i].hSocketSession = NULL;
      ___cid->mqttSessions__field[__i].nextPacketID = 0;
      return &___cid->mqttSessions__field[__i];
    }
  }
  printf("No more MQTT request data blocks available\r\n");
  return NULL;
}

bool MQTTClientImpl_isMQTTSessionDataDeleted(MQTTSessionData_t *pSessionData, void *___id)
{
  if (pSessionData == NULL) 
  {
    return true;
  }
  return !pSessionData->busy;
}

void MQTTClientImpl_deleteMQTTSessionData(MQTTSessionData_t *pSessionData, void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  if (MQTTClientImpl_isMQTTSessionDataDeleted(pSessionData, ___cid)) 
  {
    return;
  }
  if (pSessionData->pConnDatagram != NULL) 
  {
    (*___cid->socket__ops->deleteDatagram)(NULL, pSessionData->pConnDatagram, ___cid->socket__ops->__instance);
    pSessionData->pConnDatagram = NULL;
  }
  if (pSessionData->pReceivedDatagram != NULL) 
  {
    (*___cid->socket__ops->deleteDatagram)(NULL, pSessionData->pReceivedDatagram, ___cid->socket__ops->__instance);
    pSessionData->pReceivedDatagram = NULL;
  }
  pSessionData->busy = false;
}

void const* MQTTClientImpl_mqttClient_connect(MQTTConnectionParameters_t *pConnParams, void *pUserData, void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  if (pConnParams == NULL) 
  {
    return NULL;
  }
  MQTTPacket_connectData connParams = MQTTPacket_connectData_initializer;
  connParams.clientID.cstring = pConnParams->clientID;
  connParams.keepAliveInterval = pConnParams->keepAliveInterval;
  connParams.cleansession = (pConnParams->isCleanSession) ? (1) : (0);
  connParams.username.cstring = pConnParams->userName;
  connParams.password.cstring = pConnParams->password;
  
  Datagram_t *pMqttDatagram = (*___cid->socket__ops->newDatagram)(NULL, MAX_CONNECT_MESSAGE_LENGTH, PROTOCOL_SPECIFIC, ___cid->socket__ops->__instance);
  if (pMqttDatagram == NULL) 
  {
    return NULL;
  }
  
  MQTTSessionData_t *pSessionData = MQTTClientImpl_newMQTTSessionData(___cid);
  if (MQTTClientImpl_isMQTTSessionDataDeleted(pSessionData, ___cid)) 
  {
    return NULL;
  }
  pSessionData->connected = false;
  pSessionData->pConnDatagram = pMqttDatagram;
  pSessionData->pUserData = pUserData;
  pSessionData->mqttClientHandler = ___cid->mqttClientHandler__ops;
  pSessionData->hSocketSession = NULL;
  pSessionData->nextPacketID = 0;
  pSessionData->keepAliveInterval = pConnParams->keepAliveInterval;
  pSessionData->keepAliveIntervalScale = pConnParams->keepAliveIntervalScale;
  pSessionData->keepAliveTimeoutInterval = pConnParams->keepAliveTimeoutInterval;
  
  int32_t len = MQTTSerialize_connect(pSessionData->pConnDatagram->pVisiblePayload, MAX_CONNECT_MESSAGE_LENGTH, &connParams);
  
  pSessionData->pConnDatagram->visiblePayloadLength = ((size_t)(len));
  if (len == 0) 
  {
    printf("Failed to serialize MQTT CONNECT packet\r\n");
    MQTTClientImpl_deleteMQTTSessionData(pSessionData, ___cid);
    return NULL;
  }
  pSessionData->hSocketSession = (*___cid->socket__ops->connect)(pConnParams->pSocketConnectionParams, pSessionData, ___cid->socket__ops->__instance);
  
  if (pSessionData->hSocketSession == NULL) 
  {
    MQTTClientImpl_deleteMQTTSessionData(pSessionData, ___cid);
    return NULL;
  }
  return pSessionData;
}

bool MQTTClientImpl_mqttClient_isConnected(void const* hSession, void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  MQTTSessionData_t *pSessionData = ((MQTTSessionData_t *) hSession);
  if (MQTTClientImpl_isMQTTSessionDataDeleted(pSessionData, ___cid)) 
  {
    return false;
  }
  return pSessionData->connected && (*___cid->socket__ops->isConnected)(pSessionData->hSocketSession, ___cid->socket__ops->__instance);
}

bool MQTTClientImpl_mqttClient_subscribe(void const* hSession, char *topicList[], MQTTQoS_t qosList[], uint64_t const  topicCount, uint16_t subMsgId, bool duplicatedMsg, void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  MQTTSessionData_t *pSessionData = ((MQTTSessionData_t *) hSession);
  if (MQTTClientImpl_isMQTTSessionDataDeleted(pSessionData, ___cid)) 
  {
    return false;
  }
  if (___cid->keepAliveTimer__ops != NULL) 
  {
    (*___cid->keepAliveTimer__ops->cancel)(___cid->keepAliveTimer__ops->__instance);
  }
  int32_t len;
  MQTTString topicFilters[MAX_TOPICS_COUNT];
  int32_t qoss[MAX_TOPICS_COUNT];
  
  for ( int64_t __i = 0 ; __i < topicCount; __i++ )
  {
    topicFilters[__i].cstring = topicList[__i];
    topicFilters[__i].lenstring.len = 0;
    topicFilters[__i].lenstring.data = NULL;
    qoss[__i] = qosList[__i];
  }
  Datagram_t *pMqttMessage = (*___cid->socket__ops->newDatagram)(NULL, ___cid->maxSubscribeMessageLength__field, PROTOCOL_SPECIFIC, ___cid->socket__ops->__instance);
  if (pMqttMessage == NULL) 
  {
    (*___cid->mqttClientHandler__ops->connectionError)(hSession, MQTT_ERROR_PACKET_SERIALIZATION_FAILED, ___cid->mqttClientHandler__ops->__instance);
    MQTTClientImpl_deleteMQTTSessionData(pSessionData, ___cid);
    return false;
  }
  len = MQTTSerialize_subscribe(pMqttMessage->pVisiblePayload, ___cid->maxSubscribeMessageLength__field, (duplicatedMsg) ? (1) : (0), subMsgId, ((int32_t) topicCount), topicFilters, qoss);
  pMqttMessage->visiblePayloadLength = ((size_t)(len));
  if (len <= 0) 
  {
    printf("Failed to serialize MQTT SUBSCRIBE packet\r\n");
    (*___cid->mqttClientHandler__ops->connectionError)(hSession, MQTT_ERROR_PACKET_SERIALIZATION_FAILED, ___cid->mqttClientHandler__ops->__instance);
    (*___cid->socket__ops->deleteDatagram)(NULL, pMqttMessage, ___cid->socket__ops->__instance);
    MQTTClientImpl_deleteMQTTSessionData(pSessionData, ___cid);
    return false;
  }
  (*___cid->socket__ops->sendDatagram)(pSessionData->hSocketSession, pMqttMessage, NULL, ___cid->socket__ops->__instance);
  return true;
}

Datagram_t *MQTTClientImpl_mqttClient_newPublishMessage(void const* hSession, size_t payloadLength, void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  Datagram_t *pDatagram = (*___cid->socket__ops->newDatagram)(NULL, MAX_PUBLISH_HEADER_LENGTH + payloadLength, PROTOCOL_SPECIFIC, ___cid->socket__ops->__instance);
  if (pDatagram == NULL) 
  {
    return NULL;
  }
  pDatagram->pVisiblePayload += MAX_PUBLISH_HEADER_LENGTH;
  pDatagram->visiblePayloadLength -= MAX_PUBLISH_HEADER_LENGTH;
  return pDatagram;
}

bool MQTTClientImpl_mqttClient_sendPublishMessage(void const* hSession, char *topicName, Datagram_t *pMessage, MQTTPublishParameters_t *pParams, void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  MQTTSessionData_t *pSessionData = ((MQTTSessionData_t *) hSession);
  if (MQTTClientImpl_isMQTTSessionDataDeleted(pSessionData, ___cid)) 
  {
    return false;
  }
  
  if (___cid->keepAliveTimer__ops != NULL) 
  {
    (*___cid->keepAliveTimer__ops->cancel)(___cid->keepAliveTimer__ops->__instance);
  }
  
  int32_t len;
  size_t exactHeaderlength;
  uint8_t *pMessagePayload = pMessage->pVisiblePayload;
  size_t payloadLen = pMessage->visiblePayloadLength;
  MQTTString topicString = { topicName, { 0, NULL } };
  
  pMessage->pVisiblePayload -= MAX_PUBLISH_HEADER_LENGTH;
  pMessage->visiblePayloadLength += MAX_PUBLISH_HEADER_LENGTH;
  
  if (pParams->qos != MQTT_QOS_0 && !pParams->isDuplicatedMsg) 
  {
    pSessionData->nextPacketID += 1;
  }
  
  len = MQTTSerialize_publish(pMessage->pVisiblePayload, ((int32_t)(pMessage->visiblePayloadLength)), (pParams->isDuplicatedMsg) ? (1) : (0), pParams->qos, (pParams->isRetained) ? (1) : (0), pSessionData->nextPacketID, topicString, pMessagePayload, ((int32_t)(payloadLen)));
  
  /* 
   * readjust publish message length after calculating exact header length
   */
  exactHeaderlength = ((size_t)(len)) - payloadLen;
  pMessage->visiblePayloadLength -= MAX_PUBLISH_HEADER_LENGTH - exactHeaderlength;
  if (len <= 0) 
  {
    printf("Failed to serialize MQTT PUBLISH packet\r\n");
    return false;
  }
  
  (*___cid->socket__ops->sendDatagram)(pSessionData->hSocketSession, pMessage, NULL, ___cid->socket__ops->__instance);
  return true;
}

void MQTTClientImpl_mqttClient_disconnect(void const* hSession, void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  MQTTSessionData_t *pSessionData = ((MQTTSessionData_t *) hSession);
  if (MQTTClientImpl_isMQTTSessionDataDeleted(pSessionData, ___cid)) 
  {
    return;
  }
  
  /* 
   * the functions here will not be called since they prevented a socket close on a connection that experienced a half open socket error
   */
  (*___cid->socket__ops->disconnect)(pSessionData->hSocketSession, ___cid->socket__ops->__instance);
}

void *MQTTClientImpl_mqttClient_getUserData(void const* hSession, void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  MQTTSessionData_t *pSessionData = ((MQTTSessionData_t *) hSession);
  if (MQTTClientImpl_isMQTTSessionDataDeleted(pSessionData, ___cid)) 
  {
    return NULL;
  }
  
  return pSessionData->pUserData;
}

void MQTTClientImpl_clientSocketHandler_connected(void const* hSession, void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  MQTTSessionData_t *pSessionData = ((MQTTSessionData_t *)((*___cid->socket__ops->getUserData)(hSession, ___cid->socket__ops->__instance)));
  if (MQTTClientImpl_isMQTTSessionDataDeleted(pSessionData, ___cid)) 
  {
    return;
  }
  
  pSessionData->hSocketSession = hSession;
  
  (*___cid->socket__ops->sendDatagram)(hSession, pSessionData->pConnDatagram, NULL, ___cid->socket__ops->__instance);
  pSessionData->pConnDatagram = NULL;
}

void MQTTClientImpl_clientSocketHandler_datagramSent(void const* hSession, void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  if (___cid->keepAliveTimer__ops != NULL) 
  {
    MQTTSessionData_t *pSessionData = ((MQTTSessionData_t *)((*___cid->socket__ops->getUserData)(hSession, ___cid->socket__ops->__instance)));
    if (MQTTClientImpl_isMQTTSessionDataDeleted(pSessionData, ___cid)) 
    {
      return;
    }
    (*___cid->keepAliveTimer__ops->set)(((uint32_t)(s_to_ms_any(((int16_t)((pSessionData->keepAliveInterval * pSessionData->keepAliveIntervalScale)))))), true, ___cid->keepAliveTimer__ops->__instance);
  }
}

void MQTTClientImpl_clientSocketHandler_datagramReceived(void const* hSession, Datagram_t *pDatagram, void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  MQTTSessionData_t *pSessionData = ((MQTTSessionData_t *)((*___cid->socket__ops->getUserData)(hSession, ___cid->socket__ops->__instance)));
  if (MQTTClientImpl_isMQTTSessionDataDeleted(pSessionData, ___cid)) 
  {
    return;
  }
  
  if (hSession != pSessionData->hSocketSession) 
  {
    return;
  }
  
  if (pSessionData->pReceivedDatagram == NULL) 
  {
    MQTTPacketLengths_t mqttPacketLengths = { 0, 1 };
    getMQTTPacketLengths(pDatagram, &mqttPacketLengths);
    /* 
     * Allocate memory for the whole response. 1 for \0. Attention: we have to new basic datagram because we will need to compute received 
     * data length from the datagram length.
     */
    pSessionData->pReceivedDatagram = (*___cid->socket__ops->newDatagram)(hSession, mqttPacketLengths.headerLength + mqttPacketLengths.payloadLength + 1, BASIC, ___cid->socket__ops->__instance);
    if (pSessionData->pReceivedDatagram == NULL) 
    {
      printf("out of memory\r\n");
      return;
    }
    pSessionData->pReceivedDatagram->visiblePayloadLength = mqttPacketLengths.headerLength + mqttPacketLengths.payloadLength;
    
    if (pSessionData->pReceivedDatagram->visiblePayloadLength <= pDatagram->visiblePayloadLength) 
    {
      memcpy(pSessionData->pReceivedDatagram->pVisiblePayload, pDatagram->pVisiblePayload, pSessionData->pReceivedDatagram->visiblePayloadLength);
    }
    else
    {
      memcpy(pSessionData->pReceivedDatagram->pVisiblePayload, pDatagram->pVisiblePayload, pDatagram->visiblePayloadLength);
      pSessionData->pReceivedDatagram->visiblePayloadLength = pDatagram->visiblePayloadLength;
    }
  }
  else
  {
    size_t expectedDataLength;
    if (pSessionData->pReceivedDatagram->visiblePayloadLength + pDatagram->visiblePayloadLength <= pSessionData->pReceivedDatagram->length - 1) 
    {
      expectedDataLength = pDatagram->visiblePayloadLength;
    }
    else
    {
      /* 
       * Something wrong, we receive more data than expected.
       */
      expectedDataLength = pSessionData->pReceivedDatagram->length - pSessionData->pReceivedDatagram->visiblePayloadLength - 1;
    }
    memcpy(pSessionData->pReceivedDatagram->pVisiblePayload + pSessionData->pReceivedDatagram->visiblePayloadLength, pDatagram->pVisiblePayload, expectedDataLength);
    pSessionData->pReceivedDatagram->visiblePayloadLength += expectedDataLength;
  }
  
  if ((pSessionData->pReceivedDatagram->visiblePayloadLength == pSessionData->pReceivedDatagram->length - 1)) 
  {
    pSessionData->pReceivedDatagram->pVisiblePayload[pSessionData->pReceivedDatagram->visiblePayloadLength] = 0;
    pReceivedData = pSessionData->pReceivedDatagram->pVisiblePayload;
    /* 
     * TODO check MQTTPacket_read function
     */
    int32_t headerType = MQTTPacket_read(pSessionData->pReceivedDatagram->pVisiblePayload, ((int32_t)(pSessionData->pReceivedDatagram->visiblePayloadLength)), &getReceivedData);
    
    /* 
     * TODO QoS2 is not yet supported
     */
    switch (((MQTTMsgType_t)headerType))
    {
      case MQTT_MSG_CONNACK:
      {
        MQTTClientImpl_handleReceivedConnAckPacket(pSessionData, ___cid);
        break;
      }
      case MQTT_MSG_SUBACK:
      {
        MQTTClientImpl_handleReceivedSubAckPacket(pSessionData, ___cid);
        break;
      }
      case MQTT_MSG_PUBLISH:
      {
        MQTTClientImpl_handleReceivedPublishPacket(pSessionData, ___cid);
        break;
      }
      case MQTT_MSG_PUBACK:
      {
        MQTTClientImpl_handleReceivedPubAckPacket(pSessionData, ___cid);
        break;
      }
      case MQTT_MSG_PINGRESP:
      {
        MQTTClientImpl_cleanupReceivedDatagram(pSessionData, ___cid);
        if (___cid->keepAliveTimeoutTimer__ops != NULL) 
        {
          (*___cid->keepAliveTimeoutTimer__ops->cancel)(___cid->keepAliveTimeoutTimer__ops->__instance);
        }
        
        break;
      }
      case MQTT_MSG_PUBREC:
      case MQTT_MSG_PUBREL:
      case MQTT_MSG_PUBCOMP:
      {
        MQTTClientImpl_cleanupReceivedDatagram(pSessionData, ___cid);
        break;
      }
      default: {
        printf("Received MQTT message is invalid\r\n");
        (*___cid->mqttClientHandler__ops->connectionError)(pSessionData, MQTT_ERROR_INVALID_PACKET, ___cid->mqttClientHandler__ops->__instance);
        break;
      }
    }
    return;
  }
  else if (pSessionData->pReceivedDatagram->visiblePayloadLength > pSessionData->pReceivedDatagram->length - 1) {
    printf("Received MQTT message is invalid\r\n");
    (*___cid->mqttClientHandler__ops->connectionError)(pSessionData, MQTT_ERROR_INVALID_PACKET, ___cid->mqttClientHandler__ops->__instance);
  }
}

void MQTTClientImpl_clientSocketHandler_disconnected(void const* hSession, void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  MQTTSessionData_t *pSessionData = ((MQTTSessionData_t *)((*___cid->socket__ops->getUserData)(hSession, ___cid->socket__ops->__instance)));
  if (MQTTClientImpl_isMQTTSessionDataDeleted(pSessionData, ___cid)) 
  {
    return;
  }
  
  /* 
   * cancel keep alive timeout timer
   */
  if (___cid->keepAliveTimeoutTimer__ops != NULL) 
  {
    (*___cid->keepAliveTimeoutTimer__ops->cancel)(___cid->keepAliveTimeoutTimer__ops->__instance);
  }
  /* 
   * cancel keep alive timer
   */
  if (___cid->keepAliveTimer__ops != NULL) 
  {
    (*___cid->keepAliveTimer__ops->cancel)(___cid->keepAliveTimer__ops->__instance);
  }
  
  (*___cid->mqttClientHandler__ops->disconnected)(pSessionData, ___cid->mqttClientHandler__ops->__instance);
  MQTTClientImpl_deleteMQTTSessionData(pSessionData, ___cid);
}

void MQTTClientImpl_clientSocketHandler_connectionError(void const* hSession, SocketError_t error, void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  MQTTSessionData_t *pSessionData = ((MQTTSessionData_t *)((*___cid->socket__ops->getUserData)(hSession, ___cid->socket__ops->__instance)));
  if (MQTTClientImpl_isMQTTSessionDataDeleted(pSessionData, ___cid)) 
  {
    return;
  }
  
  /* 
   * cancel keep alive timeout timer
   */
  if (___cid->keepAliveTimeoutTimer__ops != NULL) 
  {
    (*___cid->keepAliveTimeoutTimer__ops->cancel)(___cid->keepAliveTimeoutTimer__ops->__instance);
  }
  /* 
   * cancel keep alive timer
   */
  if (___cid->keepAliveTimer__ops != NULL) 
  {
    (*___cid->keepAliveTimer__ops->cancel)(___cid->keepAliveTimer__ops->__instance);
  }
  
  bool wasConnected = (*___cid->socket__ops->isConnected)(pSessionData->hSocketSession, ___cid->socket__ops->__instance);
  (*___cid->mqttClientHandler__ops->connectionError)(pSessionData, socketErrorToMQTTError(error), ___cid->mqttClientHandler__ops->__instance);
  /* 
   * Remove connection data only when attempt to establish a new connection has failed. In all other cases, mqttClientHandler_disconnected() will do so
   */
  if (!wasConnected) 
  {
    MQTTClientImpl_deleteMQTTSessionData(pSessionData, ___cid);
  }
}

void MQTTClientImpl_mqttClient_deleteDatagram(void const* hSession, Datagram_t *pDatagram, void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  (*___cid->socket__ops->deleteDatagram)(hSession, pDatagram, ___cid->socket__ops->__instance);
}

Datagram_t *MQTTClientImpl_mqttClient_newDatagram(void const* hSession, size_t payloadLength, DatagramType_t type, void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  return (*___cid->socket__ops->newDatagram)(hSession, payloadLength, type, ___cid->socket__ops->__instance);
}

void MQTTClientImpl_keepAliveTimerHandler_expired(void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  /* 
   * be careful, this implementation is not complete and suppose that all sessions expires at the same time (which is not correct)
   * TODO A timer should be associated to each session and a global timer should filters the expired ones to send ping message...
   */
  for ( int16_t __i = 0 ; __i < MAX_MQTT_SESSIONS_COUNT; __i++ )
  {
    if (___cid->mqttSessions__field[__i].busy) 
    {
      printf("Issuing MQTT ping request\r\n");
      Datagram_t *pingReqMsg = (*___cid->socket__ops->newDatagram)(NULL, PINGREQ_MESSAGE_LENGTH, PROTOCOL_SPECIFIC, ___cid->socket__ops->__instance);
      size_t len = ((size_t)(MQTTSerialize_pingreq(pingReqMsg->pVisiblePayload, ((int32_t)(pingReqMsg->visiblePayloadLength)))));
      if (len < PINGREQ_MESSAGE_LENGTH) 
      {
        printf("Failed to serialize MQTT PING packet\r\n");
        (*___cid->socket__ops->deleteDatagram)(NULL, pingReqMsg, ___cid->socket__ops->__instance);
        return;
      }
      (*___cid->socket__ops->sendDatagram)(___cid->mqttSessions__field[__i].hSocketSession, pingReqMsg, NULL, ___cid->socket__ops->__instance);
      if (___cid->keepAliveTimeoutTimer__ops != NULL) 
      {
        (*___cid->keepAliveTimeoutTimer__ops->set)(((uint32_t)(s_to_ms_any(((int16_t)((___cid->mqttSessions__field[__i].keepAliveTimeoutInterval)))))), false, ___cid->keepAliveTimeoutTimer__ops->__instance);
      }
    }
  }
}

void MQTTClientImpl_keepAliveTimeoutHandler_expired(void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  /* 
   * !! Important Note !! This implementation is not complete and supposes that we support only one session at the same time (which is not correct)
   * TODO A timeout timer should be associated with each session and a global timer should filter the expired ones to report the ping error message
   */
  printf("MQTT ping request failed\r\n");
  /* 
   * cancel keep alive timer to discard sending mqtt ping message
   */
  if (___cid->keepAliveTimer__ops != NULL) 
  {
    (*___cid->keepAliveTimer__ops->cancel)(___cid->keepAliveTimer__ops->__instance);
  }
  for ( int16_t __i = 0 ; __i < MAX_MQTT_SESSIONS_COUNT; __i++ )
  {
    if (___cid->mqttSessions__field[__i].busy) 
    {
      (*___cid->mqttClientHandler__ops->connectionError)(((MQTTSessionData_t *)(&___cid->mqttSessions__field[__i])), MQTT_ERROR_PING_RESPONSE_TIMEOUT, ___cid->mqttClientHandler__ops->__instance);
    }
  }
}

void MQTTClientImpl_handleReceivedConnAckPacket(MQTTSessionData_t *pSessionData, void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  uint8_t sessionPresent;
  uint8_t connack_rc;
  if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, pSessionData->pReceivedDatagram->pVisiblePayload, ((int32_t)(pSessionData->pReceivedDatagram->visiblePayloadLength))) != 1) 
  {
    printf("Failed to deserialize MQTT CONNACK packet\r\n");
    return;
  }
  if (connack_rc != 0) 
  {
    /* 
     * TODO add MQTT error code list
     */
    (*___cid->mqttClientHandler__ops->connectionError)(pSessionData, MQTT_ERROR_CONNECTION_REFUSED, ___cid->mqttClientHandler__ops->__instance);
    MQTTClientImpl_deleteMQTTSessionData(pSessionData, ___cid);
    return;
  }
  MQTTClientImpl_cleanupReceivedDatagram(pSessionData, ___cid);
  (*___cid->mqttClientHandler__ops->connected)(pSessionData, ___cid->mqttClientHandler__ops->__instance);
  pSessionData->connected = true;
}

void MQTTClientImpl_handleReceivedSubAckPacket(MQTTSessionData_t *pSessionData, void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  uint16_t submsgid;
  int32_t subcount = 0;
  int32_t qosReturnCodes[MAX_TOPICS_COUNT];
  if (MQTTDeserialize_suback(&submsgid, MAX_TOPICS_COUNT, &subcount, qosReturnCodes, pSessionData->pReceivedDatagram->pVisiblePayload, ((int32_t)(pSessionData->pReceivedDatagram->visiblePayloadLength))) != 1) 
  {
    printf("Failed to deserialize MQTT SUBACK packet\r\n");
    (*___cid->mqttClientHandler__ops->connectionError)(pSessionData, MQTT_ERROR_PACKET_DESERIALIZATION_FAILED, ___cid->mqttClientHandler__ops->__instance);
    MQTTClientImpl_deleteMQTTSessionData(pSessionData, ___cid);
    return;
  }
  MQTTQoS_t grantedQoSs[MAX_TOPICS_COUNT];
  for ( int32_t __i = 0 ; __i < subcount; __i++ )
  {
    grantedQoSs[((uint64_t)(__i))] = ((MQTTQoS_t)qosReturnCodes[((uint64_t)(__i))]);
  }
  MQTTClientImpl_cleanupReceivedDatagram(pSessionData, ___cid);
  (*___cid->mqttClientHandler__ops->subscribed)(pSessionData, submsgid, grantedQoSs, ___cid->mqttClientHandler__ops->__instance);
}

void MQTTClientImpl_handleReceivedPublishPacket(MQTTSessionData_t *pSessionData, void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  uint8_t dup = 0;
  int32_t qos = 0;
  uint8_t retained;
  uint16_t packetID = 0;
  MQTTString topicString = 
  {
    .cstring = NULL, 
    .lenstring = 
    {
      .data = NULL, 
      .len = 0
    }
  };
  uint8_t *payload;
  int32_t payloadLen;
  if (MQTTDeserialize_publish(&dup, &qos, &retained, &packetID, &topicString, &payload, &payloadLen, pSessionData->pReceivedDatagram->pVisiblePayload, ((int32_t)(pSessionData->pReceivedDatagram->visiblePayloadLength))) != 1) 
  {
    printf("Failed to deserialize MQTT PUBLISH packet\r\n");
    (*___cid->mqttClientHandler__ops->connectionError)(pSessionData, MQTT_ERROR_PACKET_DESERIALIZATION_FAILED, ___cid->mqttClientHandler__ops->__instance);
    return;
  }
  /* 
   * narrow down visible payload
   */
  pSessionData->pReceivedDatagram->pVisiblePayload += pSessionData->pReceivedDatagram->visiblePayloadLength - payloadLen;
  pSessionData->pReceivedDatagram->visiblePayloadLength = ((size_t)(payloadLen));
  /* 
   * transmit datagram to the upper layer
   */
  (*___cid->mqttClientHandler__ops->publishMessageReceived)(pSessionData, topicString.lenstring.data, ((size_t)(topicString.lenstring.len)), pSessionData->pReceivedDatagram, ___cid->mqttClientHandler__ops->__instance);
  MQTTClientImpl_cleanupReceivedDatagram(pSessionData, ___cid);
  /* 
   * If qos 1 acknowledge the receiving of publish message
   */
  if (qos == MQTT_QOS_1) 
  {
    /* 
     * MQTT serialize pub ack
     */
    printf("Acknowledging received publish message\r\n");
    Datagram_t *pubAckmsg = (*___cid->socket__ops->newDatagram)(pSessionData, SUBACK_MESSAGE_LENGTH, PROTOCOL_SPECIFIC, ___cid->socket__ops->__instance);
    if (MQTTSerialize_ack(pubAckmsg->pVisiblePayload, ((int32_t)(pubAckmsg->visiblePayloadLength)), MQTT_MSG_PUBACK, dup, packetID) != 1) 
    {
      printf("Failed to serialize MQTT PUBACK packet\r\n");
      (*___cid->mqttClientHandler__ops->connectionError)(pSessionData, MQTT_ERROR_PACKET_SERIALIZATION_FAILED, ___cid->mqttClientHandler__ops->__instance);
      (*___cid->socket__ops->deleteDatagram)(NULL, pubAckmsg, ___cid->socket__ops->__instance);
      return;
    }
    (*___cid->socket__ops->sendDatagram)(pSessionData, pubAckmsg, NULL, ___cid->socket__ops->__instance);
  }
}

void MQTTClientImpl_handleReceivedPubAckPacket(MQTTSessionData_t *pSessionData, void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  uint8_t dup;
  uint16_t packetID;
  MQTTMsgType_t packetType;
  if (MQTTDeserialize_ack(((uint8_t *)(&packetType)), &dup, &packetID, pSessionData->pReceivedDatagram->pVisiblePayload, ((int32_t)(pSessionData->pReceivedDatagram->visiblePayloadLength))) != 1) 
  {
    printf("Failed to deserialize MQTT PUBACK packet\r\n");
    (*___cid->mqttClientHandler__ops->connectionError)(pSessionData, MQTT_ERROR_PACKET_DESERIALIZATION_FAILED, ___cid->mqttClientHandler__ops->__instance);
    return;
  }
  MQTTClientImpl_cleanupReceivedDatagram(pSessionData, ___cid);
  (*___cid->mqttClientHandler__ops->published)(pSessionData, ___cid->mqttClientHandler__ops->__instance);
}

void MQTTClientImpl_cleanupReceivedDatagram(MQTTSessionData_t *pSessionData, void *___id)
{
  MQTTClientImpl__cdata_t *___cid = ((MQTTClientImpl__cdata_t *) ___id);
  if (pSessionData != NULL && pSessionData->pReceivedDatagram != NULL) 
  {
    (*___cid->socket__ops->deleteDatagram)(NULL, pSessionData->pReceivedDatagram, ___cid->socket__ops->__instance);
    pSessionData->pReceivedDatagram = NULL;
  }
}
