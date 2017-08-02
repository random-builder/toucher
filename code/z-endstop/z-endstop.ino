
// main program

#include "a.h"
#include "adc.h"
#include "avg.h"
#include "cmd.h"
#include "out.h"
#include "srv.h"
#include "rom.h"

// process analog channel
void eval_avg(const byte ch) {
  cli(); // enter critical section
  boolean has = false;
  const int val = adc_result[ch];
  if ( val != ADC_NADA ) { // value pending
    has = true;
    adc_result[ch] = ADC_NADA; // value consumed
  }
  sei(); // leave critical section
  if (has) {
    avg_calc_fast(ch, val);
    avg_calc_slow(ch, val);
  }
}

// process all analog inputs
void loop_avg() {
  for (byte ch = 0; ch < AVG_CH_NUM; ch++) {
    eval_avg(ch);
  }
}

// logger params
#define LOG_PERIOD 3 // seconds
int LOG_LOOP = calc_log_loop();
int log_step = 0; // loop step
boolean log_fsr_stats = false; // rom config

// publish periodic report
void loop_log() {
  log_step++;
  if (log_step < LOG_LOOP) {
    return;
  }
  log_step = 0;
  LOG_LOOP = calc_log_loop();
  if (!log_fsr_stats) return; // log when enabled
  if (out_state) return; // log outside signal
  print_fsr_stats();
}

// system params
#define SYS_LOOP 1000 // sample frequency
int sys_step = 0; // loop step
long wall_time = 0; // sample time, milliseconds
long loop_time = 225; // loop period, microseconds

// load settings
void apply_rom() {
  int rate;
  rom_read(rate, fsr_fast); avg_set_fast_rate(rate);
  rom_read(rate, fsr_slow); avg_set_slow_rate(rate);
  rom_read(out_trig_hi, fsr_hi);
  rom_read(out_trig_lo, fsr_lo);
  rom_read(out_invert, fsr_inv);
  rom_read(out_log_event, log_fsr_event);
  rom_read(log_fsr_stats, log_fsr_stats);
}

// measure times
void apply_time() {
  const long this_time = millis();
  loop_time = this_time - wall_time;
  wall_time = this_time;
}

// update system state
void loop_sys() {
  sys_step++;
  if (sys_step < SYS_LOOP) {
    return;
  }
  sys_step = 0;
  apply_rom();
  apply_time();
}

// auto adjust logger period to N seconds
int calc_log_loop() {
  return LOG_PERIOD * 1000000 / loop_time;
}

// report sensor status
void print_fsr_stats() {
  char text[32];
  sprintf_P(text, PSTR("T@%-8lu | "), millis()); Serial.print(text);
  sprintf_P(text, PSTR("L=%-4lu | "), loop_time); Serial.print(text);
  for (byte ch = 0; ch < 4; ch++) {
    avg_render_status(ch, text); Serial.print(text); Serial.print(F("| "));
  }
  Serial.println();
}

// check terminal input
void loop_cmd() {
  cmd_loop();
}

// trigger output signal
void loop_out() {
  out_signal();
}

// init port
void setupComm() {
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("make: " __DATE__ " " __TIME__));
}

// main setup
void setup() {
  setupComm();
  rom_setup();
  apply_rom();
  cmd_setup();
  adc_setup();
  out_setup();
  srv_setup();
}

// main loop
void loop() {
  loop_avg();
  loop_out();
  loop_sys();
  loop_log();
  loop_cmd();
}

