#pragma once
class IComponent { //Interface that's going to be used as a blueprint for all the components

public:

  virtual void initComponent() = 0;
  virtual void calibrateComponent() = 0;
  //virtual bool componentStatus(); //true if component functions properly, false if overheating/any error
  virtual bool withinLimits() = 0; // true if value is within the certain range, threshold. false otherwise
  virtual bool isActive() = 0;
  virtual void goSleepMode() = 0; // if device is inactive for 30 seconds, goes to sleep mode (OLED screen for example)
  virtual void wakeUp() = 0; // wake the device up (all the settings are saved)

  virtual ~IComponent() = default;


private:
  
};

