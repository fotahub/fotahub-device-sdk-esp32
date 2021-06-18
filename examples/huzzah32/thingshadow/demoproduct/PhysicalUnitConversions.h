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
#ifndef PHYSICALUNITCONVERSIONS_H
#define PHYSICALUNITCONVERSIONS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "MathConstants.h"

#ifdef __cplusplus
extern "C" {
#endif

#define nounit_to_percent_any(val) (val * 100)

#define percent_to_nounit_any(val) (val / 100)

#define nounit_to_permille_any(val) (val * 1000)

#define permille_to_nounit_any(val) (val / 1000)

#define nounit_to_ppm_any(val) (val * 1000000)

#define ppm_to_nounit_any(val) (val / 1000000)

#define FULL_ROTATION (2 * M_PI)

#define degree_to_rad_any(val) (val * M_PI / 180)

#define rad_to_degree_any(val) (val * 180 / M_PI)

#define rad_to_rotation_any(val) (val * 1 / FULL_ROTATION)

#define rotation_to_rad_any(val) (val * FULL_ROTATION / 1)

#define s_to_h_any(val) (val / 3600)

#define h_to_s_any(val) (val * 3600)

#define s_to_ms_any(val) (val * 1000)

#define ms_to_s_any(val) (val / 1000)

#define PhysicalUnitConversions_ms_to_micros_any(val) (val * 1000)

#define micros_to_ms_any(val) (val / 1000)

#define s_to_micros_any(val) (PhysicalUnitConversions_ms_to_micros_any(s_to_ms_any(val)))

#define micros_to_s_any(val) (ms_to_s_any(micros_to_ms_any(val)))

#define micros_to_ns_any(val) (val * 1000)

#define ns_to_micros_any(val) (val / 1000)

#define ms_to_ns_any(val) (micros_to_ns_any(PhysicalUnitConversions_ms_to_micros_any(val)))

#define ns_to_ms_any(val) (micros_to_ms_any(ns_to_micros_any(val)))

#define Hz_to_s_any(val) (1 / val)

#define s_to_Hz_any(val) (1 / val)

#define Hz_to_kHz_any(val) (val / 1000)

#define kHz_to_Hz_any(val) (val * 1000)

#define kHz_to_ms_any(val) (1 / val)

#define ms_to_kHz_any(val) (1 / val)

#define Hz_to_MHz_any(val) (val / 1000000)

#define MHz_to_Hz_any(val) (val * 1000000)

#define kHz_to_MHz_any(val) (val / 1000)

#define PhysicalUnitConversions_MHz_to_kHz_any(val) (val * 1000)

#define MHz_to_micros_any(val) (1 / val)

#define micros_to_MHz_any(val) (1 / val)

#define Hz_to_KiHz_any(val) ((((int32_t)(val)) >> 10) * 1 / 1)

#define KiHz_to_Hz_any(val) ((((int32_t)(val)) << 10) * 1 / 1)

#define Hz_to_MiHz_any(val) ((((int32_t)(val)) >> 20) * 1 / 1)

#define MiHz_to_Hz_any(val) ((((int32_t)(val)) << 20) * 1 / 1)

#define kHz_to_MiHz_any(val) (Hz_to_MiHz_any(kHz_to_Hz_any(val)))

#define MiHz_to_kHz_any(val) (Hz_to_kHz_any(MiHz_to_Hz_any(val)))

#define KiHz_to_MiHz_any(val) ((((int32_t)(val)) >> 10) * 1 / 1)

#define MiHz_to_KiHz_any(val) ((((int32_t)(val)) << 10) * 1 / 1)

#define sampledividedrad_to_sampledividedrotation_any(val) (val * FULL_ROTATION / 1)

#define sampledividedrotation_to_sampledividedrad_any(val) (val * 1 / FULL_ROTATION)

#define A_to_mA_any(val) (val * 1000)

#define mA_to_A_any(val) (val / 1000)

#define Ohm_to_kOhm_any(val) (val / 1000)

#define kOhm_to_Ohm_any(val) (val * 1000)

#define W_to_kW_any(val) (val / 1000)

#define kW_to_W_any(val) (val * 1000)

#define Ws_to_Wh_any(val) (val * 1 / (h_to_s_any(1)))

#define Wh_to_Ws_any(val) (val * h_to_s_any(1) / 1)

#define Wh_to_kWh_any(val) (val / 1000)

#define kWh_to_Wh_any(val) (val * 1000)

#define Ws_to_kWh_any(val) (Wh_to_kWh_any(Ws_to_Wh_any(val)))

#define kWh_to_Ws_any(val) (Wh_to_Ws_any(kWh_to_Wh_any(val)))

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
