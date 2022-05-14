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
#ifndef SINGLEPARTITIONAWSIOTTHINGSHADOWDATAPLANE_H
#define SINGLEPARTITIONAWSIOTTHINGSHADOWDATAPLANE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "JSONProcessorImpl.h"
#include "AWSIoTThingShadowDocumentDefinition.h"
#include "DemoFirmwareUpdateInfo.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_DEMO_FIRMWARE_UPDATE_VERSION_LENGTH 32

#define UPDATE_PARTITION_COUNT 2

struct SinglePartitionAWSIoTThingShadowIOHandlerImpl__cdata {
  IDemoFirmwareUpdateVersionInfo__idata_t firmwareUpdateVersionInfo__srdata;
};
typedef struct SinglePartitionAWSIoTThingShadowIOHandlerImpl__cdata SinglePartitionAWSIoTThingShadowIOHandlerImpl__cdata_t;

void SinglePartitionAWSIoTThingShadowIOHandlerImpl_init(void *___id);

bool SinglePartitionAWSIoTThingShadowIOHandlerImpl_jsonDocumentIOHandler_writeOutput(struct jsontree_context *pTreeCtx, struct jsontree_msg *pMsg, void *pUserData, void *___id);

void SinglePartitionAWSIoTThingShadowIOHandlerImpl_jsonDocumentIOHandler_readInput(struct jsontree_context *pTreeCtx, struct jsonparse_state *pParser, void *pUserData, void *___id);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
