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
#ifndef DATAGRAM_H
#define DATAGRAM_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "EndpointAddressDefinitions.h"

#ifdef __cplusplus
extern "C" {
#endif

struct Datagram {
  uint8_t *pBase;
  size_t length;
  uint8_t *pVisiblePayload;
  size_t visiblePayloadLength;
};
typedef struct Datagram Datagram_t;

enum DatagramType {
  BASIC,
  PROTOCOL_SPECIFIC,
  HTTP_EXTERNAL_HEADERS
};
typedef enum DatagramType DatagramType_t;

struct IDatagramReceiveRequest__idata {
  void *__instance;
  void ((*(receiveDatagram))(size_t,void *));
};
typedef struct IDatagramReceiveRequest__idata IDatagramReceiveRequest__idata_t;

struct IDirectedDatagramReceiveRequest__idata {
  void *__instance;
  bool ((*(receiveDatagram))(EndpointAddress_t *,size_t,void *));
};
typedef struct IDirectedDatagramReceiveRequest__idata IDirectedDatagramReceiveRequest__idata_t;

/* 
 * Use this interface when receiver is operated asynchronously
 */
struct IDatagramReceiveHandler__idata {
  void *__instance;
  void ((*(datagramReceived))(Datagram_t *,void *));
  void ((*(receiveError))(Datagram_t *,uint16_t,void *));
};
typedef struct IDatagramReceiveHandler__idata IDatagramReceiveHandler__idata_t;

struct IDatagramTransmitRequest__idata {
  void *__instance;
  Datagram_t *((*(newDatagram))(size_t,void *));
  void ((*(transmitDatagram))(Datagram_t *,void *));
  void ((*(deleteDatagram))(Datagram_t *,void *));
};
typedef struct IDatagramTransmitRequest__idata IDatagramTransmitRequest__idata_t;

struct IDirectedDatagramTransmitRequest__idata {
  void *__instance;
  Datagram_t *((*(newDatagram))(size_t,void *));
  bool ((*(transmitDatagram))(EndpointAddress_t *,Datagram_t *,void *));
  void ((*(deleteDatagram))(Datagram_t *,void *));
};
typedef struct IDirectedDatagramTransmitRequest__idata IDirectedDatagramTransmitRequest__idata_t;

/* 
 * Use this interface when receiver is operated asynchronously
 */
struct IDatagramTransmitHandler__idata {
  void *__instance;
  void ((*(datagramTransmitted))(void *));
  void ((*(transmitError))(uint16_t,void *));
};
typedef struct IDatagramTransmitHandler__idata IDatagramTransmitHandler__idata_t;

struct IDatagramExchange__idata {
  void *__instance;
  Datagram_t *((*(newDatagram))(size_t,void *));
  bool ((*(transmitReceiveDatagram))(Datagram_t *,Datagram_t *,void *));
  void ((*(deleteDatagram))(Datagram_t *,void *));
};
typedef struct IDatagramExchange__idata IDatagramExchange__idata_t;

/* 
 * See https://web.cs.wpi.edu/~rek/Undergrad_Nets/B07/BitByteStuff.pdf for details
 */
struct IDatagramStreamer__idata {
  void *__instance;
  void ((*(beginDatagram))(Datagram_t *,void *));
  size_t ((*(getStreamLength))(void *));
  bool ((*(hasNextStreamCharacter))(void *));
  uint8_t ((*(getNextStreamCharacter))(void *));
};
typedef struct IDatagramStreamer__idata IDatagramStreamer__idata_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
