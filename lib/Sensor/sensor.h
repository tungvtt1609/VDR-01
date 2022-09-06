/**
 ******************************************************************************
 * @file    sensor.h
 * @author  Robot Team
 * @version V1.0.0
 * @date    27-06-2022
 * @brief
 ******************************************************************************
 */
#ifndef _SENSOR_H_
#define _SENSOR_H_

/* #include */
#include <Arduino.h>
#include "TeensyThreads.h"

/* #define */
// define _sensor_state

#define PROXIMITY_PIN_1 34
#define PROXIMITY_PIN_2 35

void main_sensor(void);

class Ultrasonic
{
public:
    Ultrasonic(uint8_t trigger_pin, uint8_t pwm_pin);
    void Init_Ultra(void);
    uint32_t Get_Distance_cm(void);

private:
    // uint8_t time_us;
    uint8_t trigger_pin_;
    uint8_t pwm_pin_;
    void Trigger_DYP(void);
};

class Proximity
{
public:
    Proximity(uint8_t proximity_pin);
    void Init_Prox(void);
    uint8_t Get_Prox(void);

private:
    uint8_t proximity_pin_;
};

#endif