//
// Created by bloom on 21.05.2025.
//
#include "TemperatureSensor.h"

void TemperatureSensor::initComponent() {
    bool status = _tempSensor.begin();
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    Serial.println("Found a valid BME280 sensor");
    _isActive = true;
}

void TemperatureSensor::calibrateComponent() {

}

void TemperatureSensor::goSleepMode() {
    if (_isActive) {
        setMode(00);
        _isActive = false;
    }
}

void TemperatureSensor::wakeUp() {
    if (!_isActive) {
        setMode(11);
        _isActive = true;
    }
}

unsigned int TemperatureSensor::getMode() {
    return _measReg.mode;
}

void TemperatureSensor::setMode(unsigned int mode) {
    if (mode == 00 || mode == 01 || mode == 10 || mode == 11 ) {
        _measReg.mode = mode;
    }
    throw std::invalid_argument("choose correct mode: 00, 01, 10, 11");
}

bool TemperatureSensor::isActive() {
    if (!_isActive) return false;
    return true;
}

int TemperatureSensor::readTemp() {
    if (!_isActive) return false;
    return static_cast<int>(_tempSensor.readTemperature());
}

bool TemperatureSensor::withinLimits() {
    return true;
}







