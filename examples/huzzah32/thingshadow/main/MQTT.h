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
  void const* ((*(connect))(MQTTConnectionParameters_t *,void *,void *));
  bool ((*(isConnected))(void const*,void *));
  void *((*(getUserData))(void const*,void *));
  Datagram_t *((*(newDatagram))(void const*,size_t,DatagramType_t,void *));
  Datagram_t *((*(newPublishMessage))(void const*,size_t,void *));
  bool ((*(subscribe))(void const*,char *[],MQTTQoS_t [],uint64_t const ,uint16_t,bool,void *));
  bool ((*(sendPublishMessage))(void const*,char *,Datagram_t *,MQTTPublishParameters_t *,void *));
  void ((*(deleteDatagram))(void const*,Datagram_t *,void *));
  void ((*(disconnect))(void const*,void *));
};
typedef struct IMQTTClient__idata IMQTTClient__idata_t;

struct IMQTTClientHandler__idata {
  void *__instance;
  void ((*(connected))(void const*,void *));
  void ((*(subscribed))(void const*,uint16_t,MQTTQoS_t [],void *));
  void ((*(published))(void const*,void *));
  void ((*(publishMessageReceived))(void const*,char *,size_t,Datagram_t *,void *));
  void ((*(disconnected))(void const*,void *));
  void ((*(connectionError))(void const*,MQTTError_t,void *));
};
typedef struct IMQTTClientHandler__idata IMQTTClientHandler__idata_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
