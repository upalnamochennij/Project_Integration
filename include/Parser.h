//
// Created by bloom on 24.05.2025.
//
#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <HTTPClient.h>
#include <WiFi.h>
#include <ctime>

#include "SensorDataParsing.h"

class Parser{

    public:
    Parser(const char *wifi_name, const char *wifi_password);
    void connectToWifi() const;
    void sendData(SensorDataParsing& sensordata);
    void setServer(String& serverName);
    void setDevice(int device); //sets device_id field in a class
    std::string getCurrentTime();

    std::string timeStamp;


    private:
    HTTPClient _httpClient;
    const char* _ssid;
    const char* _password;
    String _serverName;
    int device_id;

    static const char api_certificate[];


    //мб понадобится

};


#endif //PARSER_H
