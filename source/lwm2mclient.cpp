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
#include "mbed-client/m2minterfacefactory.h"
#include "mbed-client/m2mdevice.h"
#include "mbed-client/m2mobjectinstance.h"
#include "mbed-client/m2mresource.h"
#include "minar/minar.h"
#include "test_env.h"
#include "security.h"

// Enter your mbed Device Server's IPv6 address and Port number in
// mentioned format like FD00:FF1:CE0B:A5E1:1068:AF13:9B61:D557:5684
// For dynamic setup use something like coap://xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:5684", i.e., The IPv6 address and Port no.

const String &LWM2M_SERVER_ADDRESS = "coap://2607:f0d0:3701:9f::20:5684";

const String &MANUFACTURER = "ARM";
const String &TYPE = "type";
const String &MODEL_NUMBER = "2015";
const String &SERIAL_NUMBER = "12345";
const uint8_t STATIC_VALUE[] = "Static value";


LWM2MClient::LWM2MClient()
    : _led(LED3)
{
    _interface = NULL;
    _register_security = NULL;
    _device = NULL;
    _object = NULL;
    _error = false;
    _registered = false;
    _unregistered = false;
    _registration_updated = false;
    _value = 0;
}

LWM2MClient::~LWM2MClient()
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
}

bool LWM2MClient::create_interface()
{

    M2MInterface::NetworkStack stack = M2MInterface::Nanostack_IPv6;
    if (!_interface) {
        srand(time(NULL));
        uint16_t port = rand() % 65535 + 12345;
        _interface = M2MInterfaceFactory::create_interface(*this,
                     MBED_ENDPOINT_NAME,
                     "test",
                     600,
                     port,
                     MBED_DOMAIN,
                     M2MInterface::UDP,
                     stack,
                     "");
    }
    return (_interface == NULL) ? false : true;
}

bool LWM2MClient::register_successful()
{
    return _registered;
}

bool LWM2MClient::unregister_successful()
{
    return _unregistered;
}

bool LWM2MClient::registration_update_successful()
{
    return _registration_updated;
}

M2MSecurity *LWM2MClient::create_register_object()
{
    // Creates bootstrap server object with Bootstrap server address and other parameters
    // required for client to connect to bootstrap server.
    M2MSecurity *security = M2MInterfaceFactory::create_security(M2MSecurity::M2MServer);
    if (security) {
        security->set_resource_value(M2MSecurity::M2MServerUri, LWM2M_SERVER_ADDRESS);
        security->set_resource_value(M2MSecurity::BootstrapServer, 0);
        security->set_resource_value(M2MSecurity::SecurityMode, M2MSecurity::Certificate);
        security->set_resource_value(M2MSecurity::ServerPublicKey,SERVER_CERT,sizeof(SERVER_CERT));
        security->set_resource_value(M2MSecurity::PublicKey,CERT,sizeof(CERT));
        security->set_resource_value(M2MSecurity::Secretkey,KEY,sizeof(KEY));
    }
    return security;
}

M2MDevice *LWM2MClient::create_device_object()
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

void LWM2MClient::execute_function(void */*argument*/)
{
    _led == 0 ? _led = 1 : _led = 0;
}

M2MObject *LWM2MClient::create_generic_object()
{
    _object = M2MInterfaceFactory::create_object("Test");
    if (_object) {
        M2MObjectInstance *inst = _object->create_object_instance();
        if (inst) {
            M2MResource *res = inst->create_dynamic_resource("Dynamic",
                               "ResourceTest",
                               M2MResourceInstance::INTEGER,
                               true);
            char buffer[20];
            int size = sprintf(buffer, "%d", _value);
            res->set_operation(M2MBase::GET_PUT_POST_ALLOWED);
            res->set_value((const uint8_t *)buffer,
                           (const uint32_t)size);
            res->set_execute_function(execute_callback(this, &LWM2MClient::execute_function));
            _value++;

            inst->create_static_resource("Static",
                                         "ResourceTest",
                                         M2MResourceInstance::STRING,
                                         STATIC_VALUE,
                                         sizeof(STATIC_VALUE) - 1);
        }
    }
    return _object;
}

void LWM2MClient::update_resource()
{
    if (_object) {
        M2MObjectInstance *inst = _object->object_instance();
        if (inst) {
            M2MResource *res = inst->resource("Dynamic");
            char buffer[20];
            int size = sprintf(buffer, "%d", _value);
            res->set_value((const uint8_t *)buffer,
                           (const uint32_t)size);
            _value++;
        }
    }
}

void LWM2MClient::send_registration()
{
    if (_interface) {
        _interface->register_object(_register_security, _object_list);
    }
}

void LWM2MClient::set_register_object(M2MSecurity *&register_object)
{
    if (_register_security) {
        delete _register_security;
        _register_security = NULL;
    }
    _register_security = register_object;

}

void LWM2MClient::test_unregister()
{
    if (_interface) {
        _interface->unregister_object(NULL);
    }
}

void LWM2MClient::bootstrap_done(M2MSecurity */*server_object*/)
{
}

void LWM2MClient::object_registered(M2MSecurity */*security_object*/, const M2MServer &/*server_object*/)
{
    _registered = true;
    _unregistered = false;
}

void LWM2MClient::object_unregistered(M2MSecurity */*server_object*/)
{
    _unregistered = true;
    _registered = false;
    // This will turn on the LED on the board specifying that
    // the application has run successfully.
    notify_completion(_unregistered);
    minar::Scheduler::stop();
}

void LWM2MClient::registration_updated(M2MSecurity */*security_object*/, const M2MServer & /*server_object*/)
{
    _registration_updated = true;
}

void LWM2MClient::error(M2MInterface::Error error)
{
    _error = true;
    switch (error) {
        case M2MInterface::NetworkError:
        case M2MInterface::Timeout:
            if (!_registered) {
                FunctionPointer0<void> ur(this, &LWM2MClient::send_registration);
                minar::Scheduler::postCallback(ur.bind()).delay(minar::milliseconds(10*1000));
            }
            break;
        default:
            break;
    }
}

void LWM2MClient::value_updated(M2MBase */*base*/, M2MBase::BaseType /*type*/)
{
}

void LWM2MClient::mesh_network_handler(mesh_connection_status_t status)
{
    if (status == MESH_CONNECTED) {
        // Create LWM2M Client API interface to manage bootstrap,
        // register and unregister
        this->create_interface();

        M2MSecurity *register_object = create_register_object();

        set_register_object(register_object);

        // Create LWM2M device object specifying device resources
        // as per OMA LWM2M specification.
        M2MDevice *device_object = create_device_object();

        M2MObject *object = create_generic_object();

        // Add all the objects that you would like to register
        // into the list and pass the list for register API.
        _object_list.push_back(device_object);
        _object_list.push_back(object);

        // Issue register command.
        FunctionPointer0<void> ur(this, &LWM2MClient::send_registration);
        minar::Scheduler::postCallback(ur.bind()).delay(minar::milliseconds(1000));
    }
}
