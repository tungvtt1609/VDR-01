/**
 ******************************************************************************
 * @file    led.cpp
 * @author  Robot Team
 * @version V1.0.0
 * @date    27-06-2022
 * @brief
 ******************************************************************************
 */

/* #include */
#include "led.h"
#include <Arduino.h>
#include "charge.h"
#include "motor.h"
#include "sensor.h"

Adafruit_NeoPixel pixels(NUMPIXELS, LED_RGB_PIN, NEO_GRB + NEO_KHZ800);

uint8_t j_in_state = 0;
unsigned long g_prev_status_led = 0;

extern uint8_t _charging_state;
extern uint8_t _sensor_state;
extern uint8_t _running_state;

void main_led(void)
{
    Init_led();
    while (1)
    {
        if(_sensor_state == SONA_SHORT_DISTANCE)
        {
            Warning_state();
        } 
        else if (_charging_state == CHARGING)
        {
            Charging_state();
        }
        else if (_charging_state == LOW_BATTERY)
        {
            Low_battery_state();
        }
        else if (_charging_state == FULL_BATTERY)
        {
            Full_battery_state();
        }
        else if (_running_state == STOP)
        {
            Waiting_state();
        }
        else
        {
            Normal_state();
        }
        threads.yield();
    }
}

void Init_led(void)
{
    pixels.begin();
    pixels.setBrightness(BRIGHTNESS);
}

void Warning_state() // Nhay do
{
    for (int i = 0; i < NUMPIXELS; i++)
    {
        pixels.setPixelColor(i, RGB_RED);
    }
    threads.delay(300);
    pixels.show();

    pixels.clear();
    threads.delay(300);
    pixels.show();
}

void Low_battery_state() // Nhay vang
{
    for (int i = 0; i < NUMPIXELS; i++)
    {
        pixels.setPixelColor(i, RGB_YELLOW);
    }
    threads.delay(600);
    pixels.show();

    pixels.clear();
    threads.delay(600);
    pixels.show();
}

void Full_battery_state() // Sang xanh
{
    for (int i = 0; i < NUMPIXELS; i++)
    {
        pixels.setPixelColor(i, RGB_GREEN);
    }
    pixels.show();
}

void Blink_random()
{
    for (int i = 0; i < NUMPIXELS; i++)
    {
        pixels.setPixelColor(i, pixels.Color(random(0, 255), random(0, 255), random(0, 255)));
        pixels.show();
    }
}

void Charging_state() // Sac pin
{

    pixels.setPixelColor(j_in_state, RGB_RED);
    pixels.show();
    j_in_state++;
    if (j_in_state > NUMPIXELS)
    {
        j_in_state = 0;
    }

    pixels.setPixelColor(j_in_state, RGB_GREEN);
    pixels.show();
    j_in_state++;
    if (j_in_state > NUMPIXELS)
    {
        j_in_state = 0;
    }

    pixels.setPixelColor(j_in_state, RGB_BLUE);
    pixels.show();
    j_in_state++;
    if (j_in_state > NUMPIXELS)
    {
        j_in_state = 0;
    }
}

void Waiting_state()
{
    uint16_t i;
    for (i = 0; i < NUMPIXELS; i++)
    {
        pixels.setPixelColor(i, Wheel((i + j_in_state) & 255));
    }
    pixels.show();
    j_in_state = (j_in_state + 1) % 256;
}

void Normal_state()
{
    uint16_t i;
    for (i = 0; i < pixels.numPixels(); i++)
    {
        pixels.setPixelColor(i, Wheel(((i * 256 / NUMPIXELS) + j_in_state) & 255));
    }
    pixels.show();
    j_in_state = (j_in_state + 1) % (256 * 5);
}

uint32_t Wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85)
    {
        return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170)
    {
        WheelPos -= 85;
        return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
