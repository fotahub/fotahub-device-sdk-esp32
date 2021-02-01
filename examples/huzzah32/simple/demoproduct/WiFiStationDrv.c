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
#include "WiFiStationDrv.h"

#include "FotaHub.h"
#include "esp_event_loop.h"
#include "esp_wifi.h"
#include <string.h>

static esp_err_t wifiEventHandlerCallback(void *ctx, system_event_t *event);

void wifi_init(void)
{
  tcpip_adapter_init();
  esp_event_loop_init(&wifiEventHandlerCallback, NULL);
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);
  esp_wifi_set_mode(WIFI_MODE_STA);
  esp_wifi_set_storage(WIFI_STORAGE_RAM);
}

bool wifiStation_connect(char const *ssid, char const *passphrase)
{
  wifi_config_t config = 
  {
    .sta = 
    {
      .bssid_set = false
    }
  };
  size_t ssidLen = strlen(ssid);
  memcpy(config.sta.ssid, ssid, ssidLen);
  if (ssidLen < sizeof(config.sta.ssid)) 
  {
    config.sta.ssid[ssidLen] = 0;
  }
  size_t passphraseLen = strlen(passphrase);
  memcpy(config.sta.password, passphrase, passphraseLen);
  if (passphraseLen < sizeof(config.sta.password)) 
  {
    config.sta.password[passphraseLen] = 0;
  }
  
  esp_wifi_set_config(WIFI_IF_STA, &config);
  esp_wifi_start();
  esp_wifi_connect();
  
  return true;
}

static esp_err_t wifiEventHandlerCallback(void *ctx, system_event_t *event)
{
  switch (event->event_id)
  {
    case SYSTEM_EVENT_STA_GOT_IP:
    {
      fotahub_notifyNetworkStatusChange(NETWORK_ADAPTER_STATUS_CONNECTED);
      break;
    }
    case SYSTEM_EVENT_STA_LOST_IP:
    case SYSTEM_EVENT_STA_DISCONNECTED:
    case SYSTEM_EVENT_STA_STOP:
    case SYSTEM_EVENT_STA_DHCP_TIMEOUT:
    {
      fotahub_notifyNetworkStatusChange(NETWORK_ADAPTER_STATUS_DISCONNECTED);
      break;
    }
    default: {
      break;
    }
  }
  return ESP_OK;
}
