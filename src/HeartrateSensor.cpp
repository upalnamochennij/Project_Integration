#include "HeartrateSensor.h"
#undef I2C_BUFFER_LENGTH
#include "heartRate.h"
#include "MAX30105.h"
#include "spo2_algorithm.h"

extern SemaphoreHandle_t mutex;

void HeartrateSensor::initComponent() {
    if (_heartSensor.begin(Wire, I2C_SPEED_FAST) == false) {
        Serial.println("MAX30102 was not found. Please check wiring/power. \n");
        while (true);
    }
    _heartSensor.setup(ledBrightness, sampleAverage,
                       ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor.
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

void HeartrateSensor::readData() {
    bool collected = false;
    for (byte i = 0; i < bufferLength; i++) {
        while (_heartSensor.available() == false) //do we have new data?
            _heartSensor.check(); //Check the sensor for new data

        RBuffer[i] = _heartSensor.getRed();
        IRBuffer[i] = _heartSensor.getIR();
        _heartSensor.nextSample(); //We're finished with this sample so move to next sample

        Serial.print(F("red="));
        Serial.print(RBuffer[i], DEC);
        Serial.print(F(", ir="));
        Serial.println(IRBuffer[i], DEC);
    }

    maxim_heart_rate_and_oxygen_saturation(IRBuffer, bufferLength, RBuffer, &_sp02_value, &_sp02_valid, &_heartRate,
                                           &_heartRateValid);
    //Continuously taking samples from MAX30102.  Heart rate and SpO2 are calculated every 1 second
    while (collected == false) {
        for (byte i = 25; i < 100; i++) {
            RBuffer[i - 25] = RBuffer[i];
            IRBuffer[i - 25] = IRBuffer[i];
        }

        //25 new values
        for (byte i = 75; i < 100; i++) {
            while (_heartSensor.available() == false)
                _heartSensor.check(); //check for new data

            RBuffer[i] = _heartSensor.getRed();
            IRBuffer[i] = _heartSensor.getIR();
            _heartSensor.nextSample(); //next sample

            Serial.print(F("red="));
            Serial.print(RBuffer[i], DEC);
            Serial.print(F(", ir="));
            Serial.print(IRBuffer[i], DEC);

            Serial.print(F(", HR="));
            Serial.print(_heartRate, DEC);

            Serial.print(F(", HRvalid="));
            Serial.print(_heartRateValid, DEC);

            Serial.print(F(", SPO2="));
            Serial.print(_sp02_value, DEC);

            Serial.print(F(", SPO2Valid="));
            Serial.println(_sp02_valid, DEC);
        }

        //After gathering 25 new samples recalculate HR and SP02
        maxim_heart_rate_and_oxygen_saturation(IRBuffer, bufferLength, RBuffer, &_sp02_value,
                                               &_sp02_valid, &_heartRate, &_heartRateValid);
        collected = true;
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
