#include "HeartrateSensor.h"
#undef I2C_BUFFER_LENGTH
#include "heartRate.h"
#include "MAX30105.h"
#include "spo2_algorithm.h"

void HeartrateSensor::initComponent(){
    if (_heartSensor.begin(Wire, I2C_SPEED_FAST) == false) {
        Serial.println("MAX30102 was not found. Please check wiring/power. \n");
        while (true);
    }
    //The LEDs are very low power and won't affect the temp reading much but
    //you may want to turn off the LEDs to avoid any local heating
    _heartSensor.setup(_ledBrightness, _sampleAverage, _ledMode, _sampleRate, _pulseWidth, _adcRange); //Configure sensor.
    //_heartSensor.enableDIETEMPRDY(); //Enable the temp ready interrupt. This is required

    _heartSensor.setPulseAmplitudeRed(0x0A);
    _heartSensor.setPulseAmplitudeGreen(0);
    _isCompActive = true;
    Serial.println("MAX30102 initialized\n");
}

void HeartrateSensor::goSleepMode() {
    _heartSensor.shutDown();
    _isCompActive = false;
    Serial.println("MAX30102 shutting down\n");
}

void HeartrateSensor::wakeUp() {
    _heartSensor.wakeUp();
    Serial.println("MAX30102 waking up\n");
    _isCompActive = true;
}

uint32_t HeartrateSensor::readSPO2() {
    uint32_t RawValue = _heartSensor.getRed();
    _sp02Data.push_back(RawValue);

    for (const uint32_t &data : _sp02Data) {
        Serial.print("Max30102 sp02 reading: ");
    }
    return RawValue;
}

double HeartrateSensor::readBPM() {
    long RawValue = _heartSensor.getIR();

    if (checkForBeat(RawValue) == true) {
        // check for beat kakogo to huja daet false
        long delta = millis() - lastbeat;
        lastbeat = millis();
        bpm = 60 / (delta / 1000.0);

        //_pulseData.push_back(bpm);
        //debugging purposes
        // for (const auto &data: _pulseData) {
        //     Serial.print("Max30102 pulse bpm: \n");
        //     Serial.println(data);
        // }
    }
    Serial.print("IR=");
    Serial.print(RawValue);
    Serial.print(", BPM=");
    Serial.print(bpm);
    Serial.println("\n");

    return bpm;
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






