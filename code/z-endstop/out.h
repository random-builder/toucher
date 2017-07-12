
// output signal

#ifndef OUT_H
#define OUT_H

#include "avg.h"

// system output pins
#define SYS_LED      13 // system led
#define FSR_OUT      12 // endstop signal

// output params
long out_time_on = 0; // millis of out switch on
long out_time_off = 0; // millis of out switch off
boolean out_state = false; // output signal state
boolean out_invert = true; // invert output signal // rom config
boolean out_log_event = false; // enable on-deman logging // rom config

// publish output state
void out_write() {
  digitalWrite(SYS_LED, out_state); // blink on touch
  digitalWrite(FSR_OUT, out_invert ? !out_state : out_state); // set output signal
}

// configure pins
void out_setup() {
  out_write(); // set default
  pinMode(SYS_LED, OUTPUT);
  pinMode(FSR_OUT, OUTPUT);
}

// report event begining
void out_log_begin() {
  if (!out_log_event) return;
  char text[32];
  sprintf_P(text, PSTR("B@%-8lu | "), millis()); Serial.print(text);
  for (byte ch = 0; ch < AVG_CH_NUM; ch++) {
    avg_render_detail(ch, text); Serial.print(text); Serial.print(F("| "));
  }
  Serial.println();
}

// report event ending
void out_log_endin() {
  if (!out_log_event) return;
  char text[32];
  sprintf_P(text, PSTR("E@%-8lu | "), millis()); Serial.print(text);
  sprintf_P(text, PSTR("P=%-4lu | "), out_time_off - out_time_on); Serial.print(text);
  Serial.println();
}

// trigger levels
int out_trig_hi = 14; // upper threshold // rom config
int out_trig_lo = 7;  // lower threshold // rom config

// update output signal
void out_signal() {
  const int trigger = avg_calc_trigger();
  const boolean trig_hi = (trigger >= out_trig_hi);
  const boolean trig_lo = (trigger <= out_trig_lo);
  // use hysteresis
  if (trig_hi && !out_state) { // switch on
    out_state = true;
    out_write();
    out_time_on = millis();
    out_log_begin();
  } else if (trig_lo && out_state) { // switch off
    out_state = false;
    out_write();
    out_time_off = millis();
    out_log_endin();
  }
}

#endif // OUT_H

