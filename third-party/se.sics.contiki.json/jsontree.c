/*
 * Copyright (c) 2011-2012, Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *         JSON output generation
 * \author
 *         Niclas Finne <nfi@sics.se>
 *         Joakim Eriksson <joakime@sics.se>
 */

/*
 * IMPROVEMENT by FotaHub: Enable standalone usage without contiki-os.
 */
// #include "contiki.h"
#include "jsontree.h"
#include "jsonparse.h"
/*
 * IMPROVEMENT by FotaHub: Allow standard C library dependencies to be controlled through target.h.
 */
// #include <string.h>
#include "target.h"

/*
 * IMPROVEMENT by FotaHub: Allow standard C library dependencies to be controlled through target.h.
 * IMPROVEMENT by FotaHub: Allow DEBUB flag to be set through header files or compiler options.
 */
#ifndef DEBUG
#define DEBUG 0
#endif

/*
 * ENHANCEMENT by FotaHub: Write character safely to json document.
 */
void ICACHE_FLASH_ATTR
jsontree_write_char(struct jsontree_msg *msg, char character)
{
  if(msg->base != NULL && msg->pos < msg->size - 1) {
	  msg->base[msg->pos++] = character;
  }
}
/*---------------------------------------------------------------------------*/
/*
 * ENHANCEMENT by FotaHub: Write terminate character safely to json document.
 */
void ICACHE_FLASH_ATTR
jsontree_write_terminate_char(struct jsontree_msg *msg)
{
  if(msg->base != NULL && msg->pos < msg->size) {
	  msg->base[msg->pos] = '\0';
  }
}
/*---------------------------------------------------------------------------*/
void ICACHE_FLASH_ATTR
jsontree_write_atom(struct jsontree_msg *msg, const char *text)
{
  if(text == NULL) {
    jsontree_write_char(msg, '0');
  } else {
    while(*text != '\0') {
      jsontree_write_char(msg, *text++);
    }
  }
}
/*---------------------------------------------------------------------------*/
/*
 * ENHANCEMENT by FotaHub: Add support for non-zero-terminated strings.
 */
void ICACHE_FLASH_ATTR
jsontree_write_string(struct jsontree_msg *msg, const char *text, int len)
{
  /*
   * ENHANCEMENT by FotaHub: Enable serialization of null-valued strings.
   */
  if(text != NULL) {
    jsontree_write_char(msg, '"');
    while(len-- > 0) {
      if(*text == '"') {
        jsontree_write_char(msg, '\\');
      }
      jsontree_write_char(msg, *text++);
    }
    jsontree_write_char(msg, '"');
  } else {
    jsontree_write_atom(msg, "null");
  }
}
/*---------------------------------------------------------------------------*/
void ICACHE_FLASH_ATTR
jsontree_write_uint(struct jsontree_msg *msg, unsigned int value)
{
  char buf[10];
  int l;

  l = sizeof(buf) - 1;
  do {
    buf[l--] = '0' + (value % 10);
    value /= 10;
  } while(value > 0 && l >= 0);

  while(++l < sizeof(buf)) {
    jsontree_write_char(msg, buf[l]);
  }
}
/*---------------------------------------------------------------------------*/
void ICACHE_FLASH_ATTR
jsontree_write_int(struct jsontree_msg *msg, int value)
{
  if(value < 0) {
    jsontree_write_char(msg, '-');
    value = -value;
  }

  jsontree_write_uint(msg, value);
}
/*---------------------------------------------------------------------------*/
/*
 * ENHANCEMENT by FotaHub: Add support for bool attributes.
 */
void ICACHE_FLASH_ATTR
jsontree_write_bool(struct jsontree_msg *msg, bool value)
{
  if(value)
    jsontree_write_atom(msg, "true");
  else
    jsontree_write_atom(msg, "false");
}
/*---------------------------------------------------------------------------*/
/*
 * ENHANCEMENT by FotaHub: Enable user defined buffer where to save the serialized json document.
 */
void ICACHE_FLASH_ATTR
jsontree_setup(struct jsontree_context *js_ctx, struct jsontree_value *root, void *user)
{
  js_ctx->values[0] = root;
  /*
   *  BUGFIX by FotaHub: Include path in JSON tree reset.
   */
  // js_ctx->path = 0;
  jsontree_reset(js_ctx);
  js_ctx->user = user;
}
/*---------------------------------------------------------------------------*/
void ICACHE_FLASH_ATTR
jsontree_reset(struct jsontree_context *js_ctx)
{
  /*
   * ENHANCEMENT by FotaHub: Enable selective serialization of individual JSON tree branches.
   */
  int i;
  for(i = 0; i < JSONTREE_MAX_DEPTH; i++) {
    js_ctx->empty[i] = 1;
  }
  js_ctx->depth = 0;
  /*
   *  BUGFIX by FotaHub: Include path in JSON tree reset.
   */
  js_ctx->path = 0;
  js_ctx->index[0] = 0;
}
/*---------------------------------------------------------------------------*/
/*
 * IMPROVEMENT by FotaHub: Auto-determine depth instead of letting user have to specify it.
 */
const char * ICACHE_FLASH_ATTR
jsontree_path_name(const struct jsontree_context *js_ctx)
{
  int depth = js_ctx->depth;
  struct jsontree_object *object = NULL;
  while(depth >= 0) {
	  if(js_ctx->values[depth]->type == JSON_TYPE_OBJECT) {
		  object = (struct jsontree_object *)js_ctx->values[depth];
		  break;
	  }
	  depth--;
  }
  return object != NULL ? object->pairs[js_ctx->index[depth]].name : "";
}
/*---------------------------------------------------------------------------*/
/*
 * ENHANCEMENT by FotaHub: Enable retrieval of current JSON tree value.
 */
struct jsontree_value * ICACHE_FLASH_ATTR
jsontree_current(const struct jsontree_context *js_ctx)
{
	return js_ctx->values[js_ctx->depth];
}
/*---------------------------------------------------------------------------*/
/*
 * ENHANCEMENT by FotaHub: Enable user-defined context data to be retrieved from input and output handlers.
 */
void * ICACHE_FLASH_ATTR
jsontree_get_user(const struct jsontree_context *js_ctx) {
	return js_ctx->user;
}
/*---------------------------------------------------------------------------*/
/*
 * ENHANCEMENT by FotaHub: Enable serialization to be restricted to JSON tree values that have been marked as modified.
 */
int  ICACHE_FLASH_ATTR
jsontree_print_next(struct jsontree_context *js_ctx, struct jsontree_msg* msg, bool modifiedOnly)
{
  struct jsontree_value *v;
  int index;
#if JSONTREE_PRETTY
  int indent;
#endif

  v = js_ctx->values[js_ctx->depth];

  /* Default operation after switch is to back up one level */

  #ifdef USE_IF_ELSE_INSTEAD_OF_SWITCH_CASE
  uint8_t type = v->type;
  if(type == JSON_TYPE_OBJECT || type == JSON_TYPE_ARRAY) {
    struct jsontree_array *o = (struct jsontree_array *)v;
    struct jsontree_value *ov = NULL;
  
    /*
    * ENHANCEMENT by FotaHub: Enable serialization of null-valued JSON objects and arrays.
    */
    if(o->null) {
      jsontree_write_atom(msg, "null");
      /* Default operation: back up one level! */
      
          /*
          * ENHANCEMENT by FotaHub: Enable serialization to be restricted to JSON tree values that have been marked as modified.
          * ENHANCEMENT by FotaHub: Enabled parsing and serialization of null-valued JSON objects and arrays.
          */
          jsontree_reset_flags(v, false);
            /* Done => back up one level! */
          if(js_ctx->depth > 0) {
            /*
            * ENHANCEMENT by FotaHub: Enable selective serialization of individual JSON tree branches.
            */
            js_ctx->empty[js_ctx->depth] = 1; 
            js_ctx->index[js_ctx->depth] = 0;
            js_ctx->depth--;
            js_ctx->index[js_ctx->depth]++;
            return 1;
          }
          return 0;
      }

      /*
      * ENHANCEMENT by FotaHub: Enable serialization to be restricted to JSON tree values that have been marked as modified.
      */
      index = js_ctx->index[js_ctx->depth];
      if(o->count > 0) {
        if(v->type == JSON_TYPE_OBJECT) {
          ov = ((struct jsontree_object *)o)->pairs[index].value;
        } else {
          /*
          * IMPROVEMENT by FotaHub: Support initialization of array JSON tree values with arrays of other JSON tree values.
          */
          ov = o->values[index];
        }
      }

      if(index == 0) {
        /*
        * ENHANCEMENT by FotaHub: Enable selective serialization of individual JSON tree branches.
        * ENHANCEMENT by FotaHub: Enable serialization to be restricted to JSON tree values that have been marked as modified.
        */
        if(o->count > 0) {
          if(!modifiedOnly || ov->modified) {
            js_ctx->empty[js_ctx->depth] = 0;
          }
        }
        jsontree_write_char(msg, v->type);

  #if JSONTREE_PRETTY
        jsontree_write_char(msg, '\n');
  #endif
      }
      if(index >= o->count) {
  #if JSONTREE_PRETTY
        jsontree_write_char(msg, '\n');
        indent = js_ctx->depth;
        while(indent--) {
          jsontree_write_char(msg, ' ');
          jsontree_write_char(msg, ' ');
        }
  #endif
        jsontree_write_char(msg, v->type + 2);
        /* Default operation: back up one level! */

            /*
            * ENHANCEMENT by FotaHub: Enable serialization to be restricted to JSON tree values that have been marked as modified.
            * ENHANCEMENT by FotaHub: Enabled parsing and serialization of null-valued JSON objects and arrays.
            */
            jsontree_reset_flags(v, false);
              /* Done => back up one level! */
            if(js_ctx->depth > 0) {
              /*
              * ENHANCEMENT by FotaHub: Enable selective serialization of individual JSON tree branches.
              */
              js_ctx->empty[js_ctx->depth] = 1; 
              js_ctx->index[js_ctx->depth] = 0;
              js_ctx->depth--;
              js_ctx->index[js_ctx->depth]++;
              return 1;
            }
            return 0;
      }

      if(index > 0) {
        /*
        * ENHANCEMENT by FotaHub: Enable selective serialization of individual JSON tree branches.
        * ENHANCEMENT by FotaHub: Enable serialization to be restricted to JSON tree values that have been marked as modified.
        */
        if(!modifiedOnly || ov->modified) {
          if(js_ctx->empty[js_ctx->depth]) {
            js_ctx->empty[js_ctx->depth] = 0;
          } else {
            jsontree_write_char(msg, ',');
  #if JSONTREE_PRETTY
            jsontree_write_char(msg, '\n');
  #endif
          }

  #if JSONTREE_PRETTY
          indent = js_ctx->depth + 1;
          while(indent--) {
            jsontree_write_char(msg, ' ');
            jsontree_write_char(msg, ' ');
          }
  #endif
        }
      }

      if(v->type == JSON_TYPE_OBJECT) {
        /*
        * ENHANCEMENT by FotaHub: Enable selective serialization of individual JSON tree branches.
        * ENHANCEMENT by FotaHub: Enable serialization to be restricted to JSON tree values that have been marked as modified.
        * ENHANCEMENT by FotaHub: Add support for non-zero-terminated strings.
        */
        if(!modifiedOnly || ov->modified) {
          js_ctx->empty[js_ctx->depth] = 0;
          const char * on = ((struct jsontree_object *)o)->pairs[index].name;
          jsontree_write_string(msg, on, strlen(on));
          jsontree_write_char(msg, ':');
  #if JSONTREE_PRETTY
          jsontree_write_char(msg, ' ');
  #endif
        }
      }
      /*
      * ENHANCEMENT by FotaHub: Enable serialization to be restricted to JSON tree values that have been marked as modified.
      */
      if(!modifiedOnly || ov->modified) {
        /* TODO check max depth */
        js_ctx->depth++;          /* step down to value... */
        js_ctx->index[js_ctx->depth] = 0; /* and init index */
        js_ctx->values[js_ctx->depth] = ov;
        /* Continue on this new level */
        return 1;
      } else {
        /*
        * ENHANCEMENT by FotaHub: Enable serialization to be restricted to JSON tree values that have been marked as modified.
        * ENHANCEMENT by FotaHub: Enabled parsing and serialization of null-valued JSON objects and arrays.
        */
        jsontree_reset_flags(ov, true);

        /* Skip value and continue on current level */
        js_ctx->index[js_ctx->depth]++;
        return 1;
      }
    } else if(type == JSON_TYPE_STRING_BUFFER) {
      /*
      * IMPROVEMENT by FotaHub: Renamed jsontree_string to jsontree_string_buffer.
      * ENHANCEMENT by FotaHub: Add support for non-zero-terminated strings.
      */
        struct jsontree_string_buffer * value = (struct jsontree_string_buffer *)v;
        if(value->value != NULL) {
          jsontree_write_string(msg, value->value, strlen(value->value));
        } else {
          jsontree_write_string(msg, NULL, 0);      
        }
        /* Default operation: back up one level! */
    } else if(type == JSON_TYPE_STRING) {
      /*
      * ENHANCEMENT by FotaHub: Support retrieval of string values without copying them.
      * ENHANCEMENT by FotaHub: Add support for non-zero-terminated strings.
      */
      struct jsontree_string * value = (struct jsontree_string *)v;
      jsontree_write_string(msg, value->value, value->len);
      /* Default operation: back up one level! */
    } else if(type == JSON_TYPE_UINT) {
      jsontree_write_uint(msg, ((struct jsontree_uint *)v)->value);
      /* Default operation: back up one level! */
    } else if(type == JSON_TYPE_INT) {
      jsontree_write_int(msg, ((struct jsontree_int *)v)->value);
      /* Default operation: back up one level! */
    } else if(type == JSON_TYPE_BOOL) {
      /*
       * ENHANCEMENT by FotaHub: Add support for bool attributes.
       */
      jsontree_write_bool(msg, ((struct jsontree_bool *)v)->value);
      /* Default operation: back up one level! */
    } else if(type == JSON_TYPE_CALLBACK) {
      /* pre-formatted json string currently */
      struct jsontree_callback *callback;

      callback = (struct jsontree_callback *)v;
      if(js_ctx->index[js_ctx->depth] == 0) {
        /* First call: reset the callback status */
        js_ctx->callback_state = 0;
      }
      if(callback->output == NULL) {
        jsontree_write_string(msg, "", 0);
      } else if(callback->output(js_ctx, msg)) {
        /* The callback wants to output more */
        js_ctx->index[js_ctx->depth]++;
        return 1;
      }
      /* Default operation: back up one level! */
    } else if(type == JSON_TYPE_ENUM) {
      /*
       * ENHANCEMENT by FotaHub: Add support for enum attributes.
       */
      struct jsontree_enum *_enum = (struct jsontree_enum *)v;
      if(_enum->encoderFunc != NULL) {
        jsontree_write_char(msg, '"');
        size_t len = _enum->encoderFunc(_enum->value, msg->base + msg->pos, msg->size - msg->pos);
        msg->pos += len;
        jsontree_write_char(msg, '"');
      } else {
        jsontree_write_uint(msg, _enum->value);
      }
      /* Default operation: back up one level! */
    } else if(type == JSON_TYPE_DATA) {
      /*
       * ENHANCEMENT by FotaHub: Add support for arbitrary binary attributes.
       */
      struct jsontree_data *data = (struct jsontree_data *)v;
      if(data->encoderFunc != NULL) {
        jsontree_write_char(msg, '"');
        size_t len = data->encoderFunc(data->value, data->valueSize, msg->base + msg->pos, msg->size - msg->pos);
        msg->pos += len;
        jsontree_write_char(msg, '"');
      }
      /* Default operation: back up one level! */
    } else if(type == JSON_TYPE_S8PTR) {
      jsontree_write_int(msg, *((int8_t *)((struct jsontree_ptr *)v)->value));
      /* Default operation: back up one level! */
    } else if(type == JSON_TYPE_U8PTR) {
      jsontree_write_uint(msg, *((uint8_t *)((struct jsontree_ptr *)v)->value));
      /* Default operation: back up one level! */
    } else if(type == JSON_TYPE_S16PTR) {
      jsontree_write_int(msg, *((int16_t *)((struct jsontree_ptr *)v)->value));
      /* Default operation: back up one level! */
    } else if(type == JSON_TYPE_U16PTR) {
      jsontree_write_uint(msg, *((uint16_t *)((struct jsontree_ptr *)v)->value));
      /* Default operation: back up one level! */
    } else if(type == JSON_TYPE_S32PTR) {
      jsontree_write_int(msg, *((int32_t *)((struct jsontree_ptr *)v)->value));
      /* Default operation: back up one level! */	
    } else if(type == JSON_TYPE_U32PTR) {
      jsontree_write_uint(msg, *((uint32_t *)((struct jsontree_ptr *)v)->value));
      /* Default operation: back up one level! */	
    } else {
      /*
       * IMPROVEMENT by FotaHub: Allow standard C library dependencies to be controlled through target.h.
       */
      #if DEBUG
      PRINTF("\nError: Illegal json type:'%c'\n", v->type);
      #endif
      /*
       * IMPROVEMENT by FotaHub: Distinguish errornuous returns from normal returns.
       */
      return -1;	
    }
  #else // USE_IF_ELSE_INSTEAD_OF_SWITCH_CASE
  switch(v->type) {
  case JSON_TYPE_OBJECT:
  case JSON_TYPE_ARRAY: {
    struct jsontree_array *o = (struct jsontree_array *)v;
    struct jsontree_value *ov = NULL;
	
    /*
     * ENHANCEMENT by FotaHub: Enable serialization of null-valued JSON objects and arrays.
     */
    if(o->null) {
      jsontree_write_atom(msg, "null");
      /* Default operation: back up one level! */
      break;
    }

    /*
     * ENHANCEMENT by FotaHub: Enable serialization to be restricted to JSON tree values that have been marked as modified.
     */
    index = js_ctx->index[js_ctx->depth];
    if(o->count > 0) {
      if(v->type == JSON_TYPE_OBJECT) {
        ov = ((struct jsontree_object *)o)->pairs[index].value;
      } else {
        /*
        * IMPROVEMENT by FotaHub: Support initialization of array JSON tree values with arrays of other JSON tree values.
        */
        ov = o->values[index];
      }
    }

    if(index == 0) {
      /*
       * ENHANCEMENT by FotaHub: Enable selective serialization of individual JSON tree branches.
       * ENHANCEMENT by FotaHub: Enable serialization to be restricted to JSON tree values that have been marked as modified.
       */
      if(o->count > 0) {
        if(!modifiedOnly || ov->modified) {
          js_ctx->empty[js_ctx->depth] = 0;
        }
      }
      jsontree_write_char(msg, v->type);

#if JSONTREE_PRETTY
      jsontree_write_char(msg, '\n');
#endif
    }
    if(index >= o->count) {
#if JSONTREE_PRETTY
      jsontree_write_char(msg, '\n');
      indent = js_ctx->depth;
      while(indent--) {
        jsontree_write_char(msg, ' ');
        jsontree_write_char(msg, ' ');
      }
#endif
      jsontree_write_char(msg, v->type + 2);
      /* Default operation: back up one level! */
      break;
    }

    if(index > 0) {
      /*
       * ENHANCEMENT by FotaHub: Enable selective serialization of individual JSON tree branches.
       * ENHANCEMENT by FotaHub: Enable serialization to be restricted to JSON tree values that have been marked as modified.
       */
      if(!modifiedOnly || ov->modified) {
        if(js_ctx->empty[js_ctx->depth]) {
          js_ctx->empty[js_ctx->depth] = 0;
        } else {
          jsontree_write_char(msg, ',');
#if JSONTREE_PRETTY
          jsontree_write_char(msg, '\n');
#endif
        }

#if JSONTREE_PRETTY
        indent = js_ctx->depth + 1;
        while(indent--) {
          jsontree_write_char(msg, ' ');
          jsontree_write_char(msg, ' ');
        }
#endif
      }
    }

    if(v->type == JSON_TYPE_OBJECT) {
      /*
       * ENHANCEMENT by FotaHub: Enable selective serialization of individual JSON tree branches.
       * ENHANCEMENT by FotaHub: Enable serialization to be restricted to JSON tree values that have been marked as modified.
       * ENHANCEMENT by FotaHub: Add support for non-zero-terminated strings.
       */
      if(!modifiedOnly || ov->modified) {
        js_ctx->empty[js_ctx->depth] = 0;
        const char * on = ((struct jsontree_object *)o)->pairs[index].name;
        jsontree_write_string(msg, on, strlen(on));
        jsontree_write_char(msg, ':');
#if JSONTREE_PRETTY
        jsontree_write_char(msg, ' ');
#endif
      }
    }
    /*
     * ENHANCEMENT by FotaHub: Enable serialization to be restricted to JSON tree values that have been marked as modified.
     */
    if(!modifiedOnly || ov->modified) {
      /* TODO check max depth */
      js_ctx->depth++;          /* step down to value... */
      js_ctx->index[js_ctx->depth] = 0; /* and init index */
      js_ctx->values[js_ctx->depth] = ov;
      /* Continue on this new level */
      return 1;
    } else {
      /*
       * ENHANCEMENT by FotaHub: Enable serialization to be restricted to JSON tree values that have been marked as modified.
       * ENHANCEMENT by FotaHub: Enabled parsing and serialization of null-valued JSON objects and arrays.
       */
      jsontree_reset_flags(ov, true);

      /* Skip value and continue on current level */
      js_ctx->index[js_ctx->depth]++;
      return 1;
    }
  }
  /*
   * IMPROVEMENT by FotaHub: Renamed jsontree_string to jsontree_string_buffer.
   * ENHANCEMENT by FotaHub: Add support for non-zero-terminated strings.
   */
  case JSON_TYPE_STRING_BUFFER: {
    struct jsontree_string_buffer * value = (struct jsontree_string_buffer *)v;
    if(value->value != NULL) {
      jsontree_write_string(msg, value->value, strlen(value->value));
    } else {
      jsontree_write_string(msg, NULL, 0);      
    }
    /* Default operation: back up one level! */
    break;
  }
  /*
   * ENHANCEMENT by FotaHub: Support retrieval of string values without copying them.
   * ENHANCEMENT by FotaHub: Add support for non-zero-terminated strings.
   */
  case JSON_TYPE_STRING: {
    struct jsontree_string * value = (struct jsontree_string *)v;
    jsontree_write_string(msg, value->value, value->len);
    /* Default operation: back up one level! */
    break;
  }
  case JSON_TYPE_UINT:
    jsontree_write_uint(msg, ((struct jsontree_uint *)v)->value);
    /* Default operation: back up one level! */
    break;
  case JSON_TYPE_INT:
    jsontree_write_int(msg, ((struct jsontree_int *)v)->value);
    /* Default operation: back up one level! */
    break;
  /*
   * ENHANCEMENT by FotaHub: Add support for bool attributes.
   */
  case JSON_TYPE_BOOL:
    jsontree_write_bool(msg, ((struct jsontree_bool *)v)->value);
    /* Default operation: back up one level! */
    break;
  case JSON_TYPE_CALLBACK: {   /* pre-formatted json string currently */
    struct jsontree_callback *callback;

    callback = (struct jsontree_callback *)v;
    if(js_ctx->index[js_ctx->depth] == 0) {
      /* First call: reset the callback status */
      js_ctx->callback_state = 0;
    }
    if(callback->output == NULL) {
      jsontree_write_string(msg, "", 0);
    } else if(callback->output(js_ctx, msg)) {
      /* The callback wants to output more */
      js_ctx->index[js_ctx->depth]++;
      return 1;
    }
    /* Default operation: back up one level! */
    break;
  }
  /*
   * ENHANCEMENT by FotaHub: Add support for enum attributes.
   */
  case JSON_TYPE_ENUM: {
    struct jsontree_enum *_enum = (struct jsontree_enum *)v;
    if(_enum->encoderFunc != NULL) {
      jsontree_write_char(msg, '"');
      size_t len = _enum->encoderFunc(_enum->value, msg->base + msg->pos, msg->size - msg->pos);
      msg->pos += len;
      jsontree_write_char(msg, '"');
    } else {
      jsontree_write_uint(msg, _enum->value);
    }
    /* Default operation: back up one level! */
    break;
  }
  /*
   * ENHANCEMENT by FotaHub: Add support for arbitrary binary attributes.
   */
  case JSON_TYPE_DATA: {
    struct jsontree_data *data = (struct jsontree_data *)v;
    if(data->encoderFunc != NULL) {
      jsontree_write_char(msg, '"');
      size_t len = data->encoderFunc(data->value, data->valueSize, msg->base + msg->pos, msg->size - msg->pos);
      msg->pos += len;
      jsontree_write_char(msg, '"');
    }
    /* Default operation: back up one level! */
    break;
  }
  case JSON_TYPE_S8PTR:
	jsontree_write_int(msg, *((int8_t *)((struct jsontree_ptr *)v)->value));
    /* Default operation: back up one level! */
    break;
  case JSON_TYPE_U8PTR:
    jsontree_write_uint(msg, *((uint8_t *)((struct jsontree_ptr *)v)->value));
    /* Default operation: back up one level! */
    break;
  case JSON_TYPE_S16PTR:
    jsontree_write_int(msg, *((int16_t *)((struct jsontree_ptr *)v)->value));
    /* Default operation: back up one level! */
    break;
  case JSON_TYPE_U16PTR:
    jsontree_write_uint(msg, *((uint16_t *)((struct jsontree_ptr *)v)->value));
    /* Default operation: back up one level! */
    break;
  case JSON_TYPE_S32PTR:
    jsontree_write_int(msg, *((int32_t *)((struct jsontree_ptr *)v)->value));
    /* Default operation: back up one level! */
    break;
  case JSON_TYPE_U32PTR:
    jsontree_write_uint(msg, *((uint32_t *)((struct jsontree_ptr *)v)->value));
    /* Default operation: back up one level! */
    break;
  default:
/*
 * IMPROVEMENT by FotaHub: Allow standard C library dependencies to be controlled through target.h.
 */
#if DEBUG
    PRINTF("\nError: Illegal json type:'%c'\n", v->type);
#endif
    /*
     * IMPROVEMENT by FotaHub: Distinguish errornuous returns from normal returns.
     */
    return -1;
  }
  #endif // USE_IF_ELSE_INSTEAD_OF_SWITCH_CASE
  
  /*
   * ENHANCEMENT by FotaHub: Enable serialization to be restricted to JSON tree values that have been marked as modified.
   * ENHANCEMENT by FotaHub: Enabled parsing and serialization of null-valued JSON objects and arrays.
   */
  jsontree_reset_flags(v, false);
    /* Done => back up one level! */
  if(js_ctx->depth > 0) {
    /*
     * ENHANCEMENT by FotaHub: Enable selective serialization of individual JSON tree branches.
     */
    js_ctx->empty[js_ctx->depth] = 1; 
    js_ctx->index[js_ctx->depth] = 0;
    js_ctx->depth--;
    js_ctx->index[js_ctx->depth]++;
    return 1;
  }
  return 0;
}
/*---------------------------------------------------------------------------*/
static struct jsontree_value * ICACHE_FLASH_ATTR
find_next(struct jsontree_context *js_ctx)
{
  struct jsontree_value *v;
  int index;

  do {
    v = js_ctx->values[js_ctx->depth];

    /* Default operation after switch is to back up one level */
    
    #ifdef USE_IF_ELSE_INSTEAD_OF_SWITCH_CASE
    uint8_t type = v->type;
    if(type == JSON_TYPE_OBJECT || type == JSON_TYPE_ARRAY) {
      struct jsontree_array *o = (struct jsontree_array *)v;
      struct jsontree_value *ov;

      index = js_ctx->index[js_ctx->depth];
      if(index >= o->count) {
        /* Default operation: back up one level! */
        break;
      }

      if(v->type == JSON_TYPE_OBJECT) {
        ov = ((struct jsontree_object *)o)->pairs[index].value;
      } else {
        /*
         * IMPROVEMENT by FotaHub: Support initialization of array JSON tree values with arrays of other JSON tree values.
         */
        ov = o->values[index];
      }
      /* TODO check max depth */
      js_ctx->depth++;        /* step down to value... */
      js_ctx->index[js_ctx->depth] = 0;       /* and init index */
      js_ctx->values[js_ctx->depth] = ov;
      /* Continue on this new level */
      return ov;
    } else {
      /* Default operation: back up one level! */
    }
    #else // USE_IF_ELSE_INSTEAD_OF_SWITCH_CASE
    switch(v->type) {
    case JSON_TYPE_OBJECT:
    case JSON_TYPE_ARRAY: {
      struct jsontree_array *o = (struct jsontree_array *)v;
      struct jsontree_value *ov;

      index = js_ctx->index[js_ctx->depth];
      if(index >= o->count) {
        /* Default operation: back up one level! */
        break;
      }

      if(v->type == JSON_TYPE_OBJECT) {
        ov = ((struct jsontree_object *)o)->pairs[index].value;
      } else {
    	/*
	     * IMPROVEMENT by FotaHub: Support initialization of array JSON tree values with arrays of other JSON tree values.
	     */
        ov = o->values[index];
      }
      /* TODO check max depth */
      js_ctx->depth++;        /* step down to value... */
      js_ctx->index[js_ctx->depth] = 0;       /* and init index */
      js_ctx->values[js_ctx->depth] = ov;
      /* Continue on this new level */
      return ov;
    }
    default:
      /* Default operation: back up one level! */
      break;
    }
    #endif // USE_IF_ELSE_INSTEAD_OF_SWITCH_CASE
    
    /* Done => back up one level! */
    if(js_ctx->depth > 0) {
      js_ctx->depth--;
      js_ctx->index[js_ctx->depth]++;
    } else {
      return NULL;
    }
  } while(1);
}
/*---------------------------------------------------------------------------*/
/*
 * ENHANCEMENT by FotaHub: Enable searches for values with subtype of given type.
 *  BUGFIX by FotaHub: Consistently use int8_t to pass around JSON tree value type.
 */
static int polymorphic_match_type(struct jsontree_value* value, int8_t type) {
	/* Direct match ? */
	if(value->type == type) {
		return 1;
	}
	/* Polymorphic match ? */
	if(type == JSON_TYPE_NUMBER) {
		if(value->type == JSON_TYPE_UINT || value->type == JSON_TYPE_INT) {
			return 1;
		}
	}
	return 0;
}
/*---------------------------------------------------------------------------*/
/*
 *  BUGFIX by FotaHub: Consistently use int8_t to pass around JSON tree value type.
 */
struct jsontree_value * ICACHE_FLASH_ATTR
jsontree_find_next(struct jsontree_context *js_ctx, int8_t type)
{
  struct jsontree_value *v;

  /*
   * ENHANCEMENT by FotaHub: Enable searches for values with subtype of given type.
   */
  while((v = find_next(js_ctx)) != NULL && !polymorphic_match_type(v, type) &&
        js_ctx->path < js_ctx->depth) {
    /* search */
  }
  js_ctx->callback_state = 0;
  return js_ctx->path < js_ctx->depth ? v : NULL;
}
/*---------------------------------------------------------------------------*/
/*
 * ENHANCEMENT by FotaHub: Enable serialization to be restricted to JSON tree values that have been marked as modified.
 * ENHANCEMENT by FotaHub: Enabled parsing and serialization of null-valued JSON objects and arrays.
 */
void ICACHE_FLASH_ATTR
jsontree_reset_flags(struct jsontree_value* value, bool deep)
{
  if(value == NULL) { 
    return; 
  }
   
  value->modified = false;
   
  if(value->type == JSON_TYPE_OBJECT) { 
    struct jsontree_object* object = ((struct jsontree_object*) value);
    if(object->count > 0) { 
      object->null = false; 
    }

    if(deep) {
      int i;
      for(i = 0; i < object->count; i++) {    
        jsontree_reset_flags(object->pairs[i].value, true); 
      }
    }
  } else if(value->type == JSON_TYPE_ARRAY) { 
    struct jsontree_array* array = ((struct jsontree_array*) value); 
    if(array->count > 0) { 
      array->null = false; 
    }

    if(deep) {
      int i; 
      for(i = 0; i < array->count; i++) {    
        jsontree_reset_flags(array->values[i], true); 
      }
    }
  } 
}
