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
#ifndef SOCKETDEFINITIONS_H
#define SOCKETDEFINITIONS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "EndpointAddressDefinitions.h"
#include "TLSDefinitions.h"

#ifdef __cplusplus
extern "C" {
#endif

enum SocketConnectionType {
  SOCKET_CONNECTION_TYPE_TCP,
  SOCKET_CONNECTION_TYPE_UDP,
  SOCKET_CONNECTION_TYPE_WEBSOCKET
};
typedef enum SocketConnectionType SocketConnectionType_t;

#define SECURE_CONNECTION_READ_TIMEOUT_NONE 0

struct SecureConnectionParameters {
  TLSMaxFragmentLength_t maxFragmentLength;
  uint8_t maxDataRate;
  uint32_t readTimeout;
  bool authenticateServer;
  uint32_t rootCACertsAddress;
  uint32_t clientCertAddress;
};
typedef struct SecureConnectionParameters SecureConnectionParameters_t;

struct SocketConnectionParameters {
  SocketConnectionType_t type;
  EndpointAddress_t *pAddr;
  uint16_t port;
  SecureConnectionParameters_t *pSecureConnectionParams;
};
typedef struct SocketConnectionParameters SocketConnectionParameters_t;

struct ClientSocketConfig {
  EndpointAddressType_t type;
};
typedef struct ClientSocketConfig ClientSocketConfig_t;

struct IPv4ClientSocketConfig {
  EndpointAddressType_t type;
  uint32_t localIPAddr;
  uint32_t subnetMask;
  uint32_t gatewayAddr;
};
typedef struct IPv4ClientSocketConfig IPv4ClientSocketConfig_t;

enum SocketOpcode {
  OPCODE_TEXT = 1,
  OPCODE_BINARY = 2,
  OPCODE_CLOSE = 8,
  OPCODE_PING = 9,
  OPCODE_PONG = 10
};
typedef enum SocketOpcode SocketOpcode_t;

struct SocketFrameOptions {
  SocketOpcode_t opcode;
};
typedef struct SocketFrameOptions SocketFrameOptions_t;

/* 
 * Socket error codes (see http://www-numi.fnal.gov/offline_software/srt_public_context/WebDocs/Errors/unix_system_errors.html for details)
 */
enum SocketError {
  SOCKET_ERROR_INTERRUPTED_SYSTEM_CALL = 3,
  SOCKET_ERROR_IO = 5,
  SOCKET_ERROR_OPERATION_WOULD_BLOCK = 11,
  SOCKET_ERROR_OUT_OF_MEMORY = 12,
  SOCKET_ERROR_PERMISSION_DENIED = 13,
  SOCKET_ERROR_BAD_ADDRESS = 14,
  SOCKET_ERROR_INVALID_ARGUMENT = 22,
  SOCKET_ERROR_SOCKET_OPERATION_ON_NON_SOCKET = 38,
  SOCKET_ERROR_NODATA = 61,
  SOCKET_ERROR_TIME = 62,
  SOCKET_ERROR_COMMUNICATION_ERROR_ON_SEND = 70,
  SOCKET_ERROR_MESSAGE_TOO_LONG = 90,
  SOCKET_ERROR_OPERATION_NOT_SUPPORTED_ON_TRANSPORT_ENDPOINT = 95,
  SOCKET_ERROR_ADDRESS_FAMILY_NOT_SUPPORTED_BY_PROTOCOL = 97,
  SOCKET_ERROR_ADDRESS_ALREADY_IN_USE = 98,
  SOCKET_ERROR_CANNOT_ASSIGN_REQUESTED_ADDRESS = 99,
  SOCKET_ERROR_NETWORK_IS_DOWN = 100,
  SOCKET_ERROR_NETWORK_IS_UNREACHABLE = 101,
  SOCKET_ERROR_NETWORK_DROPPED_CONNECTION_BECAUSE_OF_RESET = 102,
  SOCKET_ERROR_SOFTWARE_CAUSED_CONNECTION_ABORT = 103,
  SOCKET_ERROR_CONNECTION_RESET_BY_PEER = 104,
  SOCKET_ERROR_NO_BUFFER_SPACE_AVAILABLE = 105,
  SOCKET_ERROR_TRANSPORT_ENDPOINT_ALREADY_CONNECTED = 106,
  SOCKET_ERROR_TRANSPORT_ENDPOINT_NOT_CONNECTED = 107,
  SOCKET_ERROR_CANNOT_SEND_AFTER_TRANSPORT_ENDPOINT_SHUTDOWN = 108,
  SOCKET_ERROR_CONNECTION_TIMED_OUT = 110,
  SOCKET_ERROR_CONNECTION_REFUSED = 111,
  SOCKET_ERROR_HOST_IS_DOWN = 112,
  SOCKET_ERROR_NO_ROUTE_TO_HOST = 113,
  SOCKET_ERROR_OPERATION_ALREADY_IN_PROGRESS = 114,
  SOCKET_ERROR_OPERATION_NOW_IN_PROGRESS = 115,
  SOCKET_ERROR_TRANSPORT_ENDPOINT_UNAVAILABLE = 125,
  SOCKET_ERROR_CONNECTION_HANDSHAKE = 126,
  SOCKET_ERROR_TOO_MANY_CONNECTIONS = 127,
  SOCKET_ERROR_SECURITY = 128,
  SOCKET_ERROR_CONNECTION_CLOSED = 129,
  SOCKET_ERROR_NETWORK_INTERFACE = 130,
  SOCKET_ERROR_RESPONSE_TIMED_OUT,
  SOCKET_ERROR_INVALID_CERTIFICATE,
  SOCKET_ERROR_UNKNOWN
};
typedef enum SocketError SocketError_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
