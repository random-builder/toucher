
// average caclulator

#include <Arduino.h>

#include "a.h"
#include "avg.h"

// rate validation limits
#define AVG_FAST_MIN 2
#define AVG_FAST_MAX 8
#define AVG_SLOW_MIN 3
#define AVG_SLOW_MAX 9

// average decay rates: decay = 2 ^ rate
int avg_fast_rate = 4; // power of fast decay
int avg_slow_rate = avg_fast_rate + 7; // power of slow decay

// field setter
void avg_set_fast_rate(const int rate) {
  avg_fast_rate = rate;
}

// field setter
void avg_set_slow_rate(const int rate) {
  avg_slow_rate = avg_fast_rate + rate; // combine exponents
}

// channel signal calculation parameters
int fast_avg_val[] = {0, 0, 0, 0}; // channel average
long fast_avg_sum[] = { 0, 0, 0, 0 }; // channel avg total
int fast_dev_val[] = { 0, 0, 0, 0 }; // channel deviation
long fast_dev_sum[] = { 0, 0, 0, 0 }; // channel dev total

// channel offset calculation parameters
int slow_avg_val[] = { 0, 0, 0, 0 }; // channel average
long slow_avg_sum[] = { 0, 0, 0, 0 }; // channel avg total
int slow_dev_val[] = { 0, 0, 0, 0 }; // channel deviation
long slow_dev_sum[] = { 0, 0, 0, 0 }; // channel dev total

// validate fast rate
boolean avg_is_fast_rate(const int rate) {
  return (AVG_FAST_MIN <= rate) && (rate <= AVG_FAST_MAX);
}

// validate slow rate
boolean avg_is_slow_rate(const int rate) {
  return (AVG_SLOW_MIN <= rate) && (rate <= AVG_SLOW_MAX);
}

// calculate fast signal average and deviation
void avg_calc_fast(const byte ch, const int val) {
  //
  int avg_val = fast_avg_val[ch];
  long avg_sum = fast_avg_sum[ch];
  avg_sum += val;
  avg_sum -= avg_val;
  avg_val = avg_sum >> avg_fast_rate; // division by decay
  fast_avg_val[ch] = avg_val;
  fast_avg_sum[ch] = avg_sum;
  //
  const int dev = abs(val - avg_val);
  int dev_val = fast_dev_val[ch];
  long dev_sum = fast_dev_sum[ch];
  dev_sum += dev;
  dev_sum -= dev_val;
  dev_val = dev_sum >> avg_fast_rate; // division by decay
  fast_dev_val[ch] = dev_val;
  fast_dev_sum[ch] = dev_sum;
}

// calculate slow signal average and deviation
void avg_calc_slow(const byte ch, const int val) {
  //
  int avg_val = slow_avg_val[ch];
  long avg_sum = slow_avg_sum[ch];
  avg_sum += val;
  avg_sum -= avg_val;
  avg_val = avg_sum >> avg_slow_rate; // division by decay
  slow_avg_val[ch] = avg_val;
  slow_avg_sum[ch] = avg_sum;
  //
  const int dev = abs(val - avg_val);
  int dev_val = slow_dev_val[ch];
  long dev_sum = slow_dev_sum[ch];
  dev_sum += dev;
  dev_sum -= dev_val;
  dev_val = dev_sum >> avg_slow_rate; // division by decay
  slow_dev_val[ch] = dev_val;
  slow_dev_sum[ch] = dev_sum;
}

// evaluate signal condition
int avg_calc_trigger() {
  int delta = 0;
  int drift = 0;
  for (byte ch = 0; ch < AVG_CH_NUM; ch++) {
    const int fast_avg = fast_avg_val[ch];
    //const int fast_dev = fast_dev_val[ch];
    const int slow_avg = slow_avg_val[ch];
    const int slow_dev = slow_dev_val[ch];
    delta += abs(fast_avg - slow_avg);
    drift += (slow_dev);
  }
  return delta - drift;
}

void avg_render_chan(byte ch, char text[]) {
  sprintf_P(text, PSTR("C%i "), ch);
}

void avg_render_fast(byte ch, char text[]) {
  sprintf_P(text, PSTR("F=%4i/%-2i "), fast_avg_val[ch], fast_dev_val[ch]);
}

void avg_render_slow(byte ch, char text[]) {
  sprintf_P(text, PSTR("S=%4i/%-2i "), slow_avg_val[ch], slow_dev_val[ch]);
}

void avg_render_diff(byte ch, char text[]) {
  sprintf_P(text, PSTR("D=%-+3i "), fast_avg_val[ch] - slow_avg_val[ch]);
}

