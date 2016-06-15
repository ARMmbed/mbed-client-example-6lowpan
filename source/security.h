/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __SECURITY_H__
#define __SECURITY_H__
 
#include <inttypes.h>
 
#define MBED_DOMAIN "decd06cc-2a32-4e5e-80d0-7a7c65b90e6e"
#define MBED_ENDPOINT_NAME "cc69e7c5-c24f-43cf-8365-8d23bb01c707"
 
const uint8_t SERVER_CERT[] = "-----BEGIN CERTIFICATE-----\r\n"
"MIIBmDCCAT6gAwIBAgIEVUCA0jAKBggqhkjOPQQDAjBLMQswCQYDVQQGEwJGSTEN\r\n"
"MAsGA1UEBwwET3VsdTEMMAoGA1UECgwDQVJNMQwwCgYDVQQLDANJb1QxETAPBgNV\r\n"
"BAMMCEFSTSBtYmVkMB4XDTE1MDQyOTA2NTc0OFoXDTE4MDQyOTA2NTc0OFowSzEL\r\n"
"MAkGA1UEBhMCRkkxDTALBgNVBAcMBE91bHUxDDAKBgNVBAoMA0FSTTEMMAoGA1UE\r\n"
"CwwDSW9UMREwDwYDVQQDDAhBUk0gbWJlZDBZMBMGByqGSM49AgEGCCqGSM49AwEH\r\n"
"A0IABLuAyLSk0mA3awgFR5mw2RHth47tRUO44q/RdzFZnLsAsd18Esxd5LCpcT9w\r\n"
"0tvNfBv4xJxGw0wcYrPDDb8/rjujEDAOMAwGA1UdEwQFMAMBAf8wCgYIKoZIzj0E\r\n"
"AwIDSAAwRQIhAPAonEAkwixlJiyYRQQWpXtkMZax+VlEiS201BG0PpAzAiBh2RsD\r\n"
"NxLKWwf4O7D6JasGBYf9+ZLwl0iaRjTjytO+Kw==\r\n"
"-----END CERTIFICATE-----\r\n";
 
const uint8_t CERT[] = "-----BEGIN CERTIFICATE-----\r\n"
"MIIBzzCCAXOgAwIBAgIEVvwvcjAMBggqhkjOPQQDAgUAMDkxCzAJBgNVBAYTAkZ\r\n"
"JMQwwCgYDVQQKDANBUk0xHDAaBgNVBAMME21iZWQtY29ubmVjdG9yLTIwMTYwHh\r\n"
"cNMTYwMjE3MTYyNDE3WhcNMTYxMjMxMDYwMDAwWjCBoTFSMFAGA1UEAxNJZGVjZ\r\n"
"DA2Y2MtMmEzMi00ZTVlLTgwZDAtN2E3YzY1YjkwZTZlL2NjNjllN2M1LWMyNGYt\r\n"
"NDNjZi04MzY1LThkMjNiYjAxYzcwNzEMMAoGA1UECxMDQVJNMRIwEAYDVQQKEwl\r\n"
"tYmVkIHVzZXIxDTALBgNVBAcTBE91bHUxDTALBgNVBAgTBE91bHUxCzAJBgNVBA\r\n"
"YTAkZJMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEkLLpLh0q/nKjxyS4Up2md\r\n"
"riE2yW8InSNyJwe/Z4oh0BdaVxYlmdnQBCp0gLnchTWTTy6MB27hef8rMEvGmOK\r\n"
"lTAMBggqhkjOPQQDAgUAA0gAMEUCICAEkEMRydAYgW8t6Lo4qVobF047d97VmQ4\r\n"
"OtgsV9nlHAiEA/to6FFugWcIUZXL6mhn4Pm09ZYDeLXv7wCgXax2y8a8=\r\n"
"-----END CERTIFICATE-----\r\n";
 
const uint8_t KEY[] = "-----BEGIN PRIVATE KEY-----\r\n"
"MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgQ2Sur28lymqjNtkw\r\n"
"49CXNmMXfglbzBMQFQdqAoGpOX+hRANCAASQsukuHSr+cqPHJLhSnaZ2uITbJbwi\r\n"
"dI3InB79niiHQF1pXFiWZ2dAEKnSAudyFNZNPLowHbuF5/yswS8aY4qV\r\n"
"-----END PRIVATE KEY-----\r\n";
 
#endif //__SECURITY_H__