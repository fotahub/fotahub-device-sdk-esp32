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
#ifndef MBEDTLSAWSIOTTHINGSHADOWSTACK_H
#define MBEDTLSAWSIOTTHINGSHADOWSTACK_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "AWSIoTThingShadowGateImpl.h"
#include "AWSIoTMQTTThingShadowImpl.h"
#include "MQTTClientImpl.h"
#include "MbedTLSClientImpl.h"
#include "AWSIoTX509CertStoreImpl.h"
#include "JSONProcessorImpl.h"
#include "DynamicDatagramPoolImpl.h"

#ifdef __cplusplus
extern "C" {
#endif

struct AWSIoTThingShadowStack__cdata {
  /* 
   * fields
   */
  AWSIoTThingShadowGateImpl__cdata_t *thingShadowGateInst__field;
  /* 
   * fields
   */
  AWSIoTMQTTThingShadowImpl__cdata_t *thingShadowInst__field;
  /* 
   * fields
   */
  MQTTClientImpl__cdata_t *mqttClientInst__field;
  /* 
   * fields
   */
  MbedTLSClientImpl__cdata_t *mbedTLSClientInst__field;
  /* 
   * fields
   */
  AWSIoTX509CertStoreImpl__cdata_t *certStoreInst__field;
  /* 
   * fields
   */
  DynamicDatagramPoolImpl__cdata_t *datagramInst__field;
  /* 
   * fields
   */
  JSONProcessorImpl__cdata_t *jsonProcessorInst__field;
  /* 
   * required ports
   */
  IDemoFirmwareUpdateVersionInfo__idata_t *firmwareUpdateVersionInfo__port;
  /* 
   * Req port ops
   */
  IJSONDocumentIOHandler__idata_t *jSONDocumentIOHandler__ops;
};
typedef struct AWSIoTThingShadowStack__cdata AWSIoTThingShadowStack__cdata_t;

void AWSIoTThingShadowStack___setup(void *___id);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
