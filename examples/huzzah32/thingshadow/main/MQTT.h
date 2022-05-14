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
#ifndef MQTT_H
#define MQTT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "Datagram.h"
#include "MQTTDefinitions.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * FIXME Why do we need the newDatagram() operation?
 */
struct IMQTTClient__idata {
  void *__instance;
  const void* ((*(connect))(MQTTConnectionParameters_t *,void *,void *));
  bool ((*(isConnected))(const void*,void *));
  void *((*(getUserData))(const void*,void *));
  Datagram_t *((*(newDatagram))(const void*,size_t,DatagramType_t,void *));
  Datagram_t *((*(newPublishMessage))(const void*,size_t,void *));
  bool ((*(subscribe))(const void*,char *[],MQTTQoS_t [],uint64_t const ,uint16_t,bool,void *));
  bool ((*(sendPublishMessage))(const void*,char *,Datagram_t *,MQTTPublishParameters_t *,void *));
  void ((*(deleteDatagram))(const void*,Datagram_t *,void *));
  void ((*(disconnect))(const void*,void *));
};
typedef struct IMQTTClient__idata IMQTTClient__idata_t;

struct IMQTTClientHandler__idata {
  void *__instance;
  void ((*(connected))(const void*,void *));
  void ((*(subscribed))(const void*,uint16_t,MQTTQoS_t [],void *));
  void ((*(published))(const void*,void *));
  void ((*(publishMessageReceived))(const void*,char *,size_t,Datagram_t *,void *));
  void ((*(disconnected))(const void*,void *));
  void ((*(connectionError))(const void*,MQTTError_t,void *));
};
typedef struct IMQTTClientHandler__idata IMQTTClientHandler__idata_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
