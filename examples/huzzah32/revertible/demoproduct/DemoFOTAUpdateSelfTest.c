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
#include "DemoFOTAUpdateSelfTest.h"

#include "FotaHub.h"
#include <stdlib.h>
#include <time.h>
#include "DemoProductInfo.h"

static bool runRandomSelfTest(uint8_t successRate);

void fotaUpdateSelfTest_init(void)
{
  srand(((uint32_t)(time(NULL))));
}

static bool runRandomSelfTest(uint8_t successRate)
{
  if (((uint8_t)((rand() % 100))) >= (successRate)) 
  {
    printf("Firmware self test failed (reason: simulated pseudo-random failure)\n");
    return false;
  }
  return true;
}

void validateFirmwareUpdateActivation(void)
{
  printf("Validating firmware update\n");
  /* 
   * Perform any sort of tests and checks to see if device behaves as expected after firmware over-the-air update
   */
  if (runRandomSelfTest(SIMULATED_ACTIVATION_SUCCESS_RATE)) 
  {
    printf("Firmware update successfully activated\n");
    fotahub_onFirmwareUpdateStatusChanged(FOTA_UPDATE_STATUS_ACTIVATION_SUCCEEDED);
  }
  else
  {
    printf("Firmware update activation failed\n");
    fotahub_onFirmwareUpdateStatusChanged(FOTA_UPDATE_STATUS_ACTIVATION_FAILED);
  }
}

void validateFirmwareUpdateReversion(void)
{
  printf("Validating previous firmware\n");
  printf("Firmware update successfully reverted\n");
  fotahub_onFirmwareUpdateStatusChanged(FOTA_UPDATE_STATUS_REVERSION_SUCCEEDED);
}
