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
        DataHeart.dataType = "Heartrate and SP02";
        DataHeart.datetime = obj_parser.getCurrentTime();

        Serial.println("HeartRate: " + String(DataHeart.heartrate));
        Serial.println("SP02: " + String(DataHeart.sp02));
        xQueueSend(dataQueue, &DataHeart, portMAX_DELAY);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void temperatureTask(void *pvParameters) {
    SensorDataParsing DataTemp; // initial way of doing it
    //SensorDataParsing* DataTemp = new SensorDataParsing();
    while (true) {
        float temper = obj_temp.readTemp();
        DataTemp.temperature = temper;
        DataTemp.dTypeEnum = SensorDataParsing::TEMPERATURE;
        DataTemp.dataType = "Temperature";
        DataTemp.datetime = obj_parser.getCurrentTime();

        Serial.println("Temperature: " + String(DataTemp.temperature));
        xQueueSend(dataQueue, &DataTemp, portMAX_DELAY);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void accelGyroTask(void *pvParameters) {
    SensorDataParsing AccelGyro;
    //SensorDataParsing* AccelGyro = new SensorDataParsing();
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
        AccelGyro.datetime = obj_parser.getCurrentTime();

        xQueueSend(dataQueue, &AccelGyro, portMAX_DELAY);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
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
        }
    }
}

void read_and_sendHeart() {
    obj_heart.readData();
    test.heartrate = obj_heart._heartRate;
    test.sp02 = obj_heart._sp02_value;
    test.dTypeEnum = SensorDataParsing::HEARTRATE_AND_SP02;
    test.datetime = obj_parser.getCurrentTime();
    obj_parser.sendData(test);
    test.reset();
}

void read_and_sendTemp() {
    test.temperature = obj_temp.readTemp();
    test.dTypeEnum = SensorDataParsing::TEMPERATURE;
    test.datetime = obj_parser.getCurrentTime();
    obj_parser.sendData(test);
    test.reset();
}

void read_and_sendMPU() {
    auto event_a = obj_mpu.readAccel();
    auto event_g = obj_mpu.readGyro();
    test.accel_x = event_a.acceleration.x;
    test.accel_y = event_a.acceleration.y;
    test.accel_z = event_a.acceleration.z;
    test.gyro_x = event_g.gyro.x;
    test.gyro_y = event_g.gyro.y;
    test.gyro_z = event_g.gyro.z;
    obj_parser.sendData(test);
    test.reset();
}

void setup() {
    Serial.begin(9600);

    obj_heart.initComponent();
    obj_oled.initComponent();
    // obj_light.initComponent(); //not connected properly right now
    obj_mpu.initComponent();
    obj_temp.initComponent();

    obj_parser.connectToWifi();

    obj_parser.setServer(serverName);
    obj_parser.setDevice(1);

    // obj_oled.connectToWifi(ssid, password);

    dataQueue = xQueueCreate(10, sizeof(SensorDataParsing));

    xTaskCreate(heartrate_SP02_Task, "heartrate_SP02_Task", 2048, NULL, 2, NULL);
    xTaskCreate(accelGyroTask, "accelGyroTask", 2048, NULL, 2, NULL);
    xTaskCreate(temperatureTask, "temperatureTask", 2048, NULL, 2, NULL);
    xTaskCreate(sendToApiTask, "sendToApiTask", 2048, NULL, 2, NULL);

    // read_and_sendHeart();
    // read_and_sendTemp();
    // read_and_sendMPU();
}

void loop() {
    //delay(100000);
    // obj_oled.checkButton();
    // obj_oled.checkRotation();
    // obj_oled.setCurrentScreen();
}
