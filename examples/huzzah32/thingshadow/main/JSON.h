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
#ifndef JSON_H
#define JSON_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "JSONDefinitions.h"
#include <jsontree.h>
#include <jsonparse.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct IJSONDocumentIOHandler__idata IJSONDocumentIOHandler__idata_t;

struct IJSONParser__idata {
  void *__instance;
  bool ((*(parse))(struct jsontree_value *,IJSONDocumentIOHandler__idata_t *,void *,char *,size_t,void *));
  void ((*(resetFlags))(struct jsontree_value *,void *));
};
typedef struct IJSONParser__idata IJSONParser__idata_t;

struct IJSONSerializer__idata {
  void *__instance;
  bool ((*(serialize))(struct jsontree_value *,JSONObjectSerializationMode_t,IJSONDocumentIOHandler__idata_t *,void *,char *,size_t,void *));
  void const* ((*(beginDocument))(struct jsontree_value *,IJSONDocumentIOHandler__idata_t *,void *,char *,size_t,void *));
  bool ((*(beginObject))(void const*,char const *,void *));
  bool ((*(addValue))(void const*,char const *,JSONObjectSerializationMode_t,void *));
  bool ((*(endObject))(void const*,void *));
  bool ((*(endDocument))(void const*,void *));
};
typedef struct IJSONSerializer__idata IJSONSerializer__idata_t;

struct IJSONDocumentIOHandler__idata {
  void *__instance;
  bool ((*(writeOutput))(struct jsontree_context *,struct jsontree_msg *,void *,void *));
  void ((*(readInput))(struct jsontree_context *,struct jsonparse_state *,void *,void *));
};

struct IJSONDocumentChangeHandler__idata {
  void *__instance;
  void ((*(documentChanged))(void *));
};
typedef struct IJSONDocumentChangeHandler__idata IJSONDocumentChangeHandler__idata_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
