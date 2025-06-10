//
// Created by bloom on 24.05.2025.
//

#include "../include/Parser.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include "SensorDataParsing.h"

using namespace std::chrono;

Parser::Parser(const char *wifi_net_name, const char *wifi_password) {
    _ssid = wifi_net_name;
    _password = wifi_password;
}

void Parser::setServer(String &serverName) {
    this->_serverName = serverName;
}

void Parser::connectToWifi() const {
    WiFi.begin(_ssid, _password);
    Serial.println("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
}

void Parser::sendData(SensorDataParsing &sensordata) {
    //String payload;
    char payload_max[512];
    memset(payload_max, 0, sizeof(payload_max)); //cleaning buffer just in case

    switch (sensordata.dTypeEnum) {
        case SensorDataParsing::STEPS:
            snprintf(payload_max, sizeof(payload_max),
                     "%s/values/add_steps?timestamp=%s&device_id=%d&steps=%d",
                     _serverName.c_str(),
                     sensordata.datetime,
                     this->device_id,
                     sensordata.steps);
            Serial.println(payload_max);
            break;
        case SensorDataParsing::HEARTRATE_AND_SP02:
            snprintf(payload_max, sizeof(payload_max),
                     "%s/values/add_PulseOxygen?timestamp=%s&device_id=%d&SPO2=%d&heartrate=%d",
                     _serverName.c_str(),
                     sensordata.datetime,
                     this->device_id,
                     sensordata.sp02,
                     sensordata.heartrate
            );
            Serial.println(payload_max);
            break;
        case SensorDataParsing::TEMPERATURE:
            snprintf(payload_max, sizeof(payload_max),
                     "%s/values/add_temperature?timestamp=%s&device_id=%d&temperature=%f",
                     _serverName.c_str(),
                     sensordata.datetime,
                     this->device_id,
                     sensordata.temperature
            );
            Serial.println(payload_max);
            break;
        case SensorDataParsing::ACCELERATION_AND_GYRO:
            snprintf(payload_max, sizeof(payload_max),
                     "%s/values/add_MPU?timestamp=%s&device_id=%d&gyro_x=%f&gyro_y=%f&gyro_z=%f&accel_x=%f&accel_y=%f&accel_z=%f",
                     _serverName.c_str(),
                     sensordata.datetime,
                     this->device_id,
                     sensordata.gyro_x,
                     sensordata.gyro_y,
                     sensordata.gyro_z,
                     sensordata.accel_x,
                     sensordata.accel_y,
                     sensordata.accel_z
            );
            Serial.println(payload_max);
            break;
        default:
            throw std::invalid_argument("Invalid sensor data type");
    }

    if (WiFi.status() == WL_CONNECTED) {
        _httpClient.begin(payload_max, api_certificate);

        int httpCode = _httpClient.GET();
        if (httpCode > 0) {
            Serial.print("HTTP Code: ");
            Serial.println(httpCode);
            String response = _httpClient.getString();
            Serial.println(response);
        } else {
            Serial.print("Error occured: ");
            Serial.println(httpCode);
        }
        _httpClient.end();
    }
    Serial.println("Disconnected from WIFI");
}

void Parser::setDevice(int device) {
    if (device > 0) {
        this->device_id = device;
    }
}

std::string Parser::getCurrentTime() {
    time_t now;
    time(&now);

    struct tm timeinfo;
    localtime_r(&now, &timeinfo);

    char buffer[25];
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02dT%02d:%02d:%02dZ",
             timeinfo.tm_year + 1900,
             timeinfo.tm_mon + 1,
             timeinfo.tm_mday,
             timeinfo.tm_hour,
             timeinfo.tm_min,
             timeinfo.tm_sec);

    return std::string(buffer);
}
