//
// Created by bloom on 18.05.2025.
//
#include <Adafruit_MPU6050.h>
#include "IComponent.h"
#include <vector>
#include <time.h>

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
    void wakeUp() override;

    sensors_event_t readAccel(); //returns an object, that contains accelo and gyro data in its' fields.
    sensors_event_t readGyro();

    sensors_event_t _accelValue,_gyroValue,_tempValue;
    std::vector<sensors_event_t> _accelArray;
    std::vector<sensors_event_t> _gyroArray;
    time_t timestampAccel;
    time_t timestampGyro;

    private:
    Adafruit_MPU6050 _mpu;
    //std::vector<sensors_event_t> temp_values;
    bool _isCompActive = false;
};

#endif //MPU6050_INTEGRATION_H
