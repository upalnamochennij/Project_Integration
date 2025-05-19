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
    double readPulse(); //reads raw data (pulse), calculates BPM and then returns it

    private:
    MAX30105 _heartSensor;
    std::vector<double> _pulseData;
    std::vector<uint32_t> _sp02Data;
    bool _isCompActive = false;

};
