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
#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ITimer__idata {
  void *__instance;
  void ((*(set))(uint32_t,bool,void *));
  bool ((*(isExpired))(void *));
  void ((*(cancel))(void *));
};
typedef struct ITimer__idata ITimer__idata_t;

struct ITimerHandler__idata {
  void *__instance;
  void ((*(expired))(void *));
};
typedef struct ITimerHandler__idata ITimerHandler__idata_t;

struct IDelayedFunctionTimer__idata {
  void *__instance;
  void ((*(schedule))(uint8_t,void *));
  void ((*(cancel))(uint8_t,void *));
};
typedef struct IDelayedFunctionTimer__idata IDelayedFunctionTimer__idata_t;

struct ILowPowerDelayGenerator__idata {
  void *__instance;
  void ((*(generateLowPowerDelay))(uint16_t,uint16_t,void *));
};
typedef struct ILowPowerDelayGenerator__idata ILowPowerDelayGenerator__idata_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
