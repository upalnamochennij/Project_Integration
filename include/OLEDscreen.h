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
#include "SensorDataParsing.h"


class OLEDscreen : public IComponent{
    public:
    OLEDscreen();
    void initComponent() override; // first initialization of component
    bool isActive() override; // checks whether OLED is currently active
    void goSleepMode() override; //put oled to a sleep mode (all the settings are saved)
    void wakeUp() override; // wake oled from sleep (all the settings are saved)
    void showTestBS(); // function to test input on oled
    void showTestBSver();

    bool withinLimits() override;
    void connectToWifi(const char *ssid, const char *password);

    void drawAlarmDefault();
    void drawAlarmFall();

    void drawHomeScreen();
    void drawMainMenu();
    void drawHeartRateScreen();
    void drawStepsScreen();
    void drawBodyTempScreen();
    void drawPlaceholderScreen();
    void drawClearScreen();

    void checkButton();
    void checkRotation();

    //!test function (remove later)
    void setCurrentScreen();

    int heartRate = 0;
    int stepCount = 0;
    float bodyTemp = 0;
    bool wifiConnected = false;
    bool buttonPressed = false;

    enum ScreenState {
        HOME_SCREEN,
        MAIN_MENU,
        HEART_RATE_SCREEN,
        STEPS_SCREEN,
        BODY_TEMP_SCREEN
        // PLACEHOLDER_SCREEN удален
    } _screenState = HOME_SCREEN;

    private:
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled_u8;
    bool _isActive = false;
    bool _isConnectedToWifi = false;

    int menuPosition = 0;
    int lastEncA = 0;

    struct tm timeinfo;

    const long gmtOffset_sec = 3600;
    const int daylightOffset_sec = 3600;

};

#endif //OLEDSCREEN_H
