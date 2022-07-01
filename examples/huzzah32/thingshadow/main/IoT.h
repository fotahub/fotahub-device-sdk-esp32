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
#ifndef IOT_H
#define IOT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "IoTDefinitions.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * TODO Add newJSONDocument()/deleteJSONDocument methods that allocate, return and act upon a datagram payload
 */
struct IIoTClient__idata {
  void *__instance;
  void const* ((*(connect))(IoTConnectionParameters_t *,void *,void *));
  void *((*(getUserData))(void const*,void *));
  bool ((*(isConnected))(void const*,void *));
  bool ((*(report))(void const*,char *,void *));
  bool ((*(get))(void const*,void *));
  void ((*(disconnect))(void const*,void *));
};
typedef struct IIoTClient__idata IIoTClient__idata_t;

struct IIoTClientHandler__idata {
  void *__instance;
  void ((*(connected))(void const*,void *));
  void ((*(status))(void const*,char const *,IoTAction_t,IoTResponseStatus_t,char *,size_t,void *));
  void ((*(desired))(void const*,char const *,char *,size_t,void *));
  void ((*(disconnected))(void const*,void *));
  void ((*(connectionError))(void const*,IoTError_t,void *));
};
typedef struct IIoTClientHandler__idata IIoTClientHandler__idata_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
