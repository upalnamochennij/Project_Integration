//
// Created by bloom on 24.05.2025.
//

#include "../include/Parser.h"

Parser::Parser(const char *wifi_net_name, const char *wifi_password) {
    _ssid = wifi_net_name;
    _password = wifi_password;
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

void Parser::sendData(String& serverName) {
    this->_serverName = serverName;
    String temp = serverName + "/add_PulseOxygen?timestamp=2025-05-25T14:30:00Z&device_id=2&SPO2=78&heartrate=63"; // nado konkretnuju hujnu zapisatj

    if (WiFi.status() == WL_CONNECTED) {
        _httpClient.begin(temp.c_str());

        int httpCode = _httpClient.GET();
        if (httpCode > 0) {
            Serial.print("HTTP Code: ");
            Serial.println(httpCode);
            String response = _httpClient.getString();
            Serial.println(response);
        }
        else {
            Serial.print("Error occured: ");
            Serial.println(httpCode);
        }
        _httpClient.end(); // не влияет на Disconnected from WIFI Guru Meditation Error: Core  1 panic'ed (LoadProhibited). Exception was unhandled.
    }
    Serial.println("Disconnected from WIFI");
}


