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
#ifndef MBEDTLSDEFINITIONS_H
#define MBEDTLSDEFINITIONS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "mbedtls/net_sockets.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/ssl.h"
#include "mbedtls/x509.h"
#include "mbedtls/pk.h"
#include "mbedtls/debug.h"

#ifdef __cplusplus
extern "C" {
#endif

struct TLSDataParams {
  mbedtls_entropy_context entropy;
  mbedtls_ctr_drbg_context ctrDrbg;
  mbedtls_ssl_context ssl;
  mbedtls_ssl_config conf;
  uint32_t flags;
  mbedtls_x509_crt serverCert;
  mbedtls_x509_crt clientCert;
  mbedtls_pk_context privateKey;
  mbedtls_net_context serverFD;
  mbedtls_net_context clientFD;
};
typedef struct TLSDataParams TLSDataParams_t;

#define CLIENT_RECEIVE_MAX_BUFFER_SIZE 4096

#define SERVER_RECEIVE_MAX_BUFFER_SIZE 4096

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
