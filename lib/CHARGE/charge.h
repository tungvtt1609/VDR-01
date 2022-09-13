/**
 ******************************************************************************
 * @file    charge.h
 * @author  Robot Team
 * @version V1.0.0
 * @date    27-06-2022
 * @brief
 ******************************************************************************
 */
#ifndef _CHARGE_H_
#define _CHARGE_H_

#include <Arduino.h>
#include "TeensyThreads.h"



// state
#define FULL_BATTERY 1
#define LOW_BATTERY 2
#define CHARGING 3
#define NORMAL_BATTERY 4

// data
#define MAX_VOLTAGE 3.65 * 8
#define MIN_VOLTAGE 2 * 8

void main_charger(void);
void Init_charge(void);
void read_charger(void);

#endif