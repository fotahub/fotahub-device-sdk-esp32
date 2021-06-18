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
#include "AWSIoTThingShadowDocumentDefinition.h"

struct jsontree_callback updateCurrentVersion = { JSON_TYPE_CALLBACK, false, &delegatingJSONTreeOutputHandler, &delegatingJSONTreeInputHandler };

struct jsontree_callback updateNewVersion = { JSON_TYPE_CALLBACK, false, &delegatingJSONTreeOutputHandler, &delegatingJSONTreeInputHandler };

struct jsontree_callback AWSIoTThingShadowDocumentDefinition_updateVerificationData = { JSON_TYPE_CALLBACK, false, &delegatingJSONTreeOutputHandler, &delegatingJSONTreeInputHandler };

struct jsontree_object nullObj = { JSON_TYPE_OBJECT, false, true, 0, NULL };

struct jsontree_pair attributeUpdateStatePairs[] = 
{
  { AWS_IOT_THING_SHADOW_JSON_ATTRIBUTE_UPDATE_CURRENT_VERSION, ((struct jsontree_value *)(&updateCurrentVersion)) }
};

struct jsontree_object attributeUpdateStateObj = { JSON_TYPE_OBJECT, false, false, ((uint8_t)((sizeof(attributeUpdateStatePairs) / sizeof(struct jsontree_pair)))), attributeUpdateStatePairs };

struct jsontree_pair statePairs[] = 
{
  { AWS_IOT_THING_SHADOW_JSON_REPORTED, ((struct jsontree_value *)(&attributeUpdateStateObj)) }, 
  { AWS_IOT_THING_SHADOW_JSON_DESIRED, ((struct jsontree_value *)(&nullObj)) }
};

struct jsontree_object stateObj = { JSON_TYPE_OBJECT, false, false, ((uint8_t)((sizeof(statePairs) / sizeof(struct jsontree_pair)))), statePairs };

struct jsontree_pair requestStateDocPairs[] = 
{
  { AWS_IOT_THING_SHADOW_JSON_STATE, ((struct jsontree_value *)(&stateObj)) }
};

struct jsontree_object requestStateDoc = { JSON_TYPE_OBJECT, false, false, ((uint8_t)((sizeof(requestStateDocPairs) / sizeof(struct jsontree_pair)))), requestStateDocPairs };

struct jsontree_pair attributeUpdateTriggerPairs[] = 
{
  { AWS_IOT_THING_SHADOW_JSON_ATTRIBUTE_UPDATE_NEW_VERSION, ((struct jsontree_value *)(&updateNewVersion)) }, 
  { AWS_IOT_THING_SHADOW_JSON_ATTRIBUTE_UPDATE_VERIFICATION_DATA, ((struct jsontree_value *)(&AWSIoTThingShadowDocumentDefinition_updateVerificationData)) }
};

struct jsontree_object attributeUpdateTriggerObj = { JSON_TYPE_OBJECT, false, false, ((uint8_t)((sizeof(attributeUpdateTriggerPairs) / sizeof(struct jsontree_pair)))), attributeUpdateTriggerPairs };

struct jsontree_pair deltaStateDocPairs[] = 
{
  { AWS_IOT_THING_SHADOW_JSON_STATE, ((struct jsontree_value *)(&attributeUpdateTriggerObj)) }
};

/* 
 * Handles every json with 'state' field in it. 
 * This contains /desired, /reported, and /delta
 */
struct jsontree_object deltaStateDoc = { JSON_TYPE_OBJECT, false, false, ((uint8_t)((sizeof(deltaStateDocPairs) / sizeof(struct jsontree_pair)))), deltaStateDocPairs };
