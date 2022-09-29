#include <Arduino.h>
#include "TeensyThreads.h"

#define RCPinFWD 3
#define RCPinSide 4

long in_max  = 2000;
long in_min  = 988;
long out_max = 1;
long out_min = -1;


void setup_RC(void);
void main_pwm(void);
void PulseTimerFWD(void);
long mapp(long x, long in_min, long in_max, long out_min, long out_max);
void PulseTimerSide(void);