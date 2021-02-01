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
#ifndef WIFIDEFINITIONS_H
#define WIFIDEFINITIONS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WIFI_SSID_MAX_LENGTH 32

#define WIFI_BSSID_MAX_LENGTH 18

#define WIFI_PASSWORD_MAX_LENGTH 64

#define WIFI_PASSWORD_MIN_LENGTH 8

#define WIFI_WEP_PASSPHRASE_MIN_LENGTH 10

#define MAC_ADDRESS_SIZE 6

#define IPV4_ADDRESS_SIZE 32

#define WIFI_RSSI_MIN_VALUE -121

enum WiFiChannel {
  WIFI_CHANNEL_1 = 0,
  WIFI_CHANNEL_2,
  WIFI_CHANNEL_3,
  WIFI_CHANNEL_4,
  WIFI_CHANNEL_5,
  WIFI_CHANNEL_6,
  WIFI_CHANNEL_7,
  WIFI_CHANNEL_8,
  WIFI_CHANNEL_9,
  WIFI_CHANNEL_10,
  WIFI_CHANNEL_11,
  WIFI_CHANNEL_12,
  WIFI_CHANNEL_13
};
typedef enum WiFiChannel WiFiChannel_t;

enum WiFiStationStatus {
  WIFI_STATION_STATUS_IDLE = 0,
  WIFI_STATION_STATUS_WRONG_PASSWORD = 1,
  WIFI_STATION_STATUS_SSID_NOT_REACHABLE = 2,
  WIFI_STATION_STATUS_ACCES_DENIED = 3,
  WIFI_STATION_STATUS_CANNOT_GET_IP = 4,
  WIFI_STATION_STATUS_MAX_USER_REACHED = 5,
  WIFI_STATION_STATUS_CONNECT_FAILED = 6,
  WIFI_STATION_STATUS_GETTING_IP = 12,
  WIFI_STATION_STATUS_CONNECTED = 7,
  WIFI_STATION_STATUS_CONNECTION_LOST = 8,
  WIFI_STATION_STATUS_DISCONNECTED = 9,
  WIFI_STATION_STATUS_STOPPED = 10,
  WIFI_STATION_STATUS_STALE = 11
};
typedef enum WiFiStationStatus WiFiStationStatus_t;

enum WiFiAccessPointStatus {
  WIFI_ACCESS_POINT_STATUS_NO_SHIELD = 255,
  WIFI_ACCESS_POINT_STATUS_IDLE = 0,
  WIFI_ACCESS_POINT_STATUS_STARTED = 1,
  WIFI_ACCESS_POINT_STATUS_CONNECTED = 2,
  WIFI_ACCESS_POINT_STATUS_DISCONNECTED = 3,
  WIFI_ACCESS_POINT_STATUS_STOPPED = 4
};
typedef enum WiFiAccessPointStatus WiFiAccessPointStatus_t;

struct IPConfig {
  uint32_t localIPAddress;
  uint32_t gatewayIPAddress;
  uint32_t subnetMask;
};
typedef struct IPConfig IPConfig_t;

enum WiFiAuthMode {
  WiFiDefinitions_WiFiAuthMode__WIFI_AUTH_OPEN = 0,
  WiFiDefinitions_WiFiAuthMode__WIFI_AUTH_WEP,
  WiFiDefinitions_WiFiAuthMode__WIFI_AUTH_WPA_PSK,
  WiFiDefinitions_WiFiAuthMode__WIFI_AUTH_WPA2_PSK,
  WiFiDefinitions_WiFiAuthMode__WIFI_AUTH_WPA_WPA2_PSK,
  WiFiDefinitions_WiFiAuthMode__WIFI_AUTH_WPA2_ENTERPRISE,
  WiFiDefinitions_WiFiAuthMode__WIFI_AUTH_MAX
};
typedef enum WiFiAuthMode WiFiAuthMode_t;

struct WiFiNetworkInfo {
  uint8_t bssid[WIFI_BSSID_MAX_LENGTH];
  /* 
   * the last one for null termination
   */
  uint8_t ssid[WIFI_SSID_MAX_LENGTH + 1];
  int8_t rssi;
  WiFiAuthMode_t authMode;
  WiFiChannel_t channel;
  bool isHidden;
};
typedef struct WiFiNetworkInfo WiFiNetworkInfo_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
