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
#include "motor.h"

volatile long StartTimeFWD = 0;
volatile long CurrentTimeFWD = 0;
volatile long PulsesFWD = 0;
float PulseWidthFWD = 0;
float resultFWD;

volatile long StartTimeSide = 0;
volatile long CurrentTimeSide = 0;
volatile long PulsesSide = 0;
float PulseWidthSide = 0;
float resultSide;

extern float g_req_linear_vel_x;
extern float g_req_linear_vel_y;
extern float g_req_linear_vel_z;

void setup_RC(void)
{   
    Serial.begin(9600);
    pinMode(RCPinFWD, INPUT_PULLUP);
    pinMode(RCPinSide, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(RCPinFWD), PulseTimerFWD, CHANGE);
    attachInterrupt(digitalPinToInterrupt(RCPinSide), PulseTimerSide, CHANGE);
}

void main_pwm(void)
{
    if(PulsesFWD < 2000){
        PulseWidthFWD = PulsesFWD;
    }
    if(PulsesSide < 2000){
        PulseWidthSide = PulsesSide;
    }

    resultFWD = mapp(PulseWidthFWD, in_min, in_max, out_min, out_max);
    resultSide = mapp(PulseWidthSide, in_min, in_max, out_min, out_max);

    g_req_linear_vel_x = resultFWD;
    g_req_linear_vel_z = resultSide;

    Serial.print(PulseWidthFWD);

    Serial.print(" ");
    Serial.println(PulseWidthSide);
}

void PulseTimerFWD(void){
    CurrentTimeFWD = micros();
    if(CurrentTimeFWD > StartTimeFWD){
        PulsesFWD = CurrentTimeFWD - StartTimeFWD;
        StartTimeFWD = CurrentTimeFWD;
    }
}

void PulseTimerSide(void){
    CurrentTimeSide = micros();
    if(CurrentTimeSide > StartTimeSide){
        PulsesSide = CurrentTimeSide - StartTimeSide;
        StartTimeSide = CurrentTimeSide;
    }
}

long mapp(long x, long in_min, long in_max, long out_min, long out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}