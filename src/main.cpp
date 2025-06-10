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

#define BUZZER_PIN 16
//H369A87FCC2
//E92C56743537

String serverName = "https://173.212.207.55";
const char *ssid = "ACSlab";
const char *password = "lab@ACS24";

const char Parser::api_certificate[] =
        "-----BEGIN CERTIFICATE-----\n"
        "MIIEKTCCAxGgAwIBAgIUSRWJ3DkatZm3MwIv2cYkt8JKSvswDQYJKoZIhvcNAQEL\n"
        "BQAwgaMxCzAJBgNVBAYTAk5MMREwDwYDVQQIDAhFbnNjaGVkZTERMA8GA1UEBwwI\n"
        "RW5zY2hlZGUxGzAZBgNVBAoMElByb2plY3RJbnRlZ3JhdGlvbjEPMA0GA1UECwwG\n"
        "c2Nob29sMRcwFQYDVQQDDA4xNzMuMjEyLjIwNy41NTEnMCUGCSqGSIb3DQEJARYY\n"
        "NTMxNzA3QHN0dWRlbnQuc2F4aW9uLm5sMB4XDTI1MDUxNDEyMzczMFoXDTI2MDUx\n"
        "NDEyMzczMFowgaMxCzAJBgNVBAYTAk5MMREwDwYDVQQIDAhFbnNjaGVkZTERMA8G\n"
        "A1UEBwwIRW5zY2hlZGUxGzAZBgNVBAoMElByb2plY3RJbnRlZ3JhdGlvbjEPMA0G\n"
        "A1UECwwGc2Nob29sMRcwFQYDVQQDDA4xNzMuMjEyLjIwNy41NTEnMCUGCSqGSIb3\n"
        "DQEJARYYNTMxNzA3QHN0dWRlbnQuc2F4aW9uLm5sMIIBIjANBgkqhkiG9w0BAQEF\n"
        "AAOCAQ8AMIIBCgKCAQEA09lQelIUliWYW74E05ttp9aUQZgbYsXlpZODHNACsKyU\n"
        "/BVgJEhGf3uAxL5WB55rXdro9dc/4NbX9zm+HNEkJf+3Ubp5Fq3LwKF3YbqOnj3y\n"
        "TNCEd28QVqrwmRQVtlSf3pAmw2UNh+h7l6yqiDdGIy5qdRZURrKnLboYJXQFC0Mq\n"
        "pZkOz6rK6j5pAI+Da6TfywbkrjQm3XpNs2Q5fQ6QV14wkvVfX5nly3Jwh4WbWTXh\n"
        "19EVY2jSxkBjKtwpEJmu1EOC7FOgs0AHLdT6KdrJsVaIQEQwZ5vkfKR/dPTaWu2G\n"
        "dQyOJwEdVH5Xci2ckqJcaofpcKK1BohrOlLckvSJNwIDAQABo1MwUTAdBgNVHQ4E\n"
        "FgQUiPdYw15e0DzxjFvnT1m92Sijll4wHwYDVR0jBBgwFoAUiPdYw15e0DzxjFvn\n"
        "T1m92Sijll4wDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEAlVQy\n"
        "paaCpgRDmr+M9jS7Ltx09dJ3TCW4UiB/6jXf0c4dQ3Qv4hZ1Aty8y0k3aBtoTTp5\n"
        "lHFEHoyNdVo8fOp+jzWyGb78FOIpAvTXWeYmaVXLc4P/ukPO4dx5VrMCgjabeyRO\n"
        "QJwkWHy1wNKR9Aa4rQ+ANqCDqwKyuZj/iRJOvo3ce57k7lV68XYBIO3/1SAvIzsX\n"
        "nTJo/5afN7ukrMlFehzPr7M7Jq53P3ZF96Rvn1/M8CHoyq2ugLpud5j+d95TqkmY\n"
        "BLwesoibECpHS8bHo7MP8TqGQ91URZt6RJXhf2tMHCaCwe3hn1/+aTkg047EtBr9\n"
        "7dMe2m114wDgVOieRA==\n"
        "-----END CERTIFICATE-----\n";

QueueHandle_t dataQueue;
SemaphoreHandle_t mutex = xSemaphoreCreateMutex();

SensorDataParsing dataForOLED;

HeartrateSensor obj_heart;
OLEDscreen obj_oled;
LightSensor obj_light;
Mpu6050_Integration obj_mpu;
TemperatureSensor obj_temp;
AlarmModule obj_alarm(obj_oled);
Parser obj_parser(ssid, password);

void heartrate_SP02_Task(void *pvParameters) {
    SensorDataParsing DataHeart;
    while (true) {
        obj_heart.readData();
        if (obj_heart._heartRateValid || obj_heart._sp02_valid) {
            DataHeart.heartrate = obj_heart._heartRate;
            DataHeart.sp02 = obj_heart._sp02_value;
        } else {
            DataHeart.heartrate = 0;
            DataHeart.sp02 = 0;
        }

        DataHeart.dTypeEnum = SensorDataParsing::HEARTRATE_AND_SP02;
        dataForOLED.heartrate = obj_heart._heartRate;
        dataForOLED.sp02 = obj_heart._sp02_value;

        strcpy(DataHeart.dataType, "Heartrate and SP02"); //for datatype
        auto timest = obj_parser.getCurrentTime();
        strcpy(DataHeart.datetime, timest.c_str()); // for datetime

        Serial.println("HeartRate: " + String(DataHeart.heartrate));
        Serial.println("SP02: " + String(DataHeart.sp02));
        xQueueSend(dataQueue, &DataHeart, portMAX_DELAY);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void displayDataOLED(void *pvParameters) {
    while (true) {
        obj_oled.bodyTemp = dataForOLED.temperature;
        obj_oled.stepCount = dataForOLED.steps;
        obj_oled.heartRate = obj_heart._heartRate;

        obj_oled.checkButton();
        obj_oled.checkRotation();
        obj_oled.setCurrentScreen();
        vTaskDelay(100 / portTICK_PERIOD_MS); //если 100 то реагирует экран быстро
    }
}

void temperatureTask(void *pvParameters) {
    SensorDataParsing DataTemp; // initial way of doing it
    while (true) {
        float temper = obj_temp.readTemp();
        DataTemp.temperature = temper;
        dataForOLED.temperature = temper;

        DataTemp.dTypeEnum = SensorDataParsing::TEMPERATURE;
        strcpy(DataTemp.dataType, "Temperature"); //for datatype
        auto timest = obj_parser.getCurrentTime();
        strcpy(DataTemp.datetime, timest.c_str()); // for datetime

        Serial.println("Temperature: " + String(DataTemp.temperature));
        xQueueSend(dataQueue, &DataTemp, portMAX_DELAY);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// void checkAlertTask(void *pvParameters) { //Initiali alertTask
//     while (true) {
//         Serial.println("Entering checkAlertTask method\n");
//         auto type = obj_alarm.checkAlertType(dataForOLED);
//         obj_alarm.selectAlert(type);
//         Serial.println("Checking for checkAlertType");
//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//     }
// }

void checkFallTask(void *pvParameters) {
    SensorDataParsing alertData;
    while (true) {
        Serial.println("Entering checkFallTask method\n");
        obj_mpu.getEvent();
        auto accel = obj_mpu.readAccel();
        auto gyro = obj_mpu.readGyro();

        alertData.dTypeEnum = SensorDataParsing::ACCELERATION_AND_GYRO;
        alertData.accel_x = accel.acceleration.x;
        alertData.accel_y = accel.acceleration.y;
        alertData.accel_z = accel.acceleration.z;
        alertData.gyro_x = gyro.gyro.x;
        alertData.gyro_y = gyro.gyro.y;
        alertData.gyro_z = gyro.gyro.z;

        std::string timest = obj_parser.getCurrentTime();
        strcpy(alertData.datetime, timest.c_str());

        bool isFalling = obj_alarm.checkForFall(alertData);
        if (isFalling == true) {
            Serial.println("FALL DETECTED\n");
            xQueueSend(dataQueue, &alertData, portMAX_DELAY);
        }else Serial.println("NO FALL\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void stepsTask(void *pvParameters) {
    SensorDataParsing DataSteps;
    unsigned long lastDetectedTime = millis();
    while (true) {
        obj_mpu.getEvent();
        auto accel = obj_mpu.readAccel();
        int step = obj_mpu.detectStep(accel);

        DataSteps.steps += step;
        dataForOLED.steps += step;

        DataSteps.dTypeEnum = SensorDataParsing::STEPS;
        strcpy(DataSteps.dataType, "Steps"); //for datatype
        std::string timest = obj_parser.getCurrentTime();
        strcpy(DataSteps.datetime, timest.c_str()); // for datetime
        Serial.println("Waiting till 5 minutes pass");

        if (millis() - lastDetectedTime >= 100000) {
            Serial.println("Steps: " + String(DataSteps.steps));
            xQueueSend(dataQueue, &DataSteps, portMAX_DELAY);
            lastDetectedTime = millis();
            DataSteps.steps = 0;
            dataForOLED.steps = 0;
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void sendToApiTask(void *pvParameters) {
    SensorDataParsing ReceivedData;
    while (true) {
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("WiFi not connected");
            obj_parser.connectToWifi();
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        } else {
            if (xQueueReceive(dataQueue, &ReceivedData, portMAX_DELAY) == pdTRUE) {
                obj_parser.sendData(ReceivedData);
            } else Serial.println("Failed to receive data\n");
        }
    }
}

void setup() {
    Serial.begin(9600);
    Wire.begin();
    Wire.setClock(400000); //400khz ultra fast

    while (mutex == nullptr) {
        Serial.println("mutex was not created");
        while (true);
    }

    obj_heart.initComponent();
    obj_oled.initComponent();
    obj_oled.connectToWifi(ssid, password);
    obj_mpu.initComponent();
    obj_temp.initComponent();

    // xSemaphoreTake(mutex, portMAX_DELAY);
    // obj_heart.initComponent();
    // xSemaphoreGive(mutex);
    //
    // xSemaphoreTake(mutex, portMAX_DELAY);
    // obj_oled.initComponent();
    // xSemaphoreGive(mutex);
    // obj_oled.connectToWifi(ssid, password);
    //
    // xSemaphoreTake(mutex, portMAX_DELAY);
    // obj_mpu.initComponent();
    // xSemaphoreGive(mutex);
    //
    // xSemaphoreTake(mutex, portMAX_DELAY);
    // obj_temp.initComponent();
    // xSemaphoreGive(mutex);

    configTime(3600, 3600, "pool.ntp.org", "time.nist.gov");

    obj_parser.connectToWifi();
    obj_parser.setServer(serverName);
    obj_parser.setDevice(1002);

    dataQueue = xQueueCreate(10, sizeof(SensorDataParsing));

    xTaskCreate(heartrate_SP02_Task, "heartrate_SP02_Task", 8192,
                NULL, 2, NULL);
    xTaskCreate(temperatureTask, "temperatureTask", 8192,
                NULL, 2, NULL);
    xTaskCreate(stepsTask, "stepsTask", 8192,
                NULL, 2, NULL);
    xTaskCreate(displayDataOLED, "displayDataOLED", 8192,
                NULL, 3, NULL);
    xTaskCreate(sendToApiTask, "sendToApiTask", 8192,
                NULL, 2, NULL);
    xTaskCreate(checkFallTask, "checkFallTask", 8192,
                NULL, 2, NULL);
    // xTaskCreate(checkAlertTask, "checkAlertTask", 8192,
    //             NULL, 2, NULL);
}

void loop() {
}
