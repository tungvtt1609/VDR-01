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
<<<<<<< HEAD
    PulseTimerFWD();
    PulseTimerSide();
    mapp(PulseWidthFWD, in_min, in_max, out_min_v, out_max_v);
    mapp(PulseWidthSide, in_min, in_max, out_min_w, out_max_w);
=======

    PulseTimerFWD();
    PulseTimerSide();
    get_rpm_right_RC();
    get_rpm_left_RC();

>>>>>>> ca4796aa6fc6dde324035d746e21b4a66d5ef8be
}

void main_pwm(void)
{
    setup_RC();
<<<<<<< HEAD
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
=======

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

        // Serial.print("PulseWidthFWD:");
        // Serial.println(PulseWidthFWD);
        // Serial.print("vel_RC_linear:");
        // Serial.println(vel_RC_linear);
        // threads.delay(1000);
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
            vel_RC_angular =(PulseWidthSide - in_min) * (out_max_w - out_min_w) / (in_max - in_min) + out_min_w;
        }

        // Serial.print("PulseWidthSide:");
        // Serial.println(PulseWidthSide);
        // Serial.print("vel_RC_angular:");
        // Serial.println(vel_RC_angular);
        // threads.delay(1000);

        vel_right = 0 - get_rpm_right_RC();
        // Serial.print("vel_right:");
        // Serial.println(vel_right);
        // threads.delay(1000);

        vel_left  = get_rpm_left_RC();
        // Serial.print("vel_left:");
        // Serial.println(vel_left);
        // threads.delay(1000);

        Write_Velocity_rpm(Right_Wheel_ID, (int32_t)vel_right);
        Write_Velocity_rpm(Left_Wheel_ID, (int32_t)vel_left);
>>>>>>> ca4796aa6fc6dde324035d746e21b4a66d5ef8be
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
    float linear_vel_right, angular_vel_right_s, angular_vel_right_mins_RC;
    angular_vel_right_s = (vel_RC_linear + vel_RC_angular * (DISTANCE_WHEEL / 2)) / (DIAMETER_WHEEL / 2); // rad/s
    angular_vel_right_mins_RC = (angular_vel_right_s / PI) * 30;
    return angular_vel_right_mins_RC;
}

float get_rpm_left_RC(){
    float linear_vel_left, angular_vel_left_s, angular_vel_left_mins_RC;
    angular_vel_left_s = (vel_RC_linear - vel_RC_angular * (DISTANCE_WHEEL / 2)) / (DIAMETER_WHEEL / 2); // rad/s
    angular_vel_left_mins_RC = (angular_vel_left_s / PI) * 30;
    return angular_vel_left_mins_RC;
}