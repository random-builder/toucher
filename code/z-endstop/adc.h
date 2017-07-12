
// analog convertor

#ifndef ADC_H
#define ADC_H

// adc clock selector
#define ADC_PS_16    ((1 << ADPS2))
#define ADC_PS_32    ((1 << ADPS2) | (1 << ADPS0))
#define ADC_PS_64    ((1 << ADPS2) | (1 << ADPS1))
#define ADC_PS_128   ((1 << ADPS2) | (1 << ADPS0) | (1 << ADPS0)) // 100 us per conversion

// module params
#define ADC_CH_NUM 4 // channel limit
#define ADC_NADA -1 // consumed value

// use adc interrupts
void adc_setup() {
  //
  ADMUX = 0;
  ADMUX |= (1 << REFS0); // set ADC reference to AVCC
  //ADMUX |= (1 << ADLAR); // right adjust the result
  //
  ADCSRA = 0;
  ADCSRA |= (1 << ADEN); // enable adc
  ADCSRA |= (1 << ADIE); // enable  interrupt
  //ADCSRA |= (1 << ADATE); // use auto trigger
  ADCSRA |= ADC_PS_128; // prescale clock
  //
  ADCSRB = 0;
  ADCSRB |= 0; // free running mode
  //
  DIDR0 = 0; // enable all channels
  //
  sei(); // enable interrupts
  ADCSRA |= (1 << ADSC); // start conversion
}

byte adc_channel = 0; // current channel index
int adc_result[ADC_CH_NUM] = {ADC_NADA, ADC_NADA, ADC_NADA, ADC_NADA}; // adc reading for channel

// adc interrupt entry point
ISR(ADC_vect) {
  adc_result[adc_channel] = ADC; // store result
  adc_channel++; // next channel
  if (adc_channel >= ADC_CH_NUM) adc_channel = 0; // rotate index
  ADMUX = (1 << REFS0) | adc_channel; // switch channel
  ADCSRA |= (1 << ADSC); // start conversion
}

#endif // ADC_H

