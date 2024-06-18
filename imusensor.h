#ifndef IMU_SENSOR_H
#define IMU_SENSOR_H

#include "wkv.h"

class IMUSensor : public WKV
{
public:
    IMUSensor(const std::string &name, const std::string &unit);
    void generateData(int frequency, double jitter, int duration_seconds) override;
};

#endif // IMU_SENSOR_H
