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
#include "Main.h"

#include "Configuration.h"
#include "DemoFOTAUpdateSelfTest.h"
#include "DemoFOTAUpdateWorkflow.h"
#include "DemoFOTAUpdateWorkflowNVSFlashPersistence.h"
#include "BlinkDrv.h"
#include "SinglePartitionMbedTLSAWSIoTThingShadowWiring.h"
#include "DemoProductInfo.h"
#include "FotaHub.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
  awsIoTThingShadowInstances__init();
  
  nvs_init();
  blink_init(BLINK_PERIOD);
  wifi_init();
  
  printf("\n--------------------------------------------------------------------------\n");
  printf("Running %s %s firmware\n", DEMO_PRODUCT_NAME, DEMO_PRODUCT_FIRMWARE_VERSION);
  printf("--------------------------------------------------------------------------\n\n");
  
  fotaUpdateWorkflow_init();
  fotaUpdateSelfTest_init();
  fotahub_init(DEMO_PRODUCT_ID, DEMO_PRODUCT_NAME, &ESP32_FOTA_UPDATE_CLIENT_CONFIG);
  
  wifiStation_connect(WIFI_STATION_SSID, WIFI_STATION_PASSPHRASE);
  
  xTaskCreate(&thingShadowTask, "thingShadowTask", TASK_DEFAULT_STACK_SIZE * 7, NULL, 10, NULL);
  xTaskCreate(&updateTask, "updateTask", TASK_DEFAULT_STACK_SIZE * 5, NULL, 10, NULL);
  xTaskCreate(&blinkTask, "blinkTask", TASK_DEFAULT_STACK_SIZE, NULL, 10, NULL);
}

void thingShadowTask(void *args)
{
  while (true)
  {
    awsIoTThingShadow_run();
    vTaskDelay(10);
  }
}

void updateTask(void *args)
{
  while (true)
  {
    fotahub_run();
    vTaskDelay(10);
  }
}

void blinkTask(void *args)
{
  while (true)
  {
    blink_run();
    vTaskDelay(10);
  }
}
