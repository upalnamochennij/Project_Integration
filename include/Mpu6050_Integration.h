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
    void wakeUp() override;

    void readData();
    sensors_event_t readAccel(); //returns accelaration with x,y,z options values
    sensors_event_t readGyro();

    private:
    Adafruit_MPU6050 _mpu;
    sensors_event_t _accelValue,_gyroValue,_tempValue;
    std::vector<sensors_event_t> _accelArray;
    std::vector<sensors_event_t> _gyroArray;
    //std::vector<sensors_event_t> temp_values;
    bool _isCompActive = false;
};

#endif //MPU6050_INTEGRATION_H
