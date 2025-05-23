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
#include <memory>

#define buzzerPin 2

HeartrateSensor obj_heart;
OLEDscreen obj_oled;
LightSensor obj_light;
Mpu6050_Integration obj_mpu;

void setup() {
  Serial.begin(9600);

  //obj_heart.initComponent();
  obj_mpu.initComponent();
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
  //obj_heart.readSPO2();
  //obj_mpu.readData();
  auto acc = obj_mpu.readAccel();
  Serial.print("Accel X: ");
  Serial.print(acc.acceleration.x);
  Serial.print("\n");
  delay(1000);
}
