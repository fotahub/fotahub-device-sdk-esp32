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
#include "JSONProcessorImpl.h"

void delegatingJSONTreeInputHandler(struct jsontree_context *pTreeCtx, struct jsonparse_state *pParseState)
{
  JSONParseCtx_t *pParseCtx = ((JSONParseCtx_t *)(jsontree_get_user(pTreeCtx)));
  if (pParseCtx != NULL && pParseCtx->documentIOHandler != NULL) 
  {
    (*pParseCtx->documentIOHandler->readInput)(pTreeCtx, pParseState, pParseCtx->pUserData, pParseCtx->documentIOHandler->__instance);
  }
}

int32_t delegatingJSONTreeOutputHandler(struct jsontree_context *pTreeCtx, struct jsontree_msg *pMsg)
{
  JSONSerializeCtx_t *pSerializeCtx = ((JSONSerializeCtx_t *)(jsontree_get_user(pTreeCtx)));
  if (pSerializeCtx != NULL && pSerializeCtx->documentIOHandler != NULL) 
  {
    return ((*pSerializeCtx->documentIOHandler->writeOutput)(pTreeCtx, pMsg, pSerializeCtx->pUserData, pSerializeCtx->documentIOHandler->__instance)) ? (1) : (0);
  }
  return 0;
}

void JSONProcessorImpl_init(void *___id)
{
  JSONProcessorImpl__cdata_t *___cid = ((JSONProcessorImpl__cdata_t *) ___id);
  ___cid->serializeCtx__field.busy = false;
}

JSONParseCtx_t *JSONProcessorImpl_newJSONParseCtx(void *___id)
{
  JSONProcessorImpl__cdata_t *___cid = ((JSONProcessorImpl__cdata_t *) ___id);
  if (!___cid->parseCtx__field.busy) 
  {
    ___cid->parseCtx__field.busy = true;
    return &___cid->parseCtx__field;
  }
  printf("No more JSON parse context blocks available\r\n");
  return NULL;
}

JSONSerializeCtx_t *JSONProcessorImpl_newJSONSerializeCtx(char *msg, size_t msgSize, void *___id)
{
  JSONProcessorImpl__cdata_t *___cid = ((JSONProcessorImpl__cdata_t *) ___id);
  if (!___cid->serializeCtx__field.busy) 
  {
    ___cid->serializeCtx__field.busy = true;
    ___cid->serializeCtx__field.msgState.base = msg;
    ___cid->serializeCtx__field.msgState.size = msgSize;
    ___cid->serializeCtx__field.msgState.pos = 0;
    return &___cid->serializeCtx__field;
  }
  printf("No more JSON serialize context blocks available\r\n");
  return NULL;
}

void JSONProcessorImpl_deleteJSONProcessorCtx(JSONProcessorCtx_t *pProcessorCtx, void *___id)
{
  pProcessorCtx->busy = false;
}

bool JSONProcessorImpl_jsonParser_parse(struct jsontree_value *pDocumentModel, IJSONDocumentIOHandler__idata_t *documentIOHandler, void *pUserData, char *msg, size_t msgLength, void *___id)
{
  JSONProcessorImpl__cdata_t *___cid = ((JSONProcessorImpl__cdata_t *) ___id);
  if (msg == NULL) 
  {
    printf("JSON message buffer must not be NULL\r\n");
  }
  JSONParseCtx_t *pParseCtx = JSONProcessorImpl_newJSONParseCtx(___cid);
  if (pParseCtx == NULL) 
  {
    return false;
  }
  pParseCtx->documentIOHandler = documentIOHandler;
  pParseCtx->pUserData = pUserData;
  
  jsontree_setup(&pParseCtx->treeCtx, pDocumentModel, pParseCtx);
  jsonparse_setup(&pParseCtx->parseState, msg, ((int32_t) msgLength));
  
  bool result = jsonparse_parse_value(&pParseCtx->treeCtx, &pParseCtx->parseState);
  
  JSONProcessorImpl_deleteJSONProcessorCtx(((JSONProcessorCtx_t *)(pParseCtx)), ___cid);
  return result;
}

void JSONProcessorImpl_jsonParser_resetFlags(struct jsontree_value *pDocumentModel, void *___id)
{
  jsontree_reset_flags(pDocumentModel, true);
}

bool JSONProcessorImpl_jsonSerializer_serialize(struct jsontree_value *pDocumentModel, JSONObjectSerializationMode_t mode, IJSONDocumentIOHandler__idata_t *documentIOHandler, void *pUserData, char *msg, size_t msgSize, void *___id)
{
  JSONProcessorImpl__cdata_t *___cid = ((JSONProcessorImpl__cdata_t *) ___id);
  if (msg == NULL) 
  {
    printf("JSON message buffer must not be NULL\r\n");
    return false;
  }
  JSONSerializeCtx_t *pSerializeCtx = JSONProcessorImpl_newJSONSerializeCtx(msg, msgSize, ___cid);
  if (pSerializeCtx == NULL) 
  {
    return false;
  }
  pSerializeCtx->documentIOHandler = documentIOHandler;
  pSerializeCtx->pUserData = pUserData;
  
  jsontree_setup(&pSerializeCtx->treeCtx, pDocumentModel, pSerializeCtx);
  
  if (!jsontree_print_current_value(&pSerializeCtx->treeCtx, &pSerializeCtx->msgState, mode == MODE_MODIFIED_ATTRIBUTES)) 
  {
    JSONProcessorImpl_deleteJSONProcessorCtx(((JSONProcessorCtx_t *)(pSerializeCtx)), ___cid);
    return false;
  }
  jsontree_write_terminate_char(&pSerializeCtx->msgState);
  
  JSONProcessorImpl_deleteJSONProcessorCtx(((JSONProcessorCtx_t *)(pSerializeCtx)), ___cid);
  return true;
}

void const* JSONProcessorImpl_jsonSerializer_beginDocument(struct jsontree_value *pDocumentModel, IJSONDocumentIOHandler__idata_t *documentIOHandler, void *pUserData, char *msg, size_t msgSize, void *___id)
{
  JSONProcessorImpl__cdata_t *___cid = ((JSONProcessorImpl__cdata_t *) ___id);
  if (msg == NULL) 
  {
    printf("JSON message buffer must not be NULL\r\n");
    return NULL;
  }
  JSONSerializeCtx_t *pSerializeCtx = JSONProcessorImpl_newJSONSerializeCtx(msg, msgSize, ___cid);
  if (pSerializeCtx == NULL) 
  {
    return NULL;
  }
  pSerializeCtx->documentIOHandler = documentIOHandler;
  pSerializeCtx->pUserData = pUserData;
  
  jsontree_setup(&pSerializeCtx->treeCtx, pDocumentModel, pSerializeCtx);
  return pSerializeCtx;
}

bool JSONProcessorImpl_jsonSerializer_beginObject(void const* hDocument, char const *path, void *___id)
{
  JSONProcessorImpl__cdata_t *___cid = ((JSONProcessorImpl__cdata_t *) ___id);
  if (hDocument == NULL) 
  {
    printf("JSON document handle must not be NULL\r\n");
    return false;
  }
  JSONSerializeCtx_t *pSerializeCtx = ((JSONSerializeCtx_t *) hDocument);
  
  if (!jsontree_print_path(&pSerializeCtx->treeCtx, &pSerializeCtx->msgState, path)) 
  {
    JSONProcessorImpl_deleteJSONProcessorCtx(((JSONProcessorCtx_t *)(pSerializeCtx)), ___cid);
    return false;
  }
  jsontree_write_terminate_char(&pSerializeCtx->msgState);
  return true;
}

bool JSONProcessorImpl_jsonSerializer_addValue(void const* hDocument, char const *path, JSONObjectSerializationMode_t mode, void *___id)
{
  JSONProcessorImpl__cdata_t *___cid = ((JSONProcessorImpl__cdata_t *) ___id);
  if (hDocument == NULL) 
  {
    printf("JSON document handle must not be NULL\r\n");
    return false;
  }
  JSONSerializeCtx_t *pSerializeCtx = ((JSONSerializeCtx_t *) hDocument);
  
  if (!jsontree_print_path(&pSerializeCtx->treeCtx, &pSerializeCtx->msgState, path)) 
  {
    JSONProcessorImpl_deleteJSONProcessorCtx(((JSONProcessorCtx_t *)(pSerializeCtx)), ___cid);
    return false;
  }
  if (!jsontree_print_current_value(&pSerializeCtx->treeCtx, &pSerializeCtx->msgState, mode == MODE_MODIFIED_ATTRIBUTES)) 
  {
    JSONProcessorImpl_deleteJSONProcessorCtx(((JSONProcessorCtx_t *)(pSerializeCtx)), ___cid);
    return false;
  }
  jsontree_write_terminate_char(&pSerializeCtx->msgState);
  return true;
}

bool JSONProcessorImpl_jsonSerializer_endObject(void const* hDocument, void *___id)
{
  JSONProcessorImpl__cdata_t *___cid = ((JSONProcessorImpl__cdata_t *) ___id);
  if (hDocument == NULL) 
  {
    printf("JSON document handle must not be NULL\r\n");
    return false;
  }
  JSONSerializeCtx_t *pSerializeCtx = ((JSONSerializeCtx_t *) hDocument);
  
  if (!jsontree_print_object_endings(&pSerializeCtx->treeCtx, &pSerializeCtx->msgState, 1)) 
  {
    JSONProcessorImpl_deleteJSONProcessorCtx(((JSONProcessorCtx_t *)(pSerializeCtx)), ___cid);
    return false;
  }
  jsontree_write_terminate_char(&pSerializeCtx->msgState);
  return true;
}

bool JSONProcessorImpl_jsonSerializer_endDocument(void const* hDocument, void *___id)
{
  JSONProcessorImpl__cdata_t *___cid = ((JSONProcessorImpl__cdata_t *) ___id);
  if (hDocument == NULL) 
  {
    printf("JSON document handle must not be NULL\r\n");
    return false;
  }
  JSONSerializeCtx_t *pSerializeCtx = ((JSONSerializeCtx_t *) hDocument);
  
  if (!jsontree_print_object_endings(&pSerializeCtx->treeCtx, &pSerializeCtx->msgState, JSONTREE_OBJECT_ENDINGS_COUNT_ALL)) 
  {
    JSONProcessorImpl_deleteJSONProcessorCtx(((JSONProcessorCtx_t *)(pSerializeCtx)), ___cid);
    return false;
  }
  jsontree_write_terminate_char(&pSerializeCtx->msgState);
  
  JSONProcessorImpl_deleteJSONProcessorCtx(((JSONProcessorCtx_t *)(pSerializeCtx)), ___cid);
  return true;
}
