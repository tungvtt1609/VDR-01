/**
 ******************************************************************************
 * @file    charge.cpp
 * @author  Robot Team
 * @version V1.0.0
 * @date    27-06-2022
 * @brief
 ******************************************************************************
 */

#include <Arduino.h>
#include "pwm.h"
#include "TeensyThreads.h"

volatile long StartTimeFWD = 0;
volatile long CurrentTimeFWD = 0;
volatile long PulsesFWD = 0;
int PulseWidthFWD = 0;

volatile long StartTimeSide = 0;
volatile long CurrentTimeSide = 0;
volatile long PulsesSide = 0;
int PulseWidthSide = 0;

void setup_RC(void)
{   
    Serial.begin(9600);
    pinMode(RCPinFWD, INPUT_PULLUP);
    pinMode(RCPinSide, INPUT_PULLUP);

}