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
#include "mbedclient.h"
#include "mbed-client/m2minterfacefactory.h"
#include "mbed-client/m2mdevice.h"
#include "mbed-client/m2mobjectinstance.h"
#include "mbed-client/m2mresource.h"
#include "minar/minar.h"
#include "core-util/FunctionPointer.h"
#include "mbed-drivers/test_env.h"
#include "security.h"

using namespace mbed::util;


// Enter ARM mbed Device Connector IPv6 address and Port number in
// format coap://<IPv6 address>:PORT. If ARM mbed Device Connector IPv6 address
// is 2607:f0d0:2601:52::20 then the URI is: "coap://2607:f0d0:2601:52::20:5684"
const String &MBED_DEVICE_CONNECTOR_URI = "coap://2607:f0d0:2601:52::20:5684";

const String &MANUFACTURER = "ARM";
const String &TYPE = "type";
const String &MODEL_NUMBER = "2015";
const String &SERIAL_NUMBER = "12345";
const uint8_t STATIC_VALUE[] = "Static value";


MbedClient::MbedClient()
    : _led(LED3)
{
    _interface = NULL;
    _register_security = NULL;
    _device = NULL;
    _object = NULL;
    _update_timer_handle = NULL;
    _registered = false;
    _registering = false;
    _updating = false;
    _value = 0;

    // Create LWM2M device object specifying device resources
    // as per OMA LWM2M specification.
    M2MDevice *device_object = create_device_object();

    M2MObject *object = create_generic_object();

    // Add all the objects that you would like to register
    // into the list and pass the list for register API.
    _object_list.push_back(device_object);
    _object_list.push_back(object);
}

MbedClient::~MbedClient()
{
    if (_register_security) {
        delete _register_security;
    }
    if (_device) {
        M2MDevice::delete_instance();
    }
    if (_object) {
        delete _object;
    }
    if (_interface) {
        delete _interface;
    }
    if (_update_timer_handle) {
        minar::Scheduler::cancelCallback(_update_timer_handle);
    }
}

bool MbedClient::create_interface()
{
    if (_interface) {
        delete _interface;
        _interface = NULL;
    }

    srand(time(NULL));
    uint16_t port = rand() % 65535 + 12345;

    _interface = M2MInterfaceFactory::create_interface(*this,
                 MBED_ENDPOINT_NAME,
                 "test",
                 3600,
                 port,
                 MBED_DOMAIN,
                 M2MInterface::UDP,
                 M2MInterface::Nanostack_IPv6,
                 "");
    return (_interface == NULL) ? false : true;
}

M2MSecurity *MbedClient::create_register_object()
{
    // Creates bootstrap server object with Bootstrap server address and other parameters
    // required for client to connect to bootstrap server.
    M2MSecurity *security = M2MInterfaceFactory::create_security(M2MSecurity::M2MServer);
    if (security) {
        security->set_resource_value(M2MSecurity::M2MServerUri, MBED_DEVICE_CONNECTOR_URI);
        security->set_resource_value(M2MSecurity::BootstrapServer, 0);
        security->set_resource_value(M2MSecurity::SecurityMode, M2MSecurity::Certificate);
        security->set_resource_value(M2MSecurity::ServerPublicKey,SERVER_CERT,sizeof(SERVER_CERT));
        security->set_resource_value(M2MSecurity::PublicKey,CERT,sizeof(CERT));
        security->set_resource_value(M2MSecurity::Secretkey,KEY,sizeof(KEY));
    }
    return security;
}

M2MDevice *MbedClient::create_device_object()
{
    // Creates device object which contains mandatory resources linked with
    // device endpoint.
    M2MDevice *device = M2MInterfaceFactory::create_device();
    if (device) {
        device->create_resource(M2MDevice::Manufacturer, MANUFACTURER);
        device->create_resource(M2MDevice::DeviceType, TYPE);
        device->create_resource(M2MDevice::ModelNumber, MODEL_NUMBER);
        device->create_resource(M2MDevice::SerialNumber, SERIAL_NUMBER);
    }
    return device;
}

void MbedClient::execute_function(void */*argument*/)
{
    _led == 0 ? _led = 1 : _led = 0;
}

M2MObject *MbedClient::create_generic_object()
{
    _object = M2MInterfaceFactory::create_object("Test");
    if (_object) {
        M2MObjectInstance *inst = _object->create_object_instance();
        if (inst) {
            M2MResource *res = inst->create_dynamic_resource("D",
                               "ResourceTest",
                               M2MResourceInstance::INTEGER,
                               true);
            char buffer[20];
            int size = sprintf(buffer, "%d", _value);
            res->set_operation(M2MBase::GET_PUT_ALLOWED);
            res->set_value((const uint8_t *)buffer,
                           (const uint32_t)size);
            res->set_execute_function(execute_callback(this, &MbedClient::execute_function));
            _value++;

            inst->create_static_resource("S",
                                         "ResourceTest",
                                         M2MResourceInstance::STRING,
                                         STATIC_VALUE,
                                         sizeof(STATIC_VALUE) - 1);
        }
    }
    return _object;
}

void MbedClient::update_resource()
{
    if (_object) {
        printf("update_resource() %d\r\n", _value);
        M2MObjectInstance *inst = _object->object_instance();
        if (inst) {
            M2MResource *res = inst->resource("D");
            char buffer[20];
            int size = sprintf(buffer, "%d", _value);
            if(res) {
                res->set_value((const uint8_t *)buffer,
                           (const uint32_t)size);
            }
            _value++;
        }
    }
}

void MbedClient::send_registration()
{
    if (_interface && !_registered && !_registering && !_updating) {
        printf("send_registration()\r\n");
        _registering = true;
        _interface->register_object(_register_security, _object_list);
    }
}

void MbedClient::set_register_object(M2MSecurity *&register_object)
{
    if (_register_security) {
        delete _register_security;
        _register_security = NULL;
    }
    _register_security = register_object;

}

void MbedClient::test_unregister()
{
    if (_interface) {
        _interface->unregister_object(NULL);
    }
}

void MbedClient::bootstrap_done(M2MSecurity */*server_object*/)
{
}

void MbedClient::object_registered(M2MSecurity */*security_object*/, const M2MServer &/*server_object*/)
{
    printf("object_registered()");
    idle();
}

void MbedClient::object_unregistered(M2MSecurity */*server_object*/)
{
    printf("object_unregistered()\r\n");
    _registered = false;
    // This will turn on the LED on the board specifying that
    // the application has run successfully.
    notify_completion(!_registered);
    minar::Scheduler::stop();
}

void MbedClient::registration_updated(M2MSecurity */*security_object*/, const M2MServer & /*server_object*/)
{
    printf("registration_updated()\r\n");
    _updating = false;
}


void MbedClient::update_registration() {
    printf("update_registration()\r\n");
    if (_registered) {
        _interface->update_registration(_register_security, 3600);
        _updating = true;
    }
}

void MbedClient::value_updated(M2MBase */*base*/, M2MBase::BaseType /*type*/)
{
}

void MbedClient::error(M2MInterface::Error error)
{
    printf("error %d\r\n", error);
    switch (error) {
        case M2MInterface::NetworkError:
        case M2MInterface::NotAllowed:
            printf("Reconnecting to server\r\n");
            minar::Scheduler::postCallback(this, &MbedClient::wait);
            break;
        default:
            break;
    }
}

void MbedClient::wait()
{
    _registering = false;
    _registered = false;
    _updating = false;

    if (_update_timer_handle) {
        minar::Scheduler::cancelCallback(_update_timer_handle);
    }

    if (_register_security) {
        delete _register_security; // Delete old one, before creating new one.
        _register_security = NULL;
    }
    // Create LWM2M Client API interface to manage bootstrap,
    // register and unregister
    if (create_interface() == false) {
        printf("Fatal error, can't create interface\r\n");
        return;
    }

    M2MSecurity *register_object = create_register_object();
    set_register_object(register_object);

    // Issue register command.
    printf("waiting 5s before sending registration...\r\n");
    FunctionPointer0<void> ur(this, &MbedClient::send_registration);
    minar::Scheduler::postCallback(ur.bind()).delay(minar::milliseconds(5 * 1000));
}

void MbedClient::idle()
{
    _registered = true;
    _registering = false;
    _updating = false;

    // Update registration in every 30s
    _update_timer_handle = minar::Scheduler::postCallback(this,&MbedClient::update_registration)
            .period(minar::milliseconds(30*1000))
            .getHandle();
}

void MbedClient::mesh_network_handler(mesh_connection_status_t status)
{
    printf("mesh_network_handler() %d\r\n", status);
    if (status == MESH_CONNECTED) {
        wait();
    }
}
