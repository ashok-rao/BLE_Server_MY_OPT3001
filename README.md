# BLE_MY_OPT3001
mbed application for BLE server with STM32F429 and ST BLE shield and TI OPT 3001 light sensor

This application needs to be flashed into a NUCLEO_F429ZI running mbed firmware. It also needs an X-NUCLEO-IDB05A1 BLE shield running v7.2 or later firmware. The latest firmware
can be found here: https://developer.mbed.org/teams/ST/code/BlueNRG-MS-Stack-Updater/

For the light sensor, I'm using a BOOSTXL-SENSORS breakout board from TI.

You'll also need a couple of jumpers, USB cables etc.

Please find the BLE_Client application under  https://github.com/ashok-rao/BLE_Client_My_OPT3001 repo.

You can verify the output of this application on a serial terminal for your respective OS.

The application will print a few messages and start reading the light sensor and show the readings with a 1sec delay.
