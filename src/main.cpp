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
extern uint8_t battery_percent = 0;     // % pin
float g_req_linear_vel_x = 0;    // vx nhan duoc tu Jetson
float g_req_linear_vel_y = 0;    // vy nhan duoc tu Jetson
float g_req_linear_vel_z = 0;    // w nhan duoc tu Jetson
int32_t velocity_L, velocity_R;  // Khai bao de in ra, ti nua k dung thi xoa
int voltage;
extern int vol_raw;

void setup()
{
  threads.addThread(setup_rosserial);
  threads.addThread(main_motor);
  threads.addThread(main_sensor);
  threads.addThread(main_charger);
  threads.addThread(main_led);
  threads.addThread(main_charger);
}

void loop()
{
  main_charger();
  Serial.print(vol_raw);
  // Serial.print("banh trai: ");
  // Serial.print(velocity_L);
  // Serial.print(" - banh phai: ");
  // Serial.print(velocity_R);
  // Serial.println();
  delay(300);

  // threads.delay(150);
  // threads.yield();
}