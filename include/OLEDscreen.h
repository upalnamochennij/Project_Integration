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
    void initComponent() override; // first initialization of component
    bool isActive() override; // checks whether OLED is currently active
    void goSleepMode() override; //put oled to a sleep mode (all the settings are saved)
    void wakeUp(); // wake oled from sleep (all the settings are saved)
    void showTestBS(); // function to test input on oled
    void calibrateComponent() override;
    bool withinLimits() override;

    private:
    Adafruit_SSD1306 oled_display;
    bool _isActive = false;
};

#endif //OLEDSCREEN_H
