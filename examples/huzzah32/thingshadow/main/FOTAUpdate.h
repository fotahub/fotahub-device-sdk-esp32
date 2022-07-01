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
#ifndef FOTAUPDATE_H
#define FOTAUPDATE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "FOTAUpdateDefinitions.h"

#ifdef __cplusplus
extern "C" {
#endif

struct IFOTAUpdateEngine__idata {
  void *__instance;
  bool ((*(downloadUpdate))(FirmwareUpdateInfo_t *,void *));
  bool ((*(applyUpdate))(FirmwareUpdateInfo_t *,void *));
  bool ((*(confirmUpdate))(FirmwareUpdateInfo_t *,void *));
  bool ((*(rollBackUpdate))(FirmwareUpdateInfo_t *,void *));
};
typedef struct IFOTAUpdateEngine__idata IFOTAUpdateEngine__idata_t;

struct IFOTAUpdateStatusHandler__idata {
  void *__instance;
  void ((*(updateStatus))(FOTAUpdateStatus_t,void *));
};
typedef struct IFOTAUpdateStatusHandler__idata IFOTAUpdateStatusHandler__idata_t;

struct IFirmwareUpdateVerificationInfoProvider__idata {
  void *__instance;
  void ((*(getUpdateVerificationInfo))(FirmwareUpdateInfo_t const *,FOTAUpdateVerificationAlgorithm_t *,uint8_t const  **,void *));
};
typedef struct IFirmwareUpdateVerificationInfoProvider__idata IFirmwareUpdateVerificationInfoProvider__idata_t;

struct IFirmwareUpdateHandler__idata {
  void *__instance;
  bool ((*(canHandle))(FirmwareUpdateInfo_t const *,void *));
  bool ((*(reconcileUpdateInfo))(FirmwareUpdateInfo_t *,void *));
  void const* ((*(beginWriteUpdate))(FirmwareUpdateInfo_t const *,size_t,void *));
  size_t ((*(writeUpdateChunk))(void const*,uint8_t const  *,size_t,void *));
  bool ((*(endWriteUpdate))(void const*,void *));
  bool ((*(applyUpdate))(FirmwareUpdateInfo_t const *,void *));
  bool ((*(confirmUpdate))(FirmwareUpdateInfo_t const *,void *));
  bool ((*(rollBackUpdate))(FirmwareUpdateInfo_t const *,void *));
  FOTAUpdateStatus_t ((*(getUpdateStatus))(FirmwareUpdateInfo_t const *,void *));
};
typedef struct IFirmwareUpdateHandler__idata IFirmwareUpdateHandler__idata_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
