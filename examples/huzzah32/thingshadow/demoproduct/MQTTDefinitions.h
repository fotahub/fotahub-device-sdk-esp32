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
#ifndef MQTTDEFINITIONS_H
#define MQTTDEFINITIONS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "SocketDefinitions.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_CONNECT_MESSAGE_LENGTH 100

#define DISCONNECT_MESSAGE_LENGTH 2

#define SUBACK_MESSAGE_LENGTH 4

#define PINGREQ_MESSAGE_LENGTH 2

#define DEFAULT_SUBSCRIBE_MESSAGE_LENGTH 500

#define MAX_PUBLISH_HEADER_LENGTH 100

#define MAX_TOPICS_COUNT 6

enum MQTTMsgType {
  MQTT_MSG_CONNECT = 1,
  MQTT_MSG_CONNACK,
  MQTT_MSG_PUBLISH,
  MQTT_MSG_PUBACK,
  MQTT_MSG_PUBREC,
  MQTT_MSG_PUBREL,
  MQTT_MSG_PUBCOMP,
  MQTT_MSG_SUBSCRIBE,
  MQTT_MSG_SUBACK,
  MQTT_MSG_UNSUBSCRIBE,
  MQTT_MSG_UNSUBACK,
  MQTT_MSG_PINGREQ,
  MQTT_MSG_PINGRESP,
  MQTT_MSG_DISCONNECT
};
typedef enum MQTTMsgType MQTTMsgType_t;

enum MQTTError {
  MQTT_ERROR_CONNECTION_REFUSED,
  MQTT_ERROR_SOCKET_CONNECTION_FAILED,
  MQTT_ERROR_SOCKET_DISCONNECTED,
  MQTT_ERROR_SESSION_DATA_DELETED,
  MQTT_ERROR_INVALID_PACKET,
  MQTT_ERROR_PACKET_SERIALIZATION_FAILED,
  MQTT_ERROR_PACKET_DESERIALIZATION_FAILED,
  MQTT_ERROR_PING_RESPONSE_TIMEOUT
};
typedef enum MQTTError MQTTError_t;

enum MQTTQoS {
  MQTT_QOS_0 = 0x00u,
  MQTT_QOS_1 = 0x01u,
  MQTT_QOS_2 = 0x02u,
/* 
   * MQTT_QOS_FAILURE is only used as return code in SUBACK packet
   */
    MQTT_QOS_FAILURE = 0x80u
};
typedef enum MQTTQoS MQTTQoS_t;

struct MQTTConnectionParameters {
  char *clientID;
  char *userName;
  char *password;
  bool isCleanSession;
  uint16_t keepAliveInterval;
  float keepAliveIntervalScale;
  uint16_t keepAliveTimeoutInterval;
  SocketConnectionParameters_t *pSocketConnectionParams;
};
typedef struct MQTTConnectionParameters MQTTConnectionParameters_t;

struct MQTTPublishParameters {
  MQTTQoS_t qos;
  bool isRetained;
  bool isDuplicatedMsg;
};
typedef struct MQTTPublishParameters MQTTPublishParameters_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
