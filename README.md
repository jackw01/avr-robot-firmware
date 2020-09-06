# avr-robot-firmware
Robot control firmware for AVR microcontrollers

A simple way to control the hardware of a differential drive robot using cheap and widely available AVR microcontrollers. Originally created for experimenting with robot navigation, visual-inertial odometry, and SLAM on [testbot86](https://github.com/jackw01/testbot86).

## Features
* Designed for easy interfacing with control software running on a computer
* Closed-loop velocity control of two DC motors with rotary encoders
* Control of two RC servo motors
* Battery voltage monitoring
* Full control of remaining digital I/O and analog input pins

<small>May become compatible with ROS2 when ROS2 becomes more mature.</small>

## Hardware Support / Build Info
`avr-robot-firmware` is developed for the ATMega328 as it is the most widely available 8-bit AVR microcontroller with a huge range of development boards to choose from. All ATMega328 Arduino boards are supported as well as other products such as the Pololu A-Star line of robot controllers. `avr-robot-firmware` is built using the PlatformIO IDE.
