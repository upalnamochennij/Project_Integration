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

    //эти два мб не будут использоваться
    alertEnum checkAlertType(SensorDataParsing &globalData);
    void selectAlert(const alertEnum &alertType);

    bool checkForFall(SensorDataParsing &globalData);

private:
    const float FALL_THRESHOLD = 0.9; // minimal acceleration threshold if falling; было 0.3 как нормальное значение,но для теста поставлю 0.9
    OLEDscreen *oledDisplay;
};


#endif //ALARMMODULE_H
