//
// Created by bloom on 27.05.2025.
//

#ifndef SENSORDATAPARSING_H
#define SENSORDATAPARSING_H
#include <cstdint>
#include <string>

struct SensorDataParsing {
    char dataType[30];
    int32_t sp02;
    int32_t heartrate;
    int temperature;
    int accel_x, accel_y, accel_z;
    int gyro_x, gyro_y, gyro_z;
    char datetime[30];
    int steps;

    enum dataType{
        HEARTRATE_AND_SP02,
        TEMPERATURE,
        ACCELERATION_AND_GYRO,
        STEPS
    } dTypeEnum;

    void reset() { //reset everything in the instance
        dataType[0] = '\0';
        datetime[0] = '\0';
        sp02 = 0;
        heartrate = 0;
        temperature = 0;
        accel_x = 0;
        accel_y = 0;
        accel_z = 0;
        gyro_x = 0;
        gyro_y = 0;
        gyro_z = 0;
        steps = 0;
    }
};

#endif //SENSORDATAPARSING_H
