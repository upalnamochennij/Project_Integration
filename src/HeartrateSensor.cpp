#include "HeartrateSensor.h"
#undef I2C_BUFFER_LENGTH
#include "heartRate.h"
#include "MAX30105.h"

void HeartrateSensor::initComponent(){
    if (_heartSensor.begin(Wire, I2C_SPEED_FAST) == false) {
        Serial.println("MAX30102 was not found. Please check wiring/power. ");
        while (true);
    }
    //The LEDs are very low power and won't affect the temp reading much but
    //you may want to turn off the LEDs to avoid any local heating
    _heartSensor.setup(); //Configure sensor. Turn off LEDs
    _heartSensor.enableDIETEMPRDY(); //Enable the temp ready interrupt. This is required

    _heartSensor.setPulseAmplitudeRed(0x0A);
    _heartSensor.setPulseAmplitudeGreen(0);

    _isCompActive = true;
    Serial.println("MAX30102 initialized");
}

void HeartrateSensor::goSleepMode() {
    _heartSensor.shutDown();
    _isCompActive = false;
    Serial.println("MAX30102 shutting down");
}

void HeartrateSensor::wakeUp() {
    _heartSensor.wakeUp();
    Serial.println("MAX30102 waking up");
    _isCompActive = true;
}

void HeartrateSensor::readSPO2() {
    //_heartSensor.setLEDMode(1); //1 for red
    _sp02Data.push_back(_heartSensor.getRed());

    for (const uint32_t &data : _sp02Data) {
        Serial.print("Max30102 sp02 reading: " + data);
    }
}

void HeartrateSensor::readPulse() {
    while (1) {
        int lastbeat = 0;
        long delta = 0;
        double bpm = 0;
        //_heartSensor.setLEDMode(0);
        auto RawValue = _heartSensor.getIR();

        if (checkForBeat(RawValue) == true) {
            // check for beat kakogo to huja daet false
            //lastbeat = millis();
            delta = millis() - lastbeat;
            bpm = 60 / (delta / 1000.0);

            Serial.print("bpm made from Max30102 raw reading: ");
            Serial.println(bpm);

            _pulseData.push_back(RawValue);

            //debugging purposes
            for (const auto &data: _pulseData) {
                Serial.print("Max30102 pulse reading: \n");
                Serial.println(data);
            }
        } else {
            Serial.println("No beat detected");
        }

        delay(1000);
    }
}

bool HeartrateSensor::isActive() {
    if (_isCompActive == true) {
        return true;
    }
    return false;
}

bool HeartrateSensor::withinLimits() {
    return true;
}

void HeartrateSensor::calibrateComponent() {

}






