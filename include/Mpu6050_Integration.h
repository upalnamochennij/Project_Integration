//
// Created by bloom on 18.05.2025.
//
#include <Adafruit_MPU6050.h>
#include "IComponent.h"
#include <vector>
#include <time.h>

#include "SensorDataParsing.h"

#ifndef MPU6050_INTEGRATION_H
#define MPU6050_INTEGRATION_H

class Mpu6050_Integration : public IComponent {
    public:
    Mpu6050_Integration() = default;
    void initComponent() override;

    bool withinLimits() override;
    bool isActive() override;
    void goSleepMode() override;
    void wakeUp() override;
    //bool detectFall(SensorDataParsing &data); pod voprosom
    int detectStep(sensors_event_t &Accel);

    sensors_event_t readAccel(); //returns an object, that contains accelo and gyro data in its fields.
    sensors_event_t readGyro();

    sensors_event_t _accelValue,_gyroValue,_tempValue;
    float accelThreshold = 1.2;  // Порог ускорения (g)
    unsigned long stepDelay = 300; // Минимальное время между шагами (мс)
    unsigned long lastStepTime = 0;
    int step = 0;

    private:
    Adafruit_MPU6050 _mpu;
    //std::vector<sensors_event_t> temp_values;
    bool _isCompActive = false;
};

#endif //MPU6050_INTEGRATION_H
