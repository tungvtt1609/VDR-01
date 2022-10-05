/**
 ******************************************************************************
 * @file    rosserial.cpp
 * @author  Robot Team
 * @version V1.0.0
 * @date    27-06-2022
 * @brief
 ******************************************************************************
 */

/* #include */
#include "rosserial.h"
#include <Arduino.h>
#include "charge.h"


extern float g_req_linear_vel_x;
extern float g_req_linear_vel_y;
extern float g_req_linear_vel_z;

extern int vol_raw;
extern uint8_t battery_percent;
extern uint8_t _charging_state;

void BatteryStatePublisher(const sensor_msgs::BatteryState &bst_msg)
{
    battery_percent = bst_msg.percentage;

    if(battery_percent >= 95){
        _charging_state = bst_msg.POWER_SUPPLY_STATUS_FULL;
        vol_raw = bst_msg.voltage;
    }
    else{
        _charging_state = bst_msg.POWER_SUPPLY_STATUS_CHARGING;
        vol_raw = bst_msg.voltage;
    }  
}

void commandCallback(const geometry_msgs::Twist &cmd_msg)
{
    g_req_linear_vel_x = cmd_msg.linear.x;
    g_req_linear_vel_y = cmd_msg.linear.y;
    g_req_linear_vel_z = cmd_msg.angular.z;
}
void setup_rosserial(void)
{

}
