
// average caclulator

#ifndef AVG_H
#define AVG_H

// number of channels
#define AVG_CH_NUM 4

// module settings
void avg_set_fast_rate(int rate);
void avg_set_slow_rate(int rate);

// validate rate
boolean avg_is_fast_rate(int rate);
boolean avg_is_slow_rate(int rate);

// update signal average and deviation
void avg_calc_fast(byte ch, int val);
void avg_calc_slow(byte ch, int val);

// evaluate signal trigger condition
int avg_calc_trigger();

// provide channel reporting status
void avg_render_detail(byte ch, char text[]);
void avg_render_status(byte ch, char text[]);

#endif // AVG_H

