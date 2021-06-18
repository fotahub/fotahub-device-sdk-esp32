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
#ifndef DATAGRAMHELPER_H
#define DATAGRAMHELPER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "Datagram.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline Datagram_t *convertToDatagram(void *buffer, size_t bufferSize);

static inline void initDatagram(Datagram_t *pDatagram, void *pPayload, size_t pPayloadLength);

static inline char *getDatagramPayloadAsString(Datagram_t *pDatagram);

static inline void resetDatagram(Datagram_t *pDatagram);

static inline Datagram_t *convertToDatagram(void *buffer, size_t bufferSize)
{
  Datagram_t *pDatagram = ((Datagram_t *) buffer);
  pDatagram->pBase = ((uint8_t *)(pDatagram)) + sizeof(Datagram_t);
  pDatagram->length = ((size_t)(bufferSize - sizeof(Datagram_t)));
  pDatagram->pVisiblePayload = pDatagram->pBase;
  pDatagram->visiblePayloadLength = pDatagram->length;
  return pDatagram;
}

static inline void initDatagram(Datagram_t *pDatagram, void *pPayload, size_t pPayloadLength)
{
  pDatagram->pBase = ((uint8_t *) pPayload);
  pDatagram->length = pPayloadLength;
  pDatagram->pVisiblePayload = pDatagram->pBase;
  pDatagram->visiblePayloadLength = pDatagram->length;
}

static inline char *getDatagramPayloadAsString(Datagram_t *pDatagram)
{
  if (pDatagram->length > pDatagram->visiblePayloadLength) 
  {
    pDatagram->pVisiblePayload[pDatagram->visiblePayloadLength] = ((uint8_t) '\0');
  }
  return ((char *)(pDatagram->pVisiblePayload));
}

static inline void resetDatagram(Datagram_t *pDatagram)
{
  pDatagram->pVisiblePayload = pDatagram->pBase;
  pDatagram->visiblePayloadLength = pDatagram->length;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
