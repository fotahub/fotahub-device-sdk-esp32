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
#ifndef AWSIOTX509CERTSTOREIMPL_H
#define AWSIOTX509CERTSTOREIMPL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "X509.h"

#ifdef __cplusplus
extern "C" {
#endif

struct AWSIoTX509CertStoreImpl__cdata {
  uint8_t aMemberSoTheStructIsNotEmpty;
};
typedef struct AWSIoTX509CertStoreImpl__cdata AWSIoTX509CertStoreImpl__cdata_t;

bool AWSIoTX509CertStoreImpl_x509DataProvider_storeX509DataItem(X509DataItemKind_t kind, uint8_t const  *pDataItem, size_t dataItemSize, void *___id);

bool AWSIoTX509CertStoreImpl_x509DataProvider_loadRootCA(uint8_t index, uint8_t const  **ppDataItem, size_t *pDataItemSize, void *___id);

void AWSIoTX509CertStoreImpl_x509DataProvider_unloadX509DataItem(uint8_t const  **ppDataItem, void *___id);

bool AWSIoTX509CertStoreImpl_x509DataProvider_loadCertificate(uint8_t const  **ppDataItem, size_t *pDataItemSize, void *___id);

bool AWSIoTX509CertStoreImpl_x509DataProvider_loadPrivateKey(uint8_t const  **ppDataItem, size_t *pDataItemSize, void *___id);

size_t AWSIoTX509CertStoreImpl_x509DataProvider_getX509RootCACount(void *___id);

bool AWSIoTX509CertStoreImpl_x509DataProvider_loadClientId(uint8_t const  **ppDataItem, size_t *pDataItemSize, void *___id);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
