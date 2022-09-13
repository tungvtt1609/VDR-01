#include <Arduino.h>
#include "TeensyThreads.h"
#include "motor.h"
#include "charge.h"
#include "led.h"
#include "rosserial.h"
#include <std_msgs/UInt16.h>
#include "sensor.h"


uint8_t _button_state = 0;       // trang thai nut nhan gan nhat
uint8_t _motion_state = 0;       // trang thai chuyen dong
uint8_t _sensor_state = 0;       // trang thai nhan biet co vat can hay k
uint8_t _charging_state = 0;     // trang thai pin hoac dang sac
uint8_t _running_state = 0;      // trang thai Robot dang dung hoac dang chay
uint8_t proxi_sensor[2] = {};    // gia tri cam bien
uint8_t battery_percent = 0;     // % pin
float g_req_linear_vel_x = 0;    // vx nhan duoc tu Jetson
float g_req_linear_vel_y = 0;    // vy nhan duoc tu Jetson
float g_req_linear_vel_z = 0;    // w nhan duoc tu Jetson
int32_t velocity_L, velocity_R;  // Khai bao de in ra, ti nua k dung thi xoa
int voltage;
extern int vol_raw;


std_msgs::UInt16 msg_left;
std_msgs::UInt16 msg_right;
ros::NodeHandle nh;

ros::Subscriber<geometry_msgs::Twist> cmd_sub("cmd_vel", commandCallback);
//ros::Publisher pub_vel_left_feedback("cmd_feedback_left", &msg_left, 100);
//ros::Publisher pub_vel_right_feedback("cmd_feedback_right", &msg_right, 100);
void setup()
{
  Serial.begin(9600);
  threads.addThread(main_charger);
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  delay(3000);
 
  nh.initNode();
  nh.getHardware()->setBaud(57600);
  nh.subscribe(cmd_sub);
  //nh.advertise(pub_vel_right_feedback);
  //nh.advertise(pub_vel_left_feedback);
  threads.addThread(main_motor);
  threads.addThread(main_sensor);
  threads.addThread(main_charger);
  
  _charging_state = NORMAL_BATTERY;

  threads.addThread(main_led);
}

void loop()
{
  msg_left.data = velocity_L;
  msg_right.data = velocity_R;
  //pub_vel_left_feedback.publish(&msg_left);
  //threads
  //pub_vel_right_feedback.publish(&msg_right);
  nh.spinOnce();
  threads.addThread(main_charger);
}