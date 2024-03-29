/**
 ******************************************************************************
 * @file    charge.cpp
 * @author  parkhoon1609
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

char read_basic_info[7] = {0xDD, 0xA5, 0x03, 0x00, 0xFF, 0xFD, 0x77};
char read_cell_vol[7] = {0xDD, 0xA5, 0x04, 0x00, 0xFF, 0xFC, 0x77};
char read_hardware_ver[7] = {0xDD, 0xA5, 0x05, 0x00, 0xFF, 0xFB, 0x77};

volatile int read_index = 0;
volatile int8_t data_index;
char data_buffer[30];
volatile int16_t vol_raw, cur_raw, remaining_capacity, norminal_capacity;
float vol_index = 0;
float cur_index = 0;


void Init_charge(void)
{
    Serial.begin(57600);
    Serial1.begin(9600);  // Serial1 (0, 1) cho VDR-01
    // Serial2.begin(9600); //Serial2 (7, 8) cho VOR-01
}

void main_charger(void)
{
    Init_charge();
    for (int i = 0; i < 7; i++)
    {
        Serial1.print(read_basic_info[i]);
    }
    delay(500);
    for (int i = 0; i < 7; i++)
    {
        Serial1.print(read_cell_vol[i]);
    }
    delay(500);
    for (int i = 0; i < 7; i++)
    {
        Serial1.print(read_hardware_ver[i]);
    }
    delay(500);


    while (1)
    {
        // get battery percent
        read_charger();
        // get_battery_voltage();
        if (_charging_state == CHARGING)
        {
            if (millis() - prev_charging_time >= 60000)
            {
                prev_charging_time = millis();
                threads.delay(10);
                battery_percent = map(vol_index, MIN_VOLTAGE, MAX_VOLTAGE, 0, 100);
                // get_percent();
                threads.delay(10);
            }
        }
        else
        {
            battery_percent = map(vol_index, MIN_VOLTAGE, MAX_VOLTAGE, 0, 100);
            // Serial.println(battery_percent);
            // get_percent();
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
    char inChar = (char)Serial1.read();
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
        norminal_capacity = data_buffer[8] << 8 | data_buffer[9];
        // get_battery_voltage();
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

// float get_battery_voltage(void){
//     vol_index = vol_raw/100.0f;
//     // Serial.print(vol_index);
//     return vol_index;
// }

float get_battery_current(void){
    cur_index = cur_raw/100.0f;
    return cur_index; 
}

// float get_percent(void){
//     battery_percent = (remaining_capacity * 1.0f / norminal_capacity) * 100.0f;
//     return battery_percent;
// }

