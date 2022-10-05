/**
 ******************************************************************************
 * @file    charge.cpp
 * @author  Robot Team
 * @version V1.0.0
 * @date    27-06-2022
 * @brief
 ******************************************************************************
 */

/* #include */
#include "charge.h"
#include <Arduino.h>
#include <SoftwareSerial.h>

extern uint8_t _charging_state;
extern uint8_t battery_percent;

unsigned long prev_charging_time = 0;

char read_basic_info[] = {0xDD, 0xA5, 0x03, 0x00, 0xFF, 0xFD, 0x77};
char read_cell_vol[] = {0xDD, 0xA5, 0x04, 0x00, 0xFF, 0xFC, 0x77};
char read_hardware_ver[] = {0xDD, 0xA5, 0x05, 0x00, 0xFF, 0xFB, 0x77};

volatile int read_index = 0;
volatile int data_index;
char data_buffer[30];
volatile int vol_raw, cur_raw, remaining_capacity;
volatile float vol_index = 0;


void Init_charge(void)
{
    Serial.begin(57600);
    Serial2.begin(9600);
}

void main_charger(void)
{
    Init_charge();
    for (int i = 0; i < 7; i++)
    {
        Serial2.print(read_basic_info[i]);
    }
    delay(500);
    for (int i = 0; i < 7; i++)
    {
        Serial2.print(read_cell_vol[i]);
    }
    delay(500);
    for (int i = 0; i < 7; i++)
    {
        Serial2.print(read_hardware_ver[i]);
    }
    delay(500);


    while (1)
    {
        // get battery percent
        read_charger();
        if (_charging_state == CHARGING)
        {
            if (millis() - prev_charging_time >= 60000)
            {
                prev_charging_time = millis();
                threads.delay(10);
                battery_percent = map(vol_index, MIN_VOLTAGE, MAX_VOLTAGE, 0, 100);
                threads.delay(10);
            }
        }
        else
        {
            battery_percent = map(vol_index, MIN_VOLTAGE, MAX_VOLTAGE, 0, 100);
            // Serial.println(battery_percent);
        }

        // status charging, low_bat
        if (vol_index >= 2.5)
        {
            if (battery_percent >= 95)
            {
                _charging_state = FULL_BATTERY;
            }
            else
            {
                _charging_state = CHARGING;
            }
        }
        else if (battery_percent <= 30)
        {
            _charging_state = LOW_BATTERY;
        }
        else
        {
            _charging_state = NORMAL_BATTERY;
        }
        // read_charger();
        threads.yield();
    }
}

// Read RS485 from PIN
void read_charger(void)
{
    char inChar = (char)Serial2.read();
    if (read_index == 2)
    {
        data_buffer[data_index] = inChar;
        data_index++;
    }

    if (data_index >= 10 && read_index == 2)
    {
        vol_raw = data_buffer[2] << 8 | data_buffer[3];
        cur_raw = data_buffer[4] << 8 | data_buffer[5];
        remaining_capacity = data_buffer[6] << 8 | data_buffer[7];
        vol_index = vol_raw * 0.01;
        // Serial.print(vol_raw);
        data_index = 0;
        read_index = 0;
    }

    if (read_index == 1)
    {
        if (inChar == 0x03)
        {
            read_index = 2;
        }
        else
        {
            read_index = 0;
        }
    }
    if (inChar == 0xDD && read_index == 0)
    {
        read_index = 1;
    }
}

