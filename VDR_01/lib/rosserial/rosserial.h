/**
 ******************************************************************************
 * @file    rosserial.h
 * @author  Robot Team
 * @version V1.0.0
 * @date    27-06-2022
 * @brief
 ******************************************************************************
 */

#ifndef _ROSSERIAL_H_
#define _ROSSERIAL_H_

#include <Arduino.h>
#include "TeensyThreads.h"
#include "ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/BatteryState.h"

void commandCallback(const geometry_msgs::Twist &cmd_msg);
void BatteryStatePublisher(const sensor_msgs::BatteryState &bst_msg);
void setup_rosserial(void);

#endif