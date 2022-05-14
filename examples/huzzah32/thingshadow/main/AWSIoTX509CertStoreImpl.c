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
#include "AWSIoTX509CertStoreImpl.h"

#include <string.h>

extern uint8_t const  AWSIoTX509CertStoreImpl_AWS_IOT_ROOT_CA_1_start[] asm("_binary_AmazonRootCA1_pem_start");
extern uint8_t const  AWSIoTX509CertStoreImpl_AWS_IOT_ROOT_CA_1_end[] asm("_binary_AmazonRootCA1_pem_end");

extern uint8_t const  AWSIoTX509CertStoreImpl_AWS_IOT_ROOT_CA_3_start[] asm("_binary_AmazonRootCA3_pem_start");
extern uint8_t const  AWSIoTX509CertStoreImpl_AWS_IOT_ROOT_CA_3_end[] asm("_binary_AmazonRootCA3_pem_end");

extern uint8_t const  AWSIoTX509CertStoreImpl_CERTIFICATE_start[] asm("_binary_Certificate_pem_start");
extern uint8_t const  AWSIoTX509CertStoreImpl_CERTIFICATE_end[] asm("_binary_Certificate_pem_end");

extern uint8_t const  AWSIoTX509CertStoreImpl_PRIVATE_KEY_start[] asm("_binary_PrivateKey_pem_start");
extern uint8_t const  AWSIoTX509CertStoreImpl_PRIVATE_KEY_end[] asm("_binary_PrivateKey_pem_end");

bool AWSIoTX509CertStoreImpl_x509DataProvider_storeX509DataItem(X509DataItemKind_t kind, uint8_t const  *pDataItem, size_t dataItemSize, void *___id)
{
  return true;
}

bool AWSIoTX509CertStoreImpl_x509DataProvider_loadRootCA(uint8_t index, uint8_t const  **ppDataItem, size_t *pDataItemSize, void *___id)
{
  if (index == 0) 
  {
    *ppDataItem = ((uint8_t const  *)(AWSIoTX509CertStoreImpl_AWS_IOT_ROOT_CA_1_start));
    *pDataItemSize = (AWSIoTX509CertStoreImpl_AWS_IOT_ROOT_CA_1_end - AWSIoTX509CertStoreImpl_AWS_IOT_ROOT_CA_1_start);
  }
  else if (index == 1) {
    *ppDataItem = ((uint8_t const  *)(AWSIoTX509CertStoreImpl_AWS_IOT_ROOT_CA_3_start));
    *pDataItemSize = (AWSIoTX509CertStoreImpl_AWS_IOT_ROOT_CA_3_end - AWSIoTX509CertStoreImpl_AWS_IOT_ROOT_CA_3_start);
  }
  return true;
}

void AWSIoTX509CertStoreImpl_x509DataProvider_unloadX509DataItem(uint8_t const  **ppDataItem, void *___id)
{
  
}

bool AWSIoTX509CertStoreImpl_x509DataProvider_loadCertificate(uint8_t const  **ppDataItem, size_t *pDataItemSize, void *___id)
{
  *ppDataItem = ((uint8_t const  *)(AWSIoTX509CertStoreImpl_CERTIFICATE_start));
  *pDataItemSize = (AWSIoTX509CertStoreImpl_CERTIFICATE_end - AWSIoTX509CertStoreImpl_CERTIFICATE_start);
  return true;
}

bool AWSIoTX509CertStoreImpl_x509DataProvider_loadPrivateKey(uint8_t const  **ppDataItem, size_t *pDataItemSize, void *___id)
{
  *ppDataItem = ((uint8_t const  *)(AWSIoTX509CertStoreImpl_PRIVATE_KEY_start));
  *pDataItemSize = (AWSIoTX509CertStoreImpl_PRIVATE_KEY_end - AWSIoTX509CertStoreImpl_PRIVATE_KEY_start);
  return true;
}

size_t AWSIoTX509CertStoreImpl_x509DataProvider_getX509RootCACount(void *___id)
{
  return 2;
}

bool AWSIoTX509CertStoreImpl_x509DataProvider_loadClientId(uint8_t const  **ppDataItem, size_t *pDataItemSize, void *___id)
{
  return false;
}
