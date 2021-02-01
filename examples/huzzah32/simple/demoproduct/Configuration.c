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
#include "Configuration.h"

FOTAUpdateClientConfig_t ESP32_FOTA_UPDATE_CLIENT_CONFIG = 
{
  .authenticateUpdateServer = true, 
  .maxUpdateDownloadFragmentLength = TLS_MAX_FRAGMENT_LENGTH_DISABLED, 
  .maxUpdateDownloadDataRate = TLS_MAX_DATA_RATE_NONE, 
  .updateDownloadTimeoutInterval = FIRMWARE_UPDATE_DOWNLOAD_TIMEOUT_DEFAULT, 
  .maxUpdateDownloadRetryCount = MAX_FIRMWARE_UPDATE_DOWNLOAD_RETRY_COUNT_DEFAULT
};

uart_config_t UART_CONFIG = 
{
  .baud_rate = 115200, 
  .data_bits = UART_DATA_8_BITS, 
  .parity = UART_PARITY_DISABLE, 
  .stop_bits = UART_STOP_BITS_1, 
  .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, 
  .rx_flow_ctrl_thresh = 0
};
