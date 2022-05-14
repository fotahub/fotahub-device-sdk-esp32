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
#ifndef AWSCREDENTIALSDEFINITIONS_H
#define AWSCREDENTIALSDEFINITIONS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AWS_DATE_LEN 8

#define AWS_TIME_LEN 8

#define HASH_HEX_LEN 64

#define MAX_HEADERS_COUNT 2

#define AUTHORIZATION_HEADER_SIZE 256

struct HTTPRequestInfo {
  char *method;
  char *path;
  char *host;
  char *queryString;
  char **headers;
  char *body;
};
typedef struct HTTPRequestInfo HTTPRequestInfo_t;

struct ClientInfo {
  char *thingName;
  char *awsRegion;
  char *awsEndpoint;
  char *awsKeyId;
  char *awsSecretKey;
  char awsDate[AWS_DATE_LEN + 1 + 1];
  char awsTime[AWS_TIME_LEN + 1 + 1];
  char *awsService;
  HTTPRequestInfo_t *pHTTPRequestInfo;
};
typedef struct ClientInfo ClientInfo_t;

struct SignatureInfo {
  char signedHeaders[60 + 1];
  char *canonicalRequest;
  char *credentialScope;
  char *stringToSign;
  char signature[HASH_HEX_LEN + 1];
};
typedef struct SignatureInfo SignatureInfo_t;

enum AWSStatus {
  TIMEOUT,
  FAILED,
  SUCCEED
};
typedef enum AWSStatus AWSStatus_t;

struct AWSTemporarySecurityCredentials {
  char *accessKeyId;
  size_t accessKeyIdLength;
  char *secretKey;
  size_t secretKeyLength;
  char *sessionToken;
  size_t sessionTokenLength;
};
typedef struct AWSTemporarySecurityCredentials AWSTemporarySecurityCredentials_t;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
