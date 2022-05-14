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
#ifndef DYNAMICDATAGRAMPOOLIMPL_H
#define DYNAMICDATAGRAMPOOLIMPL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "Datagram.h"
#include "DatagramPool.h"

#ifdef __cplusplus
extern "C" {
#endif

struct DynamicDatagramPoolImpl__cdata {
  uint8_t aMemberSoTheStructIsNotEmpty;
};
typedef struct DynamicDatagramPoolImpl__cdata DynamicDatagramPoolImpl__cdata_t;

Datagram_t *DynamicDatagramPoolImpl_datagramPool_newDatagram(size_t length, void *___id);

void DynamicDatagramPoolImpl_datagramPool_deleteDatagram(Datagram_t *pDatagram, void *___id);

Datagram_t *DynamicDatagramPoolImpl_datagramPool_resizeDatagram(Datagram_t *pDatagram, size_t newLength, void *___id);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
