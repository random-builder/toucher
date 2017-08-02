
// persisted settings

#ifndef ROM_H
#define ROM_H

#include "a.h"
#include "log.h"
#include <avr/eeprom.h>

// macros https://projectgus.com/2010/07/eeprom-access-with-arduino/
#define rom_read_into(dst_p, field, dst_size) eeprom_read_block(dst_p, (void *) offsetof(rom_struct, field), MIN(dst_size, sizeof((rom_struct*)0)->field))
#define rom_read(dst, field) rom_read_into(&dst, field, sizeof(dst))
#define rom_write_from(src_p, field, src_size) eeprom_write_block(src_p, (void *) offsetof(rom_struct, field), MIN(src_size, sizeof((rom_struct*)0)->field))
#define rom_write(src, field) { typeof(src) x = src; rom_write_from(&x, field, sizeof(x)); }

// program settings
struct rom_struct {
  int form; // format version
  int fsr_fast; // fast avg rate
  int fsr_slow; // slow avg rate
  int fsr_lo; // trigger lower threshold
  int fsr_hi; // trigger upper threshold
  boolean fsr_inv; // invert output value
  int log_mode; // logger mode config
  int srv_ini_clr; // initial servo position
};

// report macros
#define rom_print(x) rom_read(x,x); log_print(F(STR(x))); log_print(F("=")); Serial.println(x); // XXX

// report settings
void rom_report() {
  int form; rom_print(form);
  int fsr_fast; rom_print(fsr_fast);
  int fsr_slow; rom_print(fsr_slow);
  int fsr_lo; rom_print(fsr_lo);
  int fsr_hi; rom_print(fsr_hi);
  boolean fsr_inv; rom_print(fsr_inv);
  int log_mode; rom_print(log_mode);
  int srv_ini_clr; rom_print(srv_ini_clr);
}

// settings version
#define rom_form 10

// settings defaults
void rom_defaults() {
  rom_write(4, fsr_fast);
  rom_write(7, fsr_slow);
  rom_write(7, fsr_lo);
  rom_write(14, fsr_hi);
  rom_write(true, fsr_inv);
  rom_write(0, log_mode);
  rom_write(0, srv_ini_clr);
  rom_write(rom_form, form);
}

// initialize settings
void rom_setup() {
  int form;
  rom_read(form, form);
  if (form != rom_form) {
    rom_defaults();
  }
  rom_report();
}

#endif // ROM_H

