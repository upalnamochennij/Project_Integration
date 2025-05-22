//
// Created by bloom on 21.05.2025.
//
#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H
#include "IComponent.h"
#include <Adafruit_BME280.h>

class TemperatureSensor : public IComponent {

    public:
    TemperatureSensor() = default;
    void initComponent() override;
    void calibrateComponent() override;
    bool withinLimits() override;
    bool isActive() override;
    void goSleepMode() override;
    void wakeUp() override;

    float readTemperature();

    private:
    Adafruit_BME280 _tempSensor;
    bool _isActive = false;

};

#endif //TEMPERATURESENSOR_H
