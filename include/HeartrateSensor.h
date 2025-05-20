#pragma once
#include <vector>
#include "IComponent.h"
#include "MAX30105.h"

class HeartrateSensor : public IComponent{
    public:
    void calibrateComponent() override;
    void initComponent() override;
    //bool componentStatus() override;
    bool withinLimits() override;
    bool isActive() override;
    void goSleepMode() override;
    void wakeUp();

    uint32_t readSPO2();
    double readBPM(); //reads raw data (pulse), calculates BPM and then returns it

    private:
    MAX30105 _heartSensor;
    std::vector<double> _pulseData;
    std::vector<uint32_t> _sp02Data;
    bool _isCompActive = false;

    long lastbeat = 0; //maybe useless
    float bpm = 0; // maybee useless

    char bufferLength[100] = {};
    char RBuffer[100] = {}; // buffer for red light values. 4 seconds
    char IRBuffer[100] = {}; // buffer for IR light values

    byte _ledBrightness = 0;
    byte _sampleAverage = 4;
    byte _ledMode = 2;
    byte _sampleRate = 100;
    byte _pulseWidth = 411;
    int _adcRange = 2048;
};
