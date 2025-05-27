//
// Created by bloom on 18.05.2025.
//
#ifndef OLEDSCREEN_H
#define OLEDSCREEN_H

#include "IComponent.h"
#include <vector>
#include <Adafruit_SSD1306.h>
#include <time.h>
#include "WiFi.h"
#include <U8g2lib.h>


class OLEDscreen : public IComponent{
    public:
    OLEDscreen();
    void initComponent() override; // first initialization of component
    bool isActive() override; // checks whether OLED is currently active
    void goSleepMode() override; //put oled to a sleep mode (all the settings are saved)
    void wakeUp() override; // wake oled from sleep (all the settings are saved)
    void showTestBS(); // function to test input on oled
    void showTestBSver();
    void calibrateComponent() override;
    bool withinLimits() override;
    void connectToWifi(const char *ssid, const char *password);

    void setBrightness();

    void drawHomeScreen();
    void drawMainMenu();
    void drawHeartRateScreen();
    void drawStepsScreen();
    void drawBodyTempScreen();
    void drawPlaceholderScreen();

    void checkButton();
    void checkRotation();


    enum ScreenState {
        HOME_SCREEN,
        MAIN_MENU,
        HEART_RATE_SCREEN,
        STEPS_SCREEN,
        BODY_TEMP_SCREEN,
        PLACEHOLDER_SCREEN
    };
    ScreenState _screenState;

    //!test function (remove later)
    void setCurrentScreen();

    tm timeinfo;

    // Placeholders sensor data
    int heartRate = 72;
    int stepCount = 1234;
    float bodyTemp = 36.5;

    int menuPosition = 0;
    bool buttonPressed = false;
    bool wifiConnected = false;
    int lastEncA = 0;

    const long gmtOffset_sec = 3600;      // Netherlands base time (GMT+1)
    const int daylightOffset_sec = 3600;

    private:
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled_display_u8;
    bool _isActive = false;
    bool _isConnectedToWifi = false;

};

#endif //OLEDSCREEN_H
