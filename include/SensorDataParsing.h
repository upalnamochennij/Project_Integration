//
// Created by bloom on 27.05.2025.
//

#ifndef SENSORDATAPARSING_H
#define SENSORDATAPARSING_H
#include <cstdint>
#include <string>

struct SensorDataParsing {
    std::string dataType;
    int32_t sp02;
    int32_t heartrate;
    float temperature;
    float accel_x, accel_y, accel_z;
    float gyro_x, gyro_y, gyro_z;

    enum dataType{
        HEARTRATE_AND_SP02,
        TEMPERATURE,
        ACCELERATION_AND_GYRO
    };
    enum dataType dTypeEnum;
};

#endif //SENSORDATAPARSING_H
