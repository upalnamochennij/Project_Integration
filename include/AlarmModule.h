//
// Created by bloom on 21.05.2025.
//
#ifndef ALARMMODULE_H
#define ALARMMODULE_H
#include "../include/AlarmModule.h"
#include <memory>
#include "../include/Mpu6050_Integration.h"
#include "../include/HeartrateSensor.h"
#include "../include/LightSensor.h"
#include "../include/TemperatureSensor.h"
#include "../include/OLEDscreen.h"


class AlarmModule {
public:
    AlarmModule(HeartrateSensor &heartrate,
                TemperatureSensor &temperature,
                Mpu6050_Integration &mpu6050,
                OLEDscreen &oledScreen);

    ~AlarmModule();

    void alert();

private:
    OLEDscreen *oledDisplay;
    HeartrateSensor *heartrate;
    TemperatureSensor *temperature;
    Mpu6050_Integration *mpu6050;
};


#endif //ALARMMODULE_H
