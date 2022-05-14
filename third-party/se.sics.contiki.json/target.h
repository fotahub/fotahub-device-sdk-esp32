/*
 * Copyright (c) 2022 FotaHub. All rights reserved.
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
 */

/**
 * \file
 *         Target-specific definitions and standard C library replacements
 * \author
 *         FotaHub <>
 */

#ifndef TARGET_H_
#define TARGET_H_

#ifdef ESP8266

#include <osapi.h>
#include <stdlib.h>

#define PRINTF os_printf
#define ATOI atoi
#define ATOL atol
#define STRLEN os_strlen
#define STRNCMP os_strncmp

#define USE_IF_ELSE_INSTEAD_OF_SWITCH_CASE

#else

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRINTF printf
#define ATOI atoi
#define ATOL atol
#define STRLEN strlen
#define STRNCMP strncmp

#define ICACHE_FLASH_ATTR
#define ICACHE_RODATA_ATTR

#endif

#endif /* TARGET_H_ */
