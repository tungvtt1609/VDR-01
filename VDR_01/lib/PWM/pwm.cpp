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
volatile float PulseWidthFWD = 0;
volatile float resultFWD;

volatile long StartTimeSide = 0;
volatile long CurrentTimeSide = 0;
volatile long PulsesSide = 0;
volatile float PulseWidthSide = 0;
volatile float resultSide;
volatile float vel_RC_linear = 0;
volatile float vel_RC_angular = 0;

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
    float vel_right, vel_left;

    // Velocity Linear
    if(PulsesFWD < 2000){
        PulseWidthFWD = PulsesFWD;
    }

    if(abs(PulseWidthFWD - 1495) < 20)
    {
        vel_RC_linear = 0.0;
    } 
    else 
    {
        vel_RC_linear = mapp(PulseWidthFWD, in_min, in_max, out_min_v, out_max_v);
    }

    // Velocity angular
    if(PulsesSide < 2000){
        PulseWidthSide = PulsesSide;
    }

    if(abs(PulseWidthSide - 1495) < 20)
    {
        vel_RC_angular = 0.0;
    } 
    else 
    {
        vel_RC_angular = mapp(PulseWidthSide, in_min, in_max, out_min_w, out_max_w);
    }

    vel_right = 0 - get_rpm_right_RC();
    vel_left  = get_rpm_left_RC();

    Write_Velocity_rpm(Right_Wheel_ID, (int32_t)vel_right);
    Write_Velocity_rpm(Left_Wheel_ID, (int32_t)vel_left);

    // Serial.print(PulseWidthFWD);
    // Serial.print(" ");
    // Serial.println(PulseWidthSide);
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

float get_rpm_right_RC(){
    float linear_vel_right, angular_vel_right_s, angular_vel_right_mins_RC;
    linear_vel_right = vel_RC_linear + (vel_RC_angular * DISTANCE_WHEEL) / 2;
    angular_vel_right_s = linear_vel_right / (DIAMETER_WHEEL / 2); // rad/s
    angular_vel_right_mins_RC = (angular_vel_right_s * 30) / 2;
    return angular_vel_right_mins_RC;
}

float get_rpm_left_RC(){
    float linear_vel_left, angular_vel_left_s, angular_vel_left_mins_RC;
    linear_vel_left = vel_RC_linear - (vel_RC_angular * DISTANCE_WHEEL) / 2;
    angular_vel_left_s = linear_vel_left / (DIAMETER_WHEEL / 2); // rad/s
    angular_vel_left_mins_RC = (angular_vel_left_s * 30) / 2;
    return angular_vel_left_mins_RC;
}

long mapp(long x, long y, long z, long v, long t){
    return (x - in_min) * (t - v) / (z - y) + v;
}