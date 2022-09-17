#include <Arduino.h>
#include "TeensyThreads.h"

#define RCPinFWD 2
#define RCPinSide 3

void setup_RC(void);
void main_pwm(void);
void PulseTimerFWD(void);
void PulseTimerSide(void);