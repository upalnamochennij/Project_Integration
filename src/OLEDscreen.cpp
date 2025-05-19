//
// Created by bloom on 18.05.2025.
//
#include "OLEDscreen.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C //< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

OLEDscreen::OLEDscreen() : oled_display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET){
}

void OLEDscreen::initComponent(){
    if (!oled_display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println("Не удалось инициализировать дисплей!");
        while (true); // Остановись тут
    }
    _isActive = true;
}

bool OLEDscreen::isActive() {
    if (_isActive == true) return true;
    return false;
}

void OLEDscreen::goSleepMode() {
    oled_display.ssd1306_command(SSD1306_DISPLAYOFF);
    Serial.println("OLED is put to sleep mode\n");
    _isActive = false;
}

void OLEDscreen::wakeUp() {
    oled_display.ssd1306_command(SSD1306_DISPLAYON);
    Serial.println("OLED is wake up\n");
    _isActive = true;
}

void OLEDscreen::showTestBS() {
    oled_display.clearDisplay();                // Очистить дисплей
    oled_display.setTextSize(2);                // Размер текста
    oled_display.setTextColor(SSD1306_WHITE);   // Цвет текста
    oled_display.setCursor(0, 10);              // Позиция текста (x=0, y=10)
    oled_display.println(" TI PIDOR");       // Собственно текст
    oled_display.display();                     // Показать на экране
}

void OLEDscreen::calibrateComponent() {

}

bool OLEDscreen::withinLimits() {
    return true;
}







