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

#include "mbed-drivers/mbed.h"
#include "atmel-rf-driver/driverRFPhy.h"    // rf_device_register
#include "mbed-mesh-api/Mesh6LoWPAN_ND.h"
#include "mbed-mesh-api/MeshThread.h"
#include "mbed-mesh-api/MeshInterfaceFactory.h"
#include "mbed-drivers/test_env.h"
#include "mbed-client/m2minterfacefactory.h"
#include "mbed-client/m2mdevice.h"
#include "mbed-client/m2minterfaceobserver.h"
#include "mbed-client/m2minterface.h"
#include "mbed-client/m2mobjectinstance.h"
#include "mbed-client/m2mresource.h"
#include "mbed-mesh-api/AbstractMesh.h"
#include "mbedclient.h"

// Set bootstrap mode to be Thread, otherwise 6LOWPAN_ND is used
//#define APPL_BOOTSTRAP_MODE_THREAD

#define OBS_BUTTON      SW2
#define UNREG_BUTTON    SW3

static MbedClient *mbedclient;
static InterruptIn *obs_button;
static InterruptIn *unreg_button;
static Serial pc(USBTX, USBRX);

void app_start(int, char **)
{
	pc.baud(115200);  //Setting the Baud-Rate for trace output
    printf("Start mbed-client-example-6lowpan\r\n");

    // Instantiate the class which implements
    // LWM2M Client API
    mbedclient = new MbedClient();
    obs_button = new InterruptIn(OBS_BUTTON);
    unreg_button = new InterruptIn(UNREG_BUTTON);

    // This sets up the network interface configuration which will be used
    // by LWM2M Client API to communicate with mbed Device server.
    AbstractMesh *mesh_api;
    int8_t status;
#ifdef APPL_BOOTSTRAP_MODE_THREAD
    mesh_api = MeshInterfaceFactory::createInterface(MESH_TYPE_THREAD);
    uint8_t eui64[8];
    int8_t rf_device_id = rf_device_register();
    // Read mac address after registering the device.
    rf_read_mac_address(&eui64[0]);
    char *pskd = (char *)"Secret password";
    status = ((MeshThread *)mesh_api)->init(rf_device_id, AbstractMesh::mesh_network_handler_t(mbedclient, &MbedClient::mesh_network_handler), eui64, pskd);
#else /* APPL_BOOTSTRAP_MODE_THREAD */
    mesh_api = (Mesh6LoWPAN_ND *)MeshInterfaceFactory::createInterface(MESH_TYPE_6LOWPAN_ND);
    status = ((Mesh6LoWPAN_ND *)mesh_api)->init(rf_device_register(), AbstractMesh::mesh_network_handler_t(mbedclient, &MbedClient::mesh_network_handler));
#endif /* APPL_BOOTSTRAP_MODE */

    if (status != MESH_ERROR_NONE) {
        printf("Mesh network initialization failed %d!\r\n", status);
        return;
    }

    // Set up Hardware interrupt button.
    // On press of SW3 button on K64F board, example application
    // will call unregister API towards mbed Device Server
    unreg_button->fall(mbedclient, &MbedClient::test_unregister);

    // On press of SW2 button on K64F board, example application
    // will send observation towards mbed Device Server
    obs_button->fall(mbedclient, &MbedClient::update_resource);

    status = mesh_api->connect();
    if (status != MESH_ERROR_NONE) {
        printf("Can't connect to mesh network!\r\n");
        return;
    }
}
