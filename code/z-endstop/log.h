
// logger settings

#ifndef LOG_H
#define LOG_H

// on-demand fsr event reporting
#define LOG_FSR_EVENT_ALL   31 // all event fields
#define LOG_FSR_EVENT_TRIG  1  // event trigger value
#define LOG_FSR_EVENT_FAST  2  // fast average and deviation
#define LOG_FSR_EVENT_SLOW  4  // slow average and deviation
#define LOG_FSR_EVENT_DIFF  8  // difference between fast and slow average
#define LOG_FSR_EVENT_PULSE 16 // sensors event single pulse duration time

// periodic fsr status reporting
#define LOG_FSR_STATS_ALL   32 // all stats fields
#define LOG_FSR_STATS_LINE  32 // default status line

// enable all available fsr fields
#define LOG_FSR_ALL         63 // both event and stats

///

// use for internal bit mask tests
#define LOG_FSR_EVENT_SOME LOG_FSR_EVENT_ALL //
#define LOG_FSR_EVENT_SOME_CHAN (LOG_FSR_EVENT_FAST | LOG_FSR_EVENT_SLOW | LOG_FSR_EVENT_DIFF)
#define LOG_FSR_STATS_SOME LOG_FSR_STATS_ALL //

// current enabled state
int log_mode_mask = 0; // rom config

boolean log_has_mode(const int mode_mask){
  return log_mode_mask & mode_mask;
}

void log_print(char text[]) {
  Serial.print(text);
}

void log_print(const __FlashStringHelper* text) {
  Serial.print(text);
}

void log_println(char text[]) {
  Serial.println(text);
}

void log_println(const __FlashStringHelper* text) {
  Serial.println(text);
}

void log_println() {
  Serial.println();
}

void log_print_bar() {
  Serial.print(F("| "));
}

#endif // LOG_H

