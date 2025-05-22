//
// Created by bloom on 21.05.2025.
//
#ifndef ALARMMODULE_H
#define ALARMMODULE_H
#include <memory>
#include "OLEDscreen.h"


class AlarmModule {
    public:
    AlarmModule();
    void alert(float heartrate, float temperature); // need to work on mpu6050 data collection to write this function

    private:
};



#endif //ALARMMODULE_H
