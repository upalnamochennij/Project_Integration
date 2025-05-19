//
// Created by bloom on 18.05.2025.
//
#include <Adafruit_MPU6050.h>
#include "IComponent.h"
#include <vector>

#ifndef MPU6050_INTEGRATION_H
#define MPU6050_INTEGRATION_H

class Mpu6050_Integration : public IComponent {

public:
    Mpu6050_Integration() = default;
    void initComponent() override;
    void calibrateComponent() override;
    bool withinLimits() override;
    bool isActive() override;
    void goSleepMode() override;
    void wakeUp();
    void readData();

private:
    Adafruit_MPU6050 _mpu;
    sensors_event_t accel_reading,gyro_reading,temp_reading;
    std::vector<sensors_event_t> accel_values;
    std::vector<sensors_event_t> gyro_values;
    std::vector<sensors_event_t> temp_values;
    bool _isCompActive = false;
};

#endif //MPU6050_INTEGRATION_H
