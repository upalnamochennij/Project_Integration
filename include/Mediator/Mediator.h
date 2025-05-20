// Mediator.h
#pragma once
#include <string>

class BaseComponent;
class OLEDscreen;
class HeartrateSensor;

class Mediator {
public:
    virtual ~Mediator() = default;
    virtual void notify(BaseComponent* sender, std::string event) = 0;

private:
    OLEDscreen* _oledScreen = nullptr;
    HeartrateSensor* _heartrateSensor = nullptr;
};