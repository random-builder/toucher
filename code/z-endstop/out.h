
// output signal

#ifndef OUT_H
#define OUT_H

#include "avg.h"
#include "log.h"

// system output pins
#define SYS_LED      13 // system led
#define FSR_OUT      12 // endstop signal

// output params
int out_trigger = 0; // last measured difference
long out_time_on = 0; // millis of out switch on
long out_time_off = 0; // millis of out switch off
boolean out_state = false; // output signal state
boolean out_invert = true; // invert output signal // rom config

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

void out_render_trig(char text[]) {
  sprintf_P(text, PSTR("G=%2i "), out_trigger);
}

void out_render_pulse(char text[]) {
  sprintf_P(text, PSTR("P=%-4lu "), out_time_off - out_time_on);
}

// report event begining
void out_log_begin() {
  if (!log_has_mode(LOG_FSR_EVENT_SOME)) return;
  char text[32];
  sprintf_P(text, PSTR("B@%-8lu | "), millis()); log_print(text);
  if (log_has_mode(LOG_FSR_EVENT_TRIG)) {
    out_render_trig(text); log_print(text); log_print_bar();
  }
  if (log_has_mode(LOG_FSR_EVENT_SOME_CHAN)) {
    for (byte ch = 0; ch < AVG_CH_NUM; ch++) {
      avg_render_chan(ch, text); log_print(text);
      if (log_has_mode(LOG_FSR_EVENT_FAST)) {
        avg_render_fast(ch, text); log_print(text);
      }
      if (log_has_mode(LOG_FSR_EVENT_SLOW)) {
        avg_render_slow(ch, text); log_print(text);
      }
      if (log_has_mode(LOG_FSR_EVENT_DIFF)) {
        avg_render_diff(ch, text); log_print(text);
      }
      log_print_bar();
    }
  }
  log_println();
}

// report event ending
void out_log_endin() {
  if (!log_has_mode(LOG_FSR_EVENT_SOME)) return;
  char text[32];
  sprintf_P(text, PSTR("E@%-8lu | "), millis()); log_print(text);
  if (log_has_mode(LOG_FSR_EVENT_TRIG)) {
    out_render_trig(text); log_print(text); log_print_bar();
  }
  if (log_has_mode(LOG_FSR_EVENT_PULSE)) {
    out_render_pulse(text); log_print(text); log_print_bar();
  }
  log_println();
}

// trigger levels
int out_trig_hi = 14; // upper threshold // rom config
int out_trig_lo = 7;  // lower threshold // rom config

// update output signal
void out_signal() {
  out_trigger = avg_calc_trigger();
  const boolean has_hi = (out_trigger >= out_trig_hi);
  const boolean has_lo = (out_trigger <= out_trig_lo);
  // use hysteresis
  if (has_hi && !out_state) { // switch on
    out_state = true;
    out_write();
    out_time_on = millis();
    out_log_begin();
  } else if (has_lo && out_state) { // switch off
    out_state = false;
    out_write();
    out_time_off = millis();
    out_log_endin();
  }
}

#endif // OUT_H

