#!/bin/bash

# connect serial terminal to the controller

PORT="/dev/ttyUSB0"
RATE="115200"

minicom --device $PORT --baudrate $RATE 
