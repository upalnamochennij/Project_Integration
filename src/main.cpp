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
#include "heartRate.h"
#include "HeartrateSensor.h"
#include "LightSensor.h"

#define buzzerPin 2

HeartrateSensor obj_heart;
OLEDscreen obj_oled;
LightSensor obj_light;

void setup() {
  Serial.begin(9600);

  obj_heart.initComponent();
  bool heart_init_status = obj_heart.isActive();

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

}

void loop() {
  obj_heart.readSPO2();
}
