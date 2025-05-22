//
// Created by bloom on 18.05.2025.
//

#include "LightSensor.h"

void LightSensor::initComponent() {
    if (!_lightSensor.begin()) {
        Serial.println("VEML not found");
        while (1);
    }
    Serial.println("VEML was found");
    _lightSensor.enable(true);

    _lightSensor.setGain(VEML7700_GAIN_1);
    _lightSensor.setIntegrationTime(VEML7700_IT_100MS);

    _lightSensor.interruptEnable(true);
    _lightSensor.setLowThreshold(800);
    _lightSensor.setHighThreshold(10000);
    _isCompActive = true;
    Serial.println("VEML initialized");
}

void LightSensor::calibrateComponent() {

}

void LightSensor::goSleepMode() {
    if (_isCompActive == true) {
        _lightSensor.enable(false);
        _isCompActive = false;
        Serial.println("VEML put to sleep mode");
    }
}

bool LightSensor::isActive() {
    if (_isCompActive == true) {
        Serial.println("VEML currently is initialized");
        return true;
    }
    Serial.println("VEML currently is not initialized");
    return false;
}

bool LightSensor::withinLimits() {
    return true;
}

float LightSensor::readLight() {
    float lux_val = _lightSensor.readLux(VEML_LUX_CORRECTED);
    _lightValues.push_back(lux_val);

    //debugging purposes
    for (auto &current: _lightValues) {
        Serial.println("Values from lux array: ");
        Serial.print(current);
    }
    return lux_val;
}

void LightSensor::wakeUp() {
    _lightSensor.enable(true);
    _isCompActive = true;
    Serial.println("VEML woken up");
}





