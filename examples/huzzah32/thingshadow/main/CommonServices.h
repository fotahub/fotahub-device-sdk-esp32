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
#ifndef COMMONSERVICES_H
#define COMMONSERVICES_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct IRunnable__idata {
  void *__instance;
  void ((*(run))(void *));
};
typedef struct IRunnable__idata IRunnable__idata_t;

struct IDisposable__idata {
  void *__instance;
  void ((*(dispose))(void *));
};
typedef struct IDisposable__idata IDisposable__idata_t;

struct IResettable__idata {
  void *__instance;
  void ((*(reset))(void *));
};
typedef struct IResettable__idata IResettable__idata_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
