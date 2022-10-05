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
extern uint8_t _button_state;
extern uint8_t sona_distance[3];
extern uint8_t proxi_sensor[2];

Proximity proxi_1(PROXIMITY_PIN_1);
Proximity proxi_2(PROXIMITY_PIN_2);

SONA sona_1(TRIGGER_1, ECHO_1);
SONA sona_2(TRIGGER_2, ECHO_2);
SONA sona_3(TRIGGER_3, ECHO_3);

void main_sensor(void)
{
    sona_1.Init_Sona();
    sona_2.Init_Sona();
    sona_3.Init_Sona();
    proxi_1.Init_Prox();
    proxi_2.Init_Prox();
    while (1)
    {

        proxi_sensor[0] = proxi_1.Get_Prox();
        proxi_sensor[1] = proxi_2.Get_Prox();

        _sensor_state = SONA_LONG_DISTANCE;
        for(uint8_t i = 0; i < 3; i++){
            if(sona_distance[i] <= 10){
                _sensor_state = SONA_SHORT_DISTANCE;
                digitalWrite(SSR_POWER_PIN, LOW);
            }
        }
        threads.delay(100);
        threads.yield();
    }
}

void trigger_sona(void){
    digitalWrite(TRIGGER_1, LOW);
    digitalWrite(TRIGGER_2, LOW);
    digitalWrite(TRIGGER_3, LOW);
    threads.delay_us(2);
    digitalWrite(TRIGGER_1, HIGH);
    digitalWrite(TRIGGER_2, HIGH);
    digitalWrite(TRIGGER_3, HIGH);
    threads.delay_us(10);
    digitalWrite(TRIGGER_1, LOW);
    digitalWrite(TRIGGER_2, LOW);
    digitalWrite(TRIGGER_3, LOW);

}

Proximity::Proximity(uint8_t proximity_pin) : proximity_pin_(proximity_pin)
{

}

SONA::SONA(uint8_t trigger_pin, uint8_t echo_pin):
trigger_pin_(trigger_pin),
echo_pin_(echo_pin)
{ 

}


void setup(void){
    pinMode(SSR_POWER_PIN, OUTPUT);
    digitalWrite(SSR_POWER_PIN, HIGH);
}

void SONA::Init_Sona(void)
{
    pinMode(trigger_pin_, OUTPUT);
    pinMode(echo_pin_, INPUT);
}

void Proximity::Init_Prox(void)
{
    pinMode(proximity_pin_, INPUT);
}

uint32_t SONA::Get_Distance_cm(void)
{
    SONA::Trigger_DYP();
    uint32_t duration, distanceCm;
    pinMode(echo_pin_, INPUT);
    duration = pulseIn(echo_pin_, HIGH, TIME_OUT);
    distanceCm = duration / 58;
    return distanceCm;
}

uint8_t Proximity::Get_Prox(void)
{
    return digitalRead(proximity_pin_);
}