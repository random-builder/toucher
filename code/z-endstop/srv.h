
// servo control

#ifndef SRV_H
#define SRV_H

#include "rom.h"
#include <Servo.h>

// servo output pin
#define SRV_CLR    11 // deploy cleaning plate

// controller instance
Servo servo_cleaner;

boolean srv_is_angle (int deg) {
  return 0 <= deg && deg <= 180;
}

// rotate clender servo to NN degrees
void srv_cleaner(int deg) {
  servo_cleaner.write(deg);
}

// initial servo state
void srv_setup() {
  servo_cleaner.attach(SRV_CLR);
  int deg;
  rom_read(deg, srv_ini_clr); srv_cleaner(deg);
}

#endif // SRV_H

