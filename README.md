# avr-robot-firmware
Robot hardware control firmware for AVR microcontrollers

## Features
* Closed-loop velocity control of two DC motors with rotary encoders
* Control of two RC servo motors
* Battery voltage monitoring
* Full control of remaining digital I/O and analog input pins
* Designed for easy interfacing with ROS or other software running on a computer

## Hardware Support / Build Info
avr-robot-firmware is developed for ATMega328-based systems as it is the most widely available 8-bit AVR microcontroller. This means that all ATMega328 Arduino boards are supported as well as other products such as the Pololu A-Star line of robot controllers. avr-robot-firmware is built using the PlatformIO IDE.
