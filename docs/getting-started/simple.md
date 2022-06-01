# Simple FOTA update example

The *Simple* example demonstrates the most straight forward manner to make firmware over-the-air updates with FotaHub. It is kind of a "Hello World" example for FOTA updating and a perfect way to get started in that field.

## Operating principle

The device listens for a firmware update info string to be sent over the serial port. It is expected to consist of the new firmware version the device should be updated to followed by a ':' separator and the checksum or signature of the new firmware version:

`<new-version>:<verification-data>` 

For example:

`1.1:a15d5599d8745ff4f51690eb3996afcf75d0b1729d1b8cda2491b03940417521`

Once a valid update info string has been received and the contained new firmware version is different from the version of the firmware already running on the device, the firmware update procedure is triggered. It involves the device to connect to FotaHub and download the binary of the new firmware version. Thereby, it uses a dedicated URL including the id of the product representing the device in FotaHub, and the name and the version of the firmware binary to be retrieved, e.g.:

`https://bin.fotahub.com/7f52cf5e-7f0d-49dc-87fd-679ebfb94d8d/DemoProduct-1.1`

The downloaded firmware binary is stored in the device's flash memory. At the same time, the downloaded firmware binary's checksum or signature gets recalculated and compared to the checksum or signature included in the previously communicated update info string. If both match, the firmware update gets activated by restarting the device and letting boot it into the flash memory partition where the downloaded firmware binary has been stored. After the restart, the device executes the new firmware version downloaded from FotaHub. 

## Supported targets

The Simple example can be used as is with the [Adafruit Feather HUZZAH32](https://learn.adafruit.com/adafruit-huzzah32-esp32-feather) board. With some adaptations, it can also be used with any other ESP32 development board or hardware product.  

## Installation

A description of the SDKs and tools that must be available on your laptop or computer and advise how to install them can be found [here](../../README.md#installation).

## Usage

### Create a FotaHub product

Create a FotaHub product that represents your board in FotaHub as explained [here](../fotahub/create-product.md). It will be used to upload and provide firmware updates for the same. 

### Get the FotaHub Device SDK for ESP32

If not yet done so, either clone or download and uncompress the [FotaHub Device SDK for ESP32](https://github.com/fotahub/fotahub-device-sdk-esp32) to a location of your choice on your machine. 

> &#x26A0; Make sure that the path to the location of the FotaHub Device SDK for ESP32 on your machine does not contain any spaces.

### Create and run initial firmware version

1. Start the Visual Studio Code and open (`File > Folder...`) the `Simple` example included in the FotaHub Device SDK for ESP32 (`<device-sdk-root>/examples/huzzah32/simple`).

2. Go to the `Configuration.h` file in the `main` folder, and initialize the `WIFI_STATION_SSID` and `WIFI_STATION_PASSPHRASE` constants with the credentials of the Wi-Fi network you want to connect to with your board:

```c
#define WIFI_STATION_SSID "my-fancy-WiFi-SSID"

#define WIFI_STATION_PASSPHRASE "my-cryptic-WiFi-passphrase"
```

3. Open the `DemoProductInfo.h` file in the `main` folder, and initialize the `DEMO_PRODUCT_ID` and `DEMO_PRODUCT_NAME` constants with the id and the name of the previously created FotaHub product (see `Products > {{Your FotaHub product}} > Settings > General` at [Fotahub](https://fotahub.com)). Leave the `DEMO_PRODUCT_FIRMWARE_VERSION` as is for now. In case you have selected anything else than `SHA256` as the binary checksum algorithm for your FotaHub product or opted for using a signature instead, you also must adjust the `DEMO_PRODUCT_FIRMWARE_UPDATE_VERIFICATION_ALGORITHM` constant accordingly:

```c
#define DEMO_PRODUCT_ID "7f52cf5e-7f0d-49dc-87fd-679ebfb94d8d"

#define DEMO_PRODUCT_NAME "Demo Product"

#define DEMO_PRODUCT_FIRMWARE_VERSION "1.0"

#define DEMO_PRODUCT_FIRMWARE_UPDATE_VERIFICATION_ALGORITHM FOTA_UPDATE_VERIFICATION_ALGORITHM_SHA256
```

4. Open the integrated terminal (`Terminal > New Terminal`) and build the example with the help of the `idf.bat` or `idf.sh` script.
    
    When you have opted to use the ESP-IDF SDK and ESP32 toolchains through Docker, the `idf.bat`/`idf.sh` script must be run inside a Docker container providing the same. You can achieve that conveniently by making sure that Docker is running on your machine and invoking the `bind.bat`/`bind.sh` (**B**uild **IN** **D**ocker) script according to your operating system:
   
**Windows:**   
```bat
bind
```
**Linux/Mac OS X:**   
```sh
./bind.sh
```

<p style="margin-left: 2em">If you have installed the ESP-IDF SDK and ESP32 toolchains directly on your machine, you can invoke the <code>idf.bat</code>/<code>idf.sh</code> script right away. Specify the ESP-IDF install or checkout location as first and the <code>build</code> command as second argument:</p> 

**Windows:**   
```bat
idf <ESP-IDF install/checkout location> build
```
**Linux/Mac OS X:**   
```sh
./idf.sh <ESP-IDF install/checkout location> build
```

> &#x1F6C8; When you need to run this command frequently, you may want to make known the ESP-IDF install or checkout location once for all by setting an environment variable named <code>IDF_PATH</code> at user or system level. You then only need to specify the <code>build</code> command as argument when invoking the <code>idf.bat</code>/<code>idf.sh</code> script.

5. Connect your board to your laptop or computer using a USB cable, and identify the corresponding serial port (a guide explaining how to do so can be found [here](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/establish-serial-connection.html)).

> &#x26A0; It may happen that you are unable to find the serial port your board is connected to. Most of the time this is because the device driver for the USB to serial converter chip of your board is missing. You can fix that by manually installing the same as explained [here](../../README.md#general-purpose-tools).

6. Flash the firmware binary that has resulted from building the example to your board.
   
   If you have used Docker and `bind` to build the firmware binary, you will notice that two new scripts, namely `flash.bat`/`flash.sh` and `erase.bat`/`erase.sh` have been created along the way. Use the first one to flash the firmware binary to your board and provide the serial port it is connected to as argument:

**Windows:**   
```bat
flash <COM port name, e.g., COM3>
```
**Linux/Mac OS X:**   
```sh
./flash.sh <serial port name, e.g., /dev/ttyUSB0>
```

<p style="margin-left: 2em">If you have built the firmware binary with the help of the <code>idf.bat</code>/<code>idf.sh</code> script and the ESP-IDF SDK and ESP32 toolchains installed on your machine, use the same <code>idf.bat</code>/<code>idf.sh</code> script also to flash the firmware binary to your board. Specify the <code>flash</code> command as first and the serial port your board is connected to as second argument:</p> 

**Windows:**   
```bat
idf flash <COM port name, e.g., COM3>
```
**Linux/Mac OS X:**   
```sh
./idf.sh flash <serial port name, e.g., /dev/ttyUSB0>
```

7. Start your serial monitor app, point it at the serial port your board is connected to, and set the baudrate to `115200` (`Connection > Options...` in CoolTerm). Open the configured serial port (`Connection > Connect` in CoolTerm), observe how the firmware starts up and verify whether it connects to the Wi-Fi network:

![](simple-1.png "Start of initial firmware version")

<p style="margin-left: 2em">Also have a look at the red LED next to the USB port on your board (GPIO #13). It should blink with a period of approx. 1 s.</p> 

### Create and upload a new firmware version to FotaHub

1. Go back to the `DemoProductInfo.h` file in the `main` folder, and bump the `DEMO_PRODUCT_FIRMWARE_VERSION` constant:

```c
#define DEMO_PRODUCT_FIRMWARE_VERSION "1.1"
```

2. *Optional:* Make another change in your firmware, e.g., increase the blink frequency of the red LED on your board by going back to the `Configuration.h` file in the `main` folder and decreasing the `BLINK_PERIOD` constant:

```c
#define BLINK_PERIOD 250
```

3. Rebuild the example using Docker and `bind` or the `idf.bat`/`idf.sh` script in very much the same way as you did for building the initial firmware version (see step 4 in the previous section). Locate the resulting new firmware binary file named `demoproduct.bin` in the `build` folder (using the `Explorer` view).
   
> &#x1F6C8; You can locate the new firmware binary file also in your file system explorer or copy its path to the clipboard using corresponding Visual Studio Code context menu actions (`Reveal in File Explorer` and `Copy Path`). This can come quite handy in the subsequent steps.

4. Upload the new binary (`build/demoproduct.bin`) as firmware version `1.1` to your FotaHub product as explained [here](../fotahub/upload-firmware.md).

### Make your first firmare over-the-air update 

1. Go back to the serial monitor app showing the execution status of your board running the initial firmware version. Enter the new firmware version followed by a ':' separator, the checksum or signature of the same (see `Products > {{Your FotaHub product}} > Details > {{New firmware version}}` at [Fotahub](https://fotahub.com)), and a terminating line break in the text field or dialog provided for sending data over the opened serial port (`Connection > Send String...` in CoolTerm):

![](simple-2.png "Trigger of FOTA update") 

> &#x1F6C8; In most cases, your serial monitor app can be configured to automatically terminate all strings being sent with a line break so that you can't forget to do so manually upon every submission (`Connection > Options... > Transmit > Send String Options > Terminate 'Send String' Data` in CoolTerm).

2. Submit the firmware update info string to your board (`Send` button in CoolTerm). This will trigger the firmware over-the-air update procedure. Upon successful completion, the board will be restarted with the new firmware version downloaded from FotaHub. To verify that, check the firmware version in the banner showing up in the serial monitor output:

![](simple-3.png "Execution of FOTA update") 

<p style="margin-left: 2em">In case that the new firmware version includes the change wrt to the blink frequency of the red LED on your board as mentioned above, you should also see that the latter is blinking significantly faster now.</p>