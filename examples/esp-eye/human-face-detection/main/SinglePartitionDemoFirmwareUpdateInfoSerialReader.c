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
#include "SinglePartitionDemoFirmwareUpdateInfoSerialReader.h"

#include "DemoFOTAUpdateWorkflow.h"
#include "BinHexConverter.h"
#include "FotaHub.h"
#include "DemoProductInfo.h"
#include "ctype.h"
#include <stdlib.h>
#include <string.h>

enum FirmwareUpdateInfoParser__inevents {
  FirmwareUpdateInfoParser_epsilon__event,
  FirmwareUpdateInfoParser_characterReceived__event
};
typedef enum FirmwareUpdateInfoParser__inevents FirmwareUpdateInfoParser__inevents_t;

enum FirmwareUpdateInfoParser__states {
  FirmwareUpdateInfoParser_receiving_receivingUpdateVersion__state = 0,
  FirmwareUpdateInfoParser_receiving_receivingBadCharacters__state,
  FirmwareUpdateInfoParser_receiving_receivingUpdateVerificationData__state,
  FirmwareUpdateInfoParser_yielding__state
};
typedef enum FirmwareUpdateInfoParser__states FirmwareUpdateInfoParser__states_t;

struct FirmwareUpdateInfoParser__data {
  FirmwareUpdateInfoParser__states_t __currentState;
  size_t characterIdx;
};
typedef struct FirmwareUpdateInfoParser__data FirmwareUpdateInfoParser__data_t;

static void FirmwareUpdateInfoParser__init(FirmwareUpdateInfoParser__data_t *instance);

static bool FirmwareUpdateInfoParser__execute(FirmwareUpdateInfoParser__data_t *instance, FirmwareUpdateInfoParser__inevents_t event, void **arguments);

static char updateVersion[MAX_DEMO_FIRMWARE_UPDATE_VERSION_LENGTH + 1] = { 0 };

static uint8_t *updateVerificationData = NULL;

static FirmwareUpdateInfoParser__data_t updateInfoParser;

void firmwareUpdateInfoReader_init(void)
{
  FirmwareUpdateInfoParser__init(&updateInfoParser);
  {
    size_t updateVerificationDataSize = getVerificationDataSize(DEMO_PRODUCT_FIRMWARE_UPDATE_VERIFICATION_ALGORITHM);
    updateVerificationData = ((uint8_t *)(malloc(updateVerificationDataSize)));
    memset(updateVerificationData, 0, updateVerificationDataSize);
  }

}

void firmwareUpdateInfoReader_run(void)
{
  FirmwareUpdateInfoParser__execute(&updateInfoParser, FirmwareUpdateInfoParser_epsilon__event, NULL);
}

void firmwareUpdateInfoReader_onCharacterReceived(uint8_t character)
{
  uint8_t ___FirmwareUpdateInfoParser_characterReceived_character__arg = character;
  void *___FirmwareUpdateInfoParser_characterReceived__args[1] = { &___FirmwareUpdateInfoParser_characterReceived_character__arg };
  FirmwareUpdateInfoParser__execute(&updateInfoParser, FirmwareUpdateInfoParser_characterReceived__event, ___FirmwareUpdateInfoParser_characterReceived__args);
}

static void FirmwareUpdateInfoParser__init(FirmwareUpdateInfoParser__data_t *instance)
{
  instance->characterIdx = 0;
  instance->__currentState = FirmwareUpdateInfoParser_receiving_receivingUpdateVersion__state;
}

static bool FirmwareUpdateInfoParser__execute(FirmwareUpdateInfoParser__data_t *instance, FirmwareUpdateInfoParser__inevents_t event, void **arguments)
{
  switch (instance->__currentState)
  {
    case FirmwareUpdateInfoParser_receiving_receivingUpdateVersion__state:
    {
      switch (event)
      {
        case FirmwareUpdateInfoParser_characterReceived__event:
        {
          if (((*((uint8_t *)((arguments[0])))) == CARRET || (*((uint8_t *)((arguments[0])))) == LINEF) && instance->characterIdx == 0) 
          {
            /* 
             * enter target state
             */
            break;
          }
          if ((*((uint8_t *)((arguments[0])))) != COLON && isspace((*((uint8_t *)((arguments[0]))))) == 0 && instance->characterIdx < MAX_DEMO_FIRMWARE_UPDATE_VERSION_LENGTH - 1) 
          {
            /* 
             * transition actions
             */
            /* 
             * Append received character to firmware update version string
             */
            updateVersion[instance->characterIdx++] = ((char)((*((uint8_t *)((arguments[0]))))));
            
            /* 
             * enter target state
             */
            break;
          }
          if ((*((uint8_t *)((arguments[0])))) == COLON && instance->characterIdx > 0) 
          {
            /* 
             * transition actions
             */
            /* 
             * Terminate received firmware update version string
             */
            updateVersion[instance->characterIdx] = '\0';
            instance->characterIdx = 0;
            
            /* 
             * enter target state
             */
            instance->__currentState = FirmwareUpdateInfoParser_receiving_receivingUpdateVerificationData__state;
            break;
          }
          if ((*((uint8_t *)((arguments[0])))) != COLON && isspace((*((uint8_t *)((arguments[0]))))) == 0 && instance->characterIdx >= MAX_DEMO_FIRMWARE_UPDATE_VERSION_LENGTH - 1) 
          {
            /* 
             * transition actions
             */
            printf("Received firmware update version too long (max. length = %hhu)\r\n", MAX_DEMO_FIRMWARE_UPDATE_VERSION_LENGTH);
            
            /* 
             * enter target state
             */
            instance->__currentState = FirmwareUpdateInfoParser_receiving_receivingBadCharacters__state;
            break;
          }
          if ((*((uint8_t *)((arguments[0])))) == COLON && instance->characterIdx == 0) 
          {
            /* 
             * transition actions
             */
            printf("Firmware update request with missing version received\r\n");
            
            /* 
             * enter target state
             */
            instance->__currentState = FirmwareUpdateInfoParser_receiving_receivingBadCharacters__state;
            break;
          }
          if (((*((uint8_t *)((arguments[0])))) == CARRET || (*((uint8_t *)((arguments[0])))) == LINEF) && instance->characterIdx > 0) 
          {
            /* 
             * transition actions
             */
            printf("Firmware update request with missing verification data received\r\n");
            memset(updateVersion, 0, sizeof(updateVersion));
            instance->characterIdx = 0;
            
            /* 
             * enter target state
             */
          }
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    case FirmwareUpdateInfoParser_receiving_receivingBadCharacters__state:
    {
      switch (event)
      {
        case FirmwareUpdateInfoParser_characterReceived__event:
        {
          if ((*((uint8_t *)((arguments[0])))) != CARRET && (*((uint8_t *)((arguments[0])))) != LINEF) 
          {
            /* 
             * enter target state
             */
            break;
          }
          if ((*((uint8_t *)((arguments[0])))) == CARRET || (*((uint8_t *)((arguments[0])))) == LINEF) 
          {
            /* 
             * transition actions
             */
            memset(updateVersion, 0, sizeof(updateVersion));
            memset(updateVerificationData, 0, getVerificationDataSize(DEMO_PRODUCT_FIRMWARE_UPDATE_VERIFICATION_ALGORITHM));
            instance->characterIdx = 0;
            
            if (instance->__currentState == FirmwareUpdateInfoParser_receiving_receivingBadCharacters__state) 
            {
              /* 
               * enter target state
               */
              instance->__currentState = FirmwareUpdateInfoParser_receiving_receivingUpdateVersion__state;
            }
          }
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    case FirmwareUpdateInfoParser_receiving_receivingUpdateVerificationData__state:
    {
      switch (event)
      {
        case FirmwareUpdateInfoParser_characterReceived__event:
        {
          if (isxdigit((*((uint8_t *)((arguments[0]))))) != 0 && instance->characterIdx < getVerificationDataSize(DEMO_PRODUCT_FIRMWARE_UPDATE_VERIFICATION_ALGORITHM) << 1) 
          {
            /* 
             * transition actions
             */
            /* 
             * Convert received character from ASCII hex digit to binary nibble 
             * and append the latter to firmware update verification data
             */
            if (((instance->characterIdx & (1)) == 0)) 
            {
              /* 
               * High nibble received
               */
              updateVerificationData[instance->characterIdx >> 1] = convertHexDigitToBinValue(((char)((*((uint8_t *)((arguments[0]))))))) << 4;
            }
            else
            {
              /* 
               * Low nibble received
               */
              (updateVerificationData[instance->characterIdx >> 1] |= (convertHexDigitToBinValue(((char)((*((uint8_t *)((arguments[0])))))))));
            }
            instance->characterIdx++;
            
            /* 
             * enter target state
             */
            break;
          }
          if (((*((uint8_t *)((arguments[0])))) == CARRET || (*((uint8_t *)((arguments[0])))) == LINEF) && instance->characterIdx > 0) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = FirmwareUpdateInfoParser_yielding__state;
            break;
          }
          if (isxdigit((*((uint8_t *)((arguments[0]))))) != 0 && instance->characterIdx >= getVerificationDataSize(DEMO_PRODUCT_FIRMWARE_UPDATE_VERIFICATION_ALGORITHM) << 1) 
          {
            /* 
             * transition actions
             */
            printf("Received firmware update verification data too long (max. length = %d)\r\n", getVerificationDataSize(DEMO_PRODUCT_FIRMWARE_UPDATE_VERIFICATION_ALGORITHM) << 1);
            
            /* 
             * enter target state
             */
            instance->__currentState = FirmwareUpdateInfoParser_receiving_receivingBadCharacters__state;
            break;
          }
          if (((*((uint8_t *)((arguments[0])))) == CARRET || (*((uint8_t *)((arguments[0])))) == LINEF) && instance->characterIdx == 0) 
          {
            /* 
             * transition actions
             */
            printf("Firmware update request with missing verification data received\r\n");
            memset(updateVersion, 0, sizeof(updateVersion));
            memset(updateVerificationData, 0, getVerificationDataSize(DEMO_PRODUCT_FIRMWARE_UPDATE_VERIFICATION_ALGORITHM));
            instance->characterIdx = 0;
            
            if (instance->__currentState == FirmwareUpdateInfoParser_receiving_receivingUpdateVerificationData__state) 
            {
              /* 
               * enter target state
               */
              instance->__currentState = FirmwareUpdateInfoParser_receiving_receivingUpdateVersion__state;
            }
          }
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    case FirmwareUpdateInfoParser_yielding__state:
    {
      switch (event)
      {
        case FirmwareUpdateInfoParser_epsilon__event:
        {
          /* 
           * transition actions
           */
          printf("Firmware update request towards version %s received\r\n", updateVersion);
          fotaUpdateWorkflow_onFirmwareUpdateVersionChanged(updateVersion);
          instance->characterIdx = 0;
          
          if (instance->__currentState == FirmwareUpdateInfoParser_yielding__state) 
          {
            /* 
             * enter target state
             */
            instance->__currentState = FirmwareUpdateInfoParser_receiving_receivingUpdateVersion__state;
          }
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
    default: {
      break;
    }
  }
  return true;
}

void fotahub_onProvideFirmwareUpdateVerificationInfo(FirmwareUpdateInfo_t const *pUpdateInfo, FOTAUpdateVerificationAlgorithm_t *pVerificationAlgorithm, uint8_t const  **ppVerificationData)
{
  *pVerificationAlgorithm = DEMO_PRODUCT_FIRMWARE_UPDATE_VERIFICATION_ALGORITHM;
  *ppVerificationData = updateVerificationData;
}
