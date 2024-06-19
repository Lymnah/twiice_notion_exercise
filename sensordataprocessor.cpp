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

std::vector<uint64_t> SensorDataProcessor::findPeaks(const IWKV *sensor,
                                                     double start_time_s,
                                                     double end_time_s)
{
    const std::vector<uint64_t> &timestamps = sensor->getTimestampsUs();
    const std::vector<double> &data = sensor->getData();

    std::vector<uint64_t> peakTimestamps;

    uint64_t start_time_us = static_cast<uint64_t>(start_time_s * 1e6) + sensor->getStartTimeUs();
    uint64_t end_time_us = static_cast<uint64_t>(end_time_s * 1e6) + sensor->getStartTimeUs();

    // Ensure we have at least three points to compare (previous, current, next)
    if (data.size() < 3)
        return peakTimestamps;

    for (size_t i = 1; i < data.size() - 1; ++i) {
        if (timestamps[i] >= start_time_us && timestamps[i] <= end_time_us) {
            if (data[i] > data[i - 1] && data[i] > data[i + 1]) {
                peakTimestamps.push_back(timestamps[i]);
            }
        }
    }
    emit peaksDataReady(*sensor,
                        peakTimestamps,
                        sensor->getName()); // Assuming getName returns QString
    return peakTimestamps;
}

void SensorDataProcessor::calculateVelocityAndAcceleration(const IWKV &sensor,
                                                           double start_time_s,
                                                           double end_time_s,
                                                           std::vector<double> &velocities,
                                                           std::vector<double> &accelerations)
{
    const std::vector<uint64_t> &timestamps = sensor.getTimestampsUs();
    const std::vector<double> &positions = sensor.getData();

    velocities.clear();
    accelerations.clear();

    // Convert start and end time from seconds to microseconds
    uint64_t start_time_us = static_cast<uint64_t>(start_time_s * 1e6) + sensor.getStartTimeUs();
    uint64_t end_time_us = static_cast<uint64_t>(end_time_s * 1e6) + sensor.getStartTimeUs();

    // Initial indices for the start and end times
    size_t startIndex = 0, endIndex = timestamps.size() - 1;
    for (size_t i = 0; i < timestamps.size(); ++i) {
        if (timestamps[i] >= start_time_us) {
            startIndex = i;
            break;
        }
    }
    for (size_t i = startIndex; i < timestamps.size(); ++i) {
        if (timestamps[i] > end_time_us) {
            endIndex = i - 1;
            break;
        }
    }

    // Calculate velocity
    for (size_t i = startIndex; i < endIndex; ++i) {
        double dt = (timestamps[i + 1] - timestamps[i]) / 1e6; // delta time in seconds
        if (std::abs(dt) > 1e-5) { // Avoid division by zero or near-zero time intervals
            double dv = (positions[i + 1] - positions[i]) / dt;
            velocities.push_back(dv);
        }
    }

    // Calculate acceleration
    for (size_t i = 0; i < velocities.size() - 1; ++i) {
        double dt = (timestamps[startIndex + i + 1] - timestamps[startIndex + i])
                    / 1e6;         // delta time in seconds
        if (std::abs(dt) > 1e-5) { // Avoid division by zero or near-zero time intervals
            double da = (velocities[i + 1] - velocities[i]) / dt;
            accelerations.push_back(da);
        }
    }
}

void SensorDataProcessor::applyGaussianSmoothing(IWKV &sensor, int kernel_size, double sigma)
{
    std::vector<double> kernel(kernel_size);
    const auto &data = sensor.getData();
    int half_size = kernel_size / 2;
    double sum = 0.0;

    for (int i = 0; i < kernel_size; ++i) {
        int x = i - half_size;
        kernel[i] = exp(-0.5 * x * x / (sigma * sigma)) / (sqrt(2 * M_PI) * sigma);
        sum += kernel[i];
    }

    // Normalize the kernel
    for (int i = 0; i < kernel_size; ++i) {
        kernel[i] /= sum;
    }

    std::vector<double> smoothed(data.size(), 0.0);

    for (size_t i = 0; i < data.size(); ++i) {
        double sum = 0.0;
        for (int j = -half_size; j <= half_size; ++j) {
            int idx = i + j;
            if (idx >= 0 && idx < data.size()) {
                sum += data[idx] * kernel[half_size + j];
            }
        }
        smoothed[i] = sum;
    }

    sensor.setData(smoothed);
    emit sensor.sensorDataReady(sensor);
}
