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
#include "BlinkDrv.h"

static uint64_t toggleCounterValue;

void blink_init(uint16_t blinkPeriod)
{
  timer_config_t const timerConfig = 
  {
    .alarm_en = TIMER_ALARM_DIS, 
    .counter_en = TIMER_PAUSE, 
    .intr_type = TIMER_INTR_LEVEL, 
    .counter_dir = TIMER_COUNT_UP, 
    .auto_reload = TIMER_AUTORELOAD_DIS, 
    .divider = 80 / 1
  };
  
  timer_init(BLINK_TIMER_GROUP, BLINK_TIMER_INDEX, ((timer_config_t *)(&timerConfig)));
  timer_set_counter_value(BLINK_TIMER_GROUP, BLINK_TIMER_INDEX, 0);
  timer_start(BLINK_TIMER_GROUP, BLINK_TIMER_INDEX);
  
  esp_rom_gpio_pad_select_gpio(((uint8_t)(BLINK_PIN_NUMBER)));
  gpio_set_direction(BLINK_PIN_NUMBER, GPIO_MODE_INPUT_OUTPUT);
  gpio_pulldown_dis(BLINK_PIN_NUMBER);
  gpio_pullup_dis(BLINK_PIN_NUMBER);
  
  toggleCounterValue = ((uint64_t)(ms_to_micros_any(((int64_t) blinkPeriod)))) >> 1;
}

void blink_run(void)
{
  uint64_t counterValue = 0;
  timer_get_counter_value(BLINK_TIMER_GROUP, BLINK_TIMER_INDEX, &(counterValue));
  
  if (counterValue >= toggleCounterValue) 
  {
    gpio_set_level(BLINK_PIN_NUMBER, (gpio_get_level(BLINK_PIN_NUMBER) != 0) ? (0) : (1));
    timer_pause(BLINK_TIMER_GROUP, BLINK_TIMER_INDEX);
    timer_set_counter_value(BLINK_TIMER_GROUP, BLINK_TIMER_INDEX, 0);
    timer_start(BLINK_TIMER_GROUP, BLINK_TIMER_INDEX);
  }
}
