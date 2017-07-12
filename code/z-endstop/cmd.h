
// terminal interface

#ifndef CMD_H
#define CMD_H

#include "avg.h"
#include "srv.h"
#include "rom.h"
#include "xCmd.h"

// commands report
void cmd_help() {
  Serial.println(F("---help---"));
  Serial.println(F("fsr fast NN # set avg fast rate to NN"));
  Serial.println(F("fsr slow NN # set avg slow rate to NN"));
  Serial.println(F("fsr lo NN # set avg event lower trigger to NN"));
  Serial.println(F("fsr hi NN # set avg event upper trigger to NN"));
  Serial.println(F("fsr inv B # change output invert mode to 0 or 1"));
  Serial.println(F("srv clr NN # rotate cleaner servo to NN degrees"));
  Serial.println(F("log fsr event # enable on-demand report of avg"));
  Serial.println(F("log fsr stats # enable periodic report of avg"));
  Serial.println(F("log fsr off # disable all fsr reports of avg"));
  Serial.println(F("rom # report settings"));
}

// log commands
void cmd_act_log(int argc, char **argv) {
  if (argc <= 2) {
    cmd_help(); return;
  }
  if (strcmp_P(argv[1], PSTR("fsr")) == 0 ) {
    if (strcmp_P(argv[2], PSTR("stats")) == 0 ) {
      rom_write(true, log_fsr_stats); return;
    }
    if (strcmp_P(argv[2], PSTR("event")) == 0 ) {
      rom_write(true, log_fsr_event); return;
    }
    if (strcmp_P(argv[2], PSTR("off")) == 0 ) {
      rom_write(false, log_fsr_stats); rom_write(false, log_fsr_event); return;
    }
  } else {
    cmd_help();
  }
}

int read_int2(char **argv) {
  return cmdStr2Num(argv[2], 10);
}

int read_int3(char **argv) {
  return cmdStr2Num(argv[3], 10);
}

// fsr commands
void cmd_act_fsr(int argc, char **argv) {
  if (argc <= 2) {
    cmd_help(); return;
  }
  const int val = read_int2(argv);
  if (strcmp_P(argv[1], PSTR("fast")) == 0 ) {
    if (avg_is_fast_rate(val)) {
      rom_write(val, fsr_fast); return ;
    }
  }
  if (strcmp_P(argv[1], PSTR("slow")) == 0 ) {
    if (avg_is_slow_rate(val)) {
      rom_write(val, fsr_slow); return;
    }
  }
  if (strcmp_P(argv[1], PSTR("lo")) == 0 ) {
    rom_write(val, fsr_lo); return;
  }
  if (strcmp_P(argv[1], PSTR("hi")) == 0 ) {
    rom_write(val, fsr_hi); return ;
  }
  if (strcmp_P(argv[1], PSTR("inv")) == 0 ) {
    if (val == 0 || val == 1) {
      rom_write(val, fsr_inv); return;
    }
  }
  cmd_help();
}

// servo commands
void cmd_act_srv(int argc, char **argv) {
  if (argc <= 2) {
    cmd_help(); return;
  }
  int val = read_int2(argv);
  if (strcmp_P(argv[1], PSTR("clr")) == 0 ) {
    if ( srv_is_angle(val) ) {
      srv_cleaner(val); return;
    }
  }
  if (strcmp_P(argv[1], PSTR("ini")) == 0 ) {
    if (strcmp_P(argv[2], PSTR("clr")) == 0 ) {
      int val = read_int3(argv);
      if ( srv_is_angle(val) ) {
        rom_write(val, srv_ini_clr); return;
      }
    }
  }
  cmd_help();
}

// configuration commands
void cmd_act_rom(int argc, char **argv) {
  UNUSED(argc); UNUSED(argv);
  rom_report();
}

// help commands
void cmd_act_help(int argc, char **argv) {
  UNUSED(argc); UNUSED(argv);
  cmd_help();
}

// initialize commands
void cmd_setup() {
  cmdInit(&Serial);
  cmdAdd("log", cmd_act_log);
  cmdAdd("fsr", cmd_act_fsr);
  cmdAdd("srv", cmd_act_srv);
  cmdAdd("rom", cmd_act_rom);
  cmdAdd("help", cmd_act_help);
  cmdPrompt();
}

void cmd_loop() {
  cmdPoll();
}

#endif // CMD_H

