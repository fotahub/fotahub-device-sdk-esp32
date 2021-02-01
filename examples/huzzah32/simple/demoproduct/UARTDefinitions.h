/*
 *  Copyright (C) 2020 FotaHub Inc. All rights reserved.
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
#ifndef UARTDEFINITIONS_H
#define UARTDEFINITIONS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "driver/uart.h"
#include "driver/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

struct UARTPinConfig {
  gpio_num_t tx;
  gpio_num_t rx;
  gpio_num_t rts;
  gpio_num_t cts;
};
typedef struct UARTPinConfig UARTPinConfig_t;

/* 
 * If pPinConfig is NULL, then target uart will use default pin settings.
 */
struct UARTConfig {
  uart_config_t const *pBasicConfig;
  uart_port_t uartPort;
  UARTPinConfig_t const *pPinConfig;
  int32_t bufferSize;
};
typedef struct UARTConfig UARTConfig_t;

enum UARTBaudrate {
  UART_BAUDRATE_300 = 300,
  UART_BAUDRATE_600 = 600,
  UART_BAUDRATE_1200 = 1200,
  UART_BAUDRATE_2400 = 2400,
  UART_BAUDRATE_4800 = 4800,
  UART_BAUDRATE_9600 = 9600,
  UART_BAUDRATE_19200 = 19200,
  UART_BAUDRATE_38400 = 38400,
  UART_BAUDRATE_57600 = 57600,
  UART_BAUDRATE_74880 = 74880,
  UART_BAUDRATE_115200 = 115200,
  UART_BAUDRATE_230400 = 230400,
  UART_BAUDRATE_460800 = 460800,
  UART_BAUDRATE_921600 = 921600,
  UART_BAUDRATE_1843200 = 1843200,
  UART_BAUDRATE_3686400 = 3686400
};
typedef enum UARTBaudrate UARTBaudrate_t;

#define UART_DEFAULT_BUFFER_SIZE 512

extern uart_config_t const UART_BASIC_CONFIG_DEFAULT_FOR_115200_Bd;

extern UARTPinConfig_t const UART0_PIN_CONFIG_DEFAULT;

/* 
 * Default pin of UART1 for tx, rx, rts and cts are GPIO10, GPIO9, GPIO11 and GPIO6, but these pins are already connected to the integrated SPI flash of ESP0WROOM-32
 */
extern UARTPinConfig_t const UART1_PIN_CONFIG_DEFAULT;

/* 
 * Default pin of UART2 for rts and cts are GPIO8 and GPIO7, but these two pins are already connected to the integrated SPI flash of ESP0WROOM-32
 */
extern UARTPinConfig_t const UART2_PIN_CONFIG_DEFAULT;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
