//
// Created by bloom on 21.05.2025.
//
#include "../include/AlarmModule.h"
#include <memory>
#include "../include/Mpu6050_Integration.h"
#include "../include/HeartrateSensor.h"
#include "../include/LightSensor.h"
#include "../include/TemperatureSensor.h"
#include "../include/OLEDscreen.h"

AlarmModule::AlarmModule(HeartrateSensor &heartrate,
                         TemperatureSensor &temperature,
                         Mpu6050_Integration &mpu6050,
                         OLEDscreen &oledScreen) {
    this->heartrate = &heartrate;
    this->temperature = &temperature;
    this->mpu6050 = &mpu6050;
    this->oledDisplay = &oledScreen;
}

AlarmModule::~AlarmModule() = default;

void AlarmModule::alert() {
    if (mpu6050->_accelValue.acceleration.x == 30) {
        oledDisplay->showTestBS();
    }
    else if (mpu6050->_accelValue.acceleration.x == 20) {
        oledDisplay->showTestBSver();
    }
}
