//
// Created by bloom on 24.05.2025.
//
#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <HTTPClient.h>
#include <WiFi.h>

#include "SensorDataParsing.h"

class Parser{

    public:
    Parser(const char *wifi_name, const char *wifi_password);
    void connectToWifi() const;
    void sendData(SensorDataParsing& sensordata);
    void setServer(String& serverName);

    private:
    HTTPClient _httpClient;
    const char* _ssid;
    const char* _password;
    String _serverName;
};


#endif //PARSER_H
