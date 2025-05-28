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

void Parser::setServer(String& serverName) {
    this->_serverName = serverName;
}

void Parser::connectToWifi() const {
    WiFi.begin(_ssid, _password);
    Serial.println("Connecting");
    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
}

void Parser::sendData(SensorDataParsing& sensordata) {
    String payload;
    String timest_casted = String(sensordata.datetime.c_str());
    //String timest_casted = "2025-06-18T14:10:00Z";
    //payload = "https://173.212.207.55/values/add_PulseOxygen?timestamp=2025-05-31T14:20:00Z&device_id=2&SPO2=12&heartrate=23";
    switch (sensordata.dTypeEnum) {
        case SensorDataParsing::HEARTRATE_AND_SP02:
            payload = _serverName +
                      "/values/add_PulseOxygen?timestamp=" + timest_casted + "&device_id=" + this->device_id + "&SPO2=" + sensordata.sp02 +
                      "&heartrate=" + sensordata.heartrate;
            Serial.println(payload);
            break;
        case SensorDataParsing::TEMPERATURE:
            payload = _serverName + "/values/add_temperature?timestamp=" + timest_casted + "&device_id=" + this->device_id + "&temperature=" +
                      sensordata.temperature;
            Serial.println(payload);
            break;
        case SensorDataParsing::ACCELERATION_AND_GYRO:
            payload = _serverName + "/values/add_MPU?timestamp=" + timest_casted + "&device_id=" + this->device_id + "&gyro_x="
                      + sensordata.gyro_x + "&gyro_y=" + sensordata.gyro_y + "&gyro_z=" + sensordata.gyro_z +
                      "&accel_x=" + sensordata.accel_x + "&accel_y=" + sensordata.accel_y + " &accel_z=" + sensordata.
                      accel_z;
            Serial.println(payload);
            break;
        default:
            throw std::invalid_argument("Invalid sensor data type");
    }

    if (WiFi.status() == WL_CONNECTED) {
        _httpClient.begin(payload.c_str(), api_certificate);

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
        // не влияет на Disconnected from WIFI Guru Meditation Error: Core  1 panic'ed (LoadProhibited). Exception was unhandled.
    }
    Serial.println("Disconnected from WIFI");
}

void Parser::setDevice(int device) {
    if (device > 0) {
        this->device_id = device;
    }
}

std::string Parser::getCurrentTime() {
    // Получаем текущее системное время
    auto now = std::chrono::system_clock::now();
    std::time_t time_t_now = std::chrono::system_clock::to_time_t(now);

    // Получаем UTC-время
    std::tm *utc_tm = std::localtime(&time_t_now); // UTC time

    // Используем stringstream и put_time (поддерживается в C++11)
    std::ostringstream oss;
    oss << std::put_time(utc_tm, "%Y-%m-%dT%H:%M:%SZ");
    return oss.str();
}






