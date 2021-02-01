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
#include "UARTDefinitions.h"

uart_config_t const UART_BASIC_CONFIG_DEFAULT_FOR_115200_Bd = 
{
  .baud_rate = UART_BAUDRATE_115200, 
  .data_bits = UART_DATA_8_BITS, 
  .parity = UART_PARITY_DISABLE, 
  .stop_bits = UART_STOP_BITS_1, 
  .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, 
  .rx_flow_ctrl_thresh = 122
};

UARTPinConfig_t const UART0_PIN_CONFIG_DEFAULT = 
{
  .tx = GPIO_NUM_1, 
  .rx = GPIO_NUM_3, 
  .rts = GPIO_NUM_22, 
  .cts = GPIO_NUM_19
};

/* 
 * Default pin of UART1 for tx, rx, rts and cts are GPIO10, GPIO9, GPIO11 and GPIO6, but these pins are already connected to the integrated SPI flash of ESP0WROOM-32
 */
UARTPinConfig_t const UART1_PIN_CONFIG_DEFAULT = 
{
  .tx = GPIO_NUM_4, 
  .rx = GPIO_NUM_5, 
  .rts = GPIO_NUM_21, 
  .cts = GPIO_NUM_18
};

/* 
 * Default pin of UART2 for rts and cts are GPIO8 and GPIO7, but these two pins are already connected to the integrated SPI flash of ESP0WROOM-32
 */
UARTPinConfig_t const UART2_PIN_CONFIG_DEFAULT = 
{
  .tx = GPIO_NUM_17, 
  .rx = GPIO_NUM_16, 
  .rts = GPIO_NUM_26, 
  .cts = GPIO_NUM_23
};
