#include <Arduino.h>
#include "TeensyThreads.h"

#define RCPinFWD 3
#define RCPinSide 4

#define in_max 1998
#define in_min 988

#define out_max_w 1
#define out_min_w -1
#define out_max_v 0.3
#define out_min_v -0.3


void setup_RC(void);
void main_pwm(void);
void PulseTimerFWD(void);
float get_rpm_right_RC();
float get_rpm_left_RC();
long mapp(long x, long y, long z, long v, long t);
void PulseTimerSide(void);