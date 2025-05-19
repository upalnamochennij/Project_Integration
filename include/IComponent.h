#pragma once
class IComponent { //Interface that's going to be used as a blueprint for all the components

public:

  virtual void initComponent();
  virtual void calibrateComponent();
  //virtual bool componentStatus(); //true if component functions properly, false if overheating/any error
  virtual bool withinLimits(); // true if value is within the certain range, threshold. false otherwise
  virtual bool isActive();
  virtual void goSleepMode(); // if device is inactive for 30 seconds, goes to sleep mode (OLED screen for example)
  
  virtual ~IComponent() = default;


private:
  
};

