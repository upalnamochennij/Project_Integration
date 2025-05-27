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
#include "OLEDscreen.h"
#include "HeartrateSensor.h"
#include "LightSensor.h"
#include "Mpu6050_Integration.h"
#include "AlarmModule.h"
#include "Parser.h"
#include <memory>
#include "SensorDataParsing.h"
#include "../../../../../.platformio/packages/framework-arduinoespressif32/libraries/WiFi/src/WiFi.h"

#define BUZZER_PIN 2

const char *ssid = "ACSlab";
const char *password = "lab@ACS24";
String serverName = "https://173.212.207.55";

QueueHandle_t dataQueue;

HeartrateSensor obj_heart;
OLEDscreen obj_oled;
LightSensor obj_light;
Mpu6050_Integration obj_mpu;
TemperatureSensor obj_temp;
AlarmModule obj_alarm(obj_heart, obj_temp, obj_mpu, obj_oled);
Parser obj_parser(ssid, password);

void heartrate_SP02_Task(void *pvParameters) {
    SensorDataParsing DataHeart;
    while (true) {
        obj_heart.readData(); // reads both SP02 and heartrate
        DataHeart.heartrate = obj_heart._heartRate;
        DataHeart.sp02 = obj_heart._sp02_value;
        DataHeart.dTypeEnum = SensorDataParsing::HEARTRATE_AND_SP02;
        DataHeart.dataType = "Heartrate and SP02";

        Serial.println("HeartRate: " + String(DataHeart.heartrate));
        Serial.println("SP02: " + String(DataHeart.sp02));
        xQueueSend(dataQueue, &DataHeart, portMAX_DELAY);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void temperatureTask(void *pvParameters) {
    SensorDataParsing DataTemp;
    while (true) {
        float temper = obj_temp.readTemp();
        DataTemp.temperature = temper;
        DataTemp.dTypeEnum = SensorDataParsing::TEMPERATURE;
        DataTemp.dataType = "Temperature";

        Serial.println("Temperature: " + String(DataTemp.temperature));
        xQueueSend(dataQueue, &DataTemp, portMAX_DELAY);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void accelGyroTask(void *pvParameters) {
    SensorDataParsing AccelGyro;
    while (true) {
        auto accel = obj_mpu.readAccel();
        auto gyro = obj_mpu.readGyro();
        AccelGyro.dTypeEnum = SensorDataParsing::ACCELERATION_AND_GYRO;
        AccelGyro.accel_x = accel.acceleration.x;
        AccelGyro.accel_y = accel.acceleration.y;
        AccelGyro.accel_z = accel.acceleration.z;
        AccelGyro.gyro_x = gyro.gyro.x;
        AccelGyro.gyro_y = gyro.gyro.y;
        AccelGyro.gyro_z = gyro.gyro.z;

        xQueueSend(dataQueue, &AccelGyro, portMAX_DELAY);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void sendToApiTask(void *pvParameters) {
    SensorDataParsing ReceivedData;
    while (true) {
        if (xQueueReceive(dataQueue, &ReceivedData, portMAX_DELAY) == pdTRUE) {
            obj_parser.sendData(ReceivedData);
        }
    }
}

void setup() {
    Serial.begin(9600);

    obj_heart.initComponent();
    obj_oled.initComponent();
    // obj_light.initComponent(); //not connected properly right now
    obj_mpu.initComponent();
    obj_temp.initComponent();
    obj_parser.setServer(serverName);

    dataQueue = xQueueCreate(10, sizeof(SensorDataParsing));

    xTaskCreate(heartrate_SP02_Task, "heartrate_SP02_Task", 2048, NULL, 2, NULL);
    xTaskCreate(accelGyroTask, "accelGyroTask", 2048, NULL, 2, NULL);
    xTaskCreate(temperatureTask, "temperatureTask", 2048, NULL, 2, NULL);
    xTaskCreate(sendToApiTask, "sendToApiTask", 2048, NULL, 2, NULL);

    // obj_oled._screenState = OLEDscreen::HOME_SCREEN;
    // obj_oled.setCurrentScreen();
}


void loop() {
}
