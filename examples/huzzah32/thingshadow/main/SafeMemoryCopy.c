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
#include "SafeMemoryCopy.h"

void safeMemoryCopy(void *pDst, void const *pSrc, size_t size)
{
  if (size == 0) 
  {
    return;
  }
  
  uint8_t *pDst8 = ((uint8_t *) pDst);
  uint8_t *pSrc8 = ((uint8_t *) pSrc);
  
  /* 
   * Source and destination pointing at even memory address?
   */
  if ((((ptrdiff_t)(pDst8)) & 1) == 0 && (((ptrdiff_t)(pSrc8)) & 1) == 0) 
  {
    /* 
     * Perform efficient word-wise copying
     */
    size_t wordCount = size >> 1;
    while (wordCount-- > 0)
    {
      *((uint16_t *)(pDst8)) = *((uint16_t *)(pSrc8));
      
      pDst8 += 2;
      pSrc8 += 2;
    }
    
    /* 
     * Odd size?
     */
    if ((size & 1) != 0) 
    {
      /* 
       * Copy last byte
       */
      *pDst8 = *pSrc8;
    }
    
    return;
  }
  
  /* 
   * Source and destination pointing at odd memory address?
   */
  if ((((ptrdiff_t)(pDst8)) & 1) != 0 && (((ptrdiff_t)(pSrc8)) & 1) != 0) 
  {
    /* 
     * Copy first byte
     */
    *pDst8++ = *pSrc8++;
    
    /* 
     * Perform efficient word-wise copying
     */
    size_t wordCount = (size - 1) >> 1;
    while (wordCount-- > 0)
    {
      *((uint16_t *)(pDst8)) = *((uint16_t *)(pSrc8));
      
      pDst8 += 2;
      pSrc8 += 2;
    }
    
    /* 
     * Even size?
     */
    if ((size & 1) == 0) 
    {
      /* 
       * Copy last byte
       */
      *pDst8 = *pSrc8;
    }
    
    return;
  }
  
  /* 
   * Source pointing at even and destination at odd memory address?
   */
  if ((((ptrdiff_t)(pDst8)) & 1) != 0 && (((ptrdiff_t)(pSrc8)) & 1) == 0) 
  {
    /* 
     * Read word-wise but write byte-wise
     */
    size_t wordCount = size >> 1;
    while (wordCount-- > 0)
    {
      uint16_t value = *((uint16_t *)(pSrc8));
      pDst8[0] = ((uint8_t)(value));
      pDst8[1] = ((uint8_t)(value >> 8));
      
      pDst8 += 2;
      pSrc8 += 2;
    }
    
    /* 
     * Odd size?
     */
    if ((size & 1) != 0) 
    {
      /* 
       * Copy last byte
       */
      *pDst8 = *pSrc8;
    }
    
    return;
  }
  
  /* 
   * Source pointing at odd and destination at even memory address?
   */
  if ((((ptrdiff_t)(pDst8)) & 1) == 0 && (((ptrdiff_t)(pSrc8)) & 1) != 0) 
  {
    /* 
     * Read byte-wise but write word-wise
     */
    size_t wordCount = size >> 1;
    while (wordCount-- > 0)
    {
      uint16_t value = ((uint16_t)(pSrc8[0]));
      value |= ((uint16_t)(pSrc8[1])) << 8;
      *((uint16_t *)(pDst8)) = value;
      
      pDst8 += 2;
      pSrc8 += 2;
    }
    
    /* 
     * Odd size?
     */
    if ((size & 1) != 0) 
    {
      /* 
       * Copy last byte
       */
      *pDst8 = *pSrc8;
    }
    
    return;
  }
}
