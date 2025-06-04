//
// Created by bloom on 21.05.2025.
//
#include "../include/AlarmModule.h"
#include <memory>
#include "../include/OLEDscreen.h"
#include <math.h>

AlarmModule::AlarmModule(OLEDscreen &oledScreen) {
    this->oledDisplay = &oledScreen;
}

AlarmModule::~AlarmModule() = default;

AlarmModule::alertEnum AlarmModule::checkAlertType(SensorDataParsing &globalData) {
    alertEnum concreteAlert;

    auto accelZ = abs(globalData.accel_z / 16384);
    auto gyroX = abs(globalData.gyro_x / 131);

    if (accelZ < 0.4 && gyroX > 30) {
        Serial.println("Drawing fall alarm screen\n");
        concreteAlert = FALL;
    }
    else if (globalData.temperature > 37) {
        Serial.println("Drawing default alarm screen\n");
        concreteAlert = TEMPERATURE;
    }
    else if (globalData.heartrate > 120 ) {
        Serial.println("Drawing default alarm screen\n");
        concreteAlert = HEARTRATE;
    }
    else if (globalData.sp02 < 60) {
        Serial.println("Drawing default alarm screen\n");
        concreteAlert = SP02;
    }
    else concreteAlert = WITHIN_NORM;

    return concreteAlert;
}

void AlarmModule::selectAlert(const alertEnum &alertType) {
    switch (alertType) {
        case FALL:
            Serial.println("FALL");
            oledDisplay->drawAlarmFall();
        case TEMPERATURE:
            Serial.println("TEMPERATURE");
            oledDisplay->drawAlarmDefault();
        case HEARTRATE:
            Serial.println("HEARTRATE");
            oledDisplay->drawAlarmDefault();
        case SP02:
            Serial.println("SP02");
            oledDisplay->drawAlarmDefault();
        case WITHIN_NORM:
            Serial.println("WITHIN_NORM");
        default:
            Serial.println("UNKNOWN");
    }
}

