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
#include "StringHelper.h"

#include <string.h>

size_t strnlen(char const *str, size_t maxLen)
{
  size_t len = 0;
  while ((len < maxLen) && ((*str) != '\0'))
  {
    str++;
    len++;
  }
  return len;
}

char *StringHelper_strnstr(char *s1, size_t s1Len, char const *s2)
{
  size_t s2Len = strlen(s2);
  if (s2Len == 0) 
  {
    return NULL;
  }
  
  for ( int64_t __i = 0 ; __i <= (s1Len - s2Len); __i++ )
  {
    if (strncmp(s1, s2, s2Len) == 0) 
    {
      return s1;
    }
    s1++;
  }
  return NULL;
}

/* 
 * TODO Make
 */
char *strnchr(char *s, size_t len, int32_t charDelimitor)
{
  for ( int64_t __i = 0 ; __i <= len; __i++ )
  {
    if (s[__i] == charDelimitor) 
    {
      return s + __i;
    }
  }
  return NULL;
}

size_t strnspn(char const *s1, size_t s1Len, char const *s2)
{
  size_t count = 0;
  size_t s2Len = strlen(s2);
  for ( int64_t __i = 0 ; __i < s1Len; __i++ )
  {
    size_t k = 0;
    for ( int64_t __j = 0 ; __j < s2Len; __j++ )
    {
      k = __j;
      if (s1[__i] == s2[__j]) 
      {
        break;
      }
    }
    if (k == s2Len - 1) 
    {
      break;
    }
    else
    {
      count++;
    }
  }
  return count;
}

size_t strcnspn(char const *s1, size_t s1Len, char const *s2)
{
  size_t count = 0;
  size_t s2Len = strlen(s2);
  for ( int64_t __i = 0 ; __i < s1Len; __i++ )
  {
    size_t k = 0;
    for ( int64_t __j = 0 ; __j < s2Len; __j++ )
    {
      k = __j;
      if (s1[__i] == s2[__j]) 
      {
        break;
      }
    }
    if (k == s2Len - 1) 
    {
      count++;
    }
    else
    {
      break;
    }
  }
  return count;
}
