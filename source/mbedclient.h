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
#ifndef __MBEDCLIENT_H__
#define __MBEDCLIENT_H__

#include "mbed-client/m2minterfaceobserver.h"
#include "mbed-drivers/DigitalOut.h"
#include "mbed-mesh-api/mesh_interface_types.h"
#include "minar/minar.h"

class M2MDevice;
class M2MSecurity;
class M2MObject;

class MbedClient : public M2MInterfaceObserver
{
public:
    MbedClient();

    ~MbedClient();

    bool create_interface();

    M2MSecurity *create_register_object();

    M2MDevice *create_device_object();

    void execute_function(void *argument);

    M2MObject *create_generic_object();

    void update_resource();

    void send_registration();

    void set_register_object(M2MSecurity *&register_object);

    void test_unregister();

    //Callback from mbed client stack when the bootstrap
    // is successful, it returns the mbed Device Server object
    // which will be used for registering the resources to
    // mbed Device server.
    void bootstrap_done(M2MSecurity *server_object);

    //Callback from mbed client stack when the registration
    // is successful, it returns the mbed Device Server object
    // to which the resources are registered and registered objects.
    void object_registered(M2MSecurity */*security_object*/, const M2MServer &/*server_object*/);

    //Callback from mbed client stack when the unregistration
    // is successful, it returns the mbed Device Server object
    // to which the resources were unregistered.
    void object_unregistered(M2MSecurity */*server_object*/);

    void registration_updated(M2MSecurity */*security_object*/, const M2MServer & /*server_object*/);

    void update_registration(void);

    //Callback from mbed client stack if any error is encountered
    // during any of the LWM2M operations. Error type is passed in
    // the callback.
    void error(M2MInterface::Error error);

    //Callback from mbed client stack if any value has changed
    // during PUT operation. Object and its type is passed in
    // the callback.
    void value_updated(M2MBase *base, M2MBase::BaseType type);

    //Handler for mesh network status events
    void mesh_network_handler(mesh_connection_status_t status);

private:
    void wait();
    void idle();
    mbed::DigitalOut    _led;
    M2MInterface        *_interface;
    M2MSecurity         *_register_security;
    M2MDevice           *_device;
    M2MObject           *_object;
    M2MObjectList       _object_list;
    minar::callback_handle_t   _update_timer_handle;
    bool                _registered;
    bool                _registering;
    bool                _updating;
    int                 _value;
};
#endif //__MBEDCLIENT_H__
