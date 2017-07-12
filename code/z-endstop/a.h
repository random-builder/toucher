
// common features

#ifndef A_H
#define A_H

// range of long
#define LONG_MIN -2147483648L
#define LONG_MAX +2147483647L

// analog input value range
#define ADC_MIN 0
#define ADC_MID 512
#define ADC_MAX 1023

// minimum of 2 numbers
#define MIN(x,y) ( x > y ? y : x )

// maximum of 2 numbers
#define MAX(x,y) ( x > y ? x : y )

// stringinfy parameter
#define XSTR(s) STR(s)
#define STR(s) #s

#define UNUSED(v) (void) (v)

#endif // A_H
