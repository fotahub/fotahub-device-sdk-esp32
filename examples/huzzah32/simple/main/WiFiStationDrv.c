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
#include "WiFiStationDrv.h"

#include "FotaHub.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "esp_netif_types.h"
#include "esp_wifi_default.h"
#include "esp_event_base.h"
#include <string.h>

static void wifiEventHandlerCallback(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

extern char const *WIFI_EVENT;

extern char const *IP_EVENT;

static uint32_t retryConnect = 0;

void wifi_init(void)
{
  ESP_ERROR_CHECK(esp_netif_init());
  esp_event_loop_create_default();
  esp_netif_create_default_wifi_sta();
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);
  esp_event_handler_instance_t instance_any_id;
  esp_event_handler_instance_t instance_got_ip;
  esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifiEventHandlerCallback, NULL, &instance_any_id);
  esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifiEventHandlerCallback, NULL, &instance_got_ip);
  
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

static void wifiEventHandlerCallback(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
  if (event_base == IP_EVENT) 
  {
    switch (((ip_event_t)event_id))
    {
      case IP_EVENT_STA_GOT_IP:
      {
        retryConnect = 0;
        fotahub_notifyNetworkStatusChange(NETWORK_ADAPTER_STATUS_CONNECTED);
        break;
      }
      case IP_EVENT_STA_LOST_IP:
      {
        fotahub_notifyNetworkStatusChange(NETWORK_ADAPTER_STATUS_DISCONNECTED);
        break;
      }
      default: {
        break;
      }
    }
  }
  else if (event_base == WIFI_EVENT) {
    switch (((wifi_event_t)event_id))
    {
      case WIFI_EVENT_STA_DISCONNECTED:
      {
        if (retryConnect < MAX_RETRY_CONNECT) 
        {
          esp_wifi_connect();
          retryConnect++;
        }
        fotahub_notifyNetworkStatusChange(NETWORK_ADAPTER_STATUS_DISCONNECTED);
        break;
      }
      case WIFI_EVENT_STA_STOP:
      {
        fotahub_notifyNetworkStatusChange(NETWORK_ADAPTER_STATUS_DISCONNECTED);
        break;
      }
      default: {
        break;
      }
    }
  }
}
