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
#ifndef SINGLEPARTITIONDEMOFIRMWAREUPDATEINFOSERIALREADER_H
#define SINGLEPARTITIONDEMOFIRMWAREUPDATEINFOSERIALREADER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define COLON (((uint8_t) ':'))

#define LF (((uint8_t) '\n'))

#define MAX_DEMO_FIRMWARE_UPDATE_VERSION_LENGTH 32

void firmwareUpdateInfoReader_init(void);

void firmwareUpdateInfoReader_run(void);

void firmwareUpdateInfoReader_onCharacterReceived(uint8_t character);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
