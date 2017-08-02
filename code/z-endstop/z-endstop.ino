
// main program

#include "a.h"
#include "adc.h"
#include "avg.h"
#include "cmd.h"
#include "log.h"
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

// publish periodic report
void loop_log() {
  log_step++;
  if (log_step < LOG_LOOP) {
    return;
  }
  log_step = 0;
  LOG_LOOP = calc_log_loop();
  log_fsr_stats();
}

// system params
#define SYS_LOOP 1000 // sample frequency
int sys_step = 0; // loop step
long wall_time = 0; // sample time, milliseconds
long loop_time = 220; // loop period, microseconds

// load settings
void apply_rom() {
  int rate;
  rom_read(rate, fsr_fast); avg_set_fast_rate(rate);
  rom_read(rate, fsr_slow); avg_set_slow_rate(rate);
  rom_read(out_trig_hi, fsr_hi);
  rom_read(out_trig_lo, fsr_lo);
  rom_read(out_invert, fsr_inv);
  rom_read(log_mode_mask, log_mode);
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
void log_fsr_stats() {
  if (out_state) return; // log outside signal
  if (!log_has_mode(LOG_FSR_STATS_SOME)) return; // log when enabled
  char text[32];
  sprintf_P(text, PSTR("T@%-8lu | "), millis()); log_print(text);
  sprintf_P(text, PSTR("L=%-4lu | "), loop_time); log_print(text);
  for (byte ch = 0; ch < 4; ch++) {
    avg_render_chan(ch, text); log_print(text);
    avg_render_slow(ch, text); log_print(text);
    log_print_bar();
  }
  log_println();
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
  log_println();
  log_println(F("make: " __DATE__ " " __TIME__));
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

