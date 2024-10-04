# 7300  (under development)
## Arduino library for controlling TMC7300 DC motor driver ICs

[![arduino-library-badge](https://www.ardu-badge.com/badge/TMC7300.svg?)](https://www.ardu-badge.com/TMC7300)
[![PlatformIO Registry](https://badges.registry.platformio.org/packages/joshua1024/library/TMC7300.svg)](https://registry.platformio.org/libraries/joshua1024/TMC7300)
[![Arduino Lint](https://github.com/joshua-8/TMC7300/actions/workflows/arduino-lint.yml/badge.svg)](https://github.com/joshua-8/TMC7300/actions/workflows/arduino-lint.yml)

https://github.com/joshua-8/TMC7300

Doxygen reference: https://joshua-8.github.io/TMC7300/class_t_m_c7300_i_c.html

My goal is to make a good library for controlling the TMC7300 chips. To keep things simple, unlike other TMC motor driver libraries (see Acknowledgements), I do not have a plan to support other chips in a more general way using this library.

[Datasheet for TMC7300 motor driver IC](https://www.analog.com/media/en/technical-documentation/data-sheets/TMC7300_datasheet_rev1.08.pdf)

Make sure that any Serial prints have time to finish before starting a read or write. This library needs to be able to run uninterrupted in order for the timing of the UART communication with the TMC7300 to work correctly.

## Tested with
* ESP32-S3 QT Py

# Acknowledgements
* This library is based on [@bread-wolf](https://github.com/bread-wolf)'s work in the [TMCSerial](https://github.com/bread-wolf/TMCSerial) and [TMCField](https://github.com/bread-wolf/TMCField) libraries, both of which are generously licensed under the AGPL3.0 license.
* This library is based on work done by [@trinamic](https://github.com/trinamic) and [Analog Devices Inc.](https://www.analog.com) for the [TMC-API](https://github.com/trinamic/TMC-API/tree/master), which is generously licensed under the MIT license.

Find the comments in the source code of this library to see which parts are based on which of the above libraries.
