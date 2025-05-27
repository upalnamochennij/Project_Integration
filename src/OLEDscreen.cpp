//
// Created by bloom on 18.05.2025.
//
#include "OLEDscreen.h"

#define BUTTON_PIN 32           // Button
#define ROTARY_ENCODER_A_PIN 26 // CLK
#define ROTARY_ENCODER_B_PIN 27 // DT

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C //< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

OLEDscreen::OLEDscreen() : oled_display_u8(U8G2_R0){
}

void OLEDscreen::initComponent(){
    oled_display_u8.begin();
    _isActive = true;
}

bool OLEDscreen::isActive() {
    if (_isActive == true) return true;
    return false;
}

void OLEDscreen::goSleepMode() {
    oled_display_u8.sleepOn();
    Serial.println("OLED is put to sleep mode\n");
    _isActive = false;
}

void OLEDscreen::wakeUp() {
    oled_display_u8.sleepOff();
    Serial.println("OLED is wake up\n");
    _isActive = true;
}

void OLEDscreen::showTestBS() {
    oled_display_u8.clearDisplay();// Очистить дисплейста
    oled_display_u8.setCursor(0, 10);              // Позиция текста (x=0, y=10)
    oled_display_u8.println(" TI PIDOR");       // Собственно текст
    oled_display_u8.display();                     // Показать на экране
}

void OLEDscreen::showTestBSver() {
    oled_display_u8.clearDisplay();// Очистить дисплейста
    oled_display_u8.setCursor(0, 10);              // Позиция текста (x=0, y=10)
    oled_display_u8.println(" JA PIDOR");       // Собственно текст
    oled_display_u8.display();
}

void OLEDscreen::calibrateComponent() {
}

bool OLEDscreen::withinLimits() {
    return true;
}

void OLEDscreen::connectToWifi(const char *ssid, const char *password) {
    WiFi.begin(ssid, password);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Wifi connected");
        _isConnectedToWifi = true;
        configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org", "time.nist.gov");
        Serial.println("Getting time from NTP server...");

        int timeAttempts = 0;
        while (!getLocalTime(&timeinfo) && timeAttempts < 10) {
            delay(1000);
            Serial.print(".");
            timeAttempts++;
        }

        if (timeAttempts < 10) {
            Serial.println("\nTime synchronized!");
        } else {
            Serial.println("\nFailed to get time");
        }
    } else {
        Serial.println("\nWiFi connection failed");
        _isConnectedToWifi = false;
    }
}

void OLEDscreen::drawHomeScreen() {
    char timeStr[9];
    if (wifiConnected && getLocalTime(&timeinfo)) {
        sprintf(timeStr, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    } else {
        sprintf(timeStr, "12:30:00"); // Fallback time
    }
    oled_display_u8.clearDisplay();
    oled_display_u8.drawFrame(0, 0, 128, 64);
    oled_display_u8.setFont(u8g2_font_6x10_tf);

    //title
    const char* title = "Home Screen";
    int titleWidth = oled_display_u8.getStrWidth(title);
    int titleX = (128 - titleWidth) / 2;
    oled_display_u8.drawStr(titleX, 12, title);
    oled_display_u8.drawLine(0, 15, 128, 15);

    // clock display
    oled_display_u8.setFont(u8g2_font_profont22_tf);
    int timeWidth = oled_display_u8.getStrWidth(timeStr);
    int timeX = (128 - timeWidth) / 2;
    oled_display_u8.drawStr(timeX, 40, timeStr);

    // instruction text
    oled_display_u8.setFont(u8g2_font_6x10_tf);
    const char* instruction = "Press for menu";
    int instructionWidth = oled_display_u8.getStrWidth(instruction);
    int instructionX = (128 - instructionWidth) / 2;
    oled_display_u8.drawStr(instructionX, 60, instruction);

    oled_display_u8.clearBuffer();
}

void OLEDscreen::drawMainMenu() {
    oled_display_u8.clearBuffer();
    oled_display_u8.drawFrame(0, 0, 128, 64);
    oled_display_u8.setFont(u8g2_font_6x10_tf);
    oled_display_u8.drawStr(40, 12, "Main Menu");
    oled_display_u8.drawLine(0, 15, 128, 15);

    //  menu items
    const char* menuItems[] = {"Heart Rate", "Steps", "Body Temp", "Option 4", "Back to Home"};
    const int numMenuItems = sizeof(menuItems) / sizeof(menuItems[0]); // Calculate number of items automatically

    for (int i = 0; i < numMenuItems; i++) {
        int y = 25 + i * 7;  // Spacing adjusts based on number of items

        // Highlight selected menu item
        if (i == menuPosition) {
            oled_display_u8.drawBox(0, y-6, 128, 8);
            oled_display_u8.setDrawColor(0); // Draw in black on white background
        } else {
            oled_display_u8.setDrawColor(1); // Draw in white on black background
        }

        oled_display_u8.drawStr(10, y, menuItems[i]);
        oled_display_u8.setDrawColor(1); // Reset to normal
    }

    oled_display_u8.sendBuffer();
}

void OLEDscreen::drawHeartRateScreen() {
    oled_display_u8.clearBuffer();
    oled_display_u8.drawFrame(0, 0, 128, 64);
    oled_display_u8.setFont(u8g2_font_6x10_tf);

    // Center the title
    const char* title = "Heart Rate";
    int titleWidth = oled_display_u8.getStrWidth(title);
    int titleX = (128 - titleWidth) / 2;  // Calculate center position
    oled_display_u8.drawStr(titleX, 12, title);
    oled_display_u8.drawLine(0, 15, 128, 15);

    //  heart rate display
    char hrStr[10];
    sprintf(hrStr, "%d", heartRate);
    oled_display_u8.setFont(u8g2_font_profont22_tf);

    // centering
    int numWidth = oled_display_u8.getStrWidth(hrStr);
    oled_display_u8.setFont(u8g2_font_6x10_tf);
    int bpmWidth = oled_display_u8.getStrWidth("BPM");
    int totalWidth = numWidth + bpmWidth + 5; // 5 pixels spacing between number and BPM
    int startX = (128 - totalWidth) / 2;

    // Draw the heart rate number
    oled_display_u8.setFont(u8g2_font_profont22_tf);
    oled_display_u8.drawStr(startX, 40, hrStr);

    // Draw "BPM" next to the number
    oled_display_u8.setFont(u8g2_font_6x10_tf);
    oled_display_u8.drawStr(startX + numWidth + 5, 40, "BPM");

    // Center the status message based on heart rate value
    const char* statusMsg;
    if (heartRate >= 60 && heartRate <= 100) {
        statusMsg = "Normal Range";
    } else if (heartRate < 60) {
        statusMsg = "Low Rate";
    } else {
        statusMsg = "High Rate";
    }

    int statusWidth = oled_display_u8.getStrWidth(statusMsg);
    int statusX = (128 - statusWidth) / 2;
    oled_display_u8.drawStr(statusX, 55, statusMsg);

    // Center the back instruction
    const char* backMsg = "Press to go back";
    int backWidth = oled_display_u8.getStrWidth(backMsg);
    int backX = (128 - backWidth) / 2;
    oled_display_u8.drawStr(backX, 63, backMsg);

    oled_display_u8.sendBuffer();
}

void OLEDscreen::drawStepsScreen() {
    oled_display_u8.clearBuffer();
    oled_display_u8.drawFrame(0, 0, 128, 64);
    oled_display_u8.setFont(u8g2_font_6x10_tf);

    // Center the title
    const char* title = "Steps";
    int titleWidth = oled_display_u8.getStrWidth(title);
    int titleX = (128 - titleWidth) / 2;
    oled_display_u8.drawStr(titleX, 12, title);
    oled_display_u8.drawLine(0, 15, 128, 15);

    // steps display
    char stepsStr[10];
    sprintf(stepsStr, "%d", stepCount);
    oled_display_u8.setFont(u8g2_font_profont17_tf);
    int stepsWidth = oled_display_u8.getStrWidth(stepsStr);
    int stepsX = (128 - stepsWidth) / 2;
    oled_display_u8.drawStr(stepsX, 35, stepsStr);

    // Center the description text
    oled_display_u8.setFont(u8g2_font_6x10_tf);
    const char* description = "Today's Count";
    int descWidth = oled_display_u8.getStrWidth(description);
    int descX = (128 - descWidth) / 2;
    oled_display_u8.drawStr(descX, 45, description);

    // Center the back instruction
    const char* backMsg = "Press to go back";
    int backWidth = oled_display_u8.getStrWidth(backMsg);
    int backX = (128 - backWidth) / 2;
    oled_display_u8.drawStr(backX, 63, backMsg);

    oled_display_u8.sendBuffer();
}

void OLEDscreen::drawBodyTempScreen() {
    oled_display_u8.clearBuffer();
    oled_display_u8.drawFrame(0, 0, 128, 64);
    oled_display_u8.setFont(u8g2_font_6x10_tf);

    // Center the title
    const char* title = "Body Temp";
    int titleWidth = oled_display_u8.getStrWidth(title);
    int titleX = (128 - titleWidth) / 2;
    oled_display_u8.drawStr(titleX, 12, title);
    oled_display_u8.drawLine(0, 15, 128, 15);

    //temperature display
    char tempStr[10];
    sprintf(tempStr, "%.1fC", bodyTemp);
    oled_display_u8.setFont(u8g2_font_profont17_tf);
    int tempWidth = oled_display_u8.getStrWidth(tempStr);
    int tempX = (128 - tempWidth) / 2;
    oled_display_u8.drawStr(tempX, 35, tempStr);

    // Center the status message based on temperature value
    oled_display_u8.setFont(u8g2_font_6x10_tf);
    const char* statusMsg;
    if (bodyTemp >= 36.1 && bodyTemp <= 37.2) {
        statusMsg = "Normal";
    } else {
        statusMsg = "Check temp";
    }

    int statusWidth = oled_display_u8.getStrWidth(statusMsg);
    int statusX = (128 - statusWidth) / 2;
    oled_display_u8.drawStr(statusX, 45, statusMsg);

    // Center the back instruction
    const char* backMsg = "Press to go back";
    int backWidth = oled_display_u8.getStrWidth(backMsg);
    int backX = (128 - backWidth) / 2;
    oled_display_u8.drawStr(backX, 63, backMsg);

    oled_display_u8.sendBuffer();
}

void OLEDscreen::drawPlaceholderScreen() {
    oled_display_u8.clearBuffer();
    oled_display_u8.drawFrame(0, 0, 128, 64);
    oled_display_u8.setFont(u8g2_font_6x10_tf);

    // Center the title
    const char* title = "Option 4";
    int titleWidth = oled_display_u8.getStrWidth(title);
    int titleX = (128 - titleWidth) / 2;
    oled_display_u8.drawStr(titleX, 12, title);
    oled_display_u8.drawLine(0, 15, 128, 15);

    // Center the placeholder messages
    const char* msg1 = "Coming Soon...";
    int msg1Width = oled_display_u8.getStrWidth(msg1);
    int msg1X = (128 - msg1Width) / 2;
    oled_display_u8.drawStr(msg1X, 35, msg1);

    const char* msg2 = "Feature TBD";
    int msg2Width = oled_display_u8.getStrWidth(msg2);
    int msg2X = (128 - msg2Width) / 2;
    oled_display_u8.drawStr(msg2X, 45, msg2);

    // Center the back instruction
    const char* backMsg = "Press to go back";
    int backWidth = oled_display_u8.getStrWidth(backMsg);
    int backX = (128 - backWidth) / 2;
    oled_display_u8.drawStr(backX, 63, backMsg);

    oled_display_u8.sendBuffer();
}

void OLEDscreen::checkButton() {
    if (digitalRead(BUTTON_PIN) == LOW && !buttonPressed) {
        delay(50); // Debounce
        if (digitalRead(BUTTON_PIN) == LOW) {
            buttonPressed = true;

            switch (currentScreen) {
                case HOME_SCREEN:
                    currentScreen = MAIN_MENU;
                    menuPosition = 0;
                    Serial.println("Switched to Main Menu");
                    break;

                case MAIN_MENU:
                    // Handle menu selection
                    switch (menuPosition) {
                    case 0: // Heart Rate
                            currentScreen = HEART_RATE_SCREEN;
                            Serial.println("Switched to Heart Rate Screen");
                            break;
                    case 1: // Steps
                            currentScreen = STEPS_SCREEN;
                            Serial.println("Switched to Steps Screen");
                            break;
                    case 2: // Body Temp
                            currentScreen = BODY_TEMP_SCREEN;
                            Serial.println("Switched to Body Temp Screen");
                            break;
                    case 3: // Placeholder
                            currentScreen = PLACEHOLDER_SCREEN;
                            Serial.println("Switched to Placeholder Screen");
                            break;
                    case 4: // Back to Home
                            currentScreen = HOME_SCREEN;
                            Serial.println("Returned to Home Screen");
                            break;
                    }
                    break;

                case HEART_RATE_SCREEN:
                case STEPS_SCREEN:
                case BODY_TEMP_SCREEN:
                case PLACEHOLDER_SCREEN:
                    currentScreen = MAIN_MENU;
                    Serial.println("Returned to Main Menu");
                    break;
            }
        }
    } else if (digitalRead(BUTTON_PIN) == HIGH) {
        buttonPressed = false;
    }
}

void OLEDscreen::checkRotation() {
    if (currentScreen == MAIN_MENU) {
        // Calculate menu size dynamically
        const char* menuItems[] = {"Heart Rate", "Steps", "Body Temp", "Option 4", "Back to Home"};
        const int numMenuItems = sizeof(menuItems) / sizeof(menuItems[0]);

        int currentEncA = digitalRead(ROTARY_ENCODER_A_PIN);
        int currentEncB = digitalRead(ROTARY_ENCODER_B_PIN);

        // Detect falling edge on CLK (pin A)
        if (lastEncA == HIGH && currentEncA == LOW) {

            if (currentEncB == HIGH) {
                // Rotate RIGHT (clockwise) tp move DOWN in menu
                menuPosition++;
                if (menuPosition >= numMenuItems) menuPosition = 0; // Wrap to top
                Serial.print("RIGHT rotation - Menu position: ");
                Serial.println(menuPosition);

            } else {
                // Rotate LEFT (counter-clockwise) to move UP in menu
                menuPosition--;
                if (menuPosition < 0) menuPosition = numMenuItems - 1; // Wrap to bottom
                Serial.print("LEFT rotation - Menu position: ");
                Serial.println(menuPosition);
            }
        }
        lastEncA = currentEncA;
    }
}

void OLEDscreen::setCurrentScreen(ScreenState currentState) {
    switch (currentState) {
        case HOME_SCREEN:
            drawHomeScreen();
            break;
        case MAIN_MENU:
            drawMainMenu();
            break;
        case HEART_RATE_SCREEN:
            drawHeartRateScreen();
            break;
        case STEPS_SCREEN:
            drawStepsScreen();
            break;
        case BODY_TEMP_SCREEN:
            drawBodyTempScreen();
            break;
        case PLACEHOLDER_SCREEN:
            drawPlaceholderScreen();
            break;
    }
    delay(10);
}








