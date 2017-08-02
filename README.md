# Toucher

<img src="pict/2017-07-31_18-29-41.jpg?raw=true" align="right" alt="board" width="40%">

### Purpose

Provides bed leveling FSR sensor controller for square heat beds.

### Features

- 4 sensor points
- 85C maximum bed temperature
- serial/usb terminal command interface
- flexible controller configuration and logging
- connects in place of mechanical endstop switch on Z

### Materials

Uses [Arduino Nano](https://github.com/random-builder/toucher/blob/master/docs/nano-pinout.pdf)
with [FSR-402](https://github.com/random-builder/toucher/blob/master/docs/FSR-402.pdf) sensors.
See [schema.pdf](https://github.com/random-builder/fsr-endstop/blob/master/schema/schema.pdf).

### Algorithm

Tracks fast and slow exponential averages for all channels 
to detect summary averages cross-over events.

### Reference Glass Mount

Provides "floating suspension",
when horizontal mount forces are maximal, 
vertical mount forces are minimal,
and there is a small downward force inclination.

Model: [Level: Wanhao D6 Glass Sheet Mount](https://www.thingiverse.com/thing:2461546)

### Reference Marlin Build

Works on Wanhao D6 / Monoprice Clone printer.

Github branch: [Random Builder / wanhao-d6-probe](https://github.com/random-builder/Marlin/tree/wanhao-d6-probe)

### Similar Projects

Github project: [John Socha-Leialoha FSR Endstop](https://github.com/JohnSL/FSR_Endstop)

### Setup Instructions

See [wiki page](https://github.com/random-builder/fsr-endstop/wiki).
