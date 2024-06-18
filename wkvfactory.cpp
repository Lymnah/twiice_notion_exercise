#include "wkvfactory.h"
#include "hipsensor.h"
#include "imusensor.h"

std::unique_ptr<WKV> WKVFactory::createSensor(const std::string &type)
{
    if (type == "IMU") {
        return std::make_unique<IMUSensor>("3-axis IMU", "deg/s");
    } else if (type == "Hip") {
        return std::make_unique<HipSensor>("hip_sensor", "deg");
    } else {
        return nullptr; // Or throw an exception or handle errors as necessary
    }
}
