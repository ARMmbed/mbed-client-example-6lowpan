#include "lwm2mclient.h"

#include "lwm2m-client/m2minterfacefactory.h"
#include "lwm2m-client/m2mdevice.h"
#include "lwm2m-client/m2mobjectinstance.h"
#include "lwm2m-client/m2mresource.h"

// Enter your mbed Device Server's IPv6 address and Port number in
// mentioned format like FD00:FF1:CE0B:A5E1:1068:AF13:9B61:D557:5683

const String &MBED_SERVER_ADDRESS = "coap://FD00:FF1:CE0B:A5E0:1068:AF13:9B61:D557:5683";

const String &MANUFACTURER = "ARM";
const String &TYPE = "type";
const String &MODEL_NUMBER = "2015";
const String &SERIAL_NUMBER = "12345";
const uint8_t STATIC_VALUE[] = "Static value";


LWM2MClient::LWM2MClient()
:_led(LED3)
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

LWM2MClient::~LWM2MClient() {
    if(_register_security){
        delete _register_security;
    }
    if(_device) {
        M2MDevice::delete_instance();
    }
    if(_object) {
        delete _object;
    }
    if(_interface) {
        delete _interface;
    }
}

bool LWM2MClient::create_interface() {

    M2MInterface::NetworkStack stack = M2MInterface::Nanostack_IPv6;
    if(!_interface) {
        _interface = M2MInterfaceFactory::create_interface(*this,
                                                  "lwm2m-client-6lowpan-endpoint",
                                                  "test",
                                                  60,
                                                  5683,
                                                  "",
                                                  M2MInterface::UDP,
                                                  stack,
                                                  "");
    }
    return (_interface == NULL) ? false : true;
}

bool LWM2MClient::register_successful() {
    return _registered;
}

bool LWM2MClient::unregister_successful() {
    return _unregistered;
}

bool LWM2MClient::registration_update_successful() {
    return _registration_updated;
}

M2MSecurity* LWM2MClient::create_register_object() {
    // Creates bootstrap server object with Bootstrap server address and other parameters
    // required for client to connect to bootstrap server.
    M2MSecurity *security = M2MInterfaceFactory::create_security(M2MSecurity::M2MServer);
    if(security) {
        security->set_resource_value(M2MSecurity::M2MServerUri, MBED_SERVER_ADDRESS);
    }
    return security;
}

M2MDevice* LWM2MClient::create_device_object() {
    // Creates device object which contains mandatory resources linked with
    // device endpoint.
    M2MDevice *device = M2MInterfaceFactory::create_device();
    if(device) {
        device->create_resource(M2MDevice::Manufacturer,MANUFACTURER);
        device->create_resource(M2MDevice::DeviceType,TYPE);
        device->create_resource(M2MDevice::ModelNumber,MODEL_NUMBER);
        device->create_resource(M2MDevice::SerialNumber,SERIAL_NUMBER);
    }
    return device;
}

void LWM2MClient::execute_function(void */*argument*/) {
    _led == 0 ? _led = 1 : _led = 0;
}

M2MObject* LWM2MClient::create_generic_object() {
    _object = M2MInterfaceFactory::create_object("Test");
    if(_object) {
        M2MObjectInstance* inst = _object->create_object_instance();
        if(inst) {
            M2MResource* res = inst->create_dynamic_resource("Dynamic","ResourceTest",true);
            char buffer[20];
            int size = sprintf(buffer,"%d",_value);
            res->set_operation(M2MBase::GET_PUT_POST_ALLOWED);
            res->set_value((const uint8_t*)buffer,
                           (const uint32_t)size);
            res->set_execute_function(execute_callback(this,&LWM2MClient::execute_function));
            _value++;

            inst->create_static_resource("Static",
                                         "ResourceTest",
                                         STATIC_VALUE,
                                         sizeof(STATIC_VALUE)-1);
        }
    }
    return _object;
}

void LWM2MClient::update_resource() {
    if(_object) {
        M2MObjectInstance* inst = _object->object_instance();
        if(inst) {
            M2MResource* res = inst->resource("Dynamic");
            char buffer[20];
            int size = sprintf(buffer,"%d",_value);
            res->set_value((const uint8_t*)buffer,
                           (const uint32_t)size,
                           true);
            _value++;
        }
    }
}

void LWM2MClient::test_register(M2MObjectList object_list){
    if(_interface) {
        _interface->register_object(_register_security, object_list);
    }
}

void LWM2MClient::set_register_object(M2MSecurity *&register_object){
    if(_register_security) {
        delete _register_security;
        _register_security = NULL;
    }
    _register_security = register_object;

}

void LWM2MClient::test_unregister() {
    if(_interface) {
        _interface->unregister_object(NULL);
    }
}

void LWM2MClient::bootstrap_done(M2MSecurity */*server_object*/){
}

void LWM2MClient::object_registered(M2MSecurity */*security_object*/, const M2MServer &/*server_object*/){
    _registered = true;
    _unregistered = false;    
}

void LWM2MClient::object_unregistered(M2MSecurity */*server_object*/){
    _unregistered = true;
    _registered = false;
}

void LWM2MClient::registration_updated(M2MSecurity */*security_object*/, const M2MServer & /*server_object*/){
    _registration_updated = true;
}

void LWM2MClient::error(M2MInterface::Error /*error*/){
    _error = true;
}

void LWM2MClient::value_updated(M2MBase */*base*/, M2MBase::BaseType /*type*/) {
}
