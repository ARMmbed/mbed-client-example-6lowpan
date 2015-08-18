# Getting started with mesh networking

This document describes how to use mesh networking to communicate with mbed Device Server.

This example application demonstrates how to:

* connect to a **6LoWPAN ND** or **Thread** type of mesh network
* register, unregister, read resource values and send observations to mbed Device Server
* disconnect from the network

By default example application makes **6LoWPAN ND** bootstrap, to change bootstrap mode to **Thread**
you need to define (uncomment) macro `APPL_BOOTSTRAP_MODE_THREAD` in file `source/main.cpp`. 


## Required hardware

* An FRDM-K64F development board (Client end-point)
* An mbed 6LoWPAN shield (with Radio Module)
* An mbed 6LoWPAN Gateway router
* A micro-USB cable
* A micro-USB charger for powering mbed 6LoWPAN Gateway
* Ethernet cable
* A PC running the mbed Device Server (mDS)

![](img/hw_photo.png) 


## Required software

* [Yotta](http://docs.yottabuild.org/#installing) - to build the example programs.
* [mbed Device Server (mDS)](https://silver.arm.com/browse/SEN00) - mbed Device Server, where the example application connects to.

## Optional software
* [Wireshark](https://www.wireshark.org/) - for packet inspection and network debugging.
* [Putty](http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html) - for serial terminal emulation

## Reference Block Diagram
![](img/Dia.png) 

* The communication link between the Gateway router and the mbed Device Server is Ethernet. 
* The Gateway router is powered up using a microUSB charger connected to a wall socket.
* To flash the Gateway router with the Gateway router firmware, you need a microUSB cable. (See instructions below)
* You can also power up your Gateway router with microUSB cable connected to the PC if you do not have a microUSB charger.
* The wireless link between the FRDM-K64F board (Client end-point) and the Gateway router is following IEEE 802.15.4 standard. 
* The FRDM-K64F board (Client end-point) can also be powered up by either a microUSB charger or a microUSB cable. If you are using microUSB, you can view the debug/trace messages utilizing an optional software listed above, for example Putty.


##Test environment setup
#### Server Side

* Connect the mbed 6LoWPAN Gateway Router to a PC running mDS with an ethernet cable. 
* Connect the micro-USB cable to the mbed 6LoWPAN Gateway Router. It will be shown in your computer as a removable memory.
* The firmware for the Gateway is located in the `GW_Binary` folder in the root of this example. You should select the right binary based on the application bootstrap mode.
	* if **6LoWPAN ND** boostrap is used then `gateway6LoWPANStatic.bin` must be used
	* if **Thread** bootstrap is selected then `gatewayThreadStatic.bin` must be used
* Copy the Gateway binary file to the mbed 6LoWPAN Gateway router to flash the device. The device will reboot automatically after flashing. If it does not happen, push the **Reset** button on the board.
* Download and run the mDS server on your PC (see instructions below).

#####Set static configuration to PC

First you need to set your computer to run on IP address `fd00:ff1:ce0b:a5e0::1` and use 64bit
network mask. (fd00:ff1:ce0b:a5e0::1/64).

* Make sure that the PC you are running mDS server on, is not connected to the internet.
* Change your PC IP address to `fd00:ff1:ce0b:a5e0::1/64`

**Setting up IP in Windows:**

	1. Famialiarize yourself with [Microsoft: Changing TCP/IP settings](http://windows.microsoft.com/en-gb/windows/change-tcp-ip-settings)
	2. Go to IPv6 properties.
	3. Select "Use following"
	4. Enter `fd00:ff1:ce0b:a5e0::1`
	5. Enter subnet prefix length: `64`
	6. Click OK.

**Note!**  
You might need a separate driver for Windows OS. [Download](https://developer.mbed.org/handbook/Windows-serial-configuration) the mbed Windows Serial Port driver.

**Setting up IP in Linux:**

```
ifconfig eth0 add fd00:ff1:ce0b:a5e0::1/64
```

#####Dynamic configuration

**Note!**, this section applies only if you plan to run the mDS from a network server

Binaries `gateway6LoWPANDynamic.bin` and `gatewayThreadDynamic.bin` will work when mDS is running on the IPv6 network. You should use the 
binary that corresponds your application bootstrap mode and flash it according to the instructions above.

There is no need to adjust PC settings in this case as the mbed 6LoWPAN Gateway will receive the IPv6 address from the network.


#### Client Side
* Connect the FRDM-K64F development board and mbed 6LoWPAN shield together.
* Configure the `mbed-client-example-6lowpan` application to use the Device Server's (mDS) IPv6 address:
    * On Windows:
         * in the PC running the mDS, open the command prompt and type _ipconfig_
         * section **Ethernet adapter Local Area Connection** field **IPv6 address** contains the IPv6 address
    * On Mac OS X and Linux:
         * in the Mac/Linux machine running the mDS, open the terminal and type _ifconfig_
         * under the appropriate device (usually eth0, en0, or something similar), look for the `inet6` address that looks similar to `FD00:FF1:CE0B:A5E1:1068:AF13:9B61:D557`. That is your IPv6 address.
    * copy the IPv6 address to the string `MBED_SERVER_ADDRESS` at line 11 in the file `/source/lwm2mclient.cpp`
    * the address format is `coap://<IPv6 address>:PORT`. For example, if your server's IP address is `FD00:FF1:CE0B:A5E1:1068:AF13:9B61:D557`,  you would enter `coap://FD00:FF1:CE0B:A5E1:1068:AF13:9B61:D557:5683` where `5683` is the port number. The `FD` prefix tells you that it is a Unique local IPv6 address.
* Build the `mbed-client-example-6lowpan` with `Yotta` (see [Build instructions](#build-instructions)).
* Depending upon your Radio Module (mbed 6LoWPAN Shield), you need to configure an appropriate Radio Channel. Default channels for Gateway Router are as follows:
	* 2.4 GHz ISM band    -  **Channel 12**
	* Sub-GHz ISM band  - **Channel 4**
* Configure the `mbed-client-example-6lowpan` application to use an appropriate Radio Channel based on your hardware:
	* Open the file `yotta_modules/mbed-mesh-api/source/include/static_config.h`
    	* for **6LoWPAN-ND**, change the macro `SCAN_CHANNEL_LIST` to either **4** (1<<4) or **12** (1<<12).
        * for **Thread**, change the macro `THREAD_RF_CHANNEL` to either **4** or **12**.	
	* Use Channel 4 for Sub-GHz module and Channel 12 for 2.4 GHz module.
	* In order to correctly identify what Radio Module you have, please see the section named `Radio Module Identification` below.
* Build the application again using the command
```
yotta build
```
* Load the mbed-client-example-6lowpan application binary to the FRDM-K64F board (see [Running the example application](#running-the-example-application))


**Note!** 
You might need to open UDP port 5683 in your computer firewall for mDS to communicate with this example application. 

##Radio Module Identification
* Please make sure that you are using same Radio modules on both Server and Client sides. 
* If your Radio module on the Gateway router supports 2.4 GHz frequency band, you must use an mbed 6LoWPAN shield on the Client side which contains a 2.4 GHz Radio module (e.g. Atmel RF-233).   
* If your Radio module on the Gateway router supports sub-GHz frequency band, you must use an mbed 6LoWPAN shield on the Client side which contains a sub-GHz Radio module (e.g. Atmel RF-212B)   
* An easy way to identify which frequency band your setup uses, is to check the **Antenna size** on the Radio module.
* Sub-GHz band antenna is larger in size as compared to 2.4 GHz antenna.
* For Client side (mbed 6LoWPAN Shield connected to FRDM-K64F board), see the image below.
  ![](img/Radio_Identifications.png) 
* For Gateway router, see the image below.
  ![](img/Radio_Identifications_GW.png) 

### Downloading mbed Device Server (mDS)

The example application will register to mbed Device Server. Install the mDS on PC.

* The free developer version of the mbed Device Server is used with this example. Download it from [ARM silver](https://silver.arm.com/browse/SEN00).
* Click the **Download Now** button in the right hand side pane.  
* Unzip the package on your PC. You should see the following files:

```
Device Server.tar.gz
Device Server Clients.tar.gz
Device Server Tools.tar.gz
Ref Apps.tar.gz
```

**NOTE!** 
These instructions are valid for the Device Server 2.2 release!

### Starting the mbed Device Server (mDS)

* Extract `Device Server.tar.gz`.
* Go to the `bin` folder of the `Device Server` package that you just extracted.
* Run the start script:
    - If you are running mDS on Linux OS, run the `runDS.sh` in a new shell.
    - If you are running mDS on Windows, run the `runDS.bat` in a new command prompt.
		
This will start the mbed Device Server on your system.

### Starting the WebUI (_Connected home_ reference app)

* Extract `Ref Apps.tar.gz`.
* Go to the `bin` folder in the `Connected-home-trial` reference app that you extracted.
* Run the start script:	
    - If you are running mDS on Linux OS, run the `runConnectedHome.sh` in a new shell.	
    - If you are running mDS on Windows, run the `runConnectedHome.bat` in a new command prompt.	
		
This will start the WebUI on your system.

## Build instructions
		
#### General 
* Install yotta. See instructions [here](http://docs.yottabuild.org/#installing).
* Install the necessary toolchains. Refer to the yotta installation instructions, see [Required software](#required-software), to learn how to install the toolchains. 

**Note:** 
In Linux environment, before using the command:
```
 pip install yotta
```
 use this command 
```
pip install --pre pyusb
``` 

* Set the `IPv6` address of the Device Server in `source/lwm2mclient.cpp` if you have not done it yet. The instructions are in [Client side](#client-side) section of this document. 
* At the command line, move to the root of this example application.
* Set up the target device.

```
yotta target frdm-k64f-gcc
```

* Build the binary

```
yotta build
```

The executable file will be created in the `/build/frdm-k64f-gcc/source/` folder.

### Running the example application

1. Find the binary file named `mbed-client-example-6lowpan.bin` in the folder `mbed-client-example-6lowpan/build/frdm-k64f-gcc/source/`.
2. Copy the binary to the USB mass storage root of the FRDM-K64F development board. It will be automatically flashed to target MCU. After flashing, the board will restart itself.
3. Press the **Reset** button of the development board if it does not restart automatically.
4. The program begins execution and will start registration to the mbed Device Server.
5. After a successful registration, the program will automatically start sending observations every 10 seconds.

### Test Usage

* Make sure that the Device Server and Connected Home App are up and running.
* On the server side where the Device Server is running, open a browser and type:

```
https://localhost:8081
```

This will open the WebUI for Device Server Admin Control.
*  Click on the tab named **End-points**. You should be able to see **lwm2m endpoint** activated there. An example is shown below.

![](img/DS-EndPoint.PNG) 

* Open another tab in the browser and type:

```
//localhost:8082
```

* Use the following username and password:

```
User-name = demo
Password = demo
```

This will open the WebUI for the Connected Home Application. Click on the **End-points** tab.

![](img/CH-endpoint.PNG) 

* Click **lwm2m-client-6lowpan-endpoint**. It will bring you to a panel where you will see panes named **Device** and **Test** as shown below.

![](img/CH-endpoint_Device.PNG)  

* When you click the **Read** button, you will actually retrieve information from the client.

* Open the **Test** pane.
* Click the **Read** buttons. **/Test/0/Static** should return `Static value` string whereas **/Test/0/Dynamic** returns the count of button presses on the client board. 

![](img/CH-endpoint_Test.PNG) 

* Press the indicated **Dynamic Activity** button on the board. 

![](img/board.PNG) 

* You should be able to see an increment in **/Test/0/Dynamic** count.
* Press the indicated **un-register** button in to un-register your device from the mbed Device Server. You will see that the end-point has been removed from the WebUI and the device status is `Stale` in Connected Home app.
* To register your device again, simply press the **Restart** button.

## Debug trace at client side

* To watch the debug trace at the mbed client, you can use either Wireshark or some sort of terminal emulation. Please consider the following example instructions using Putty and Linux.

* Open Putty with root privileges on the client side PC.

```
$ sudo putty 
```

* Give the following command to see which serial port your mbed client controller is connected to.

```
dmesg
```

* Go the **Serial** category in Putty.
* Type your identified port in the section **Serial line to connect to**, for example `/dev/ttyACM0`.
* In **Configure the serial line** section, enter the following details:

				* Baud-rate = 9600
				* Data bits = 8
				* Stop bits = 1
				* flow control = xon/xoff.

* Click on the **Session** category. Save the session with desired name, for example `mbed_trace`.
* Select **Connection_type** as `Serial`.
* Click **Open**.

This will provide you with the debug trace of the client.



