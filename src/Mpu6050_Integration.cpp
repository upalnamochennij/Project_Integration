//
// Created by bloom on 18.05.2025.
//
#include "Mpu6050_Integration.h"
#include <math.h>


using namespace std;

void Mpu6050_Integration::initComponent() {
    if (!_mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip\n");
        while (1) {
            delay(10);
        }
    }
    Serial.println("MPU6050 Found!\n");
}

void Mpu6050_Integration::goSleepMode() {
    _mpu.enableSleep(true);
    _isCompActive = false;
    Serial.println("Mpu6050 is put into sleep mode\n");
}

void Mpu6050_Integration::wakeUp() {
    _mpu.enableSleep(false);
    _isCompActive = true;
    Serial.println("Mpu6050 woken up\n");
}

void Mpu6050_Integration::calibrateComponent() {

}

bool Mpu6050_Integration::isActive() {
    if (_isCompActive == true) {
        Serial.println("Mpu6050 is currently active\n");
        return true;
    }
    Serial.println("Mpu6050 is not active\n");
    return false;
}

void Mpu6050_Integration::readData() {
    _mpu.getEvent(&_accelValue,&_gyroValue,&_tempValue);
    Serial.print("Acceleration on x: ");
    Serial.print(_accelValue.acceleration.x);
    Serial.print(", Y: ");
    Serial.print(_accelValue.acceleration.y);
    Serial.print(", Z: ");
    Serial.print(_accelValue.acceleration.z);
    delay(1000);

    Serial.println("\n");
    Serial.print("rotation X: ");
    Serial.print(_gyroValue.gyro.x);
    Serial.print(", Y: ");
    Serial.print(_gyroValue.gyro.y);
    Serial.print(", Z: ");
    Serial.print(_gyroValue.gyro.z);
    Serial.print(" rad/s");
    Serial.println("\n");
    delay(1000);

    _accelArray.push_back(_accelValue);
    _gyroArray.push_back(_gyroValue);
}

sensors_event_t Mpu6050_Integration::readAccel() {
    _mpu.getEvent(&_accelValue,&_gyroValue,&_tempValue);
    return _accelValue;
}

sensors_event_t Mpu6050_Integration::readGyro() {
    _mpu.getEvent(&_gyroValue,&_gyroValue,&_tempValue);
    return _gyroValue;
}

bool Mpu6050_Integration::withinLimits() {
    for (sensors_event_t accelCurrent: _accelArray) {
        auto &acc = accelCurrent.acceleration;
        float total_accel = sqrt(acc.x * acc.x + acc.y * acc.y + acc.z * acc.z);
        if (total_accel < 2.0) {
            Serial.print("Falling down");
        }
    }
    for (sensors_event_t gyroCurrent: _gyroArray) {
        auto &gyro = gyroCurrent.gyro;
    }
    return true;
}





