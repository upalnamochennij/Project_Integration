//
// Created by bloom on 18.05.2025.
//
#include "Mpu6050_Integration.h"
#include <iostream>
#include <math.h>

using namespace std;
extern SemaphoreHandle_t mutex;

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

bool Mpu6050_Integration::isActive() {
    if (_isCompActive == true) {
        Serial.println("Mpu6050 is currently active\n");
        return true;
    }
    Serial.println("Mpu6050 is not active\n");
    return false;
}

void Mpu6050_Integration::getEvent() {
    _mpu.getEvent(&_accelValue, &_gyroValue, &_tempValue);
}

sensors_event_t Mpu6050_Integration::readAccel() {
    return _accelValue;
}

sensors_event_t Mpu6050_Integration::readGyro() {
    return _gyroValue;
}

bool Mpu6050_Integration::withinLimits() {
    return true;
}

int Mpu6050_Integration::detectStep(sensors_event_t &Accel) {
    // Считаем модуль ускорения (норму вектора)
    float magnitude = sqrt(Accel.acceleration.x * Accel.acceleration.x +
                           Accel.acceleration.y * Accel.acceleration.y +
                           Accel.acceleration.z * Accel.acceleration.z);
    // Вычитаем 1g (гравитацию)
    float netAccel = fabs(magnitude - 9.81);

    unsigned long currentTime = millis();

    // Если ускорение выше порога и прошло достаточно времени — считаем шаг
    if (netAccel > accelThreshold && currentTime - lastStepTime > stepDelay) {
        step = 1;
        lastStepTime = currentTime;
        Serial.print("Step detected!");
        Serial.println(step);
    } else step = 0;

    return step;
}


