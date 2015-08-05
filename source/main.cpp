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
#include "atmel-rf-driver/driverRFPhy.h"    // rf_device_register
#include "mbed-mesh-api/Mesh6LoWPAN_ND.h"
#include "test_env.h"

#include "lwm2m-client/m2minterfacefactory.h"
#include "lwm2m-client/m2mdevice.h"
#include "lwm2m-client/m2minterfaceobserver.h"
#include "lwm2m-client/m2minterface.h"
#include "lwm2m-client/m2mobjectinstance.h"
#include "lwm2m-client/m2mresource.h"
#include "ns_trace.h"

#define OBS_BUTTON SW2
#define UNREG_BUTTON SW3


volatile uint8_t mesh_network_state = MESH_DISCONNECTED;

void mesh_network_callback(mesh_connection_status_t mesh_state)
{
    tr_info("Network established");
    mesh_network_state = mesh_state;
}

void trace_printer(const char* str)
{
  printf("%s\r\n", str);
}

int main() {

    // Instantiate the class which implements
    // LWM2M Client API
    LWM2MClient lwm2mclient;

    // This sets up the network interface configuration which will be used
    // by LWM2M Client API to communicate with mbed Device server.

    Mesh6LoWPAN_ND *mesh_api = Mesh6LoWPAN_ND::getInstance();
    int8_t status;

    status = mesh_api->init(rf_device_register(), mesh_network_callback);
    if (status != MESH_ERROR_NONE) {
        tr_error("Mesh network initialization failed %d!", status);
        return 1;
    }

    status = mesh_api->connect();
    if (status != MESH_ERROR_NONE) {
        tr_error("Can't connect to mesh network!");
        return 1;
    }

    do {
        mesh_api->processEvent();
    } while(mesh_network_state != MESH_CONNECTED);

    // Set up Hardware interrupt button.
    InterruptIn obs_button(OBS_BUTTON);
    InterruptIn unreg_button(UNREG_BUTTON);

    // On press of SW3 button on K64F board, example application
    // will call unregister API towards mbed Device Server
    unreg_button.fall(&lwm2mclient,&LWM2MClient::test_unregister);

    // On press of SW2 button on K64F board, example application
    // will send observation towards mbed Device Server
    obs_button.fall(&lwm2mclient,&LWM2MClient::update_resource);

    // Create LWM2M Client API interface to manage bootstrap,
    // register and unregister

    lwm2mclient.create_interface();
    M2MSecurity *register_object = lwm2mclient.create_register_object();

    register_object->set_resource_value(M2MSecurity::SecurityMode, M2MSecurity::NoSecurity);

    lwm2mclient.set_register_object(register_object);

    // Create LWM2M device object specifying device resources
    // as per OMA LWM2M specification.
    M2MDevice* device_object = lwm2mclient.create_device_object();

    M2MObject* object = lwm2mclient.create_generic_object();

    // Add all the objects that you would like to register
    // into the list and pass the list for register API.
    M2MObjectList object_list;
    object_list.push_back(device_object);
    object_list.push_back(object);

    // Issue register command.
    lwm2mclient.test_register(object_list);

    // Wait till the register callback is called successfully.
    // Callback comes in object_registered()
    /* wait network to be established */
    do {
        mesh_api->processEvent();
    } while(!lwm2mclient.register_successful());

    // Wait for the unregister successful callback,
    // Callback comes in object_unregsitered(), this will be
    // waiting for user to press SW2 button on K64F board.
    /* wait network to be established */
    do {
        mesh_api->processEvent();
    } while(!lwm2mclient.unregister_successful());

    // This will turn on the LED on the board specifying that
    // the application has run successfully.
    notify_completion(lwm2mclient.unregister_successful() &&
                      lwm2mclient.register_successful());


    // Disconnect the connect and teardown the network interface
    mesh_api->disconnect();

    // Delete device object created for registering device
    // resources.
    if(device_object) {
        M2MDevice::delete_instance();
    }
    if(object) {
        delete object;
    }
    return 0;
}
