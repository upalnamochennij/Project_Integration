#pragma once
#include <vector>
#include "IComponent.h"
#include "MAX30105.h"

class HeartrateSensor : public IComponent{

    public:
    void initComponent() override;
    bool withinLimits() override;
    bool isActive() override;
    void goSleepMode() override;
    void wakeUp() override;

    void readData();

    // for maxim_heart_rate_and_oxygen_saturation() method
    byte bufferLength = 100;
    uint32_t RBuffer[100] = {}; // buffer for red light values. 4 seconds
    uint32_t IRBuffer[100] = {}; // buffer for IR light values
    int32_t _sp02_value = 0;
    int8_t  _sp02_valid = false;
    int32_t _heartRate = 0;
    int8_t  _heartRateValid = false;

    private:
    MAX30105 _heartSensor;
    bool _isCompActive = false;

    byte ledBrightness = 60; //Options: 0=Off to 255=50mA
    byte sampleAverage = 2; //Options: 1, 2, 4, 8, 16, 32
    byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
    byte sampleRate = 100; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
    int pulseWidth = 411; //Options: 69, 118, 215, 411
    int adcRange = 4096; //Options: 2048, 4096, 8192, 16384
};
