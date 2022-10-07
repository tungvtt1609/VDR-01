/**
  ******************************************************************************
  * @file    sensor.h
  * @author  dongts1 - M3 Team
  * @version V1.0.0
  * @date    19-10-2021
  * @brief   .
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
void trigger_5_ultra(void);


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