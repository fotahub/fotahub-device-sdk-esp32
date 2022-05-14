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
#ifndef AWSIOTMQTTTHINGSHADOWHELPER_H
#define AWSIOTMQTTTHINGSHADOWHELPER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "AWSIoTDefinitions.h"
#include "MQTTDefinitions.h"

#ifdef __cplusplus
extern "C" {
#endif

void buildAWSIoTThingShadowTopicName(char *topicName, char const *thingName, IoTAction_t action, IoTSubTopicKind_t subTopicKind);

IoTAction_t getAWSIoTThingShadowActionfromTopicName(char *topicName, size_t topicNameLen);

IoTSubTopicKind_t getAWSIoTThingShadowSubTopicfromTopicName(char *topicName, size_t topicNameLen);

IoTResponseStatus_t subTopicToAWSIoTThingShadowAckStatus(IoTSubTopicKind_t subTopic);

char *awsIoTThingShadowActionToString(IoTAction_t action);

char *awsIoTThingShadowAckStatusToString(IoTResponseStatus_t status);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
