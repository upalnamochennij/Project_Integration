#include <Arduino.h>
#include "IComponent.h"
#include "Adafruit_VEML7700.h"
#include "Adafruit_BME280.h"
#include "Adafruit_MPU6050.h"
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <ios>
#include "OLEDscreen.h"
#include "HeartrateSensor.h"
#include "LightSensor.h"
#include "Mpu6050_Integration.h"
#include "AlarmModule.h"
#include "Parser.h"
#include <memory>
#include "../../../../../.platformio/packages/framework-arduinoespressif32/libraries/WiFi/src/WiFi.h"

#define BUZZER_PIN 2

const char *ssid = "ACSlab";
const char *password = "lab@ACS24";
String serverName = "https://173.212.207.55/values";

HeartrateSensor obj_heart;
OLEDscreen obj_oled;
LightSensor obj_light;
Mpu6050_Integration obj_mpu;
sensors_event_t event;
TemperatureSensor obj_temp;
// std::unique_ptr<AlarmModule> obj_alarm;
AlarmModule obj_alarm(obj_heart,obj_temp,obj_mpu,obj_oled);
Parser obj_parser(ssid, password);

void setup() {
  Serial.begin(9600);
  // obj_heart.initComponent();
  // obj_mpu.initComponent();
  // obj_oled.initComponent();
  //obj_light.initComponent();

  //rabotaet otdeljno bez problem
  // obj_oled.initComponent();
  // obj_oled.showTestBS();
  // delay(2000);
  // obj_oled.goSleepMode();
  // delay(2000);
  // obj_oled.wakeUp();

  // LightSensor obj_light;
  // obj_light.initComponent();
  // auto light_activ_status = obj_light.isActive();
  //
  // Serial.println("VEML initialization status: ");
  // Serial.println(light_activ_status ? "true" : "false");
  // delay(3000);
  //
  // obj_light.goSleepMode();
  // light_activ_status = obj_light.isActive();
  // Serial.println("VEML initialization status: ");
  // Serial.println(light_activ  _status ? "true" : "false");
  //
  // delay(3000);
  // obj_light.wakeUp();
  // light_activ_status = obj_light.isActive();
  // Serial.println("VEML initialization status: ");
  // Serial.println(light_activ_status ? "true" : "false");

  // obj_mpu._accelValue.acceleration.x = 30;
  // Serial.println(obj_mpu._accelValue.acceleration.x);
  // obj_alarm.alert();
  // delay(2000);
  //
  // obj_mpu._accelValue.acceleration.x = 20;
  // Serial.println(obj_mpu._accelValue.acceleration.x);
  // obj_alarm.alert();
  // delay(2000);
  obj_parser.connectToWifi();
}

void loop() {
  obj_parser.sendData(serverName);
  // auto acc = obj_mpu.readAccel();
  // Serial.print("Accel X: ");
  // Serial.print(acc.acceleration.x);
  // Serial.print("\n");
  // delay(200);
  // Serial.print("Accel Y: ");
  // Serial.print(acc.acceleration.y);
  // Serial.print("\n");
  // delay(200);
  // Serial.print("Accel Z: ");
  // Serial.print(acc.acceleration.z);
  // Serial.print("\n");
  // delay(1000);

  //obj_heart.readSPO2();
}
