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
#include "JSONTreeExtensions.h"

#include <string.h>

static struct jsontree_value *jsontree_next_array_item_value(struct jsontree_context *js_ctx);

static void jsontree_enter_value(struct jsontree_context *js_ctx, struct jsontree_value *value);

static struct jsontree_value *jsontree_exit_value(struct jsontree_context *js_ctx);

static struct jsontree_value *jsontree_find_pair_value(struct jsontree_context *js_ctx, char const *pairName, size_t pairNameLength);

static bool jsonparse_do_parse_value(struct jsontree_context *js_ctx, struct jsonparse_state *parseState);

bool jsontree_print_path(struct jsontree_context *js_ctx, struct jsontree_msg *msg, char const *path)
{
  struct jsontree_value *value = js_ctx->values[js_ctx->depth];
  js_ctx->callback_state = 0;
  
  char const *segment = path;
  if (segment == NULL) 
  {
    return false;
  }
  
  char const *nextSegment;
  do {
    nextSegment = strstr(segment, JSON_OBJECT_PATH_SEPARATOR);
    
    size_t length;
    if (nextSegment != NULL) 
    {
      length = nextSegment - segment;
      nextSegment++;
    }
    else
    {
      length = strlen(segment);
    }
    
    if (length > 0) 
    {
      if (value->type != JSON_TYPE_OBJECT) 
      {
        value = NULL;
      }
      else
      {
        struct jsontree_object *object = ((struct jsontree_object *)(value));
        value = NULL;
        for ( int16_t __i = 0 ; __i < object->count; __i++ )
        {
          char const *attributeName = object->pairs[__i].name;
          if (length == strlen(attributeName) && strncmp(segment, attributeName, length) == 0) 
          {
            js_ctx->index[js_ctx->depth] = __i;
            jsontree_print_current_attribute_name(js_ctx, msg);
            
            value = object->pairs[__i].value;
            js_ctx->depth++;
            js_ctx->values[js_ctx->depth] = value;
            js_ctx->index[js_ctx->depth] = 0;
            break;
          }
        }
      }
    }
    
    segment = nextSegment;
  }
  while (nextSegment != NULL && strlen(nextSegment) > 0 && value != NULL);
  
  js_ctx->path = js_ctx->depth;
  return value != NULL;
}

bool jsontree_print_current_attribute_name(struct jsontree_context *js_ctx, struct jsontree_msg *msg)
{
  struct jsontree_value *value = js_ctx->values[js_ctx->depth];
  if (value->type != JSON_TYPE_OBJECT) 
  {
    return false;
  }
  
  struct jsontree_object *object = ((struct jsontree_object *)(value));
  uint16_t index = js_ctx->index[js_ctx->depth];
  if (index >= object->count) 
  {
    return false;
  }
  
  if (js_ctx->empty[js_ctx->depth] != 0) 
  {
    jsontree_write_char(msg, ((char)(value->type)));
    js_ctx->empty[js_ctx->depth] = 0;
  }
  else
  {
    jsontree_write_char(msg, ',');
  }
  
  char const *name = object->pairs[index].name;
  jsontree_write_string(msg, name, ((int32_t)(strlen(name))));
  jsontree_write_char(msg, ':');
  return true;
}

bool jsontree_print_current_value(struct jsontree_context *js_ctx, struct jsontree_msg *msg, bool modifiedOnly)
{
  while (js_ctx->path <= js_ctx->depth)
  {
    int32_t result = jsontree_print_next(js_ctx, msg, modifiedOnly);
    if (result < 0) 
    {
      /* 
       * Something went wrong
       */
      return false;
    }
    else if (result == 0) {
      /* 
       * Reached end of JSON tree
       */
      break;
    }
  }
  return true;
}

bool jsontree_print_object_endings(struct jsontree_context *js_ctx, struct jsontree_msg *msg, uint8_t count)
{
  /* 
   * All object/document endings printed already?
   */
  if (js_ctx->path == ((uint8_t)(EOF))) 
  {
    return true;
  }
  
  if (count == JSONTREE_OBJECT_ENDINGS_COUNT_ALL || count > js_ctx->depth) 
  {
    count = js_ctx->depth + 1;
  }
  
  while (count-- > 0)
  {
    struct jsontree_value *value = js_ctx->values[js_ctx->depth];
    if (value->type != JSON_TYPE_OBJECT) 
    {
      return false;
    }
    
    if (js_ctx->empty[js_ctx->depth] == 0) 
    {
      jsontree_write_char(msg, ((char)((value->type + 2))));
    }
    
    if (js_ctx->depth > 0) 
    {
      js_ctx->empty[js_ctx->depth] = 1;
      js_ctx->index[js_ctx->depth] = 0;
      
      js_ctx->depth--;
      js_ctx->index[js_ctx->depth]++;
      
      js_ctx->path = js_ctx->depth;
    }
    else
    {
      js_ctx->path = ((uint8_t)(EOF));
    }
  }
  
  return true;
}

static struct jsontree_value *jsontree_next_array_item_value(struct jsontree_context *js_ctx)
{
  struct jsontree_value *value = js_ctx->values[js_ctx->depth];
  if (value->type == JSON_TYPE_ARRAY) 
  {
    struct jsontree_array *array = ((struct jsontree_array *)(value));
    uint16_t i = js_ctx->index[js_ctx->depth];
    if (i < array->count) 
    {
      struct jsontree_value *itemValue = array->values[i];
      js_ctx->index[js_ctx->depth]++;
      return itemValue;
    }
  }
  return NULL;
}

static void jsontree_enter_value(struct jsontree_context *js_ctx, struct jsontree_value *value)
{
  /* 
   * TODO: check max depth
   */
  js_ctx->depth++;
  js_ctx->values[js_ctx->depth] = value;
  js_ctx->index[js_ctx->depth] = 0;
  js_ctx->path = js_ctx->depth;
}

static struct jsontree_value *jsontree_exit_value(struct jsontree_context *js_ctx)
{
  js_ctx->depth--;
  js_ctx->path = js_ctx->depth;
  return js_ctx->values[js_ctx->depth];
}

static struct jsontree_value *jsontree_find_pair_value(struct jsontree_context *js_ctx, char const *pairName, size_t pairNameLength)
{
  struct jsontree_value *value = js_ctx->values[js_ctx->depth];
  if (value->type == JSON_TYPE_OBJECT) 
  {
    struct jsontree_object *object = ((struct jsontree_object *)(value));
    for ( int16_t __i = 0 ; __i < object->count; __i++ )
    {
      if (strlen(object->pairs[__i].name) == pairNameLength && strncmp(object->pairs[__i].name, pairName, pairNameLength) == 0) 
      {
        js_ctx->index[js_ctx->depth] = __i;
        return object->pairs[__i].value;
      }
    }
  }
  return NULL;
}

bool jsonparse_parse_value(struct jsontree_context *js_ctx, struct jsonparse_state *parseState)
{
  jsontree_reset_flags(js_ctx->values[js_ctx->depth], true);
  return jsonparse_do_parse_value(js_ctx, parseState);
}

static bool jsonparse_do_parse_value(struct jsontree_context *js_ctx, struct jsonparse_state *parseState)
{
  struct jsontree_value *value = js_ctx->values[js_ctx->depth];
  if (value == NULL) 
  {
    return false;
  }
  
  bool insideObject = false;
  bool insideArray = false;
  while (jsonparse_has_next(parseState) != 0)
  {
    struct jsontree_value *oldValue = value;
    if (insideArray) 
    {
      struct jsontree_value *arrayItemValue = jsontree_next_array_item_value(js_ctx);
      if (arrayItemValue != NULL) 
      {
        value = arrayItemValue;
        value->modified = true;
        jsontree_enter_value(js_ctx, arrayItemValue);
      }
    }
    int32_t type = jsonparse_next(parseState);
    switch (type)
    {
      case JSON_TYPE_OBJECT:
      {
        value->modified = true;
        insideObject = true;
        break;
      }
      case JSON_TYPE_PAIR_NAME:
      {
        struct jsontree_value *pairValue = jsontree_find_pair_value(js_ctx, jsonparse_get_value(parseState), ((size_t)(jsonparse_get_len(parseState))));
        if (pairValue != NULL) 
        {
          value = pairValue;
          value->modified = true;
          jsontree_enter_value(js_ctx, pairValue);
        }
        if (!jsonparse_do_parse_value(js_ctx, parseState)) 
        {
          return false;
        }
        break;
      }
      case JSON_TYPE_OBJECT_END:
      {
        insideObject = false;
        break;
      }
      case JSON_TYPE_ARRAY:
      {
        value->modified = true;
        insideArray = true;
        break;
      }
      case JSON_TYPE_ARRAY_END:
      {
        insideArray = false;
        break;
      }
      case JSON_TYPE_STRING:
      {
        if (value->type == JSON_TYPE_STRING) 
        {
          value->modified = true;
          struct jsontree_string *string = ((struct jsontree_string *)(value));
          string->value = jsonparse_get_value_as_string(parseState);
          string->len = strlen(string->value);
        }
        else if (value->type == JSON_TYPE_STRING_BUFFER) {
          value->modified = true;
          struct jsontree_string_buffer *string = ((struct jsontree_string_buffer *)(value));
          if (string->value != NULL) 
          {
            jsonparse_copy_value_as_string(parseState, string->value, ((int32_t)(string->size)));
          }
        }
        else if (value->type == JSON_TYPE_CALLBACK) {
          value->modified = true;
          struct jsontree_callback *callback = ((struct jsontree_callback *)(value));
          if (callback->input != NULL) 
          {
            (*callback->input)(js_ctx, parseState);
          }
        }
        else if (value->type == JSON_TYPE_ENUM) {
          value->modified = true;
          struct jsontree_enum *_enum = ((struct jsontree_enum *)(value));
          if (_enum->decoderFunc != NULL) 
          {
            _enum->value = (*_enum->decoderFunc)(parseState->msg.base + parseState->vstart, parseState->vlen);
          }
          else
          {
            _enum->value = jsonparse_get_value_as_uint(parseState);
          }
        }
        else if (value->type == JSON_TYPE_DATA) {
          value->modified = true;
          struct jsontree_data *data = ((struct jsontree_data *)(value));
          if (data->decoderFunc != NULL) 
          {
            (*data->decoderFunc)(parseState->msg.base + parseState->vstart, parseState->vlen, data->value, data->valueSize);
          }
        }
        break;
      }
      case JSON_TYPE_NULL:
      {
        if (value->type == JSON_TYPE_OBJECT) 
        {
          struct jsontree_object *object = ((struct jsontree_object *)(value));
          object->null = true;
        }
        else if (value->type == JSON_TYPE_ARRAY) {
          struct jsontree_array *array = ((struct jsontree_array *)(value));
          array->null = true;
        }
        else if (value->type == JSON_TYPE_STRING) {
          value->modified = true;
          struct jsontree_string *string = ((struct jsontree_string *)(value));
          string->value = "";
          string->len = 0;
        }
        else if (value->type == JSON_TYPE_STRING_BUFFER) {
          value->modified = true;
          struct jsontree_string_buffer *string = ((struct jsontree_string_buffer *)(value));
          if (string->value != NULL) 
          {
            string->value[0] = '\0';
            string->size = 1;
          }
        }
        else
        {
          value->modified = true;
          struct jsontree_callback *callback = ((struct jsontree_callback *)(value));
          if (callback->input != NULL) 
          {
            (*callback->input)(js_ctx, parseState);
          }
        }
        break;
      }
      case JSON_TYPE_NUMBER:
      {
        if (value->type == JSON_TYPE_UINT) 
        {
          value->modified = true;
          ((struct jsontree_uint *)(value))->value = jsonparse_get_value_as_uint(parseState);
        }
        else if (value->type == JSON_TYPE_INT) {
          value->modified = true;
          ((struct jsontree_int *)(value))->value = jsonparse_get_value_as_int(parseState);
        }
        else if (value->type == JSON_TYPE_U8PTR) {
          value->modified = true;
          *((uint8_t *)(((struct jsontree_ptr *)(value))->value)) = ((uint8_t)(jsonparse_get_value_as_uint(parseState)));
        }
        else if (value->type == JSON_TYPE_S8PTR) {
          value->modified = true;
          *((int8_t *)(((struct jsontree_ptr *)(value))->value)) = ((int8_t)(jsonparse_get_value_as_int(parseState)));
        }
        else if (value->type == JSON_TYPE_U16PTR) {
          value->modified = true;
          *((uint16_t *)(((struct jsontree_ptr *)(value))->value)) = ((uint16_t)(jsonparse_get_value_as_uint(parseState)));
        }
        else if (value->type == JSON_TYPE_S16PTR) {
          value->modified = true;
          *((int16_t *)(((struct jsontree_ptr *)(value))->value)) = ((int16_t)(jsonparse_get_value_as_int(parseState)));
        }
        else if (value->type == JSON_TYPE_U32PTR) {
          value->modified = true;
          *((uint32_t *)(((struct jsontree_ptr *)(value))->value)) = ((uint32_t)(jsonparse_get_value_as_uint(parseState)));
        }
        else if (value->type == JSON_TYPE_S32PTR) {
          value->modified = true;
          *((int32_t *)(((struct jsontree_ptr *)(value))->value)) = ((int32_t)(jsonparse_get_value_as_int(parseState)));
        }
        else if (value->type == JSON_TYPE_CALLBACK) {
          value->modified = true;
          struct jsontree_callback *callback = ((struct jsontree_callback *)(value));
          if (callback->input != NULL) 
          {
            (*callback->input)(js_ctx, parseState);
          }
        }
        break;
      }
      case JSON_TYPE_TRUE:
      {
        if (value->type == JSON_TYPE_BOOL) 
        {
          value->modified = true;
          ((struct jsontree_bool *)(value))->value = true;
        }
        else if (value->type == JSON_TYPE_CALLBACK) {
          value->modified = true;
          struct jsontree_callback *callback = ((struct jsontree_callback *)(value));
          if (callback->input != NULL) 
          {
            (*callback->input)(js_ctx, parseState);
          }
        }
        break;
      }
      case JSON_TYPE_FALSE:
      {
        if (value->type == JSON_TYPE_BOOL) 
        {
          value->modified = true;
          ((struct jsontree_bool *)(value))->value = false;
        }
        else if (value->type == JSON_TYPE_CALLBACK) {
          value->modified = true;
          struct jsontree_callback *callback = ((struct jsontree_callback *)(value));
          if (callback->input != NULL) 
          {
            (*callback->input)(js_ctx, parseState);
          }
        }
        break;
      }
      case JSON_TYPE_END:
      {
        break;
      }
      case JSON_TYPE_ERROR:
      {
        printf("JSON parse error: %d\n", jsonparse_get_error(parseState));
        return false;
      }
      default: {
        printf("Unknown JSON element type: %i\n", type);
        return false;
      }
    }
    
    if (value != oldValue) 
    {
      value = jsontree_exit_value(js_ctx);
    }
    if (!insideArray && !insideObject) 
    {
      return true;
    }
  }
  return true;
}
