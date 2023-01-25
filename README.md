# How to connect esp32 on azure iot hub using mqtt protocol

> This template was tested with IDF 4.4.3 version

## Table of Content
* [ESP TLS](#esp-tls)
* [Configure WiFi](#configure-wifi)
* [Configure Azure Enviroment](#configure-azure-enviroment)
* [Getting SaS Token](#how-generate-sas-token-using-vscode-plugin)


## Esp TLS

this project depends on [cryptoauthlib](https://github.com/espressif/esp-cryptoauthlib) component that already is included inside this repo as part of source code , you can check a newer version on it source or add it.

### Configuring ESP-TLS
run menuconfig `idf.py menuconfig` select component config > ESP-TLS.
- Enable Secure Element

if not have a server certificate or don't want to configure one, enable the followings options
- Enable: Allow potentially insecure options (if you don't want to configure a server certificate)
- Enable: Skip server certificate

go to component config > mbedTLS
- Enable Hardware ECDSA sign acceleration when using ATECC608A
- Enable Hardware ECDSA verify acceleration when using ATECC608A
## Configure WiFi

* WiFi SSID          [main](main/main.c)             
* WiFi Password      [main](main/main.c)             
* WiFi authmode      [wifi](main/wifi.c) (default: WPA2_PSK,line 100)

## Configure Azure Enviroment

Firstly create an [Azure IoT Hub](https://learn.microsoft.com/pt-br/azure/iot-hub/iot-hub-create-through-portal), create at least one device to copy it device id

at main modify following macros

* IOTHUB_NAME: your iot hub name, you can visualize it on URL {iot-hub-name}.azure-devices.net

* IOTHUB_DEVID: device id

* IOTHUB_KEY: SaS Token, incluing all generated


## How generate SaS Token using vscode plugin

open vscode and install azure account and azure iot hub extensions

press Crtl + shift + P and tip: azure: Sign in to azure cloud and log in on azure using your account

after logged, find Azure IoT Hub tab at file explorer, right click on your device and select the option: "Generate SaS Token for Device"
![image](midia/SasToken-using-plugin-1.png)
