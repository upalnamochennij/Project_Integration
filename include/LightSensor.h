//
// Created by bloom on 18.05.2025.
//
#include "IComponent.h"
#include <vector>
#include <Adafruit_VEML7700.h>

#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

class LightSensor : public IComponent {

    public:
    LightSensor() = default;
    void initComponent() override; //initialize VEML7700 for the first time
    void calibrateComponent() override;
    bool withinLimits() override;
    bool isActive() override; // checks if VEML7700 is currently active
    void goSleepMode() override; // put VEML7700 into sleep mode (configurations and settings still remained)
    float readLight(); //reads LUX values from VEML7700
    void wakeUp(); // wakes VEML7700 from temporary sleep (configurations and settings still saved)

    private:
    Adafruit_VEML7700 lightSensor;
    std::vector<float> _lightValues;
    bool _isCompActive = false;
};

#endif //LIGHTSENSOR_H
