#include <Arduino.h>
#include "TeensyThreads.h"
#include "motor.h"
#include "charge.h"
#include "led.h"
#include "rosserial.h"
#include "sensor.h"


uint8_t _button_state = 0;       // trang thai nut nhan gan nhat
uint8_t _motion_state = 0;       // trang thai chuyen dong
uint8_t _sensor_state = 0;       // trang thai nhan biet co vat can hay k
uint8_t _charging_state = 0;     // trang thai pin hoac dang sac
uint8_t _running_state = 0;      // trang thai Robot dang dung hoac dang chay
uint8_t proxi_sensor[2] = {};    // gia tri cam bien
extern int battery_percent = 0;     // % pin
float g_req_linear_vel_x = 0;    // vx nhan duoc tu Jetson
float g_req_linear_vel_y = 0;    // vy nhan duoc tu Jetson
float g_req_linear_vel_z = 0;    // w nhan duoc tu Jetson
int32_t velocity_L, velocity_R;  // Khai bao de in ra, ti nua k dung thi xoa
int voltage;
extern int vol_raw;

ros::NodeHandle nh;

ros::Subscriber<geometry_msgs::Twist> cmd_sub("cmd_vel", commandCallback);

void setup()
{
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  delay(3000);
  // threads.addThread(setup_rosserial);
  nh.initNode();
  nh.getHardware()->setBaud(57600);
  nh.subscribe(cmd_sub);

  threads.addThread(main_motor);

  threads.addThread(main_sensor); // _sensor_state = ULTRA_SHORT_DISTAN;

  threads.addThread(main_charger);
  _charging_state = NORMAL_BATTERY;
  threads.addThread(main_led);
}

void loop()
{
  nh.spinOnce();
}