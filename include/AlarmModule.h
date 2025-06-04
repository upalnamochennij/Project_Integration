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
    enum alertEnum {
        FALL = 1,
        TEMPERATURE = 2,
        SP02 = 3,
        HEARTRATE = 4,
        WITHIN_NORM = 5
    };

    AlarmModule(OLEDscreen &oledScreen);
    ~AlarmModule();

    alertEnum checkAlertType(SensorDataParsing &globalData); //берет данные из dataForOLED и в разных случаях чет выводит
    void selectAlert(const alertEnum& alertType);

private:
    OLEDscreen *oledDisplay;
};


#endif //ALARMMODULE_H
