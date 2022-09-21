#include <Arduino.h>
#include "TeensyThreads.h"

#define RCPinFWD 2
#define RCPinSide 3

#define in_max 2000
#define in_min 988
#define out_max 1
#define out_min -1


void setup_RC(void);
void main_pwm(void);
void PulseTimerFWD(void);
long mapp(long x, long in_min, long in_max, long out_min, long out_max);
void PulseTimerSide(void);