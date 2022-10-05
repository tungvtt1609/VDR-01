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

#define SONA_LONG_DISTANCE 1
#define SONA_SHORT_DISTANCE 2

#define TRIGGER_1 8
#define ECHO_1 7
#define TRIGGER_2 24
#define ECHO_2 25
#define TRIGGER_3 29
#define ECHO_3 28

// define SSR and ESTOP
#define ESTOP_PIN 6
#define SSR_POWER_PIN 3

#define TIME_OUT 5000

void main_sensor(void);
void trigger_sona(void);
void setup(void);

class SONA
{
public:
    SONA(uint8_t trigger_pin, uint8_t echo_pin);
    void Init_Sona(void);
    uint32_t Get_Distance_cm(void);

private:
    // uint8_t time_us;
    uint8_t trigger_pin_;
    uint8_t echo_pin_;
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