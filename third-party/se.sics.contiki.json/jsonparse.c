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

#include "jsonparse.h"
/*
 * IMPROVEMENT by FotaHub: Allow standard C library dependencies to be controlled through target.h.
 */
// #include <stdlib.h>
// #include <string.h>
#include "target.h"

/*--------------------------------------------------------------------*/
static int ICACHE_FLASH_ATTR
push(struct jsonparse_state *state, char c)
{
  state->stack[state->depth] = c;
  state->depth++;
  state->vtype = 0;
  return state->depth < JSONPARSE_MAX_DEPTH;
}
/*--------------------------------------------------------------------*/
static void ICACHE_FLASH_ATTR
modify(struct jsonparse_state *state, char c)
{
  if(state->depth > 0) {
    state->stack[state->depth - 1] = c;
  }
}
/*--------------------------------------------------------------------*/
static char ICACHE_FLASH_ATTR
pop(struct jsonparse_state *state)
{
  if(state->depth == 0) {
    /*
     * IMPROVEMENT by FotaHub: Return JSON_TYPE_ERROR only in case of JSON_ERROR_SYNTAX and JSON_TYPE_END otherwise.
     */
	// return JSON_TYPE_ERROR;
    return JSON_TYPE_END;
  }
  state->depth--;
  state->vtype = state->stack[state->depth];
  return state->stack[state->depth];
}
/*--------------------------------------------------------------------*/
/* will pass by the value and store the start and length of the value for
   atomic types */
/*--------------------------------------------------------------------*/
static char ICACHE_FLASH_ATTR
atomic(struct jsonparse_state *state, char type)
{
  char c;
  const char *str;
  int len;

  state->vstart = state->msg.pos;
  if(type == JSON_TYPE_STRING || type == JSON_TYPE_PAIR_NAME) {
    while((c = state->msg.base[state->msg.pos++]) && c != '"') {
      if(c == '\\') {
        state->msg.pos++;           /* skip current char */
      }
    }
    if(c != '"') {
      state->error = JSON_ERROR_SYNTAX;
      return JSON_TYPE_ERROR;
    }
    state->vlen = state->msg.pos - state->vstart - 1;
  } else if(type == JSON_TYPE_NUMBER) {
    do {
      c = state->msg.base[state->msg.pos];
      if((c < '0' || c > '9') && c != '.') {
        c = 0;
      } else {
        state->msg.pos++;
      }
    } while(c);
    /* need to back one step since first char is already gone */
    state->vstart--;
    state->vlen = state->msg.pos - state->vstart;
  } else if(type == JSON_TYPE_NULL || type == JSON_TYPE_TRUE || type == JSON_TYPE_FALSE) {
    state->vstart--;

    #ifdef USE_IF_ELSE_INSTEAD_OF_SWITCH_CASE
    if(type == JSON_TYPE_NULL) {
      str = "null";
    } else if(type == JSON_TYPE_TRUE) {
      str = "true";
    } else if(type == JSON_TYPE_FALSE) {
      str = "false";
    } else {
      str = "";
    }
    #else // USE_IF_ELSE_INSTEAD_OF_SWITCH_CASE
    switch (type) {
    case JSON_TYPE_NULL:  str = "null";  break;
    case JSON_TYPE_TRUE:  str = "true";  break;
    case JSON_TYPE_FALSE: str = "false"; break;
    default:              str = "";      break;
    }
    #endif // USE_IF_ELSE_INSTEAD_OF_SWITCH_CASE

    while((c = state->msg.base[state->msg.pos]) && c != ' ' && c != ',' && c != ']' && c != '}') {
      state->msg.pos++;
    }

    state->vlen = state->msg.pos - state->vstart;
    len = STRLEN(str);
    len = state->vlen > len ? state->vlen : len;

    if(STRNCMP(str, &state->msg.base[state->vstart], len) != 0) {
      state->error = JSON_ERROR_SYNTAX;
      return JSON_TYPE_ERROR;
    }
  }

  state->vtype = type;
  return state->vtype;
}
/*--------------------------------------------------------------------*/
static void ICACHE_FLASH_ATTR
skip_ws(struct jsonparse_state *state)
{
  char c;

  while(state->msg.pos < state->msg.len &&
        ((c = state->msg.base[state->msg.pos]) == ' ' || c == '\n')) {
    state->msg.pos++;
  }
}
/*--------------------------------------------------------------------*/
static int ICACHE_FLASH_ATTR
is_atomic(struct jsonparse_state *state)
{
  char v = state->vtype;
  if(v == 'N' || v == '"' || v == '0' || v == 'n' || v == 't' || v == 'f') {
    return 1;
  } else {
    return 0;
  }
}
/*--------------------------------------------------------------------*/
void ICACHE_FLASH_ATTR
jsonparse_setup(struct jsonparse_state *state, char *json, int len)
{
  state->msg.base = json;
  state->msg.len = len;
  state->msg.pos = 0;
  state->depth = 0;
  /*
   * IMPROVEMENT by FotaHub: Use enum literal rather than underlying integer value.
   */
  state->error = JSON_ERROR_OK;
  state->vtype = 0;
  state->stack[0] = 0;
}
/*--------------------------------------------------------------------*/
int ICACHE_FLASH_ATTR
jsonparse_next(struct jsonparse_state *state)
{
  char c;
  char s;
  char v;

  skip_ws(state);
  /*
   * ENHANCEMENT by FotaHub: Make sure that parsing stops at the end of the JSON string.
   */
  if(state->msg.pos < state->msg.len) {
	c = state->msg.base[state->msg.pos];
	s = jsonparse_get_type(state);
	v = state->vtype;
	state->msg.pos++;
  } else {
	c = 0;
	s = 0;
	v = 0;
  }

  #ifdef USE_IF_ELSE_INSTEAD_OF_SWITCH_CASE
  if(c == '{') {
    if((s == 0 && v == 0) || s == '[' || s == ':') {
      push(state, c);
    } else {
      state->error = JSON_ERROR_UNEXPECTED_OBJECT;
      return JSON_TYPE_ERROR;
    }
    return c;
  } else if(c == '}') {
    if((s == ':' && v != ',' && v != 0 ) || (s == '{' && v == 0)) {
      pop(state);
    } else {
      state->error = JSON_ERROR_UNEXPECTED_END_OF_OBJECT;
      return JSON_TYPE_ERROR;
    }
    return c;
  } else if(c == ']') {
    if(s == '[' && v != ',') {
      pop(state);
    } else {
      state->error = JSON_ERROR_UNEXPECTED_END_OF_ARRAY;
      return JSON_TYPE_ERROR;
    }
    return c;
  } else if(c == ':') {
    if(s == '{' && v == 'N') {
      modify(state, ':');
      state->vtype = 0;
    } else {
      state->error = JSON_ERROR_SYNTAX;
      return JSON_TYPE_ERROR;
    }
    return jsonparse_next(state);
  } else if(c == ',') {
    if(s == ':' && v != 0) {
      modify(state, '{');
      state->vtype = c;
    } else if(s == '[') {
      state->vtype = c;
    } else {
      state->error = JSON_ERROR_SYNTAX;
      return JSON_TYPE_ERROR;
    }
    /*
     *  BUGFIX by FotaHub: Don't return ',' as parse result but continue parsing instead (just as it is already the case for ':').
     */
    //return c;
    return jsonparse_next(state);
    } else if(c == '"') {
      if((s == 0 && v == 0) || s == '{' || s == '[' || s == ':') {
        return atomic(state, c = (s == '{' ? JSON_TYPE_PAIR_NAME : c));
      } else {
        state->error = JSON_ERROR_UNEXPECTED_STRING;
        return JSON_TYPE_ERROR;
      }
      return c;
    } else if(c == '[') {
      if((s == 0 && v == 0) || s == '[' || s == ':') {
        push(state, c);
      } else {
        state->error = JSON_ERROR_UNEXPECTED_ARRAY;
        return JSON_TYPE_ERROR;
      }
      return c;
    } else if(c == 0) {
      /*
     * ENHANCEMENT by FotaHub: Return JSON_TYPE_ERROR only in case of JSON_ERROR_XXX errors and use JSON_TYPE_END to indicate that parsing is finished.
     */
    if(v == 0 || state->depth > 0) {
      state->error = JSON_ERROR_SYNTAX;
      return JSON_TYPE_ERROR;
    }
	// return JSON_TYPE_ERROR;
    return JSON_TYPE_END;
    } else {
      if(s == 0 || s == ':' || s == '[') {
        if(v != 0 && v != ',') {
          state->error = JSON_ERROR_SYNTAX;
          return JSON_TYPE_ERROR;
        }
        if(c == '-' || (c <= '9' && c >= '0')) {
          return atomic(state, JSON_TYPE_NUMBER);
        } else if(c == 'n') {
          return atomic(state, JSON_TYPE_NULL);
        } else if(c == 't') {
          return atomic(state, JSON_TYPE_TRUE);
        } else if(c == 'f') {
          return atomic(state, JSON_TYPE_FALSE);
        } else {
          state->error = JSON_ERROR_SYNTAX;
          return JSON_TYPE_ERROR;
        }
      } else if(s == '{') {
        state->error = JSON_ERROR_SYNTAX;
        return JSON_TYPE_ERROR;
      }
    }
  #else // USE_IF_ELSE_INSTEAD_OF_SWITCH_CASE
  switch(c) {
  case '{':
    if((s == 0 && v == 0) || s == '[' || s == ':') {
      push(state, c);
    } else {
      state->error = JSON_ERROR_UNEXPECTED_OBJECT;
      return JSON_TYPE_ERROR;
    }
    return c;
  case '}':
    if((s == ':' && v != ',' && v != 0 ) || (s == '{' && v == 0)) {
      pop(state);
    } else {
      state->error = JSON_ERROR_UNEXPECTED_END_OF_OBJECT;
      return JSON_TYPE_ERROR;
    }
    return c;
  case ']':
    if(s == '[' && v != ',') {
      pop(state);
    } else {
      state->error = JSON_ERROR_UNEXPECTED_END_OF_ARRAY;
      return JSON_TYPE_ERROR;
    }
    return c;
  case ':':
    if(s == '{' && v == 'N') {
      modify(state, ':');
      state->vtype = 0;
    } else {
      state->error = JSON_ERROR_SYNTAX;
      return JSON_TYPE_ERROR;
    }
    return jsonparse_next(state);
  case ',':
    if(s == ':' && v != 0) {
      modify(state, '{');
      state->vtype = c;
    } else if(s == '[') {
      state->vtype = c;
    } else {
      state->error = JSON_ERROR_SYNTAX;
      return JSON_TYPE_ERROR;
    }
    /*
     *  BUGFIX by FotaHub: Don't return ',' as parse result but continue parsing instead (just as it is already the case for ':').
     */
    //return c;
    return jsonparse_next(state);
  case '"':
    if((s == 0 && v == 0) || s == '{' || s == '[' || s == ':') {
      return atomic(state, c = (s == '{' ? JSON_TYPE_PAIR_NAME : c));
    } else {
      state->error = JSON_ERROR_UNEXPECTED_STRING;
      return JSON_TYPE_ERROR;
    }
    return c;
  case '[':
    if((s == 0 && v == 0) || s == '[' || s == ':') {
      push(state, c);
    } else {
      state->error = JSON_ERROR_UNEXPECTED_ARRAY;
      return JSON_TYPE_ERROR;
    }
    return c;
  case 0:
    /*
     * ENHANCEMENT by FotaHub: Return JSON_TYPE_ERROR only in case of JSON_ERROR_XXX errors and use JSON_TYPE_END to indicate that parsing is finished.
     */
    if(v == 0 || state->depth > 0) {
      state->error = JSON_ERROR_SYNTAX;
      return JSON_TYPE_ERROR;
    }
	// return JSON_TYPE_ERROR;
    return JSON_TYPE_END;
  default:
    if(s == 0 || s == ':' || s == '[') {
      if(v != 0 && v != ',') {
        state->error = JSON_ERROR_SYNTAX;
        return JSON_TYPE_ERROR;
      }
      if(c == '-' || (c <= '9' && c >= '0')) {
        return atomic(state, JSON_TYPE_NUMBER);
      } else if(c == 'n') {
        return atomic(state, JSON_TYPE_NULL);
      } else if(c == 't') {
        return atomic(state, JSON_TYPE_TRUE);
      } else if(c == 'f') {
        return atomic(state, JSON_TYPE_FALSE);
      } else {
        state->error = JSON_ERROR_SYNTAX;
        return JSON_TYPE_ERROR;
      }
    } else if(s == '{') {
      state->error = JSON_ERROR_SYNTAX;
      return JSON_TYPE_ERROR;
    }
  }
  #endif // USE_IF_ELSE_INSTEAD_OF_SWITCH_CASE

  return 0;
}
/*--------------------------------------------------------------------*/
/*
 * IMPROVEMENT by FotaHub: Renamed jsonparse_copy_value() to jsonparse_copy_value_as_string().
 */
/* copies the json value at the current position
 * works only on "atomic" values such as string, number, null, false, true
 */
/*--------------------------------------------------------------------*/
int ICACHE_FLASH_ATTR
jsonparse_copy_value_as_string(struct jsonparse_state *state, char *str, int size)
{
  int i, o;
  char c;

  if(!is_atomic(state)) {
    return JSON_TYPE_ERROR;
  }
  for(i = 0, o = 0; i < state->vlen && o < size - 1; i++) {
    c = state->msg.base[state->vstart + i];
    if(c == '\\') {
      i++;

      #ifdef USE_IF_ELSE_INSTEAD_OF_SWITCH_CASE
      char ch = state->msg.base[state->vstart + i];
      if(ch == '"') {
        str[o++] = '"';
      } else if(ch == '\\') {
        str[o++] = '\\';
      } else if(ch == '/') {
        str[o++] = '/';
      } else if(ch == 'b') {
        str[o++] = '\b';
      } else if(ch == 'f') {
        str[o++] = '\f';
      } else if(ch == 'n') {
        str[o++] = '\n';
      } else if(ch == 'r') {
        str[o++] = '\r';
      } else if(ch == 't') {
        str[o++] = '\t';
      }
      #else // USE_IF_ELSE_INSTEAD_OF_SWITCH_CASE
      switch(state->msg.base[state->vstart + i]) {
      case '"':  str[o++] = '"';  break;
      case '\\': str[o++] = '\\'; break;
      case '/':  str[o++] = '/';  break;
      case 'b':  str[o++] = '\b'; break;
      case 'f':  str[o++] = '\f'; break;
      case 'n':  str[o++] = '\n'; break;
      case 'r':  str[o++] = '\r'; break;
      case 't':  str[o++] = '\t'; break;
      }
      #endif // USE_IF_ELSE_INSTEAD_OF_SWITCH_CASE
      
      continue;
    }
    str[o++] = c;
  }
  str[o] = 0;
  return state->vtype;
}
/*--------------------------------------------------------------------*/
/*
 * ENHANCEMENT by FotaHub: Support retrieval of string values without copying them.
 */
const char* ICACHE_FLASH_ATTR
jsonparse_get_value_as_string(struct jsonparse_state *state)
{
  if(jsonparse_copy_value_as_string(state, &state->msg.base[state->vstart], state->vlen + 1) == JSON_TYPE_ERROR)
  {
    return NULL;
  }
  return &state->msg.base[state->vstart];
}
/*--------------------------------------------------------------------*/
int ICACHE_FLASH_ATTR
jsonparse_get_value_as_int(struct jsonparse_state *state)
{
  if(state->vtype != JSON_TYPE_NUMBER) {
    return 0;
  }
  return ATOI(&state->msg.base[state->vstart]);
}
/*--------------------------------------------------------------------*/
/*
 * ENHANCEMENT by FotaHub: Support retrieval of unsigned int values.
 */
unsigned int ICACHE_FLASH_ATTR
jsonparse_get_value_as_uint(struct jsonparse_state *state)
{
  return (unsigned int) jsonparse_get_value_as_int(state);
}
/*--------------------------------------------------------------------*/
long ICACHE_FLASH_ATTR
jsonparse_get_value_as_long(struct jsonparse_state *state)
{
  if(state->vtype != JSON_TYPE_NUMBER) {
    return 0;
  }
  return ATOL(&state->msg.base[state->vstart]);
}
/*--------------------------------------------------------------------*/
/*
 * ENHANCEMENT by FotaHub: Support retrieval of unsigned long values.
 */
unsigned long ICACHE_FLASH_ATTR
jsonparse_get_value_as_ulong(struct jsonparse_state *state)
{
  return (unsigned long) jsonparse_get_value_as_long(state);
}
/*--------------------------------------------------------------------*/
/*
 * ENHANCEMENT by FotaHub: Support retrieval of bool values.
 */
bool ICACHE_FLASH_ATTR
jsonparse_get_value_as_bool(struct jsonparse_state *state)
{
  if(jsonparse_strcmp_value(state, "true") == 0)
    return true;
  else
    return false;
}
/*--------------------------------------------------------------------*/
/* strcmp - assume no strange chars that needs to be stuffed in string... */
/*--------------------------------------------------------------------*/
int ICACHE_FLASH_ATTR
jsonparse_strcmp_value(struct jsonparse_state *state, const char *str)
{
  /*
   *  BUGFIX by FotaHub: Return false when the length of values is different.
   */
  if(STRLEN(str) != state->vlen) {
	return -1;
  }

  if(!is_atomic(state)) {
    return -1;
  }
  return STRNCMP(str, &state->msg.base[state->vstart], state->vlen);
}
/*--------------------------------------------------------------------*/
/*
 * ENHANCEMENT by FotaHub: Allow for direct access to current JSON value.
 */
/* get a pointer the json value at the current position
 */
/*--------------------------------------------------------------------*/
const char * ICACHE_FLASH_ATTR
jsonparse_get_value(struct jsonparse_state *state) {
  return &state->msg.base[state->vstart];
}
/*--------------------------------------------------------------------*/
/* get the length of the json value at the current position
 */
/*--------------------------------------------------------------------*/
int ICACHE_FLASH_ATTR
jsonparse_get_len(struct jsonparse_state *state)
{
  return state->vlen;
}
/*--------------------------------------------------------------------*/
int ICACHE_FLASH_ATTR
jsonparse_get_type(struct jsonparse_state *state)
{
  if(state->depth == 0) {
    return 0;
  }
  return state->stack[state->depth - 1];
}
/*--------------------------------------------------------------------*/
int ICACHE_FLASH_ATTR
jsonparse_has_next(struct jsonparse_state *state)
{
  return state->msg.pos < state->msg.len;
}
/*--------------------------------------------------------------------*/
/*
 * ENHANCEMENT by FotaHub: Enable retrieval of parse errors.
 */
enum jsonparse_error ICACHE_FLASH_ATTR
jsonparse_get_error(struct jsonparse_state *state)
{
  return state->error;
}
/*--------------------------------------------------------------------*/
