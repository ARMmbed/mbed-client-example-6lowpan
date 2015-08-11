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

#include "lwm2mclient.h"
#include "minar/minar.h"
#include "atmel-rf-driver/driverRFPhy.h"    // rf_device_register
#include "mbed-mesh-api/Mesh6LoWPAN_ND.h"
#include "test_env.h"

#include "mbed-client/m2minterfacefactory.h"
#include "mbed-client/m2mdevice.h"
#include "mbed-client/m2minterfaceobserver.h"
#include "mbed-client/m2minterface.h"
#include "mbed-client/m2mobjectinstance.h"
#include "mbed-client/m2mresource.h"
#include "ns_trace.h"
#include "mbed-mesh-api/AbstractMesh.h"

#define OBS_BUTTON SW2
#define UNREG_BUTTON SW3

static LWM2MClient *lwm2mclient = NULL;
static InterruptIn *obs_button;
static InterruptIn *unreg_button;

void trace_printer(const char* str)
{
  printf("%s\r\n", str);
}

void app_start(int, char**) {

    // Instantiate the class which implements
    // LWM2M Client API
    lwm2mclient = new LWM2MClient();
    obs_button = new InterruptIn(OBS_BUTTON);
    unreg_button = new InterruptIn(UNREG_BUTTON);

    // This sets up the network interface configuration which will be used
    // by LWM2M Client API to communicate with mbed Device server.

    Mesh6LoWPAN_ND *mesh_api = Mesh6LoWPAN_ND::getInstance();
    int8_t status;

    status = mesh_api->init(rf_device_register(), AbstractMesh::MeshNetworkHandler_t(lwm2mclient,&LWM2MClient::mesh_network_handler));
    if (status != MESH_ERROR_NONE) {
        tr_error("Mesh network initialization failed %d!", status);
        return;
    }

    // Set up Hardware interrupt button.
  //  InterruptIn obs_button(OBS_BUTTON);
   // InterruptIn unreg_button(UNREG_BUTTON);

    // On press of SW3 button on K64F board, example application
    // will call unregister API towards mbed Device Server
    unreg_button->fall(lwm2mclient,&LWM2MClient::test_unregister);

    // On press of SW2 button on K64F board, example application
    // will send observation towards mbed Device Server
    obs_button->fall(lwm2mclient,&LWM2MClient::update_resource);

    status = mesh_api->connect();
    if (status != MESH_ERROR_NONE) {
        tr_error("Can't connect to mesh network!");
        return;
    }
}
