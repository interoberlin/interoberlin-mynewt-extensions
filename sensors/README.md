# The WunderBar Sensor Modules Firmware Repository [![Build Status](https://travis-ci.org/relayr/relayr-firmware-nrf51-modules.svg)](https://travis-ci.org/relayr/relayr-firmware-nrf51-modules)

**DISCLAIMER: Please note that this firmware version is for evaluation only. It is not feature complete, or backwards compatible, has not undergone extensive testing and is likely unstable. If you choose to experiment with it please note that support for it is limited at this stage.** 

Welcome to the Sensor Modules Firmware Repository. This repository contains the latest version of the WunderBar sensor modules firmware. Please note that this version of the firmware ***does not*** currently run on the sensor modules, due to a number of hardware limitations. This *will be* the firmware available on the sensor modules once the new hardware version of the WunderBar is released.

**This firmware version is a "work in progress" and currently only includes the basic functionality of retrieving values from the sensor. Please note that this version of the Sensor Modules will not work witht the current firmware on the Master Module**

Despite the fact that it isn't currently available on the sensor modules, you could still use this firmware version. To do that you would need to make sure perquisites are met.

## Prerequisites for using this firmware version

- Solder a Standard 9-pin Samtec FTSH connector for the ARM CoreSight 10 pin SWD/JTAG header. See [this specification document](http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0314h/Chdhbiad.html) for additional details

The connector should be soldered to the board in the following manner:

<img src="docs/images/connector.jpg" width=500px> 


- Connect the connector to a Segger flasher. You could use [this one](https://www.segger.com/jlink-lite-cortexm.html), for instance.


- Obtain a license for the [nRF SDK](https://www.nordicsemi.com/eng/Products/Bluetooth-Smart-Bluetooth-low-energy/nRF51822), soft device version 7. You can obtain a license when buying a development kit.

- Download the [GNU Tools for ARM Embedded Processors](https://launchpad.net/gcc-arm-embedded), unpack it and add its ```/bin``` folder to your ```$PATH```

## Installing the firmware

1. Clone the repository on your local machine.
2. Initialize and update it using the following:

	`git submodule init` 
and 
	`git submodule update`

3. Follow the install instructions for [```nrf51-sdk```](https://github.com/relayr/nrf51-sdk).
4. Navigate to the folder of the sensor you wish to flash (e.g. ```cd wunderbar/temp_rh```).
5. Flash the application by typing `make flash`.
6. [OPTIONAL] You might need to flash the softdevice. Download the S110 softdevice **v7** from Nordic, unpack the S110 hex file into your application folder and type ```make flash-all```.


## License

The MIT License (MIT)

Copyright (c) 2015 relayr (iThings4U GmbH)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
