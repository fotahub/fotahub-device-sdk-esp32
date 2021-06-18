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
#include "SinglePartitionAWSIoTThingShadowDataPlane.h"

#include "FotaHub.h"
#include "DemoProductInfo.h"
#include "BinHexConverter.h"
#include <stdlib.h>
#include <string.h>

static char updateVersion[MAX_DEMO_FIRMWARE_UPDATE_VERSION_LENGTH + 1] = { 0 };

static uint8_t *SinglePartitionAWSIoTThingShadowDataPlane_updateVerificationData = NULL;

void fotahub_onProvideFirmwareUpdateVerificationInfo(FirmwareUpdateInfo_t const *pUpdateInfo, FOTAUpdateVerificationAlgorithm_t *pVerificationAlgorithm, uint8_t const  **ppVerificationData)
{
  *pVerificationAlgorithm = DEMO_PRODUCT_FIRMWARE_UPDATE_VERIFICATION_ALGORITHM;
  *ppVerificationData = SinglePartitionAWSIoTThingShadowDataPlane_updateVerificationData;
}

void SinglePartitionAWSIoTThingShadowIOHandlerImpl_init(void *___id)
{
  SinglePartitionAWSIoTThingShadowIOHandlerImpl__cdata_t *___cid = ((SinglePartitionAWSIoTThingShadowIOHandlerImpl__cdata_t *) ___id);
  ___cid->firmwareUpdateVersionInfo__srdata.updateVersion = updateVersion;
  
  size_t updateVerificationDataSize = getVerificationDataSize(DEMO_PRODUCT_FIRMWARE_UPDATE_VERIFICATION_ALGORITHM);
  SinglePartitionAWSIoTThingShadowDataPlane_updateVerificationData = ((uint8_t *)(malloc(updateVerificationDataSize)));
  memset(SinglePartitionAWSIoTThingShadowDataPlane_updateVerificationData, 0, updateVerificationDataSize);
}

bool SinglePartitionAWSIoTThingShadowIOHandlerImpl_jsonDocumentIOHandler_writeOutput(struct jsontree_context *pTreeCtx, struct jsontree_msg *pMsg, void *pUserData, void *___id)
{
  char const *path = jsontree_path_name(pTreeCtx);
  if (strncmp(path, AWS_IOT_THING_SHADOW_JSON_ATTRIBUTE_UPDATE_CURRENT_VERSION, strlen(AWS_IOT_THING_SHADOW_JSON_ATTRIBUTE_UPDATE_CURRENT_VERSION)) == 0) 
  {
    jsontree_write_string(pMsg, DEMO_PRODUCT_FIRMWARE_VERSION, ((int32_t)(strlen(DEMO_PRODUCT_FIRMWARE_VERSION))));
  }
  
  /* 
   * Return false to say that we don't need to output any other siblings (object attributes or array items) than we actually did at this time
   */
  return false;
}

void SinglePartitionAWSIoTThingShadowIOHandlerImpl_jsonDocumentIOHandler_readInput(struct jsontree_context *pTreeCtx, struct jsonparse_state *pParser, void *pUserData, void *___id)
{
  char const *path = jsontree_path_name(pTreeCtx);
  if (strncmp(path, AWS_IOT_THING_SHADOW_JSON_ATTRIBUTE_UPDATE_NEW_VERSION, strlen(AWS_IOT_THING_SHADOW_JSON_ATTRIBUTE_UPDATE_NEW_VERSION)) == 0) 
  {
    jsonparse_copy_value_as_string(pParser, updateVersion, ((int32_t)(sizeof(updateVersion))));
  }
  else if (strncmp(path, AWS_IOT_THING_SHADOW_JSON_ATTRIBUTE_UPDATE_VERIFICATION_DATA, strlen(AWS_IOT_THING_SHADOW_JSON_ATTRIBUTE_UPDATE_VERIFICATION_DATA)) == 0) {
    char const *value = jsonparse_get_value(pParser);
    size_t valueLength = ((size_t)(jsonparse_get_len(pParser)));
    size_t updateVerificationDataSize = getVerificationDataSize(DEMO_PRODUCT_FIRMWARE_UPDATE_VERIFICATION_ALGORITHM);
    convertHexString8ToBinData(((char *)(value)), valueLength, SinglePartitionAWSIoTThingShadowDataPlane_updateVerificationData, updateVerificationDataSize);
  }
}
