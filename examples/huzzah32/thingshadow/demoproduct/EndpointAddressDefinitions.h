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
#ifndef ENDPOINTADDRESSDEFINITIONS_H
#define ENDPOINTADDRESSDEFINITIONS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

enum EndpointAddressType {
  ENDPOINT_ADDRESS_TYPE_IPV4_ADDRESS,
  ENDPOINT_ADDRESS_TYPE_IPV6_ADDRESS,
  ENDPOINT_ADDRESS_TYPE_HOST_NAME,
  ENDPOINT_ADDRESS_TYPE_CUSTOM
};
typedef enum EndpointAddressType EndpointAddressType_t;

struct EndpointAddressDefinitions_EndpointAddress {
  EndpointAddressType_t type;
};
typedef struct EndpointAddressDefinitions_EndpointAddress EndpointAddress_t;

struct IPv4EndpointAddress {
  EndpointAddressType_t type;
  uint32_t ipAddress;
};
typedef struct IPv4EndpointAddress IPv4EndpointAddress_t;

struct HostNameEndpointAddress {
  EndpointAddressType_t type;
  char *hostName;
};
typedef struct HostNameEndpointAddress HostNameEndpointAddress_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
