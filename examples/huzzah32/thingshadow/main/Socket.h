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
  void const* ((*(connect))(SocketConnectionParameters_t *,void *,void *));
  bool ((*(isConnected))(void const*,void *));
  void *((*(getUserData))(void const*,void *));
  Datagram_t *((*(newDatagram))(void const*,size_t,DatagramType_t,void *));
  Datagram_t *((*(resizeDatagram))(void const*,Datagram_t *,size_t,void *));
  void ((*(sendDatagram))(void const*,Datagram_t *,SocketFrameOptions_t *,void *));
  void ((*(deleteDatagram))(void const*,Datagram_t *,void *));
  void ((*(disconnect))(void const*,void *));
};
typedef struct IClientSocket__idata IClientSocket__idata_t;

struct IClientSocketHandler__idata {
  void *__instance;
  void ((*(connected))(void const*,void *));
  void ((*(datagramSent))(void const*,void *));
  void ((*(datagramReceived))(void const*,Datagram_t *,void *));
  void ((*(disconnected))(void const*,void *));
  void ((*(connectionError))(void const*,SocketError_t,void *));
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
  Datagram_t *((*(newDatagram))(void const*,size_t,void *));
  void ((*(sendDatagram))(void const*,Datagram_t *,void *));
  void ((*(deleteDatagram))(void const*,Datagram_t *,void *));
  void ((*(disconnect))(void const*,void *));
  void ((*(close))(void *));
};
typedef struct IServerSocket__idata IServerSocket__idata_t;

struct IServerSocketHandler__idata {
  void *__instance;
  void ((*(accepted))(void const*,void *));
  bool ((*(datagramReceived))(void const*,Datagram_t *,void *));
  void ((*(datagramSent))(void const*,bool,void *));
  void ((*(disconnected))(void const*,void *));
  void ((*(closed))(void *));
};
typedef struct IServerSocketHandler__idata IServerSocketHandler__idata_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
