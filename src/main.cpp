#include <Arduino.h>
#include "IComponent.h"
#include "Adafruit_VEML7700.h"
#include "Adafruit_BME280.h"
#include "Adafruit_MPU6050.h"
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <ios>

#include "heartRate.h"
#include "HeartrateSensor.h"
#include "LightSensor.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16


#define buzzerPin 2

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Не удалось инициализировать дисплей!"));
    while (true); // Остановись тут
  }

  display.clearDisplay();                // Очистить дисплей
  display.setTextSize(2);                // Размер текста
  display.setTextColor(SSD1306_WHITE);   // Цвет текста
  display.setCursor(0, 10);              // Позиция текста (x=0, y=10)
  display.println(" TI PIDOR");       // Собственно текст
  display.display();                     // Показать на экране

  // HeartrateSensor obj_heart;
  // obj_heart.initComponent();
  // bool heart_init_status = obj_heart.isInitialized();
  // obj_heart.readPulse();

  // LightSensor obj_light;
  // obj_light.initComponent();
  // auto light_activ_status = obj_light.isActive();
  //
  // Serial.println("VEML initialization status: ");
  // Serial.println(light_activ_status ? "true" : "false");
  // delay(3000);
  //
  // obj_light.goSleepMode();
  // light_activ_status = obj_light.isActive();
  // Serial.println("VEML initialization status: ");
  // Serial.println(light_activ_status ? "true" : "false");
  //
  // delay(3000);
  // obj_light.wakeUp();
  // light_activ_status = obj_light.isActive();
  // Serial.println("VEML initialization status: ");
  // Serial.println(light_activ_status ? "true" : "false");

}

void loop() {

}
