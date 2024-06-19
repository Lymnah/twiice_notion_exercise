#include "sensordataprocessor.h"
#include <boost/math/interpolators/cardinal_cubic_b_spline.hpp>
#include <iostream>

void SensorDataProcessor::resampleData(IWKV *base_sensor,
                                       IWKV *resampled_sensor,
                                       int target_rate,
                                       double start_time_s,
                                       double end_time_s)
{
    if (!base_sensor || !resampled_sensor) {
        std::cerr << "Invalid sensor pointers provided." << std::endl;
        return;
    }

    // Ensure non-null pointers for safety
    if (end_time_s < start_time_s) {
        std::cerr << "Invalid start time or end time provided." << std::endl;
        return; // Optionally, throw an exception or handle error
    }

    const std::vector<uint64_t> &timestamps = base_sensor->getTimestampsUs();
    const std::vector<double> &data = base_sensor->getData();

    if (timestamps.empty() || data.empty()) {
        std::cerr << "Sensor data is empty." << std::endl;
        return;
    }

    // Use the actual start time from the sensor data generation
    uint64_t start_time_us = base_sensor->getStartTimeUs()
                             + static_cast<uint64_t>(start_time_s * 1e6);
    uint64_t end_time_us = start_time_us + static_cast<uint64_t>((end_time_s - start_time_s) * 1e6);

    // Initialize the spline using actual microsecond timestamps
    boost::math::interpolators::cardinal_cubic_b_spline<double>
        spline(data.begin(),
               data.end(),
               timestamps.front(),
               (timestamps.back() - timestamps.front()) / (timestamps.size() - 1));

    double time_step_s = 1.0 / target_rate;

    for (uint64_t current_time_us = start_time_us; current_time_us <= end_time_us;
         current_time_us += static_cast<uint64_t>(time_step_s * 1e6)) {
        if (current_time_us >= timestamps.front() && current_time_us <= timestamps.back()) {
            double interpolated_value = spline(current_time_us);
            resampled_sensor->addDataPoint(current_time_us, interpolated_value);
        }
    }

    resampled_sensor->setStartTimeUs(base_sensor->getStartTimeUs());
    emit resampled_sensor->sensorDataReady(*resampled_sensor);
}
