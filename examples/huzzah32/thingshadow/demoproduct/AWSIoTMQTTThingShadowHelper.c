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
#include "AWSIoTMQTTThingShadowHelper.h"

#include "StringHelper.h"
#include <string.h>

void buildAWSIoTThingShadowTopicName(char *topicName, char const *thingName, AWSIoTThingShadowAction_t action, AWSIoTThingShadowSubTopicKind_t subTopicKind)
{
  size_t length = strlen(thingName) - 3;
  
  if (action == AWS_IOT_THING_SHADOW_ACTION_UPDATE) 
  {
    length += AWS_IOT_THING_SHADOW_TOPIC_UPDATE_LENGTH;
    switch (subTopicKind)
    {
      case AWS_IOT_THING_SHADOW_SUBTOPIC_KIND_ACCEPTED:
      {
        length += AWS_IOT_THING_SHADOW_SUBTOPIC_ACCEPTED_LENGTH;
        snprintf(topicName, length, AWS_IOT_THING_SHADOW_TOPIC_UPDATE_PATTERN, thingName, AWS_IOT_THING_SHADOW_SUBTOPIC_ACCEPTED);
        break;
      }
      case AWS_IOT_THING_SHADOW_SUBTOPIC_KIND_REJECTED:
      {
        length += AWS_IOT_THING_SHADOW_SUBTOPIC_REJECTED_LENGTH;
        snprintf(topicName, length, AWS_IOT_THING_SHADOW_TOPIC_UPDATE_PATTERN, thingName, AWS_IOT_THING_SHADOW_SUBTOPIC_REJECTED);
        break;
      }
      case AWS_IOT_THING_SHADOW_SUBTOPIC_KIND_DELTA:
      {
        length += AWS_IOT_THING_SHADOW_SUBTOPIC_DELTA_LENGTH;
        snprintf(topicName, length, AWS_IOT_THING_SHADOW_TOPIC_UPDATE_PATTERN, thingName, AWS_IOT_THING_SHADOW_SUBTOPIC_DELTA);
        break;
      }
      default: {
        snprintf(topicName, length, AWS_IOT_THING_SHADOW_TOPIC_UPDATE_PATTERN, thingName, AWS_IOT_THING_SHADOW_SUBTOPIC_NONE);
        break;
      }
    }
  }
  else if (action == AWS_IOT_THING_SHADOW_ACTION_GET) {
    length += AWS_IOT_THING_SHADOW_TOPIC_GET_LENGTH;
    switch (subTopicKind)
    {
      case AWS_IOT_THING_SHADOW_SUBTOPIC_KIND_ACCEPTED:
      {
        length += AWS_IOT_THING_SHADOW_SUBTOPIC_ACCEPTED_LENGTH;
        snprintf(topicName, length, AWS_IOT_THING_SHADOW_TOPIC_GET_PATTERN, thingName, AWS_IOT_THING_SHADOW_SUBTOPIC_ACCEPTED);
        break;
      }
      case AWS_IOT_THING_SHADOW_SUBTOPIC_KIND_REJECTED:
      {
        length += AWS_IOT_THING_SHADOW_SUBTOPIC_ACCEPTED_LENGTH;
        snprintf(topicName, length, AWS_IOT_THING_SHADOW_TOPIC_GET_PATTERN, thingName, AWS_IOT_THING_SHADOW_SUBTOPIC_REJECTED);
        break;
      }
      default: {
        snprintf(topicName, length, AWS_IOT_THING_SHADOW_TOPIC_GET_PATTERN, thingName, AWS_IOT_THING_SHADOW_SUBTOPIC_NONE);
        break;
      }
    }
  }
  
  topicName[length] = '\0';
}

AWSIoTThingShadowAction_t getAWSIoTThingShadowActionfromTopicName(char *topicName, size_t topicNameLen)
{
  if ((topicName == NULL) || (topicNameLen == 0)) 
  {
    return AWS_IOT_THING_SHADOW_ACTION_INTERNAL;
  }
  
  if (StringHelper_strnstr(topicName, topicNameLen, AWS_IOT_THING_SHADOW_TOPIC_UPDATE) != NULL) 
  {
    return AWS_IOT_THING_SHADOW_ACTION_UPDATE;
  }
  if (StringHelper_strnstr(topicName, topicNameLen, AWS_IOT_THING_SHADOW_TOPIC_GET) != NULL) 
  {
    return AWS_IOT_THING_SHADOW_ACTION_GET;
  }
  
  return AWS_IOT_THING_SHADOW_ACTION_INTERNAL;
}

AWSIoTThingShadowSubTopicKind_t getAWSIoTThingShadowSubTopicfromTopicName(char *topicName, size_t topicNameLen)
{
  if ((topicName == NULL) || (topicNameLen == 0)) 
  {
    return AWS_IOT_THING_SHADOW_SUBTOPIC_KIND_NONE;
  }
  
  if (StringHelper_strnstr(topicName, topicNameLen, AWS_IOT_THING_SHADOW_SUBTOPIC_ACCEPTED) != NULL) 
  {
    return AWS_IOT_THING_SHADOW_SUBTOPIC_KIND_ACCEPTED;
  }
  if (StringHelper_strnstr(topicName, topicNameLen, AWS_IOT_THING_SHADOW_SUBTOPIC_REJECTED) != NULL) 
  {
    return AWS_IOT_THING_SHADOW_SUBTOPIC_KIND_REJECTED;
  }
  if (StringHelper_strnstr(topicName, topicNameLen, AWS_IOT_THING_SHADOW_SUBTOPIC_DELTA) != NULL) 
  {
    return AWS_IOT_THING_SHADOW_SUBTOPIC_KIND_DELTA;
  }
  return AWS_IOT_THING_SHADOW_SUBTOPIC_KIND_NONE;
}

AWSIoTThingShadowAckStatus_t subTopicToAWSIoTThingShadowAckStatus(AWSIoTThingShadowSubTopicKind_t subTopic)
{
  switch (subTopic)
  {
    case AWS_IOT_THING_SHADOW_SUBTOPIC_KIND_ACCEPTED:
    {
      return AWS_IOT_THING_SHADOW_ACK_STATUS_ACCEPTED;
    }
    case AWS_IOT_THING_SHADOW_SUBTOPIC_KIND_REJECTED:
    {
      return AWS_IOT_THING_SHADOW_ACK_STATUS_REJECTED;
    }
    default: {
      return AWS_IOT_THING_SHADOW_ACK_STATUS_NONE;
    }
  }
}

AWSIoTError_t mqttToAWSIoTError(MQTTError_t mqttError)
{
  switch (mqttError)
  {
    case MQTT_ERROR_SOCKET_DISCONNECTED:
    {
      return AWS_IOT_ERROR_MQTT_DISCONNECTED;
    }
    case MQTT_ERROR_SOCKET_CONNECTION_FAILED:
    {
      return AWS_IOT_ERROR_NET_CONNECT_FAILED;
    }
    case MQTT_ERROR_SESSION_DATA_DELETED:
    {
      return AWS_IOT_ERROR_SESSION_DATA_DELETED;
    }
    case MQTT_ERROR_PACKET_SERIALIZATION_FAILED:
    {
      return AWS_IOT_ERROR_MQTT_THING_SHADOW_TOPIC_SUBSCRIPTION;
    }
    default: {
      printf("MQTT error socket connection FAILED: %d\n", mqttError);
      return AWS_IOT_ERROR_MQTT_CONNECTION;
    }
  }
}

char *awsIoTThingShadowActionToString(AWSIoTThingShadowAction_t action)
{
  switch (action)
  {
    case AWS_IOT_THING_SHADOW_ACTION_GET:
    {
      return "ThingShadow Get";
    }
    case AWS_IOT_THING_SHADOW_ACTION_UPDATE:
    {
      return "ThingShadow Update";
    }
    case AWS_IOT_THING_SHADOW_ACTION_DELETE:
    {
      return "ThingShadow Delete";
    }
    case AWS_IOT_THING_SHADOW_ACTION_INTERNAL:
    {
      return "ThingShadow None";
    }
    default: {
      return "Unknown";
    }
  }
}

char *awsIoTThingShadowAckStatusToString(AWSIoTThingShadowAckStatus_t status)
{
  switch (status)
  {
    case AWS_IOT_THING_SHADOW_ACK_STATUS_TIMEOUT:
    {
      return "Timeout";
    }
    case AWS_IOT_THING_SHADOW_ACK_STATUS_REJECTED:
    {
      return "Rejected";
    }
    case AWS_IOT_THING_SHADOW_ACK_STATUS_ACCEPTED:
    {
      return "Accepted";
    }
    case AWS_IOT_THING_SHADOW_ACK_STATUS_NONE:
    {
      return "None";
    }
    default: {
      return "Unknown";
    }
  }
}
