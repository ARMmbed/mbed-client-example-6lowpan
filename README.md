# Getting started with mesh networking

This document describes how to use mesh networking to communicate with mbed Device Server.

This example application demonstrates how to:

* connect to a 6LoWPAN ND type of mesh network
* register, unregister, read resource values and send observations to mbed Device Server
* disconnect from the network

## Required hardware

* A frdm-k64f development board
* A mbed 6LoWPAN shield
* A mbed 6LoWPAN Gateway
* A micro-USB cable
* A micro-USB charger for powering mbed 6LoWPAN Gateway
* A PC running mbed Device Server

## Required software

* [Yotta](http://docs.yottabuild.org/#installing) - to build the example programs.
* [mbed Device Server (mDS)](#download-mbed-device-server-mds) - mbed Device Server, where example application connects.

## Optional software
* [Wireshark](https://www.wireshark.org/) - for packet inspection/network debugging.

## Test environment setup

* Connect frdm-k64f development board and mbed 6LoWPAN shield together
* Connect mbed 6LoWPAN Gateway to PC running mDS vith ethernet cable
* Download and run mDS server on PC (see instructions below)
* Connect micro-USB charger to mbed 6LoWPAN Gateway
* Configure the lwm2m-client-6lowpan-example application to use server IPv6 address
    * in PC running mDS open command prompt and type _ipconfig_
    * section `Ethernet adapter Local Area Connection` field `Link-local IPv6 address` contains IPv6 address
    * copy IPv6 address to define `MBED_SERVER_ADDRESS` in file `./source/m2mlwclient.cpp`
* Build the lwm2m-client-6lowpan-example application with Yotta (see `Build instructions` below)
* Load the lwm2m-client-6lowpan-example application to the FRDM-K64F board

**Note!** You might need to open UDP port 5683 in your computer firewall for mDS to communicate with this example application. 

### Downloading mbed Device Server (mDS)

The example application will register to mbed Device Server. Install mDS on PC.

1. The free developer version of the mbed Device Server is used with this example. Download it from [ARM silver](https://silver.arm.com/browse/SEN00).
2. Unzip the package on PC. You should see the following files:
NOTE: These instructions have been made for Device Server 2.4 release!
```
Device Server.tar.gz
Device Server Clients.tar.gz
Device Server Tools.tar.gz
Ref Apps.tar.gz
```

### Starting the mbed Device Server (mDS)

1. Go to the `bin` folder of the Device Server package that you downloaded.
2. Run the start script:
    - If you are running mDS on Linux OS, run the `runDS.sh` in a new shell.
    - If you are running mDS on Windows, run the `runDS.bat` in a new command prompt.
		
This will start the mbed Device Server on your system.

### Starting the WebUI ("Connected home" reference app)

1. Go to the `bin` folder in the Connected-home-trial reference app that you downloaded.
2. Run the start script:	
    - If you are running mDS on Linux OS, run the `runConnectedHome.sh` in a new shell.	
    - If you are running mDS on Windows, run the `runConnectedHome.bat` in a new command prompt.	
		
This will start the WebUI on your system.

## Build instructions
		
#### General 
1. Install yotta. See instructions [here](http://docs.yottabuild.org/#installing).
2. Install the necessary toolchains. Refer to the yotta installation instructions (in step 1) to learn how to install the toolchains.
3. In the command prompt, move to the root of this example application.
4. Open file `source/lwm2mclient.cpp` and edit your mbed Device Server's Ipv6 address in place of `MBED_SERVER_ADDRESS`. Address format is `coap://<IPv6 address>:PORT`. For example, if your server's IP address is `FD00:FF1:CE0B:A5E1:1068:AF13:9B61:D557`, you would enter `coap://FD00:FF1:CE0B:A5E1:1068:AF13:9B61:D557:5683`.
5. Select connection mode from below
6. Set up the target device, `yotta target frdm-k64f`.
7. In the command prompt type `yotta build`. The executable file will be created to `/build/frdm-k64f-gcc/source/` folder.

### Running the example application

1. Find the binary file named `lwm2m-client-6lowpan-example.bin` in the folder `lwm2m-client-6lowpan-example/build/frdm-k64f-gcc/source/`.
2. Copy he binary to the USB mass storage root of the frdm-k64f development board. It will be automatically flashed to target MCU.
3. Press the reset button of frdm-development board
4. The program begins execution and will start registration to the mbed Device Server giving console output as `Registering endpoint` and when the registration is successful it will display `Registered`.
5. After a successful registration, the program will automatically start sending observations after every 10 seconds and it will be visible on console as `Sending observation` along with the value which increments everytime the obeservation is sent like `Value sent 1` and so on. These values can be observed on mbed Connected Home under **/Test/0/D** resource.

### Flashing router application

The router firmware is located in the root folder of the coap-example application called `mbed_6LP_Gateway.bin`.

Connect the mbed gateway to your computer with a micro USB cable. It will shown in your computer as a removable memory. Copy the `mbed_6LP_Gateway.bin` file to the mbed gateway to flash the device. To reboot the new firmware, click **Reset** after flashing.

