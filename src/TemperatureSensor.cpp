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

}

void TemperatureSensor::wakeUp() {

}

bool TemperatureSensor::isActive() {
    return true;
}

float TemperatureSensor::readTemperature() {
    return 0.0;
}

bool TemperatureSensor::withinLimits() {
    return true;
}







