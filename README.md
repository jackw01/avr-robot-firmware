# robot-bridge-firmware
AVR robot control firmware for interfacing between a Raspberry Pi or other computer and timing-sensitive hardware functions

## What does it do?
Raspberry Pi and other single-board computers are nice for robotics control applications, but they are not built for performing real-time functions such as motor control, reading rotary encoders, high-speed control loops, or interfacing with devices like WS2812B LEDs that use highly timing-sensitive protocols. AVR microcontrollers (Arduino) are very good at these things, but not so much at doing any serious computation.

This firmware allows a microcontroller to bridge the gap between high-level control code running on a computer and low-level hardware control functions requiring precise timing such as closed loop motor control.

## Build and Deploy
robot-bridge-firmware is built using the PlatformIO IDE. All ATMega328 boards are supported (32kb of program memory is required).
