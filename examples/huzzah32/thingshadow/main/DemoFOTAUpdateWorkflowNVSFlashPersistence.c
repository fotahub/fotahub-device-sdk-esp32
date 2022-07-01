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
#include "DemoFOTAUpdateWorkflowNVSFlashPersistence.h"

#include "Configuration.h"
#include "DemoFOTAUpdateWorkflowPersistence.h"
#include "nvs_flash.h"
#include <stdlib.h>

void saveState(int16_t state)
{
  nvs_handle *nvsHandle = ((nvs_handle *)(malloc(sizeof(nvs_handle))));
  if (nvsHandle == NULL) 
  {
    return;
  }
  if (nvs_open(DEMO_PRODUCT_STORAGE_NAMESPACE, NVS_READWRITE, nvsHandle) == ESP_OK) 
  {
    nvs_set_u8(*nvsHandle, DEMO_PRODUCT_FLASH_STORE_KEY, ((uint8_t) state));
    nvs_commit(*nvsHandle);
    nvs_close(*nvsHandle);
    free(nvsHandle);
    return;
  }
  free(nvsHandle);
  printf("Failed write statemachine state to NVS\r\n");
}

int16_t loadState(void)
{
  int16_t state = 0;
  nvs_handle *nvsHandle = ((nvs_handle *)(malloc(sizeof(nvs_handle))));
  if (nvsHandle == NULL) 
  {
    return 0;
  }
  if (nvs_open(DEMO_PRODUCT_STORAGE_NAMESPACE, NVS_READONLY, nvsHandle) == ESP_OK) 
  {
    nvs_get_u8(*nvsHandle, DEMO_PRODUCT_FLASH_STORE_KEY, ((uint8_t *)(&state)));
  }
  nvs_close(*nvsHandle);
  free(nvsHandle);
  /* 
   * Return initial state when persisted state is read from an uninitialized or 
   * freshly erased NVS flash sector
   */
  if (state == -1) 
  {
    return 0;
  }
  return state;
}
