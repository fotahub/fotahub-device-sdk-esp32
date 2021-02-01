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
#include "WiFiHelper.h"

static esp_err_t setMode(wifi_mode_t mode, bool enable);

esp_err_t enableStationMode(void)
{
  return setMode(WIFI_MODE_STA, true);
}

esp_err_t disableStationMode(void)
{
  return setMode(WIFI_MODE_STA, false);
}

esp_err_t enableAccessPointMode(void)
{
  return setMode(WIFI_MODE_AP, true);
}

esp_err_t disableAccessPointMode(void)
{
  return setMode(WIFI_MODE_AP, false);
}

static esp_err_t setMode(wifi_mode_t mode, bool enable)
{
  wifi_mode_t currentMode;
  esp_err_t error = ESP_OK;
  
  esp_wifi_get_mode(&currentMode);
  printf("Old wifi mode: %u\n", ((uint32_t)((currentMode))));
  
  bool isEnabled = (currentMode & mode) != 0;
  if (isEnabled != enable) 
  {
    if (enable) 
    {
      error = esp_wifi_set_mode(((wifi_mode_t)((currentMode | mode))));
    }
    else
    {
      wifi_mode_t nextMode = ((wifi_mode_t)((currentMode & (~mode))));
      if (nextMode == WIFI_MODE_NULL) 
      {
        /* 
         * Error message "W (66643) wifi: Do not support NONE mode any more!" is print out in case of WIFI_MODE_NULL.
         * Therefore, we deduce that ESP IDF doesn't support WIFI_MODE_NULL any more. Need to call esp_wifi_stop instead.
         */
        esp_wifi_set_mode(WIFI_MODE_NULL);
        error = esp_wifi_stop();
      }
      else
      {
        error = esp_wifi_set_mode(nextMode);
      }
    }
  }
  return error;
}

bool getIPConfig(IPConfig_t *pIPConfig, wifi_mode_t mode)
{
  if (pIPConfig == NULL) 
  {
    return false;
  }
  tcpip_adapter_ip_info_t info;
  if (tcpip_adapter_get_ip_info((mode == WIFI_MODE_AP) ? (TCPIP_ADAPTER_IF_AP) : (TCPIP_ADAPTER_IF_STA), &info) == ESP_OK) 
  {
    pIPConfig->localIPAddress = info.ip.addr;
    pIPConfig->gatewayIPAddress = info.gw.addr;
    pIPConfig->subnetMask = info.netmask.addr;
    return true;
  }
  return false;
}
