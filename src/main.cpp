#include <Arduino.h>
#include "TeensyThreads.h"
#include "motor.h"
#include "charge.h"
#include "led.h"
#include "rosserial.h"
#include <std_msgs/Float32.h>
#include "sensor.h"
#include "pwm.h"


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
extern int32_t velocity_L, velocity_R;  // Khai bao de in ra, ti nua k dung thi xoa
int voltage;
extern int vol_raw;
extern float vol_index;
extern bool STATE_ROS;

std_msgs::Float32 msg_left;
std_msgs::Float32 msg_right;
std_msgs::Float32 msg_vol;
std_msgs::Float32 msg_bat;
ros::NodeHandle nh;

ros::Subscriber<geometry_msgs::Twist> cmd_sub("cmd_vel", commandCallback);
ros::Publisher pub_vel_left_feedback("cmd_feedback_left", &msg_left);
ros::Publisher pub_vel_right_feedback("cmd_feedback_right", &msg_right);
ros::Publisher pub_vol_fb("cmd_vol_fb", &msg_vol);
ros::Publisher pub_bat_fb("cmd_bat_fb", &msg_bat);
void setup()
{
  Serial.begin(9600);

  STATE_ROS = true;

  nh.initNode();
  nh.getHardware()->setBaud(57600);
  nh.subscribe(cmd_sub);

  nh.advertise(pub_vel_right_feedback);
  nh.advertise(pub_vel_left_feedback);

  nh.advertise(pub_vol_fb);
  nh.advertise(pub_bat_fb);  

  threads.addThread(main_motor);
  threads.addThread(main_sensor);
  threads.addThread(main_charger);
  threads.addThread(main_led);

}

void loop()
{
  msg_left.data = velocity_L;
  msg_right.data = velocity_R;

  msg_bat.data = battery_percent;
  msg_vol.data = vol_index;

  pub_vel_left_feedback.publish(&msg_left);
  pub_vel_right_feedback.publish(&msg_right);

  pub_vol_fb.publish(&msg_vol);
  pub_bat_fb.publish(&msg_bat);

  threads.delay(10);

  threads.addThread(main_charger);
  threads.addThread(main_motor);
  
  threads.yield();

  nh.spinOnce();
}