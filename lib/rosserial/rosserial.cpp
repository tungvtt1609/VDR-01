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
extern bool STATE_ROS;

extern int vol_raw;
extern uint8_t battery_percent;
extern uint8_t _charging_state;

void commandCallback(const geometry_msgs::Twist &cmd_msg)
{
    if (STATE_ROS == false)
    {
        g_req_linear_vel_x = 0;
        g_req_linear_vel_y = 0;
        g_req_linear_vel_z = 0;
    }
    else{
        g_req_linear_vel_x = cmd_msg.linear.x;
        g_req_linear_vel_y = cmd_msg.linear.y;
        g_req_linear_vel_z = cmd_msg.angular.z;
    }
    
}
void setup_rosserial(void)
{

}
