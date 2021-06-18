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
#include "SocketHelper.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* 
 * Converts given physical maximum fragment length into MaxFragmentLength enum value as per RFC 6066 section 4 (see https://tools.ietf.org/html/rfc6066#page-8 for details).
 */

char *getCertsDir(char *certsDir, size_t maxCertsDirLength)
{
  char *workspaceDir = getenv("WORKSPACE");
  if (workspaceDir != NULL) 
  {
    snprintf(certsDir, maxCertsDirLength, "%s%s", workspaceDir, "/code/certs");
  }
  else
  {
    strncpy(certsDir, "./certs", maxCertsDirLength);
  }
  return certsDir;
}

uint8_t convertMaxFragmentLengthToMFLCode(TLSMaxFragmentLength_t maxFragmentLength)
{
  switch (maxFragmentLength)
  {
    case TLS_MAX_FRAGMENT_LENGTH_512:
    {
      return 1;
    }
    case TLS_MAX_FRAGMENT_LENGTH_1024:
    {
      return 2;
    }
    case TLS_MAX_FRAGMENT_LENGTH_2048:
    {
      return 3;
    }
    case TLS_MAX_FRAGMENT_LENGTH_4096:
    {
      return 4;
    }
    default: {
      return 0;
    }
  }
}

void ipv4ToStr(uint32_t ipAddr, char *ipAddrStr, size_t ipAddrStrSize)
{
  if (ipAddrStr == NULL || ipAddrStrSize < 15) 
  {
    return;
  }
  sprintf(ipAddrStr, "%d.%d.%d.%d", ((uint8_t)(((ipAddr >> 24) & 0xFFu))), ((uint8_t)(((ipAddr >> 16) & 0xFFu))), ((uint8_t)(((ipAddr >> 8) & 0xFFu))), ((uint8_t)((ipAddr & 0xFFu))));
}

uint32_t strToIPv4(char *ipAddrStr, size_t ipAddrStrLength)
{
  if (ipAddrStr == NULL || ipAddrStrLength < 7) 
  {
    return 0u;
  }
  char tmpStr[15 + 1] = "";
  uint8_t ipAddr[4] = { 0, 0, 0, 0 };
  uint8_t i = 0;
  
  strncpy(tmpStr, ipAddrStr, ipAddrStrLength);
  char *token = strtok(((char *)(tmpStr)), ".");
  while (token != NULL)
  {
    ipAddr[i++] = ((uint8_t)(atoi(token)));
    token = strtok(NULL, ".");
  }
  
  return ((((uint32_t)(ipAddr[0])) << 24) | (((uint32_t)(ipAddr[1])) << 16) | (((uint32_t)(ipAddr[2])) << 8) | ipAddr[3]);
}
