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
#ifndef SOCKET_H
#define SOCKET_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "Datagram.h"
#include "SocketDefinitions.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * Design decision: There are no socket() and close() primitives for explicit socket "object" lifecycle management. Instead, the socket "objects" are assumed to be created statically along with the component instances implementing this interface. Nevertheless, component implementations are free to create/destroy their socket "objects" dynamically in the runnables implementing connect()/disconnect().
 */
struct IClientSocket__idata {
  void *__instance;
  const void* ((*(connect))(SocketConnectionParameters_t *,void *,void *));
  bool ((*(isConnected))(const void*,void *));
  void *((*(getUserData))(const void*,void *));
  Datagram_t *((*(newDatagram))(const void*,size_t,DatagramType_t,void *));
  Datagram_t *((*(resizeDatagram))(const void*,Datagram_t *,size_t,void *));
  void ((*(sendDatagram))(const void*,Datagram_t *,SocketFrameOptions_t *,void *));
  void ((*(deleteDatagram))(const void*,Datagram_t *,void *));
  void ((*(disconnect))(const void*,void *));
};
typedef struct IClientSocket__idata IClientSocket__idata_t;

struct IClientSocketHandler__idata {
  void *__instance;
  void ((*(connected))(const void*,void *));
  void ((*(datagramSent))(const void*,void *));
  void ((*(datagramReceived))(const void*,Datagram_t *,void *));
  void ((*(disconnected))(const void*,void *));
  void ((*(connectionError))(const void*,SocketError_t,void *));
};
typedef struct IClientSocketHandler__idata IClientSocketHandler__idata_t;

struct IClientSocketConfig__idata {
  void *__instance;
  void ((*(setConfig))(ClientSocketConfig_t const *,void *));
  void ((*(getConfig))(ClientSocketConfig_t *,void *));
};
typedef struct IClientSocketConfig__idata IClientSocketConfig__idata_t;

/* 
 * Design decision: There are no socket() and close() primitives for explicit socket "object" lifecycle management. Instead, the socket "objects" are assumed to be created statically along with the component instances implementing this interface. Nevertheless, component implementations are free to create/destroy their socket "objects" dynamically in the runnables implementing bind()/disconnect().
 */
struct IServerSocket__idata {
  void *__instance;
  bool ((*(bind))(uint32_t,uint16_t,bool,void *));
  bool ((*(listen))(void *));
  Datagram_t *((*(newDatagram))(const void*,size_t,void *));
  void ((*(sendDatagram))(const void*,Datagram_t *,void *));
  void ((*(deleteDatagram))(const void*,Datagram_t *,void *));
  void ((*(disconnect))(const void*,void *));
  void ((*(close))(void *));
};
typedef struct IServerSocket__idata IServerSocket__idata_t;

struct IServerSocketHandler__idata {
  void *__instance;
  void ((*(accepted))(const void*,void *));
  bool ((*(datagramReceived))(const void*,Datagram_t *,void *));
  void ((*(datagramSent))(const void*,bool,void *));
  void ((*(disconnected))(const void*,void *));
  void ((*(closed))(void *));
};
typedef struct IServerSocketHandler__idata IServerSocketHandler__idata_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
