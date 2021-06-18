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
#ifndef AWSIOTDEFINITIONS_H
#define AWSIOTDEFINITIONS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "Datagram.h"
#include "AWSCredentialsDefinitions.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AWS_HEADER_SECURITY_TOKEN "X-Amz-Security-Token"

#define AWS_IOT_SERVICE "iotdata"

#define AWS_IOT_WEBSOCKET_PROTOCOL_NAME "mqtt"

#define AWS_IOT_WEBSOCKET_PATH "/mqtt"

#define MAX_AWS_MQTT_KEEP_ALIVE_VALUE 1180

#define AWS_MQTT_KEEP_ALIVE_VALUE_SCALE 1.2f

#define MAX_AWS_MQTT_KEEP_ALIVE_TIMEOUT_VALUE 5

#define MAX_AWS_IOT_THING_NAME_LENGTH 36

#define AWS_IOT_THING_SHADOW_JSON_STATE "state"

#define AWS_IOT_THING_SHADOW_JSON_REPORTED "reported"

#define AWS_IOT_THING_SHADOW_JSON_DESIRED "desired"

#define AWS_IOT_THING_SHADOW_JSON_DELTA "delta"

#define AWS_IOT_THING_SHADOW_JSON_CLIENT_TOKEN "clientToken"

#define AWS_IOT_THING_SHADOW_JSON_METADATA "metadata"

#define AWS_IOT_THING_SHADOW_JSON_VERSION "version"

#define AWS_IOT_THING_SHADOW_JSON_TIMESTAMP "timestamp"

#define AWS_IOT_THING_SHADOW_JSON_REPORTED_PATH "/state/reported"

#define AWS_IOT_THING_SHADOW_JSON_DESIRED_PATH "/state/desired"

#define AWS_IOT_THING_SHADOW_TOPIC_UPDATE "update"

#define AWS_IOT_THING_SHADOW_TOPIC_UPDATE_PATTERN "$aws/things/%s/shadow/update%s"

#define AWS_IOT_THING_SHADOW_TOPIC_UPDATE_LENGTH 30

#define AWS_IOT_THING_SHADOW_TOPIC_GET "get"

#define AWS_IOT_THING_SHADOW_TOPIC_GET_PATTERN "$aws/things/%s/shadow/get%s"

#define AWS_IOT_THING_SHADOW_TOPIC_GET_LENGTH 27

#define AWS_IOT_THING_SHADOW_SUBTOPIC_NONE ""

#define AWS_IOT_THING_SHADOW_SUBTOPIC_ACCEPTED "/accepted"

#define AWS_IOT_THING_SHADOW_SUBTOPIC_ACCEPTED_LENGTH 9

#define AWS_IOT_THING_SHADOW_SUBTOPIC_REJECTED "/rejected"

#define AWS_IOT_THING_SHADOW_SUBTOPIC_REJECTED_LENGTH 9

#define AWS_IOT_THING_SHADOW_SUBTOPIC_DELTA "/delta"

#define AWS_IOT_THING_SHADOW_SUBTOPIC_DELTA_LENGTH 6

#define AWS_IOT_THING_SHADOW_REST_PATH_PATTERN "/things/%s/shadow"

#define MAX_AWS_IOT_THING_SHADOW_TOPIC_NAME_LENGTH (MAX_AWS_IOT_THING_NAME_LENGTH + AWS_IOT_THING_SHADOW_TOPIC_UPDATE_LENGTH + AWS_IOT_THING_SHADOW_SUBTOPIC_ACCEPTED_LENGTH)

#define MAX_AWS_IOT_THING_SHADOW_HANDLER_COUNT 1

#define AWS_IOT_THING_SHADOW_SUBTOPICS_COUNT 5

enum AWSIoTThingShadowAction {
  AWS_IOT_THING_SHADOW_ACTION_GET,
  AWS_IOT_THING_SHADOW_ACTION_UPDATE,
  AWS_IOT_THING_SHADOW_ACTION_DELETE,
  AWS_IOT_THING_SHADOW_ACTION_INTERNAL
};
typedef enum AWSIoTThingShadowAction AWSIoTThingShadowAction_t;

enum AWSIoTThingShadowAckStatus {
  AWS_IOT_THING_SHADOW_ACK_STATUS_TIMEOUT,
  AWS_IOT_THING_SHADOW_ACK_STATUS_REJECTED,
  AWS_IOT_THING_SHADOW_ACK_STATUS_ACCEPTED,
  AWS_IOT_THING_SHADOW_ACK_STATUS_NONE
};
typedef enum AWSIoTThingShadowAckStatus AWSIoTThingShadowAckStatus_t;

enum AWSIoTThingShadowSubTopicKind {
  AWS_IOT_THING_SHADOW_SUBTOPIC_KIND_NONE,
  AWS_IOT_THING_SHADOW_SUBTOPIC_KIND_ACCEPTED,
  AWS_IOT_THING_SHADOW_SUBTOPIC_KIND_REJECTED,
  AWS_IOT_THING_SHADOW_SUBTOPIC_KIND_DOCUMENTS,
  AWS_IOT_THING_SHADOW_SUBTOPIC_KIND_DELTA
};
typedef enum AWSIoTThingShadowSubTopicKind AWSIoTThingShadowSubTopicKind_t;

enum AWSIoTQoS {
  AWS_IOT_QOS_0,
  AWS_IOT_QOS_1
};
typedef enum AWSIoTQoS AWSIoTQoS_t;

enum AWSIoTError {
  AWS_IOT_ERROR_OUT_OF_MEMORY,
  AWS_IOT_ERROR_MQTT_CONNECTION,
  AWS_IOT_ERROR_MQTT_DISCONNECTED,
  AWS_IOT_ERROR_MQTT_THING_SHADOW_TOPIC_SUBSCRIPTION,
  AWS_IOT_ERROR_MQTT_CONNECTION_PARAMETERS,
  AWS_IOT_ERROR_SESSION_DATA_DELETED,
  AWS_IOT_ERROR_NET_CONNECT_FAILED,
  AWS_IOT_ERROR_FETCHING_CREDENTIAL_FAILED
};
typedef enum AWSIoTError AWSIoTError_t;

struct AWSIoTConnectionParameters {
  char *thingName;
  AWSIoTQoS_t qos;
  uint8_t timeout;
  void *pDownstreamConnParams;
  AWSTemporarySecurityCredentials_t *pCredentials;
};
typedef struct AWSIoTConnectionParameters AWSIoTConnectionParameters_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
