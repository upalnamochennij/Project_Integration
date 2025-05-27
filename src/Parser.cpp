//
// Created by bloom on 24.05.2025.
//

#include "../include/Parser.h"

#include "SensorDataParsing.h"

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

void Parser::sendData(SensorDataParsing &sensordata) {
    String payload;

    switch (sensordata.dTypeEnum) {
        case SensorDataParsing::HEARTRATE_AND_SP02:
            payload = _serverName +
                      "/values/add_PulseOxygen?timestamp=2025-05-21T14:30:00Z&device_id=2&SPO2=" + sensordata.sp02 +
                      "&heartrate=" + sensordata.heartrate;
            break;
        case SensorDataParsing::TEMPERATURE:
            payload = _serverName + "/values/add_temperature?timestamp=2025-05-21T14:30:00Z&device_id=2&temperature=" +
                      sensordata.temperature;
            break;
        case SensorDataParsing::ACCELERATION_AND_GYRO:
            payload = _serverName + "/values/add_MPU?timestamp=2025-05-21T14:30:00Z&device_id=2&gyro_x="
                      + sensordata.gyro_x + "&gyro_y=" + sensordata.gyro_y + "&gyro_z=" + sensordata.gyro_z +
                      "&accel_x=" + sensordata.accel_x + "&accel_y=" + sensordata.accel_y + " &accel_z=" + sensordata.
                      accel_z;
            break;
        default:
            throw std::invalid_argument("Invalid sensor data type");
    }

    if (WiFi.status() == WL_CONNECTED) {
        _httpClient.begin(payload.c_str());

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


