#!/bin/bash

# update controller firmware via ssh

# device signature
#    ATmega328P      1e950f
#    ATmega328       1e9514



location=$(dirname "$0")

HOST="octopi"
FORM="atmega328"
PORT="/dev/ttyUSB0"
RATE="57600"
NAME="z-endstop"
FILE="$NAME.ino.eightanaloginputs.hex"
PATH_LOC="$location/../code/$NAME/$FILE"
PATH_REM="/home/pi/$FILE"

CMD_COMM="stty -F $PORT $RATE raw"
CMD_DUDE="avrdude -v -c arduino -b $RATE -P $PORT -p $FORM -D -U flash:w:$PATH_REM"

echo "### copy file"
scp $PATH_LOC $HOST:$PATH_REM 

echo "### reset port"
ssh $HOST $CMD_COMM

echo "### update code"
ssh $HOST $CMD_DUDE
