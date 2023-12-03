# 7300  (under development)
## Arduino library for controlling TMC7300 DC motor driver ICs

https://github.com/joshua-8/TMC7300

## [Doxygen reference](https://joshua-8.github.io/TMC7300/)

My goal is to make a good library for controlling the TMC7300 chips, and to keep things simple, unlike other TMC motor driver libraries (see Acknowledgements) I do not have a plan to support other chips in a more general way using this library.

 [Datasheet for TMC7300 motor driver IC](https://www.analog.com/media/en/technical-documentation/data-sheets/TMC7300_datasheet_rev1.08.pdf)

## Tested with
* Arduino Uno
* ESP32 development board
* ESP32-S3 QT Py

# Acknowledgements
* This library is based on @bread-wolf's work in the [TMCSerial](https://github.com/bread-wolf/TMCSerial) and[TMCField](https://github.com/bread-wolf/TMCField) libraries, both of which are generously licensed under the AGPL3.0 license.
* This library is also based on work done by @trinamic and Analog Devices Inc. in the [TMC-API](https://github.com/trinamic/TMC-API/tree/master), which is generously licensed under the MIT license.

Find the comments in the code of this library to see which parts are based on which of the above libraries.
