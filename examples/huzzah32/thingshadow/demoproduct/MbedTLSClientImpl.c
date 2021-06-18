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
#include "MbedTLSClientImpl.h"

#include <stdlib.h>
#include <string.h>
#include "mbedtls/error.h"

MbedTLSNetwork_t *MbedTLSClientImpl_newMbedTLSNetwork(void *___id)
{
  MbedTLSClientImpl__cdata_t *___cid = ((MbedTLSClientImpl__cdata_t *) ___id);
  for ( int16_t __i = 0 ; __i < MAX_SOCKET_CONNECTION_COUNT; __i++ )
  {
    if (!___cid->networks__field[__i].busy) 
    {
      ___cid->networks__field[__i].busy = true;
      ___cid->networks__field[__i].pUserData = NULL;
      ___cid->networks__field[__i].eventFlags.connected = false;
      ___cid->networks__field[__i].eventFlags.dataSent = false;
      ___cid->networks__field[__i].eventFlags.disconnecting = false;
      ___cid->networks__field[__i].eventFlags.reserved = 0;
      ___cid->networks__field[__i].pTLSDataParams = ((TLSDataParams_t *)(malloc(sizeof(TLSDataParams_t))));
      if (___cid->networks__field[__i].pTLSDataParams == NULL) 
      {
        printf("No more memory available\n");
        return NULL;
      }
      return &___cid->networks__field[__i];
    }
  }
  printf("No more mbedTLS network blocks available\n");
  return NULL;
}

bool MbedTLSClientImpl_isMbedTLSNetworkDeleted(MbedTLSNetwork_t *pNetwork, void *___id)
{
  if (pNetwork == NULL) 
  {
    return true;
  }
  return !(pNetwork->busy);
}

void MbedTLSClientImpl_freeMbedTLSNetworkResources(MbedTLSNetwork_t *pNetwork, bool closeNotify, void *___id)
{
  if (pNetwork == NULL) 
  {
    return;
  }
  if (closeNotify) 
  {
    mbedtls_ssl_close_notify(&(pNetwork->pTLSDataParams->ssl));
  }
  mbedtls_net_free(&(pNetwork->pTLSDataParams->serverFD));
  mbedtls_x509_crt_free(&(pNetwork->pTLSDataParams->serverCert));
  mbedtls_x509_crt_free(&(pNetwork->pTLSDataParams->clientCert));
  mbedtls_pk_free(&(pNetwork->pTLSDataParams->privateKey));
  mbedtls_ssl_free(&(pNetwork->pTLSDataParams->ssl));
  mbedtls_ssl_config_free(&(pNetwork->pTLSDataParams->conf));
  mbedtls_ctr_drbg_free(&(pNetwork->pTLSDataParams->ctrDrbg));
  mbedtls_entropy_free(&(pNetwork->pTLSDataParams->entropy));
  
  if (pNetwork->pTLSDataParams != NULL) 
  {
    free(pNetwork->pTLSDataParams);
    pNetwork->pTLSDataParams = NULL;
  }
}

void MbedTLSClientImpl_resetMbedTLSNetworkExtendedData(MbedTLSNetwork_t *pNetwork, void *___id)
{
  MbedTLSClientImpl__cdata_t *___cid = ((MbedTLSClientImpl__cdata_t *) ___id);
  if (MbedTLSClientImpl_isMbedTLSNetworkDeleted(pNetwork, ___cid)) 
  {
    return;
  }
  pNetwork->eventFlags.connected = false;
  pNetwork->eventFlags.dataSent = false;
  pNetwork->eventFlags.disconnecting = false;
  pNetwork->eventFlags.reserved = 0;
  pNetwork->busy = false;
  pNetwork->pUserData = NULL;
}

void MbedTLSClientImpl_deleteMbedTLSNetwork(MbedTLSNetwork_t *pNetwork, bool closeNotify, void *___id)
{
  MbedTLSClientImpl__cdata_t *___cid = ((MbedTLSClientImpl__cdata_t *) ___id);
  if (MbedTLSClientImpl_isMbedTLSNetworkDeleted(pNetwork, ___cid)) 
  {
    return;
  }
  
  /* 
   * The free order is important, we must free first resources then extended data.
   */
  MbedTLSClientImpl_freeMbedTLSNetworkResources(pNetwork, closeNotify, ___cid);
  MbedTLSClientImpl_resetMbedTLSNetworkExtendedData(pNetwork, ___cid);
}

const void* MbedTLSClientImpl_socket_connect(SocketConnectionParameters_t *pConnParams, void *pUserData, void *___id)
{
  MbedTLSClientImpl__cdata_t *___cid = ((MbedTLSClientImpl__cdata_t *) ___id);
  if (pConnParams == NULL) 
  {
    return NULL;
  }
  MbedTLSNetwork_t *pNetwork = MbedTLSClientImpl_newMbedTLSNetwork(___cid);
  if (MbedTLSClientImpl_isMbedTLSNetworkDeleted(pNetwork, ___cid)) 
  {
    return NULL;
  }
  
  pNetwork->pTLSDataParams->flags = 0;
  pNetwork->pUserData = pUserData;
  
  TLSDataParams_t *pTLSDataParams = pNetwork->pTLSDataParams;
  mbedtls_net_init(&(pTLSDataParams->serverFD));
  mbedtls_ssl_init(&(pTLSDataParams->ssl));
  mbedtls_ssl_config_init(&(pTLSDataParams->conf));
  mbedtls_ctr_drbg_init(&(pTLSDataParams->ctrDrbg));
  mbedtls_x509_crt_init(&(pTLSDataParams->serverCert));
  mbedtls_x509_crt_init(&(pTLSDataParams->clientCert));
  mbedtls_pk_init(&(pTLSDataParams->privateKey));
  mbedtls_entropy_init(&(pTLSDataParams->entropy));
  
  /* 
   * Added debug configuration
   */
  
  char *data = "mbedTLSClient";
  int32_t ret = mbedtls_ctr_drbg_seed(&(pTLSDataParams->ctrDrbg), &mbedtls_entropy_func, &(pTLSDataParams->entropy), ((uint8_t *)(data)), strlen(data));
  if (ret != 0) 
  {
    printf("Failed! mbedtls_ctr_drbg_seed returned %i\n", ret);
    MbedTLSClientImpl_deleteMbedTLSNetwork(pNetwork, true, ___cid);
    return NULL;
  }
  
  if (___cid->x509DataProvider__ops != NULL) 
  {
    uint8_t const  *pX509DataItem = NULL;
    size_t x509DataSize;
    if (pConnParams->pSecureConnectionParams->authenticateServer) 
    {
      size_t count = (*___cid->x509DataProvider__ops->getX509RootCACount)(___cid->x509DataProvider__ops->__instance);
      if (count < 1) 
      {
        printf("No root CA found!\n");
        MbedTLSClientImpl_deleteMbedTLSNetwork(pNetwork, true, ___cid);
        return NULL;
      }
      for ( int64_t __i = 0 ; __i < count; __i++ )
      {
        (*___cid->x509DataProvider__ops->loadRootCA)(((uint8_t)(__i)), &pX509DataItem, &x509DataSize, ___cid->x509DataProvider__ops->__instance);
        if ((ret = mbedtls_x509_crt_parse(&pTLSDataParams->serverCert, pX509DataItem, x509DataSize)) < 0) 
        {
          printf("Failed!  mbedtls_x509_crt_parse returned %i while parsing root cert\n", ret);
          MbedTLSClientImpl_deleteMbedTLSNetwork(pNetwork, true, ___cid);
          (*___cid->x509DataProvider__ops->unloadX509DataItem)(&pX509DataItem, ___cid->x509DataProvider__ops->__instance);
          return NULL;
        }
      }
      (*___cid->x509DataProvider__ops->unloadX509DataItem)(&pX509DataItem, ___cid->x509DataProvider__ops->__instance);
    }
    if ((*___cid->x509DataProvider__ops->loadCertificate)(&pX509DataItem, &x509DataSize, ___cid->x509DataProvider__ops->__instance)) 
    {
      if ((ret = mbedtls_x509_crt_parse(&(pTLSDataParams->clientCert), pX509DataItem, x509DataSize)) < 0) 
      {
        printf("Failed!  mbedtls_x509_crt_parse returned %i while parsing client cert\n", ret);
        MbedTLSClientImpl_deleteMbedTLSNetwork(pNetwork, true, ___cid);
        (*___cid->x509DataProvider__ops->unloadX509DataItem)(&pX509DataItem, ___cid->x509DataProvider__ops->__instance);
        return NULL;
      }
      (*___cid->x509DataProvider__ops->unloadX509DataItem)(&pX509DataItem, ___cid->x509DataProvider__ops->__instance);
    }
    if ((*___cid->x509DataProvider__ops->loadPrivateKey)(&pX509DataItem, &x509DataSize, ___cid->x509DataProvider__ops->__instance)) 
    {
      if ((ret = mbedtls_pk_parse_key(&(pTLSDataParams->privateKey), pX509DataItem, x509DataSize, ((uint8_t *) ""), 0)) < 0) 
      {
        printf("Failed!  mbedtls_x509_crt_parse returned %i while parsing private key\n", ret);
        MbedTLSClientImpl_deleteMbedTLSNetwork(pNetwork, true, ___cid);
        (*___cid->x509DataProvider__ops->unloadX509DataItem)(&pX509DataItem, ___cid->x509DataProvider__ops->__instance);
        return NULL;
      }
      (*___cid->x509DataProvider__ops->unloadX509DataItem)(&pX509DataItem, ___cid->x509DataProvider__ops->__instance);
    }
  }
  
  char portBuffer[5 + 1] = { 0 };
  snprintf(portBuffer, 6, "%d", pConnParams->port);
  char ipBuffer[15 + 1] = { 0 };
  if (pConnParams->pAddr->type == ENDPOINT_ADDRESS_TYPE_IPV4_ADDRESS) 
  {
    ipv4ToStr(((IPv4EndpointAddress_t *)(pConnParams->pAddr))->ipAddress, ipBuffer, 16);
  }
  if ((ret = mbedtls_net_connect(&(pTLSDataParams->serverFD), (pConnParams->pAddr->type == ENDPOINT_ADDRESS_TYPE_IPV4_ADDRESS) ? (ipBuffer) : (((HostNameEndpointAddress_t *)(pConnParams->pAddr))->hostName), portBuffer, MBEDTLS_NET_PROTO_TCP)) != 0) 
  {
    printf("Failed! mbedtls_net_connect returned %i\n", ret);
    MbedTLSClientImpl_deleteMbedTLSNetwork(pNetwork, true, ___cid);
    return NULL;
  }
  if ((ret = mbedtls_net_set_nonblock(&(pTLSDataParams->serverFD))) != 0) 
  {
    printf("Failed! net_set_(non)block() returned %i\n", ret);
    MbedTLSClientImpl_deleteMbedTLSNetwork(pNetwork, true, ___cid);
    return NULL;
  }
  /* 
   * Setting up the SSL/TLS structure...
   */
  if ((ret = mbedtls_ssl_config_defaults(&(pTLSDataParams->conf), MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT)) != 0) 
  {
    printf("Failed! mbedtls_ssl_config_defaults returned %i\n", ret);
    MbedTLSClientImpl_deleteMbedTLSNetwork(pNetwork, true, ___cid);
    return NULL;
  }
  
  if (pConnParams->pSecureConnectionParams != NULL && pConnParams->pSecureConnectionParams->maxFragmentLength != TLS_MAX_FRAGMENT_LENGTH_DISABLED) 
  {
    /* 
     * TODO Check why mbed TLS client can deal with handshake fragmentation on ESP32
     */
    if ((ret = mbedtls_ssl_conf_max_frag_len(&(pTLSDataParams->conf), convertMaxFragmentLengthToMFLCode(pConnParams->pSecureConnectionParams->maxFragmentLength))) != 0) 
    {
      printf("Failed! mbedtls_ssl_conf_max_frag_len returned %i\n", ret);
      MbedTLSClientImpl_deleteMbedTLSNetwork(pNetwork, true, ___cid);
      return NULL;
    }
  }
  
  if (pConnParams->pSecureConnectionParams->authenticateServer) 
  {
    mbedtls_ssl_conf_authmode(&(pTLSDataParams->conf), MBEDTLS_SSL_VERIFY_REQUIRED);
  }
  else
  {
    mbedtls_ssl_conf_authmode(&(pTLSDataParams->conf), MBEDTLS_SSL_VERIFY_OPTIONAL);
  }
  mbedtls_ssl_conf_rng(&(pTLSDataParams->conf), &mbedtls_ctr_drbg_random, &(pTLSDataParams->ctrDrbg));
  mbedtls_ssl_conf_ca_chain(&(pTLSDataParams->conf), &(pTLSDataParams->serverCert), NULL);
  ret = mbedtls_ssl_conf_own_cert(&(pTLSDataParams->conf), &(pTLSDataParams->clientCert), &(pTLSDataParams->privateKey));
  if (ret != 0) 
  {
    printf("Failed! mbedtls_ssl_conf_own_cert returned %i\n", ret);
    MbedTLSClientImpl_deleteMbedTLSNetwork(pNetwork, true, ___cid);
    return NULL;
  }
  
  if ((ret = mbedtls_ssl_setup(&(pTLSDataParams->ssl), &(pTLSDataParams->conf))) != 0) 
  {
    printf("Failed! mbedtls_ssl_setup returned %i\n", ret);
    MbedTLSClientImpl_deleteMbedTLSNetwork(pNetwork, true, ___cid);
    return NULL;
  }
  if ((ret = mbedtls_ssl_set_hostname(&(pTLSDataParams->ssl), (pConnParams->pAddr->type == ENDPOINT_ADDRESS_TYPE_IPV4_ADDRESS) ? (ipBuffer) : (((HostNameEndpointAddress_t *)(pConnParams->pAddr))->hostName))) != 0) 
  {
    printf("Failed! mbedtls_ssl_set_hostname returned %i\n", ret);
    MbedTLSClientImpl_deleteMbedTLSNetwork(pNetwork, true, ___cid);
    return NULL;
  }
  
  /* 
   * Note: handshake steps during connection are performed in blocking mode
   */
  mbedtls_ssl_set_bio(&(pTLSDataParams->ssl), &(pTLSDataParams->serverFD), &mbedtls_net_send, NULL, &mbedtls_net_recv_timeout);
  
  uint32_t readTimeout = (pConnParams->pSecureConnectionParams->readTimeout != SECURE_CONNECTION_READ_TIMEOUT_NONE) ? (pConnParams->pSecureConnectionParams->readTimeout) : (DEFAULT_READ_TIMEOUT);
  mbedtls_ssl_conf_read_timeout(&(pTLSDataParams->conf), readTimeout);
  
  /* 
   * Performing the SSL/TLS handshake...
   */
  while ((ret = mbedtls_ssl_handshake(&(pTLSDataParams->ssl))) != 0)
  {
    if (((ret != MBEDTLS_ERR_SSL_WANT_READ) && (ret != MBEDTLS_ERR_SSL_WANT_WRITE))) 
    {
      printf("Failed! mbedtls_ssl_handshake returned %i\n", ret);
      if (ret == MBEDTLS_ERR_X509_CERT_VERIFY_FAILED) 
      {
        printf("Unable to verify the server's certificate\n");
      }
      MbedTLSClientImpl_deleteMbedTLSNetwork(pNetwork, true, ___cid);
      return NULL;
    }
  }
  
  /* 
   * added setting of blocking mode TBC with all platforms
   */
  if ((ret = mbedtls_net_set_block(&(pTLSDataParams->serverFD))) != 0) 
  {
    printf("Failed! net_set_block() returned %i\n", ret);
    MbedTLSClientImpl_deleteMbedTLSNetwork(pNetwork, true, ___cid);
    return NULL;
  }
  mbedtls_ssl_set_bio(&(pTLSDataParams->ssl), &(pTLSDataParams->serverFD), &mbedtls_net_send, &mbedtls_net_recv, NULL);
  
  if (___cid->mutex__ops != NULL) 
  {
    (*___cid->mutex__ops->lock)(___cid->mutex__ops->__instance);
  }
  pNetwork->eventFlags.connected = true;
  if (___cid->mutex__ops != NULL) 
  {
    (*___cid->mutex__ops->unlock)(___cid->mutex__ops->__instance);
  }
  return pNetwork;
}

bool MbedTLSClientImpl_socket_isConnected(const void* hSession, void *___id)
{
  MbedTLSClientImpl__cdata_t *___cid = ((MbedTLSClientImpl__cdata_t *) ___id);
  MbedTLSNetwork_t *pNetwork = ((MbedTLSNetwork_t *) hSession);
  if (MbedTLSClientImpl_isMbedTLSNetworkDeleted(pNetwork, ___cid)) 
  {
    return false;
  }
  if (pNetwork->pTLSDataParams == NULL) 
  {
    return false;
  }
  if (pNetwork->busy && !pNetwork->eventFlags.disconnecting && pNetwork->pTLSDataParams->ssl.state == MBEDTLS_SSL_HANDSHAKE_OVER) 
  {
    return true;
  }
  return false;
}

void *MbedTLSClientImpl_socket_getUserData(const void* hSession, void *___id)
{
  MbedTLSClientImpl__cdata_t *___cid = ((MbedTLSClientImpl__cdata_t *) ___id);
  MbedTLSNetwork_t *pNetwork = ((MbedTLSNetwork_t *) hSession);
  if (MbedTLSClientImpl_isMbedTLSNetworkDeleted(pNetwork, ___cid)) 
  {
    return NULL;
  }
  
  return pNetwork->pUserData;
}

Datagram_t *MbedTLSClientImpl_socket_newDatagram(const void* hSession, size_t payloadLength, DatagramType_t type, void *___id)
{
  MbedTLSClientImpl__cdata_t *___cid = ((MbedTLSClientImpl__cdata_t *) ___id);
  return (*___cid->datagramPool__ops->newDatagram)(payloadLength, ___cid->datagramPool__ops->__instance);
}

Datagram_t *MbedTLSClientImpl_socket_resizeDatagram(const void* hSession, Datagram_t *pDatagram, size_t newSize, void *___id)
{
  MbedTLSClientImpl__cdata_t *___cid = ((MbedTLSClientImpl__cdata_t *) ___id);
  return (*___cid->datagramPool__ops->resizeDatagram)(pDatagram, newSize, ___cid->datagramPool__ops->__instance);
}

void MbedTLSClientImpl_socket_sendDatagram(const void* hSession, Datagram_t *pDatagram, SocketFrameOptions_t *pOptions, void *___id)
{
  MbedTLSClientImpl__cdata_t *___cid = ((MbedTLSClientImpl__cdata_t *) ___id);
  MbedTLSNetwork_t *pNetwork = ((MbedTLSNetwork_t *) hSession);
  if (MbedTLSClientImpl_isMbedTLSNetworkDeleted(pNetwork, ___cid)) 
  {
    return;
  }
  
  bool isErrorFlag = false;
  size_t writtenSoFar = 0;
  for ( int32_t ret = 0 ; writtenSoFar < pDatagram->visiblePayloadLength; writtenSoFar += ((size_t)(ret)) )
  {
    while ((ret = mbedtls_ssl_write(&(pNetwork->pTLSDataParams->ssl), pDatagram->pVisiblePayload + writtenSoFar, pDatagram->visiblePayloadLength - writtenSoFar)) <= 0)
    {
      if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE) 
      {
        printf("Failed! mbedtls_ssl_write returned %i\n", ret);
        /* 
         * All other negative return values indicate connection needs to be reset, will be caught in ping request so ignored here
         */
        isErrorFlag = true;
        break;
      }
    }
    if (isErrorFlag) 
    {
      break;
    }
  }
  
  if (isErrorFlag) 
  {
    MbedTLSClientImpl_freeMbedTLSNetworkResources(pNetwork, true, ___cid);
    for ( uint8_t ___pc = 0 ; ___pc < MAX_CLIENT_SOCKET_HANDLER_COUNT; ___pc++ )
    {
      if (___cid->clientSocketHandlers__ops[___pc] != NULL && ___cid->clientSocketHandlers__ops[___pc]->__instance != NULL) 
      {
        (*___cid->clientSocketHandlers__ops[___pc]->connectionError)(pNetwork, SOCKET_ERROR_COMMUNICATION_ERROR_ON_SEND, ___cid->clientSocketHandlers__ops[___pc]->__instance);
      }
    }
    MbedTLSClientImpl_resetMbedTLSNetworkExtendedData(pNetwork, ___cid);
    return;
  }
  else
  {
    (*___cid->datagramPool__ops->delete)(pDatagram, ___cid->datagramPool__ops->__instance);
    
    if (___cid->mutex__ops != NULL) 
    {
      (*___cid->mutex__ops->lock)(___cid->mutex__ops->__instance);
    }
    pNetwork->eventFlags.dataSent = true;
    if (___cid->mutex__ops != NULL) 
    {
      (*___cid->mutex__ops->unlock)(___cid->mutex__ops->__instance);
    }
  }
}

void MbedTLSClientImpl_socket_deleteDatagram(const void* hSession, Datagram_t *pDatagram, void *___id)
{
  MbedTLSClientImpl__cdata_t *___cid = ((MbedTLSClientImpl__cdata_t *) ___id);
  (*___cid->datagramPool__ops->delete)(pDatagram, ___cid->datagramPool__ops->__instance);
}

void MbedTLSClientImpl_socket_disconnect(const void* hSession, void *___id)
{
  MbedTLSClientImpl__cdata_t *___cid = ((MbedTLSClientImpl__cdata_t *) ___id);
  MbedTLSNetwork_t *pNetwork = ((MbedTLSNetwork_t *) hSession);
  if (MbedTLSClientImpl_isMbedTLSNetworkDeleted(pNetwork, ___cid)) 
  {
    return;
  }
  int32_t ret = 0;
  do {
    ret = mbedtls_ssl_close_notify(&(pNetwork->pTLSDataParams->ssl));
  }
  while (ret == MBEDTLS_ERR_SSL_WANT_WRITE);
  
  if (___cid->mutex__ops != NULL) 
  {
    (*___cid->mutex__ops->lock)(___cid->mutex__ops->__instance);
  }
  pNetwork->eventFlags.disconnecting = true;
  if (___cid->mutex__ops != NULL) 
  {
    (*___cid->mutex__ops->unlock)(___cid->mutex__ops->__instance);
  }
}

void MbedTLSClientImpl_activity_run(void *___id)
{
  MbedTLSClientImpl__cdata_t *___cid = ((MbedTLSClientImpl__cdata_t *) ___id);
  for ( int16_t __i = 0 ; __i < MAX_SOCKET_CONNECTION_COUNT; __i++ )
  {
    MbedTLSNetwork_t *pNetwork = &___cid->networks__field[__i];
    if (pNetwork->busy) 
    {
      if (pNetwork->eventFlags.disconnecting) 
      {
        MbedTLSClientImpl_freeMbedTLSNetworkResources(pNetwork, false, ___cid);
        for ( uint8_t ___pc = 0 ; ___pc < MAX_CLIENT_SOCKET_HANDLER_COUNT; ___pc++ )
        {
          if (___cid->clientSocketHandlers__ops[___pc] != NULL && ___cid->clientSocketHandlers__ops[___pc]->__instance != NULL) 
          {
            (*___cid->clientSocketHandlers__ops[___pc]->disconnected)(pNetwork, ___cid->clientSocketHandlers__ops[___pc]->__instance);
          }
        }
        MbedTLSClientImpl_resetMbedTLSNetworkExtendedData(pNetwork, ___cid);
        continue;
      }
      if (pNetwork->eventFlags.connected) 
      {
        if (___cid->mutex__ops != NULL) 
        {
          (*___cid->mutex__ops->lock)(___cid->mutex__ops->__instance);
        }
        pNetwork->eventFlags.connected = false;
        if (___cid->mutex__ops != NULL) 
        {
          (*___cid->mutex__ops->unlock)(___cid->mutex__ops->__instance);
        }
        for ( uint8_t ___pc = 0 ; ___pc < MAX_CLIENT_SOCKET_HANDLER_COUNT; ___pc++ )
        {
          if (___cid->clientSocketHandlers__ops[___pc] != NULL && ___cid->clientSocketHandlers__ops[___pc]->__instance != NULL) 
          {
            (*___cid->clientSocketHandlers__ops[___pc]->connected)(pNetwork, ___cid->clientSocketHandlers__ops[___pc]->__instance);
          }
        }
        continue;
      }
      if (pNetwork->eventFlags.dataSent) 
      {
        
        if (___cid->mutex__ops != NULL) 
        {
          (*___cid->mutex__ops->lock)(___cid->mutex__ops->__instance);
        }
        pNetwork->eventFlags.dataSent = false;
        if (___cid->mutex__ops != NULL) 
        {
          (*___cid->mutex__ops->unlock)(___cid->mutex__ops->__instance);
        }
        for ( uint8_t ___pc = 0 ; ___pc < MAX_CLIENT_SOCKET_HANDLER_COUNT; ___pc++ )
        {
          if (___cid->clientSocketHandlers__ops[___pc] != NULL && ___cid->clientSocketHandlers__ops[___pc]->__instance != NULL) 
          {
            (*___cid->clientSocketHandlers__ops[___pc]->datagramSent)(pNetwork, ___cid->clientSocketHandlers__ops[___pc]->__instance);
          }
        }
        continue;
      }
      if (MbedTLSClientImpl_socket_isConnected(pNetwork, ___cid)) 
      {
        size_t maxFragmentLength = mbedtls_ssl_get_max_frag_len(&(pNetwork->pTLSDataParams->ssl));
        uint8_t fragment[maxFragmentLength];
        /* 
         * to be checked
         */
        int32_t ret = mbedtls_ssl_read(&(pNetwork->pTLSDataParams->ssl), fragment, maxFragmentLength);
        
        if (ret == MBEDTLS_ERR_SSL_WANT_READ || ret == MBEDTLS_ERR_SSL_WANT_WRITE || ret == MBEDTLS_ERR_SSL_TIMEOUT) 
        {
          continue;
        }
        if (ret == MBEDTLS_ERR_SSL_PEER_CLOSE_NOTIFY || ret == 0 || ret == MBEDTLS_ERR_NET_CONN_RESET) 
        {
          /* 
           * Can't use deleteMbedTLSNetwork directly as there may be another connection attempt inside of disconnected handler
           */
          MbedTLSClientImpl_freeMbedTLSNetworkResources(pNetwork, true, ___cid);
          for ( uint8_t ___pc = 0 ; ___pc < MAX_CLIENT_SOCKET_HANDLER_COUNT; ___pc++ )
          {
            if (___cid->clientSocketHandlers__ops[___pc] != NULL && ___cid->clientSocketHandlers__ops[___pc]->__instance != NULL) 
            {
              (*___cid->clientSocketHandlers__ops[___pc]->disconnected)(pNetwork, ___cid->clientSocketHandlers__ops[___pc]->__instance);
            }
          }
          MbedTLSClientImpl_resetMbedTLSNetworkExtendedData(pNetwork, ___cid);
          continue;
        }
        if (ret < 0) 
        {
          printf("Failed, mbedtls_ssl_read returned %i\n", ret);
          MbedTLSClientImpl_freeMbedTLSNetworkResources(pNetwork, true, ___cid);
          for ( uint8_t ___pc = 0 ; ___pc < MAX_CLIENT_SOCKET_HANDLER_COUNT; ___pc++ )
          {
            if (___cid->clientSocketHandlers__ops[___pc] != NULL && ___cid->clientSocketHandlers__ops[___pc]->__instance != NULL) 
            {
              (*___cid->clientSocketHandlers__ops[___pc]->connectionError)(pNetwork, SOCKET_ERROR_NODATA, ___cid->clientSocketHandlers__ops[___pc]->__instance);
            }
          }
          MbedTLSClientImpl_resetMbedTLSNetworkExtendedData(pNetwork, ___cid);
          continue;
        }
        Datagram_t datagram;
        initDatagram(&datagram, fragment, ((size_t)(ret)));
        for ( uint8_t ___pc = 0 ; ___pc < MAX_CLIENT_SOCKET_HANDLER_COUNT; ___pc++ )
        {
          if (___cid->clientSocketHandlers__ops[___pc] != NULL && ___cid->clientSocketHandlers__ops[___pc]->__instance != NULL) 
          {
            (*___cid->clientSocketHandlers__ops[___pc]->datagramReceived)(pNetwork, &datagram, ___cid->clientSocketHandlers__ops[___pc]->__instance);
          }
        }
      }
    }
  }
}
