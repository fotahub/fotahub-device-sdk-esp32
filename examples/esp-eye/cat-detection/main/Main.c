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
#include "Main.h"

#include "DemoFOTAUpdateSelfTest.h"
#include "Configuration.h"
#include "DemoFOTAUpdateWorkflow.h"
#include "SinglePartitionDemoFirmwareUpdateInfoSerialReader.h"
#include "BlinkDrv.h"
#include "DemoProductInfo.h"
#include "FotaHub.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "NVSDrv.h"
#include "cat_detection_runner.h"

void app_main(void)
{
  nvs_init();
  uart_init(UART_UNIT, &UART_CONFIG, UART_TX_PIN, UART_RX_PIN);
  blink_init(BLINK_PERIOD);
  wifi_init();
  
  printf("\r\n--------------------------------------------------------------------------\r\n");
  printf("Running cat face detection %s %s TinyML model\r\n", DEMO_PRODUCT_NAME, DEMO_PRODUCT_FIRMWARE_VERSION);
  printf("--------------------------------------------------------------------------\r\n\r\n");
  
  firmwareUpdateInfoReader_init();
  fotaUpdateWorkflow_init();
  fotahub_init(DEMO_PRODUCT_ID, DEMO_PRODUCT_NAME, &ESP32_FOTA_UPDATE_CLIENT_CONFIG);
  
  wifiStation_connect(WIFI_STATION_SSID, WIFI_STATION_PASSPHRASE);
  
  xTaskCreate(&demoTask, "demoTask", TASK_DEFAULT_STACK_SIZE * 5, NULL, 10, NULL);
}

void demoTask(void *args)
{
  cat_detection_runner();
  while (true)
  {
    uart_recvTask();
    firmwareUpdateInfoReader_run();
    fotahub_run();
    blink_run();
    vTaskDelay(10);
  }
}
