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

    get_rpm_right_RC();
    get_rpm_left_RC();
    
    PulseTimerSide();
    PulseTimerFWD();
}

void main_pwm(void)
{
    setup_RC();

    while (1){

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
            vel_RC_linear = (PulseWidthFWD - in_min) * (out_max_v - out_min_v) / (in_max - in_min) + out_min_v;
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
            vel_RC_angular = (PulseWidthSide - in_min) * (out_max_w - out_min_w) / (in_max - in_min) + out_min_w;
        }

        // Check condition active
        if(analogRead(RCPinFWD) == 0 && analogRead(RCPinSide) == 0)
        {
            vel_left = 0.0;
            vel_right = 0.0;
            Motor_disable();
        }
        else
        {
            vel_right = 0 - get_rpm_right_RC();
            vel_left  = get_rpm_left_RC();    
        }

        // vel_right = 0 - get_rpm_right_RC();
        // vel_left  = get_rpm_left_RC();

        Write_Velocity_rpm(Right_Wheel_ID, (int32_t)vel_right);
        Write_Velocity_rpm(Left_Wheel_ID, (int32_t)vel_left);

    }
    
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
    float angular_vel_right_s, angular_vel_right_mins_RC;
    angular_vel_right_s = (vel_RC_linear + vel_RC_angular * (DISTANCE_WHEEL / 2)) / (DIAMETER_WHEEL / 2); // rad/s
    angular_vel_right_mins_RC = (angular_vel_right_s / PI) * 30;
    return angular_vel_right_mins_RC;
}

float get_rpm_left_RC(){
    float angular_vel_left_s, angular_vel_left_mins_RC;
    angular_vel_left_s = (vel_RC_linear - vel_RC_angular * (DISTANCE_WHEEL / 2)) / (DIAMETER_WHEEL / 2); // rad/s
    angular_vel_left_mins_RC = (angular_vel_left_s / PI) * 30;
    return angular_vel_left_mins_RC;
}

long mapp(long x, long y, long z, long v, long t){
    return (x - y) * (t - v) / (z - y) + v;
}