# can-esp32-s3

CAN Bus library for the ESP32-S3

This is a learning project, it is my first time using a ESP32 chip.

# Hardware

Using this DevKit from [Amazon](https://amzn.eu/d/3hGfP8k).

<img src="docs/esp32_s3_pinout.jpg" alt="ESP32-S3 DevKit Pinout" width="700"/>

<img src="docs/esp32_s3_components.jpg" alt="ESP32-S3 DevKit Components on PCB" width="400"/>

# Build-system

## Install ESP-IDF

Follow this [tutorial](https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/get-started/linux-macos-setup.html#get-started-prerequisites).

## Configure

```
idf.py set-target esp32s3
idf.py menuconfig
```

## Build and flash

Don't forget to activate ESP IDF environment.
This is also required for `clangd` LSP to work.

```
. $HOME/esp/esp-idf/export.sh
```

Or if you have set an alias:

```
get_idf
```

```
idf.py build
```

```
idf.py -p PORT flash
```

# Tips

Header files like `freertos/task.h` can be found under `$IDF_PATH/components/`.
For this example at `$IDF_PATH/components/freertos/FreeRTOS-Kernel/include/freertos/task.h`.
