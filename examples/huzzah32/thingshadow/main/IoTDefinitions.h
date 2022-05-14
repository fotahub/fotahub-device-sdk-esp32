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
#ifndef IOTDEFINITIONS_H
#define IOTDEFINITIONS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MQTT_MAX_KEEP_ALIVE_VALUE 1180

#define MQTT_KEEP_ALIVE_VALUE_SCALE 1.2f

#define MQTT_MAX_KEEP_ALIVE_TIMEOUT_VALUE 5

#define IOT_MAX_DEVICE_NAME_LENGTH 36

enum IoTAction {
  IOT_ACTION_GET,
  IOT_ACTION_REPORT,
  IOT_ACTION_DESIRED,
  IOT_ACTION_DELETE,
  IOT_ACTION_INTERNAL
};
typedef enum IoTAction IoTAction_t;

enum IoTResponseStatus {
  IOT_RESPONSE_STATUS_TIMEOUT,
  IOT_RESPONSE_STATUS_REJECTED,
  IOT_RESPONSE_STATUS_ACCEPTED,
  IOT_RESPONSE_STATUS_NONE
};
typedef enum IoTResponseStatus IoTResponseStatus_t;

enum IoTSubTopicKind {
  IOT_SUBTOPIC_KIND_NONE,
  IOT_SUBTOPIC_KIND_ACCEPTED,
  IOT_SUBTOPIC_KIND_REJECTED,
  IOT_SUBTOPIC_KIND_DOCUMENTS,
  IOT_SUBTOPIC_KIND_DELTA
};
typedef enum IoTSubTopicKind IoTSubTopicKind_t;

enum IoTQoS {
  IOT_QOS_0,
  IOT_QOS_1
};
typedef enum IoTQoS IoTQoS_t;

enum IoTError {
  IOT_ERROR_OUT_OF_MEMORY,
  IOT_ERROR_MQTT_CONNECTION,
  IOT_ERROR_MQTT_DISCONNECTED,
  IOT_ERROR_MQTT_TOPIC_SUBSCRIPTION,
  IOT_ERROR_MQTT_CONNECTION_PARAMETERS,
  IOT_ERROR_SESSION_DATA_DELETED,
  IOT_ERROR_NET_CONNECT_FAILED,
  IOT_ERROR_FETCHING_CREDENTIAL_FAILED
};
typedef enum IoTError IoTError_t;

struct IoTConnectionParameters {
  char *deviceName;
  IoTQoS_t qos;
  uint8_t timeout;
  void *pDownstreamConnParams;
  void *pCredentials;
};
typedef struct IoTConnectionParameters IoTConnectionParameters_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
