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
    _heartSensor.setup(ledBrightness,sampleAverage,
        ledMode,sampleRate, pulseWidth, adcRange); //Configure sensor.
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

void HeartrateSensor::readData() { //actually reads both bpm and heartrate
    for (char i = 0; i < bufferLength; i++) {
        while (!_heartSensor.available()) {
            _heartSensor.check();
        }

        RBuffer[i] = _heartSensor.getRed();
        IRBuffer[i] = _heartSensor.getIR();
        _heartSensor.nextSample();
    }
    maxim_heart_rate_and_oxygen_saturation(IRBuffer,bufferLength,RBuffer,
        &_sp02_value,&_sp02_valid,
        &_heartRate,&_heartRateValid);

    while (1) {
        for (auto i = 25; i < 100; i++) {
            RBuffer[i - 25] = RBuffer[i];
            IRBuffer[i - 25] = IRBuffer[i];
        }

        for (int i = 75; i < 100; i++) {
            while (_heartSensor.available() == false) //do we have new data?
                _heartSensor.check(); //Check the sensor for new data

            RBuffer[i] = _heartSensor.getRed();
            IRBuffer[i] = _heartSensor.getIR();
            _heartSensor.nextSample();
        }

        maxim_heart_rate_and_oxygen_saturation(IRBuffer,bufferLength,RBuffer,
        &_sp02_value,&_sp02_valid,
        &_heartRate,&_heartRateValid);

        Serial.println("BPM: ");
        Serial.print(_heartRate);
        Serial.println("\n");
        Serial.print("SPO2: ");
        Serial.print(_sp02_value);
        Serial.println("\n");
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






