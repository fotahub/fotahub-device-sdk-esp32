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
#ifndef JSONTREEEXTENSIONS_H
#define JSONTREEEXTENSIONS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "JSONDefinitions.h"
#include <jsontree.h>
#include <jsonparse.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define JSONTREE_OBJECT_ENDINGS_COUNT_ALL (((uint8_t)(EOF)))

bool jsontree_print_path(struct jsontree_context *js_ctx, struct jsontree_msg *msg, char const *path);

bool jsontree_print_current_attribute_name(struct jsontree_context *js_ctx, struct jsontree_msg *msg);

bool jsontree_print_current_value(struct jsontree_context *js_ctx, struct jsontree_msg *msg, bool modifiedOnly);

bool jsontree_print_object_endings(struct jsontree_context *js_ctx, struct jsontree_msg *msg, uint8_t count);

bool jsonparse_parse_value(struct jsontree_context *js_ctx, struct jsonparse_state *parseState);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
