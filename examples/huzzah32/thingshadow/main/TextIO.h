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
#ifndef TEXTIO_H
#define TEXTIO_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ITextReader__idata {
  void *__instance;
  char ((*(readChar))(const void*,void *));
};
typedef struct ITextReader__idata ITextReader__idata_t;

struct ITextWriter__idata {
  void *__instance;
  void ((*(writeChar))(const void*,char,void *));
};
typedef struct ITextWriter__idata ITextWriter__idata_t;

typedef size_t ((*(textIoEnumTextEncoderFunc))(uint32_t,char *,size_t));

typedef uint32_t ((*(textIoEnumTextDecoderFunc))(char const *,size_t));

struct IDataTextEncoder__idata {
  void *__instance;
  void ((*(encode))(void const *,size_t,char *,size_t,void *));
  void ((*(decode))(char const *,size_t,void *,size_t,void *));
};
typedef struct IDataTextEncoder__idata IDataTextEncoder__idata_t;

typedef size_t ((*(textIoDataTextEncoderFunc))(void const *,size_t,char *,size_t));

typedef size_t ((*(textIoDataTextDecoderFunc))(char const *,size_t,void *,size_t));

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
