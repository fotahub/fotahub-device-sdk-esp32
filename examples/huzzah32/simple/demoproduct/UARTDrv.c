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
#include "UARTDrv.h"

#include "freertos/FreeRTOS.h"
#include <stdlib.h>

static uart_port_t uartUnit;

void uart_init(uart_port_t unit, uart_config_t *config, uint8_t txPin, uint8_t rxPin)
{
  uartUnit = unit;
  /* 
   * Set UART parameters
   */
  uart_param_config(unit, config);
  /* 
   * Set UART pins
   */
  uart_set_pin(unit, txPin, rxPin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  /* 
   * Setup UART driver, this must be called after setting pins
   */
  uart_driver_install(unit, UART_BUFFER_SIZE, UART_BUFFER_SIZE, 0, NULL, 0);
  /* 
   * This function must be executed after uart_driver_install(), when the driver object is initialized
   */
  uart_set_mode(unit, UART_MODE_UART);
}

void uart_recvTask(void)
{
  size_t rxDataLen = 0;
  if (uart_get_buffered_data_len(uartUnit, &rxDataLen) != ESP_OK) 
  {
    return;
  }
  if (rxDataLen == 0) 
  {
    return;
  }
  /* 
   * Configure a temporary buffer for the incoming data
   */
  uint8_t *data = ((uint8_t *)(malloc(rxDataLen)));
  
  uart_read_bytes(uartUnit, data, ((uint32_t)(rxDataLen)), 100 / portTICK_RATE_MS);
  
  for ( int64_t __i = 0 ; __i < rxDataLen; __i++ )
  {
    firmwareUpdateInfoReader_onCharacterReceived(*(data + __i));
  }
  if (data != NULL) 
  {
    free(data);
  }
}
