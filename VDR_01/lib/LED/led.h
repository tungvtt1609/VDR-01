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

#define LIGHT_PIN 33
#define LED_RGB_PIN 21

#define NUMPIXELS 20
#define BRIGHTNESS 50

#define RGB_RED pixels.Color(255, 0, 0)
#define RGB_GREEN pixels.Color(0, 255, 0)
#define RGB_BLUE pixels.Color(0, 0, 255)
#define RGB_WHITE pixels.Color(127, 127, 127)
#define RGB_YELLOW pixels.Color(255, 255, 0)

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