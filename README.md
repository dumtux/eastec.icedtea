# IcedTEA - Turntable motor control PCB design

## Overview

Design and prototype a PCB that will accurately control the rotation speed a turntable via pulley. The
board will be connected to a motor with self-encoding and will monitor and adjust the speed of the
motor to ensure target speed is maintained.

## Features

* DC power management
* Onboard microchip for firmware
* Target speed switch (33.33rpm or 45 rpm)
* Display of current & target speed (via OLED or 8 segment display)
* Fine adjustment of target speed +/- 10% via pot
* USB C for power & firmware upgrade
* Motor control and sensing
* Auto adjustment to ensure target speed is achieved
* Motor overload cutout and warning

## References

* [MSP430G2x53 datasheet](https://www.ti.com/lit/ds/symlink/msp430g2553.pdf)
* [OLED Graphic Display Interfacing with MSP -EXP430G2 TI Launchpad](https://www.electronicwings.com/ti-launchpad/oled-graphic-display-interfacing-with-msp-exp430g2-ti-launchpad)
