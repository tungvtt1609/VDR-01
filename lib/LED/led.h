/**
 ******************************************************************************
 * @file    led.h
 * @author  Robot Team
 * @version V1.0.0
 * @date    27-06-2022
 * @brief
 ******************************************************************************
 */

#ifndef _LED_H_
#define _LED_H_

#include <Arduino.h>
#include "TeensyThreads.h"
#include <Adafruit_NeoPixel.h>

void main_led(void);
void Init_led(void);
void Warning_state();
void Low_battery_state();
void Blink_random();
void Charging_state();
void Full_battery_state();
void Waiting_state();
void Normal_state();
uint32_t Wheel(byte WheelPos);

#endif