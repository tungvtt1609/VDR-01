/**
 ******************************************************************************
 * @file    motor.cpp
 * @author  Robot Team
 * @version V1.0.0
 * @date    27-06-2022
 * @brief
 ******************************************************************************
 */

/* #include */
#include "motor.h"
#include <Arduino.h>
#include "FlexCAN_T4.h"

extern uint8_t _running_state;

extern float g_req_linear_vel_x;
extern float g_req_linear_vel_y;
extern float g_req_linear_vel_z;
extern int32_t velocity_L, velocity_R;

extern uint8_t _sensor_state;

FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> can1;
CAN_message_t msg;

void main_motor(void)
{
  setup_motor();

  // if(can1.available() < 0)
  // {
  //   Motor_disable();
  // }

  while (1)
  {
    int32_t right_ve, left_ve;

    right_ve = 0 -get_rpm_right();
    left_ve = get_rpm_left();

    Write_Velocity_rpm(Right_Wheel_ID, (int32_t)right_ve);
    Write_Velocity_rpm(Left_Wheel_ID, (int32_t)left_ve);
    
    velocity_R = Read_Velocity_rpm(Right_Wheel_ID);
    velocity_L = Read_Velocity_rpm(Left_Wheel_ID);
    
    threads.yield();
  }
}

void setup_motor(void)
{
  // 1. Init CAN and Baudrate
  can1.begin();
  can1.setBaudRate(1000000);
  // 2. set thanh controllabe status: send: 000 01 02
  CANopen_Activate(Left_Wheel_ID);
  CANopen_Activate(Right_Wheel_ID);
  // 3.1. set mode van toc: send: 602 2F 60 60 00 03 00 00 00
  CANopen_Mode_SET(Left_Wheel_ID, PV_Mode);
  CANopen_Mode_SET(Right_Wheel_ID, PV_Mode);
  // 3.2. set thoi gian thay doi toc do tu 0rpm den 3000rpm (tang toc 6083h, giam toc 6084h), don vi ms,
  // set 6083h = 1s - send: 602 23 83 60 00 E8 03 00 00
  // set 6084h = 1s - send: 602 23 84 60 00 E8 03 00 00
  SDO_Write_OD(Left_Wheel_ID, SDO_W4, 0x6083, 0x00, 0x000013880);
  SDO_Write_OD(Left_Wheel_ID, SDO_W4, 0x6084, 0x00, 0x0000C350);
  SDO_Write_OD(Right_Wheel_ID, SDO_W4, 0x6083, 0x00, 0x000013880);
  SDO_Write_OD(Right_Wheel_ID, SDO_W4, 0x6084, 0x00, 0x0000C350);
  // 3.3. Set toc do muc tieu: 60FFh,don vi 0,1rpm
  // set 60FFh = 10rpm - send: 602 23 FF 60 00 64 00 00 00
  SDO_Write_OD(Left_Wheel_ID, SDO_W4, 0x60FF, 0x00, 0x00000000);
  SDO_Write_OD(Right_Wheel_ID, SDO_W4, 0x60FF, 0x00, 0x00000000);
  // 4 enable
  Motor_enable();
  
}

void CANopen_Activate(uint8_t CANopen_ID)
{
  msg.buf[0] = 0x01;
  msg.buf[1] = CANopen_ID;
  msg.id = 0x000;
  msg.len = 2;
  can1.write(msg);
  threads.delay(5);
}

void CANopen_Mode_SET(uint8_t CANopen_ID, uint8_t CANopen_mode)
{
  msg.buf[0] = SDO_W1;
  msg.buf[1] = 0x60;
  msg.buf[2] = 0x60;
  msg.buf[3] = 0x00;
  msg.buf[4] = CANopen_mode;
  msg.buf[5] = 0x00;
  msg.buf[6] = 0x00;
  msg.buf[7] = 0x00;
  msg.id = 0x600 + CANopen_ID;
  msg.len = 8;
  can1.write(msg);
  threads.delay(5);
}

void SDO_Write_OD(uint8_t CANopen_ID, uint8_t CMD, uint16_t Index, uint8_t SubIndex, uint32_t DATA)
{
  msg.buf[0] = CMD;
  msg.buf[1] = (uint8_t)(Index & 0xFF);
  msg.buf[2] = (uint8_t)(Index >> 8 & 0xFF);
  msg.buf[3] = SubIndex;
  msg.buf[4] = (uint8_t)(DATA & 0xFF);
  msg.buf[5] = (uint8_t)(DATA >> 8 & 0xFF);
  msg.buf[6] = (uint8_t)(DATA >> 16 & 0xFF);
  msg.buf[7] = (uint8_t)(DATA >> 24 & 0xFF);
  msg.id = 0x600 + CANopen_ID;
  msg.len = 8;
  can1.write(msg);
  threads.delay(5);
}

void Motor_enable(void)
{
  // 4. set controlword 6040h de enable driver:
  // Lan dau enable phai ghi lan luot 0x06, 0x07, 0x0F.
  // set 6040h = 6 - chuan bi - send: 602 2B 40 60 00 06 00 00 00
  SDO_Write_OD(Left_Wheel_ID, SDO_W4, 0x6040, 0x00, 0x06);
  SDO_Write_OD(Right_Wheel_ID, SDO_W4, 0x6040, 0x00, 0x06);
  // set 6040h = 7 - Disable - send: 602 2B 40 60 00 06 00 00 00
  SDO_Write_OD(Left_Wheel_ID, SDO_W4, 0x6040, 0x00, 0x07);
  SDO_Write_OD(Right_Wheel_ID, SDO_W4, 0x6040, 0x00, 0x07);
  // set 6040h = F - Enable - send: 602 2B 40 60 00 06 00 00 00
  SDO_Write_OD(Left_Wheel_ID, SDO_W4, 0x6040, 0x00, 0x0F);
  SDO_Write_OD(Right_Wheel_ID, SDO_W4, 0x6040, 0x00, 0x0F);
  // Dong co chay voi toc do 60FFh sau khi enable
}

void Motor_disable(void)
{
  // set 6040h = 7 - Disable - send: 602 2B 40 60 00 06 00 00 00
  SDO_Write_OD(Left_Wheel_ID, SDO_W4, 0x6040, 0x00, 0x07);
  SDO_Write_OD(Right_Wheel_ID, SDO_W4, 0x6040, 0x00, 0x07);
}

void Write_Velocity_rpm(uint8_t CANopen_ID, uint32_t DATA)
{
  DATA = DATA * 10;
  if (DATA >= 0)
  {
    SDO_Write_OD(CANopen_ID, SDO_W4, 0x60FF, 0x00, DATA);
  }
  else
  {
    SDO_Write_OD(CANopen_ID, SDO_W4, 0x60FF, 0x00, ~DATA + 1);
  }
}

int32_t Read_Velocity_rpm(uint8_t CANopen_ID)
{
  uint32_t v_data;
  CAN_message_t fb_msg;
  CAN_message_t v_msg;
  SDO_Write_OD(CANopen_ID, SDO_RD, 0x606C, 0x00, 0);
  can1.read(fb_msg);
  if ((fb_msg.buf[1] == 0x6C) & (fb_msg.id == (uint32_t)(0x580 + CANopen_ID)))
  {
    v_msg = fb_msg;
  }
  v_data = v_msg.buf[4] | (v_msg.buf[5] << 8) | (v_msg.buf[6] << 16) | (v_msg.buf[7] << 24);
  return (int32_t)v_data;
}

float get_rpm_right()
{
  float v_r, w_r_s, w_r_mins;
  v_r = g_req_linear_vel_x + (g_req_linear_vel_z * DISTANCE_WHEEL) / 2; // rad/s
  w_r_s = v_r / (DIAMETER_WHEEL / 2);
  w_r_mins = (w_r_s * 30) / 2;
  return w_r_mins;
}

float get_rpm_left()
{
  float v_l, w_l_s, w_l_mins;
  v_l = g_req_linear_vel_x - (g_req_linear_vel_z * DISTANCE_WHEEL) / 2;
  w_l_s = v_l / (DIAMETER_WHEEL / 2);
  w_l_mins = (w_l_s * 30) / 2;
  return w_l_mins;
}

