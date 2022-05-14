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
#ifndef JSONPROCESSORIMPL_H
#define JSONPROCESSORIMPL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "JSON.h"
#include "JSONTreeExtensions.h"
#include <jsonparse.h>

#ifdef __cplusplus
extern "C" {
#endif

struct JSONProcessorCtx {
  bool busy;
};
typedef struct JSONProcessorCtx JSONProcessorCtx_t;

struct JSONParseCtx {
  bool busy;
  struct jsontree_context treeCtx;
  struct jsonparse_state parseState;
  IJSONDocumentIOHandler__idata_t *documentIOHandler;
  void *pUserData;
};
typedef struct JSONParseCtx JSONParseCtx_t;

struct JSONSerializeCtx {
  bool busy;
  struct jsontree_context treeCtx;
  struct jsontree_msg msgState;
  IJSONDocumentIOHandler__idata_t *documentIOHandler;
  void *pUserData;
};
typedef struct JSONSerializeCtx JSONSerializeCtx_t;

struct JSONProcessorImpl__cdata {
  /* 
   * fields
   */
  JSONParseCtx_t parseCtx__field;
  /* 
   * fields
   */
  JSONSerializeCtx_t serializeCtx__field;
};
typedef struct JSONProcessorImpl__cdata JSONProcessorImpl__cdata_t;

void delegatingJSONTreeInputHandler(struct jsontree_context *pTreeCtx, struct jsonparse_state *pParseState);

int32_t delegatingJSONTreeOutputHandler(struct jsontree_context *pTreeCtx, struct jsontree_msg *pMsg);

void JSONProcessorImpl_init(void *___id);

JSONParseCtx_t *JSONProcessorImpl_newJSONParseCtx(void *___id);

JSONSerializeCtx_t *JSONProcessorImpl_newJSONSerializeCtx(char *msg, size_t msgSize, void *___id);

void JSONProcessorImpl_deleteJSONProcessorCtx(JSONProcessorCtx_t *pProcessorCtx, void *___id);

bool JSONProcessorImpl_jsonParser_parse(struct jsontree_value *pDocumentModel, IJSONDocumentIOHandler__idata_t *documentIOHandler, void *pUserData, char *msg, size_t msgLength, void *___id);

void JSONProcessorImpl_jsonParser_resetFlags(struct jsontree_value *pDocumentModel, void *___id);

bool JSONProcessorImpl_jsonSerializer_serialize(struct jsontree_value *pDocumentModel, JSONObjectSerializationMode_t mode, IJSONDocumentIOHandler__idata_t *documentIOHandler, void *pUserData, char *msg, size_t msgSize, void *___id);

const void* JSONProcessorImpl_jsonSerializer_beginDocument(struct jsontree_value *pDocumentModel, IJSONDocumentIOHandler__idata_t *documentIOHandler, void *pUserData, char *msg, size_t msgSize, void *___id);

bool JSONProcessorImpl_jsonSerializer_beginObject(const void* hDocument, char const *path, void *___id);

bool JSONProcessorImpl_jsonSerializer_addValue(const void* hDocument, char const *path, JSONObjectSerializationMode_t mode, void *___id);

bool JSONProcessorImpl_jsonSerializer_endObject(const void* hDocument, void *___id);

bool JSONProcessorImpl_jsonSerializer_endDocument(const void* hDocument, void *___id);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
