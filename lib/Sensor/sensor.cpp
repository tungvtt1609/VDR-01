/**
 ******************************************************************************
 * @file    sensor.cpp
 * @author  Robot Team
 * @version V1.0.0
 * @date    27-06-2022
 * @brief
 ******************************************************************************
 */

/* #include */
#include "sensor.h"
#include <Arduino.h>

extern uint8_t _sensor_state;
extern uint32_t ultra_distance[5];
extern uint8_t proxi_sensor[2];

Proximity proxi_1(PROXIMITY_PIN_1);
Proximity proxi_2(PROXIMITY_PIN_2);

void main_sensor(void)
{

    proxi_1.Init_Prox();
    proxi_2.Init_Prox();
    while (1)
    {

        proxi_sensor[0] = proxi_1.Get_Prox();
        proxi_sensor[1] = proxi_2.Get_Prox();

        threads.delay(100);

        threads.yield();
    }
}

Proximity::Proximity(uint8_t proximity_pin) : proximity_pin_(proximity_pin)
{
}

void Proximity::Init_Prox(void)
{
    pinMode(proximity_pin_, INPUT);
}

uint8_t Proximity::Get_Prox(void)
{
    return digitalRead(proximity_pin_);
}