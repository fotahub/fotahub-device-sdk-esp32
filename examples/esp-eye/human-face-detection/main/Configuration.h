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
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "FOTAUpdateDefinitions.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "driver/timer.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WIFI_STATION_SSID "Galaxy A10s3731"

#define WIFI_STATION_PASSPHRASE "rhqr2377"

#define UART_TX_PIN (((uint8_t)(GPIO_NUM_1)))

#define UART_RX_PIN (((uint8_t)(GPIO_NUM_3)))

#define UART_UNIT UART_NUM_0

#define BLINK_PERIOD 1000

#define TASK_DEFAULT_STACK_SIZE 4096

extern FOTAUpdateClientConfig_t ESP32_FOTA_UPDATE_CLIENT_CONFIG;

extern uart_config_t UART_CONFIG;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
