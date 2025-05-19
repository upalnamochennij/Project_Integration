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
    void readSPO2();
    void readPulse();

    private:
    MAX30105 _heartSensor;
    std::vector<uint32_t> _pulseData;
    std::vector<uint32_t> _sp02Data;
    bool _isCompActive = false;

};
