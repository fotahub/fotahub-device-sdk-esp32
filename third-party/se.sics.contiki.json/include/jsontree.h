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

#ifndef JSONTREE_H_
#define JSONTREE_H_

/*
 * IMPROVEMENT by FotaHub: Enable standalone usage without contiki-os.
 */
// #include "contiki-conf.h"
#include "json.h"
/*
 * ENHANCEMENT by FotaHub: Enable usage of size_t.
 */
#include <stddef.h>
#include <stdint.h>
/*
 * ENHANCEMENT by FotaHub: Add support for bool attributes.
 */
#include <stdbool.h>

#ifdef JSONTREE_CONF_MAX_DEPTH
#define JSONTREE_MAX_DEPTH JSONTREE_CONF_MAX_DEPTH
#else
#define JSONTREE_MAX_DEPTH 10
#endif /* JSONTREE_CONF_MAX_DEPTH */

#ifdef JSONTREE_CONF_PRETTY
#define JSONTREE_PRETTY JSONTREE_CONF_PRETTY
#else
#define JSONTREE_PRETTY 0
#endif /* JSONTREE_CONF_PRETTY */

/*
 * ENHANCEMENT by FotaHub: Enable user-defined context data
 * IMPROVEMENT by FotaHub: Use char instead of int as type for c parameter and remove unused return type.
 */

struct jsontree_context {
  struct jsontree_value *values[JSONTREE_MAX_DEPTH];
  uint16_t index[JSONTREE_MAX_DEPTH];
  /*
   * ENHANCEMENT by FotaHub: Enable selective serialization of individual JSON tree branches.
   */
  uint8_t empty[JSONTREE_MAX_DEPTH];
  uint8_t depth;
  uint8_t path;
  int callback_state;
  /*
   * ENHANCEMENT by FotaHub: Enable user-defined context data.
   */
  void* user;
};

/*
 * ENHANCEMENT by FotaHub: Add message state to help serializing output json document.
 */
struct jsontree_msg {
  char *base;
  int size;
  int pos;
};

struct jsontree_value {
  /*
   * ENHANCEMENT by FotaHub: Use only 7 bit to represent JSON tree value type 
   * and leverage last bit to flag JSON tree value modifications.
   */
  uint8_t type : 7;
  uint8_t modified : 1;
  /* followed by a value */
};

/*
 * IMPROVEMENT by FotaHub: Renamed jsontree_string to jsontree_string_buffer.
 */
struct jsontree_string_buffer {
  /*
   * ENHANCEMENT by FotaHub: Use only 7 bit to represent JSON tree value type 
   * and leverage last bit to flag JSON tree value modifications.
   */
  uint8_t type : 7;
  uint8_t modified : 1;
  /*
   * ENHANCEMENT by FotaHub: Enable jsontree_string_buffer to be used for document model-based parsing.
   */
  char *value;
  size_t size;
};

/*
 * ENHANCEMENT by FotaHub: Support retrieval of string values without copying them.
 */
struct jsontree_string {
  /*
   * ENHANCEMENT by FotaHub: Use only 7 bit to represent JSON tree value type 
   * and leverage last bit to flag JSON tree value modifications.
   */
  uint8_t type : 7;
  uint8_t modified : 1;
  char const *value;
  size_t len;
};

struct jsontree_uint {
  /*
   * ENHANCEMENT by FotaHub: Use only 7 bit to represent JSON tree value type 
   * and leverage last bit to flag JSON tree value modifications.
   */
  uint8_t type : 7;
  uint8_t modified : 1;
  unsigned int value;
};

struct jsontree_int {
  /*
   * ENHANCEMENT by FotaHub: Use only 7 bit to represent JSON tree value type 
   * and leverage last bit to flag JSON tree value modifications.
   */
  uint8_t type : 7;
  uint8_t modified : 1;
  int value;
};

/*
 * ENHANCEMENT by FotaHub: Add support for bool attributes.
 */
struct jsontree_bool { 
  uint8_t type : 7;
  uint8_t modified : 1;
  bool value; 
};

/*
 * IMPROVEMENT by FotaHub: Provide explicit function types for output and input handers.
 */
struct jsonparse_state;
typedef int output_handler(struct jsontree_context *js_ctx, struct jsontree_msg *msg);
typedef void input_handler(struct jsontree_context *js_ctx, struct jsonparse_state *parser);

/* NOTE: the jsontree_callback input will receive a jsonparse state */
/*
 * IMPROVEMENT by FotaHub: Provide explicit function types for output and input handers.
 * IMPROVEMENT by FotaHub: Rename set callback to input.
 */
struct jsontree_callback {
  /*
   * ENHANCEMENT by FotaHub: Use only 7 bit to represent JSON tree value type 
   * and leverage last bit to flag JSON tree value modifications.
   */
  uint8_t type : 7;
  uint8_t modified : 1;
  output_handler *output;
  input_handler *input;
};

struct jsontree_pair {
  const char *name;
  struct jsontree_value *value;
};

struct jsontree_object {
  /*
   * ENHANCEMENT by FotaHub: Use only 7 bit to represent JSON tree value type 
   * and leverage last bit to flag JSON tree value modifications.
   */
  uint8_t type : 7;
  uint8_t modified : 1;
  /*
   * ENHANCEMENT by FotaHub: Use only 7 bit to represent number of JSON pairs 
   * and leverage first bit to flag null-valued JSON objects.
   */
  uint8_t null : 1;
  uint8_t count : 7;
  struct jsontree_pair *pairs;
};

struct jsontree_array {
  /*
   * ENHANCEMENT by FotaHub: Use only 7 bit to represent JSON tree value type 
   * and leverage last bit to flag JSON tree value modifications.
   */
  uint8_t type : 7;
  uint8_t modified : 1;
  /*
   * ENHANCEMENT by FotaHub: Use only 7 bit to represent number of JSON pairs 
   * and leverage first bit to flag null-valued JSON arrays.
   */
  uint8_t null : 1;
  uint8_t count : 7;
  struct jsontree_value **values;
};

struct jsontree_ptr {
  /*
   * ENHANCEMENT by FotaHub: Use only 7 bit to represent JSON tree value type 
   * and leverage last bit to flag JSON tree value modifications.
   */
  uint8_t type : 7;
  uint8_t modified : 1;
  /*
   * ENHANCEMENT by FotaHub: Enable jsontree_ptr to be used for document model-based parsing.
   */
  void *value;
};

/*
 * ENHANCEMENT by FotaHub: Add support for enum attributes.
 */
typedef size_t EnumTextEncoderFunc(uint32_t, char*, size_t);
typedef uint32_t EnumTextDecoderFunc(const char*, size_t);
struct jsontree_enum {
  /*
   * ENHANCEMENT by FotaHub: Use only 7 bit to represent JSON tree value type 
   * and leverage last bit to flag JSON tree value modifications.
   */
  uint8_t type : 7;
  uint8_t modified : 1;
  unsigned int value;
  EnumTextEncoderFunc* encoderFunc;
  EnumTextDecoderFunc* decoderFunc;
};

/*
 * ENHANCEMENT by FotaHub: Add support for arbitrary binary attributes.
 */
typedef size_t DataTextEncoderFunc(const void*, size_t, char*, size_t);
typedef size_t DataTextDecoderFunc(const char*, size_t, void*, size_t);
struct jsontree_data {
  /*
   * ENHANCEMENT by FotaHub: Use only 7 bit to represent JSON tree value type 
   * and leverage last bit to flag JSON tree value modifications.
   */
  uint8_t type : 7;
  uint8_t modified : 1;
  void *value;
  size_t valueSize;
  DataTextEncoderFunc* encoderFunc;
  DataTextDecoderFunc* decoderFunc;
};

/*
 * IMPROVEMENT by FotaHub: Renamed jsontree_string to jsontree_string_buffer.
 * ENHANCEMENT by FotaHub: Enable jsontree_string_buffer to be used for document model-based parsing.
 */
#define JSONTREE_STRING_BUFFER(buf, size) {JSON_TYPE_STRING_BUFFER, false, (buf), (size)}
/*
 * ENHANCEMENT by FotaHub: Support retrieval of string values without copying them.
 */
#define JSONTREE_STRING() {JSON_TYPE_STRING, false, NULL, 0}
/*
 * ENHANCEMENT by FotaHub: Add initializer macros for uint and int JSON tree values.
 */
#define JSONTREE_UINT {JSON_TYPE_UINT, false, 0}
#define JSONTREE_INT {JSON_TYPE_INT, false, 0}
/*
 * ENHANCEMENT by FotaHub: Add support for bool attributes.
 */
#define JSONTREE_BOOL {JSON_TYPE_BOOL, false, false}
#define JSONTREE_PAIR(name, value) {(name), false, (struct jsontree_value *)(value)}
/*
 * IMPROVEMENT by FotaHub: Rename set callback to input.
 */
#define JSONTREE_CALLBACK(output, input) {JSON_TYPE_CALLBACK, false, (output), (input)}

#define JSONTREE_OBJECT(name, ...)                                      \
  static struct jsontree_pair jsontree_pair_##name[] = {__VA_ARGS__};   \
  static struct jsontree_object name = {                                \
    JSON_TYPE_OBJECT, false, false				                              \
    sizeof(jsontree_pair_##name)/sizeof(struct jsontree_pair),          \
    jsontree_pair_##name }

#define JSONTREE_OBJECT_EXT(name, ...)                                  \
  static struct jsontree_pair jsontree_pair_##name[] = {__VA_ARGS__};   \
  struct jsontree_object name = {                                       \
    JSON_TYPE_OBJECT, false, false				                              \
    sizeof(jsontree_pair_##name)/sizeof(struct jsontree_pair),          \
    jsontree_pair_##name }

/*
 * IMPROVEMENT by FotaHub: Support initialization of array JSON tree values with pointers to other JSON tree values.
 */
#define JSONTREE_ARRAY(name, count, ...)                                \
  static struct jsontree_value *jsontree_value_##name[count] = {__VA_ARGS__}; \
  static struct jsontree_array name = {                                 \
    JSON_TYPE_ARRAY, false, false                                       \
    count,                        										                  \
    jsontree_value_##name }

/*
 * ENHANCEMENT by FotaHub: Enable user-defined context data to be passed to character output handler
 *  and retrieved from input and output handlers.
 */
void jsontree_setup(struct jsontree_context *js_ctx,
                    struct jsontree_value *root, void *user);
void jsontree_reset(struct jsontree_context *js_ctx);

/*
 * IMPROVEMENT by FotaHub: Auto-determine depth instead of letting user have to specify it.
 */
const char *jsontree_path_name(const struct jsontree_context *js_ctx);

/*
 * ENHANCEMENT by FotaHub: Enable retrieval of current JSON tree value.
 */
struct jsontree_value *jsontree_current(const struct jsontree_context *js_ctx);

/*
 * ENHANCEMENT by FotaHub: Enable user-defined context data to be retrieved from input and output handlers.
 */
void *jsontree_get_user(const struct jsontree_context *js_ctx);

/*
 * ENHANCEMENT by FotaHub: Write character safely to json document.
 */
void jsontree_write_char(struct jsontree_msg* msg,
		char character);

/*
 * ENHANCEMENT by FotaHub: Write terminate character safely to json document.
 */
void jsontree_write_terminate_char(struct jsontree_msg* msg);

void jsontree_write_uint(struct jsontree_msg* msg,
                         unsigned int value);
void jsontree_write_int(struct jsontree_msg* msg, int value);
/*
 * ENHANCEMENT by FotaHub: Add support for bool attributes.
 */
void jsontree_write_bool(struct jsontree_msg* msg, bool value);
void jsontree_write_atom(struct jsontree_msg* msg,
                         const char *text);
/*
 * ENHANCEMENT by FotaHub: Add support for non-zero-terminated strings.
 */
void jsontree_write_string(struct jsontree_msg* msg,
                           const char *text, int len);
/*
 * ENHANCEMENT by FotaHub: Enable serialization to be restricted to JSON tree values that have been marked as modified.
 */
int jsontree_print_next(struct jsontree_context *js_ctx, struct jsontree_msg* msg, bool allAttributes);

/*
 *  BUGFIX by FotaHub: Consistently use int8_t to represent JSON tree value type.
 */
struct jsontree_value *jsontree_find_next(struct jsontree_context *js_ctx,
                                          int8_t type);

/*
 * ENHANCEMENT by FotaHub: Enable serialization to be restricted to JSON tree values that have been marked as modified.
 * ENHANCEMENT by FotaHub: Enabled parsing and serialization of null-valued JSON objects and arrays.
 */
void jsontree_reset_flags(struct jsontree_value* value, bool deep);

#endif /* JSONTREE_H_ */
