/**
 ******************************************************************************
 * @file    motor.cpp
 * @author  Robot Team
 * @version V1.0.0
 * @date    27-06-2022
 * @brief
 ******************************************************************************
 */
#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <Arduino.h>
#include "TeensyThreads.h"

// define _running_state
#define RUNNING 1
#define STOP 2

#define Left_Wheel_ID 01
#define Right_Wheel_ID 02
// tham so cua driver
#define SDO_W1 0x2F // ask write 1 byte
#define SDO_W2 0x2B // ask write 2 byte
#define SDO_W4 0x23 // ask write 4 byte
#define SDO_RD 0x40 // ask read data
#define PP_Mode  1
#define PV_Mode 3
#define PT_Mode  4

#define DISTANCE_WHEEL 0.385 // met
#define DIAMETER_WHEEL 0.169

#define LINEAR_X 1
#define ANGULAR_Z 2

void main_motor(void);
void setup_motor(void);
void get_wheel_velocity(void);
void set_wheel_velocity(void);
void get_fb_wheel_velocity(void);
void get_fb_robot_velocity(void);
void CANopen_Activate(uint8_t CANopen_ID);
void CANopen_Mode_SET(uint8_t CANopen_ID, uint8_t CANopen_mode);
void SDO_Write_OD(uint8_t CANopen_ID, uint8_t CMD, uint16_t Index, uint8_t SubIndex, uint32_t DATA);
void Motor_enable(void);
void Motor_disable(void);
void Write_Velocity_rpm(uint8_t CANopen_ID, uint32_t DATA);
int32_t Read_Velocity_rpm(uint8_t CANopen_ID);
float get_rpm_right();
float get_rpm_left();

#endif