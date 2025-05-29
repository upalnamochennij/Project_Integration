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

const char *ssid = "H369A87FCC2";
const char *password = "E92C56743537";

const char Parser::api_certificate[] = \
        "-----BEGIN CERTIFICATE-----\n" \
        "MIIEKTCCAxGgAwIBAgIUSRWJ3DkatZm3MwIv2cYkt8JKSvswDQYJKoZIhvcNAQEL\n" \
        "BQAwgaMxCzAJBgNVBAYTAk5MMREwDwYDVQQIDAhFbnNjaGVkZTERMA8GA1UEBwwI\n" \
        "RW5zY2hlZGUxGzAZBgNVBAoMElByb2plY3RJbnRlZ3JhdGlvbjEPMA0GA1UECwwG\n" \
        "c2Nob29sMRcwFQYDVQQDDA4xNzMuMjEyLjIwNy41NTEnMCUGCSqGSIb3DQEJARYY\n" \
        "NTMxNzA3QHN0dWRlbnQuc2F4aW9uLm5sMB4XDTI1MDUxNDEyMzczMFoXDTI2MDUx\n" \
        "NDEyMzczMFowgaMxCzAJBgNVBAYTAk5MMREwDwYDVQQIDAhFbnNjaGVkZTERMA8G\n" \
        "A1UEBwwIRW5zY2hlZGUxGzAZBgNVBAoMElByb2plY3RJbnRlZ3JhdGlvbjEPMA0G\n" \
        "A1UECwwGc2Nob29sMRcwFQYDVQQDDA4xNzMuMjEyLjIwNy41NTEnMCUGCSqGSIb3\n" \
        "DQEJARYYNTMxNzA3QHN0dWRlbnQuc2F4aW9uLm5sMIIBIjANBgkqhkiG9w0BAQEF\n" \
        "AAOCAQ8AMIIBCgKCAQEA09lQelIUliWYW74E05ttp9aUQZgbYsXlpZODHNACsKyU\n" \
        "/BVgJEhGf3uAxL5WB55rXdro9dc/4NbX9zm+HNEkJf+3Ubp5Fq3LwKF3YbqOnj3y\n" \
        "TNCEd28QVqrwmRQVtlSf3pAmw2UNh+h7l6yqiDdGIy5qdRZURrKnLboYJXQFC0Mq\n" \
        "pZkOz6rK6j5pAI+Da6TfywbkrjQm3XpNs2Q5fQ6QV14wkvVfX5nly3Jwh4WbWTXh\n" \
        "19EVY2jSxkBjKtwpEJmu1EOC7FOgs0AHLdT6KdrJsVaIQEQwZ5vkfKR/dPTaWu2G\n" \
        "dQyOJwEdVH5Xci2ckqJcaofpcKK1BohrOlLckvSJNwIDAQABo1MwUTAdBgNVHQ4E\n" \
        "FgQUiPdYw15e0DzxjFvnT1m92Sijll4wHwYDVR0jBBgwFoAUiPdYw15e0DzxjFvn\n" \
        "T1m92Sijll4wDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEAlVQy\n" \
        "paaCpgRDmr+M9jS7Ltx09dJ3TCW4UiB/6jXf0c4dQ3Qv4hZ1Aty8y0k3aBtoTTp5\n" \
        "lHFEHoyNdVo8fOp+jzWyGb78FOIpAvTXWeYmaVXLc4P/ukPO4dx5VrMCgjabeyRO\n" \
        "QJwkWHy1wNKR9Aa4rQ+ANqCDqwKyuZj/iRJOvo3ce57k7lV68XYBIO3/1SAvIzsX\n" \
        "nTJo/5afN7ukrMlFehzPr7M7Jq53P3ZF96Rvn1/M8CHoyq2ugLpud5j+d95TqkmY\n" \
        "BLwesoibECpHS8bHo7MP8TqGQ91URZt6RJXhf2tMHCaCwe3hn1/+aTkg047EtBr9\n" \
        "7dMe2m114wDgVOieRA==\n" \
        "-----END CERTIFICATE-----\n";

String serverName = "https://173.212.207.55";

QueueHandle_t dataQueue;
SensorDataParsing test;

HeartrateSensor obj_heart;
OLEDscreen obj_oled;
LightSensor obj_light;
Mpu6050_Integration obj_mpu;
TemperatureSensor obj_temp;
AlarmModule obj_alarm(obj_heart, obj_temp, obj_mpu, obj_oled);
Parser obj_parser(ssid, password);

void heartrate_SP02_Task(void *pvParameters) {
    SensorDataParsing DataHeart;
    //SensorDataParsing* DataHeart = new SensorDataParsing();
    while (true) {
        obj_heart.readData(); // reads both SP02 and heartrate
        DataHeart.heartrate = obj_heart._heartRate;
        DataHeart.sp02 = obj_heart._sp02_value;
        DataHeart.dTypeEnum = SensorDataParsing::HEARTRATE_AND_SP02;
        strcpy(DataHeart.dataType, "Heartrate and SP02"); //for datatype
        auto timest = obj_parser.getCurrentTime();
        strcpy(DataHeart.datetime, timest.c_str()); // for datetime

        Serial.println("HeartRate: " + String(DataHeart.heartrate));
        Serial.println("SP02: " + String(DataHeart.sp02));
        xQueueSend(dataQueue, &DataHeart, portMAX_DELAY);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void temperatureTask(void *pvParameters) {
    SensorDataParsing DataTemp; // initial way of doing it
    //SensorDataParsing* DataTemp = new SensorDataParsing();
    while (true) {
        int temper = obj_temp.readTemp();
        DataTemp.temperature = temper;
        DataTemp.dTypeEnum = SensorDataParsing::TEMPERATURE;
        strcpy(DataTemp.dataType, "Temperature"); //for datatype
        auto timest = obj_parser.getCurrentTime();
        strcpy(DataTemp.datetime, timest.c_str()); // for datetime

        Serial.println("Temperature: " + String(DataTemp.temperature));
        xQueueSend(dataQueue, &DataTemp, portMAX_DELAY);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void stepsTask(void *pvParameters) {
    SensorDataParsing DataSteps; // initial way of doing it
    //SensorDataParsing* DataTemp = new SensorDataParsing();
    while (true) {
        auto accel = obj_mpu.readAccel();
        int step = obj_mpu.countSteps(accel);
        DataSteps.steps = step;
        DataSteps.dTypeEnum = SensorDataParsing::STEPS;
        strcpy(DataSteps.dataType, "Steps"); //for datatype
        auto timest = obj_parser.getCurrentTime();
        strcpy(DataSteps.datetime, timest.c_str()); // for datetime

        Serial.println("Steps: " + String(DataSteps.steps));
        xQueueSend(dataQueue, &DataSteps, portMAX_DELAY);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void accelGyroTask(void *pvParameters) {
    SensorDataParsing AccelGyro;
    //SensorDataParsing* AccelGyro = new SensorDataParsing();
    while (true) {
        auto accel = obj_mpu.readAccel();
        auto gyro = obj_mpu.readGyro();
        AccelGyro.dTypeEnum = SensorDataParsing::ACCELERATION_AND_GYRO;
        AccelGyro.accel_x = static_cast<int>(accel.acceleration.x);
        AccelGyro.accel_y = static_cast<int>(accel.acceleration.y);
        AccelGyro.accel_z = static_cast<int>(accel.acceleration.z);
        AccelGyro.gyro_x = static_cast<int>(gyro.gyro.x);
        AccelGyro.gyro_y = static_cast<int>(gyro.gyro.y);
        AccelGyro.gyro_z = static_cast<int>(gyro.gyro.z);
        strcpy(AccelGyro.dataType, "Accel & Gyro"); //for datatype
        std::string timest = obj_parser.getCurrentTime();
        strcpy(AccelGyro.datetime, timest.c_str()); // for datetime

        xQueueSend(dataQueue, &AccelGyro, portMAX_DELAY);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void sendToApiTask(void *pvParameters) {
    SensorDataParsing ReceivedData;
    //SensorDataParsing* ReceivedData = new SensorDataParsing();
    while (true) {
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("WiFi not connected");
            obj_parser.connectToWifi();
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
        else {
            if (xQueueReceive(dataQueue, &ReceivedData, portMAX_DELAY) == pdTRUE) {
                obj_parser.sendData(ReceivedData);
            }
            else Serial.println("Failed to receive data\n");
        }
    }
    vTaskDelete(NULL);
}


void setup() {
    Serial.begin(9600);

    obj_heart.initComponent();
    obj_oled.initComponent();
    // obj_light.initComponent(); //not connected properly right now
    obj_mpu.initComponent();
    obj_temp.initComponent();

    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    obj_parser.connectToWifi();

    obj_parser.setServer(serverName);
    obj_parser.setDevice(1);

    // obj_oled.connectToWifi(ssid, password);

    dataQueue = xQueueCreate(10, sizeof(SensorDataParsing));

    xTaskCreate(heartrate_SP02_Task, "heartrate_SP02_Task", 4096, NULL, 2, NULL);
    xTaskCreate(accelGyroTask, "accelGyroTask", 4096, NULL, 2, NULL);
    xTaskCreate(temperatureTask, "temperatureTask", 4096, NULL, 2, NULL);
    xTaskCreate(sendToApiTask, "sendToApiTask", 8192, NULL, 2, NULL);
    xTaskCreate(stepsTask, "stepsTask", 4096, NULL, 2, NULL);
}

void loop() {
    // auto accel = obj_mpu.readAccel();
    // int steps = obj_mpu.countSteps(accel);
    // Serial.print("Steps count: ");
    // Serial.println(steps);
    // delay(500);

    //delay(100000);
    // obj_oled.checkButton();
    // obj_oled.checkRotation();
    // obj_oled.setCurrentScreen();
}
