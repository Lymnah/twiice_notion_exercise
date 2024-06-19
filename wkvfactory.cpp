#include "wkvfactory.h"
#include "hipsensor.h"
#include "imusensor.h"

std::unique_ptr<WKV> WKVFactory::createSensor(const std::string &type, const std::string &name)
{
    if (type == "IMU") {
        return std::make_unique<IMUSensor>(name, "deg/s");
    } else if (type == "HIP") {
        return std::make_unique<HipSensor>(name, "deg");
    } else {
        return nullptr; // Or throw an exception or handle errors as necessary
    }
}
