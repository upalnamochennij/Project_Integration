//
// Created by bloom on 18.05.2025.
//
#include "Mpu6050_Integration.h"

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
    _mpu.getEvent(&accel_reading,&gyro_reading,&temp_reading);
    accel_values.push_back(accel_reading);
    gyro_values.push_back(gyro_reading);
}

bool Mpu6050_Integration::withinLimits() {
    return true;
}





