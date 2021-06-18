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
#include "DynamicDatagramPoolImpl.h"

#include <stdlib.h>
#include <string.h>
#include "SafeMemoryCopy.h"

Datagram_t *DynamicDatagramPoolImpl_datagramPool_newDatagram(size_t length, void *___id)
{
  if (length == 0) 
  {
    printf("Requested datagram length must be at least 1\n");
    return NULL;
  }
  /* 
   * Allocate and initialize new datagram
   */
  Datagram_t *pDatagram = ((Datagram_t *)(malloc(sizeof(Datagram_t) + length)));
  if (pDatagram == NULL) 
  {
    printf("Out of memory\n");
    return NULL;
  }
  pDatagram->pBase = ((uint8_t *)(((ptrdiff_t)(pDatagram)) + sizeof(Datagram_t)));
  pDatagram->length = length;
  pDatagram->pVisiblePayload = pDatagram->pBase;
  pDatagram->visiblePayloadLength = length;
  /* 
   * Return datagram
   */
  return ((Datagram_t *)(pDatagram));
}

void DynamicDatagramPoolImpl_datagramPool_delete(Datagram_t *pDatagram, void *___id)
{
  if (pDatagram == NULL) 
  {
    return;
  }
  /* 
   * Release datagram
   */
  free(pDatagram);
}

Datagram_t *DynamicDatagramPoolImpl_datagramPool_resizeDatagram(Datagram_t *pDatagram, size_t newLength, void *___id)
{
  DynamicDatagramPoolImpl__cdata_t *___cid = ((DynamicDatagramPoolImpl__cdata_t *) ___id);
  if (newLength == 0) 
  {
    printf("Requested datagram length must be at least 1\n");
    return NULL;
  }
  /* 
   * Datagram not yet allocated?
   */
  if (pDatagram == NULL) 
  {
    return DynamicDatagramPoolImpl_datagramPool_newDatagram(newLength, ___cid);
  }
  /* 
   * Datagram already having specified new length?
   */
  if (newLength == pDatagram->length) 
  {
    return pDatagram;
  }
  /* 
   * New length sufficiently high to include all downstream protocol headers & trailers
   * located before & behind current visible payload?
   */
  if (newLength < pDatagram->length - pDatagram->visiblePayloadLength) 
  {
    printf("Requested new datagram length must remain high enough to include at least all downstream protocol headers & trailers located before & behind current visible payload\n");
    return NULL;
  }
  
  /* 
   * Allocate new datagram with specified new length
   */
  Datagram_t *pResizedDatagram = DynamicDatagramPoolImpl_datagramPool_newDatagram(newLength, ___cid);
  if (pResizedDatagram == NULL) 
  {
    return NULL;
  }
  
  size_t headerDataLength = pDatagram->pVisiblePayload - pDatagram->pBase;
  size_t trailerDataLength = pDatagram->length - headerDataLength - pDatagram->visiblePayloadLength;
  uint8_t *oldTrailerDataPos = pDatagram->pVisiblePayload + pDatagram->visiblePayloadLength;
  uint8_t *newTrailerDataPos = pResizedDatagram->pBase + pResizedDatagram->length - trailerDataLength;
  int64_t lengthDelta = ((int64_t) newLength) - ((int64_t)(pDatagram->length));
  /* 
   * Copy other protocol's header data + current protocol's payload data
   */
  if (lengthDelta > 0) 
  {
    safeMemoryCopy(pResizedDatagram->pBase, pDatagram->pBase, pDatagram->length - trailerDataLength);
  }
  else
  {
    safeMemoryCopy(pResizedDatagram->pBase, pDatagram->pBase, newLength - trailerDataLength);
  }
  /* 
   * Copy other protocol's trailer data if any
   */
  if (trailerDataLength > 0) 
  {
    safeMemoryCopy(newTrailerDataPos, oldTrailerDataPos, trailerDataLength);
  }
  /* 
   * Restore visible payload and update its length
   */
  pResizedDatagram->pVisiblePayload += pDatagram->pVisiblePayload - pDatagram->pBase;
  pResizedDatagram->visiblePayloadLength = ((size_t)((((int64_t)(pDatagram->visiblePayloadLength)) + lengthDelta)));
  
  /* 
   * Delete old datagram
   */
  DynamicDatagramPoolImpl_datagramPool_delete(pDatagram, ___cid);
  return pResizedDatagram;
}
