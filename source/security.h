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
 
#define MBED_DOMAIN "6afdbb5e-6775-43f1-8871-3006df615f8e"
#define MBED_ENDPOINT_NAME "b27ec4aa-ec4b-4b7f-a69f-5c5dcda69953"
 
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
"MIIBzjCCAXOgAwIBAgIEcS1ZJzAMBggqhkjOPQQDAgUAMDkxCzAJBgNVBAYTAkZ\r\n"
"JMQwwCgYDVQQKDANBUk0xHDAaBgNVBAMME21iZWQtY29ubmVjdG9yLTIwMTYwHh\r\n"
"cNMTYwMzA3MDc1NzQ5WhcNMTYxMjMxMDYwMDAwWjCBoTFSMFAGA1UEAxNJNmFmZ\r\n"
"GJiNWUtNjc3NS00M2YxLTg4NzEtMzAwNmRmNjE1ZjhlL2IyN2VjNGFhLWVjNGIt\r\n"
"NGI3Zi1hNjlmLTVjNWRjZGE2OTk1MzEMMAoGA1UECxMDQVJNMRIwEAYDVQQKEwl\r\n"
"tYmVkIHVzZXIxDTALBgNVBAcTBE91bHUxDTALBgNVBAgTBE91bHUxCzAJBgNVBA\r\n"
"YTAkZJMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE0VNCkn1VpXZAq01pK0EVR\r\n"
"Oun+bOq1XZNpiNPGLbWxQ7FukcaklVQ4NjgXYu+OrRxEUGVUWjLOI+eBetTd5kb\r\n"
"1DAMBggqhkjOPQQDAgUAA0cAMEQCIAmeNJpzXpC9AYI59iWSC1GMXimHGkh+NbS\r\n"
"oveMjpaerAiALiMTvBVez5J1Zmw0SLHRZcLmdXydiQvcvGM0g9zGefw==\r\n"
"-----END CERTIFICATE-----\r\n";
 
const uint8_t KEY[] = "-----BEGIN PRIVATE KEY-----\r\n"
"MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgdfgopBpyehCdVP1G\r\n"
"2O0mgYeqclcqoYRUZLw9KcEnWjahRANCAATRU0KSfVWldkCrTWkrQRVE66f5s6rV\r\n"
"dk2mI08YttbFDsW6RxqSVVDg2OBdi746tHERQZVRaMs4j54F61N3mRvU\r\n"
"-----END PRIVATE KEY-----\r\n";
 
#endif //__SECURITY_H__
