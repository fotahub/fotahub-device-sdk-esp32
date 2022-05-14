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
#ifndef X509_H
#define X509_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "X509Definitions.h"

#ifdef __cplusplus
extern "C" {
#endif

struct IX509DataProvider__idata {
  void *__instance;
  bool ((*(storeX509DataItem))(X509DataItemKind_t,uint8_t const  *,size_t,void *));
  size_t ((*(getX509RootCACount))(void *));
  bool ((*(loadRootCA))(uint8_t,uint8_t const  **,size_t *,void *));
  bool ((*(loadCertificate))(uint8_t const  **,size_t *,void *));
  bool ((*(loadPrivateKey))(uint8_t const  **,size_t *,void *));
  bool ((*(loadClientId))(uint8_t const  **,size_t *,void *));
  void ((*(unloadX509DataItem))(uint8_t const  **,void *));
};
typedef struct IX509DataProvider__idata IX509DataProvider__idata_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
