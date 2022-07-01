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
#ifndef MBEDTLSCLIENTIMPL_H
#define MBEDTLSCLIENTIMPL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "Socket.h"
#include "CommonServices.h"
#include "SocketHelper.h"
#include "Mutex.h"
#include "DatagramPool.h"
#include "DatagramHelper.h"
#include "X509.h"
#include "MbedTLSDefinitions.h"
#include "SocketHelper.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_CLIENT_SOCKET_HANDLER_COUNT 1

#define MAX_SOCKET_CONNECTION_COUNT 3

#define DEFAULT_READ_TIMEOUT 10

struct MbedTLSEventFlags {
  uint8_t connected:1;
  uint8_t dataSent:1;
  uint8_t disconnecting:1;
  uint8_t reserved:5;
};
typedef struct MbedTLSEventFlags MbedTLSEventFlags_t;

struct MbedTLSNetwork {
  TLSDataParams_t *pTLSDataParams;
  MbedTLSEventFlags_t eventFlags;
  bool busy;
  void *pUserData;
};
typedef struct MbedTLSNetwork MbedTLSNetwork_t;

struct MbedTLSClientImpl__cdata {
  /* 
   * fields
   */
  MbedTLSNetwork_t networks__field[MAX_SOCKET_CONNECTION_COUNT];
  /* 
   * Req port ops
   */
  IMutex__idata_t *mutex__ops;
  /* 
   * Req port ops
   */
  IDatagramPool__idata_t *datagramPool__ops;
  /* 
   * Req port ops
   */
  IClientSocketHandler__idata_t *clientSocketHandlers__ops[MAX_CLIENT_SOCKET_HANDLER_COUNT];
  /* 
   * Req port ops
   */
  IX509DataProvider__idata_t *x509DataProvider__ops;
};
typedef struct MbedTLSClientImpl__cdata MbedTLSClientImpl__cdata_t;

MbedTLSNetwork_t *MbedTLSClientImpl_newMbedTLSNetwork(void *___id);

bool MbedTLSClientImpl_isMbedTLSNetworkDeleted(MbedTLSNetwork_t *pNetwork, void *___id);

void MbedTLSClientImpl_freeMbedTLSNetworkResources(MbedTLSNetwork_t *pNetwork, bool closeNotify, void *___id);

void MbedTLSClientImpl_resetMbedTLSNetworkExtendedData(MbedTLSNetwork_t *pNetwork, void *___id);

void MbedTLSClientImpl_deleteMbedTLSNetwork(MbedTLSNetwork_t *pNetwork, bool closeNotify, void *___id);

void const* MbedTLSClientImpl_socket_connect(SocketConnectionParameters_t *pConnParams, void *pUserData, void *___id);

bool MbedTLSClientImpl_socket_isConnected(void const* hSession, void *___id);

void *MbedTLSClientImpl_socket_getUserData(void const* hSession, void *___id);

Datagram_t *MbedTLSClientImpl_socket_newDatagram(void const* hSession, size_t payloadLength, DatagramType_t type, void *___id);

Datagram_t *MbedTLSClientImpl_socket_resizeDatagram(void const* hSession, Datagram_t *pDatagram, size_t newSize, void *___id);

void MbedTLSClientImpl_socket_sendDatagram(void const* hSession, Datagram_t *pDatagram, SocketFrameOptions_t *pOptions, void *___id);

void MbedTLSClientImpl_socket_deleteDatagram(void const* hSession, Datagram_t *pDatagram, void *___id);

void MbedTLSClientImpl_socket_disconnect(void const* hSession, void *___id);

void MbedTLSClientImpl_activity_run(void *___id);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
