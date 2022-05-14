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
#ifndef CONNECTIONMANAGEMENTDEFINITIONS_H
#define CONNECTIONMANAGEMENTDEFINITIONS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

enum ConnectionStatus {
  CONNECTION_STATE_CONNECTING_TO_NETWORK = -1,
  CONNECTION_STATE_DISCONNECTING_FROM_NETWORK = -2,
  CONNECTION_STATE_CHECKING_CONNECTIVITY = -3,
  CONNECTION_STATE_CONNECTING_TO_CLOUD = -4,
  CONNECTION_STATE_DISCONNECTING_FROM_CLOUD = -5,
  CONNECTION_STATUS_DISCONNECTED = 0,
  CONNECTION_STATUS_NETWORK_CONNECTION_ERROR = 1,
  CONNECTION_STATUS_CONNECTED_TO_NETWORK = 2,
  CONNECTION_STATUS_CONNECTIVITY_UNAVAILABLE = 3,
  CONNECTION_STATUS_CONNECTIVITY_AVAILABLE = 4,
  CONNECTION_STATUS_DISCONNECTED_FROM_CLOUD = 5,
  CONNECTION_STATUS_CLOUD_CONNECTION_ERROR = 6,
  CONNECTION_STATUS_CONNECTED_TO_CLOUD = 7
};
typedef enum ConnectionStatus ConnectionStatus_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
