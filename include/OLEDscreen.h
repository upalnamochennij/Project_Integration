//
// Created by bloom on 18.05.2025.
//
#ifndef OLEDSCREEN_H
#define OLEDSCREEN_H

#include "IComponent.h"
#include <vector>
#include <Adafruit_SSD1306.h>

class OLEDscreen : public IComponent{
    public:
    OLEDscreen();
    void initComponent() override;
    bool isActive() override;
    void goSleepMode() override;
    void wakeUp();
    void showTestBS();
    void calibrateComponent() override;
    bool withinLimits() override;

    private:
    Adafruit_SSD1306 oled_display;
    bool _isActive = false;
};

#endif //OLEDSCREEN_H
