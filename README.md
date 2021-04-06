# FotaHub&reg; Embedded Device SDK for ESP32&reg;
The FotaHub&reg; Device SDK for ESP32&reg; enables developers to create firmware for ESP32&reg;-based IoT devices that can be updated through [FotaHub](http://fotahub.com). It provides an C99-based FotaHub client library along with a set of examples illustrating how the latter can be used and integrated in real applications.

# Installation

To use the FotaHub® Device SDK for ESP32&reg;, the prerequisites described in the following must be available on your laptop or computer. The easiest and quickest way to make that happen is to use Docker. Alternatively, it is possible to install the complete set of prerequisites directly on your machine.

## ESP32 SDK and toolchain

When using Docker, nothing but [Docker](https://docs.docker.com/get-docker) itself needs to be installed to cover the ESP32-specific preqrequisites. 

Otherwise, the following ESP32-related SDKs and tools must be installed: 
* The [ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/v3.2.5/get-started/index.html#get-esp-idf) **v3.2.2**
* The [ESP32 toolchain](https://docs.espressif.com/projects/esp-idf/en/v3.2.5/get-started/index.html#setup-toolchain)
## General purpose tools

In addition to the ESP32-related prerequisites, the following general purpose tools need to be available on your laptop or computer:
* A text editor or IDE of your choice, e.g., [Visual Studio Code](https://code.visualstudio.com)  
* A serial monitor of your choice, e.g., [CoolTerm](https://freeware.the-meiers.org) (a nice quick start guide can be found [here](https://learn.adafruit.com/getting-started-with-binho-nova/quickstart-with-coolterm))
* The device driver for the USB to serial converter chip of your ESP32 board (in case it does not get installed automatically by the operating system). Typically, it is one of the following:
  * CP210x: [CP210x USB to UART Bridge Virtual COM Port (VCP) drivers](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers)
  * FTDI: [FTDI Virtual COM Port Drivers](https://ftdichip.com/drivers/vcp-drivers)

# Getting started
The best way to get started with the FotaHub&reg; Device SDK for ESP32&reg; is to try out and study the included examples:
* [Simple](docs/getting-started/simple.md) - "Hello World" of firmware over-the-air updating with FotaHub&reg;
* [Revertible](docs/getting-started/revertible.md) - Downloads and launches a new firmware version provided at FotaHub&reg;, reverts to the previous version if something goes wrong
# API Reference
View the [C API reference](include/FotaHub.h) for detailed information about the FotaHub&reg; client library (:construction: *under construction*).

# License and third-party software
Unless specifically indicated otherwise in a file included in this project, FotaHub&reg; Device SDK for ESP32&reg; files are provided under the *Apache-2.0* license. See the [LICENSE](LICENSE) file for the full text of this license.

The terms of the license attributed to this project do not alter any rights or obligations you may have under the licenses of any third-party software this project includes. Refer to the [NOTICE](NOTICE.md) file for detailed information about the third-party software which is redistributed through this project.
