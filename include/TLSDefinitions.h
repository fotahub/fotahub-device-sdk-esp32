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
#ifndef TLSDEFINITIONS_H
#define TLSDEFINITIONS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

enum TLSMaxFragmentLength {
  TLS_MAX_FRAGMENT_LENGTH_DISABLED = 0,
  TLS_MAX_FRAGMENT_LENGTH_512 = 512,
  TLS_MAX_FRAGMENT_LENGTH_1024 = 1024,
  TLS_MAX_FRAGMENT_LENGTH_2048 = 2048,
  TLS_MAX_FRAGMENT_LENGTH_4096 = 4096
};
typedef enum TLSMaxFragmentLength TLSMaxFragmentLength_t;

#define TLS_DEFAULT_FRAGMENT_LENGTH 16384

#define TLS_MAX_DATA_RATE_NONE 0

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
