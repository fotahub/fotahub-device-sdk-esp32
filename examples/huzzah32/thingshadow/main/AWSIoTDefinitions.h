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
#ifndef AWSIOTDEFINITIONS_H
#define AWSIOTDEFINITIONS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "Datagram.h"
#include "AWSCredentialsDefinitions.h"
#include "IoTDefinitions.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AWS_HEADER_SECURITY_TOKEN "X-Amz-Security-Token"

#define AWS_IOT_SERVICE "iotdata"

#define AWS_IOT_WEBSOCKET_PROTOCOL_NAME "mqtt"

#define AWS_IOT_WEBSOCKET_PATH "/mqtt"

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

#define MAX_AWS_IOT_THING_SHADOW_TOPIC_NAME_LENGTH (IOT_MAX_DEVICE_NAME_LENGTH + AWS_IOT_THING_SHADOW_TOPIC_UPDATE_LENGTH + AWS_IOT_THING_SHADOW_SUBTOPIC_ACCEPTED_LENGTH)

#define MAX_AWS_IOT_THING_SHADOW_HANDLER_COUNT 1

#define AWS_IOT_THING_SHADOW_SUBTOPICS_COUNT 5

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
