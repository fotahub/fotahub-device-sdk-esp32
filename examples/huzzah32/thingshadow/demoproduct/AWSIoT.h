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
#ifndef AWSIOT_H
#define AWSIOT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "AWSIoTDefinitions.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * TODO Add newJSONDocument()/deleteJSONDocument methods that allocate, return and act upon a datagram payload
 */
struct IAWSIoTThingShadow__idata {
  void *__instance;
  const void* ((*(connect))(AWSIoTConnectionParameters_t *,void *,void *));
  void *((*(getUserData))(const void*,void *));
  bool ((*(isConnected))(const void*,void *));
  bool ((*(update))(const void*,char *,void *));
  bool ((*(get))(const void*,void *));
  void ((*(disconnect))(const void*,void *));
};
typedef struct IAWSIoTThingShadow__idata IAWSIoTThingShadow__idata_t;

struct IAWSIoTThingShadowHandler__idata {
  void *__instance;
  void ((*(connected))(const void*,void *));
  void ((*(status))(const void*,char const *,AWSIoTThingShadowAction_t,AWSIoTThingShadowAckStatus_t,char *,size_t,void *));
  void ((*(delta))(const void*,char const *,char *,size_t,void *));
  void ((*(disconnected))(const void*,void *));
  void ((*(connectionError))(const void*,AWSIoTError_t,void *));
};
typedef struct IAWSIoTThingShadowHandler__idata IAWSIoTThingShadowHandler__idata_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
