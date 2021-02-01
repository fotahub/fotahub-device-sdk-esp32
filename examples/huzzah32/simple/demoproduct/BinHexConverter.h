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
#ifndef BINHEXCONVERTER_H
#define BINHEXCONVERTER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t convertHexDigitToBinValue(char inputChr);

size_t convertBinValueToHexString8(uint8_t inputValue, char *outputStr, size_t outputStrSize, bool upperCase);

uint8_t convertHexString8ToBinValue(char *inputStr, size_t inputStrLength);

size_t convertBinDataToHexString8(uint8_t *inputData, size_t inputDataSize, char *outputStr, size_t outputStrSize, bool upperCase);

size_t convertHexString8ToBinData(char *inputStr, size_t inputStrLength, uint8_t *pOutputData, size_t outputDataSize);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
