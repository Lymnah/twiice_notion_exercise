#include "hipsensor.h"

HipSensor::HipSensor(const std::string &name, const std::string &unit)
    : WKV(name, unit)
{}

void HipSensor::generateData(int frequency, double jitter, int duration_seconds) {}
