#include "imusensor.h"

IMUSensor::IMUSensor(const std::string &name, const std::string &unit)
    : WKV(name, unit)
{}

void IMUSensor::generateData(int frequency, double jitter, int duration_seconds) {}
