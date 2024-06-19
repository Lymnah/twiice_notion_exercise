#ifndef HIP_SENSOR_H
#define HIP_SENSOR_H

#include "wkv.h"

class HipSensor : public WKV
{
public:
    HipSensor(const std::string &name, const std::string &unit);
    void generateData(int frequency,
                      double jitter,
                      int duration_seconds,
                      std::optional<IWKV *> other_sensor_ptr = std::nullopt) override;
};

#endif // HIP_SENSOR_H
