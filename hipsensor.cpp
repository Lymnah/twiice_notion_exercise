#include "hipsensor.h"
#include <chrono>
#include <random>

HipSensor::HipSensor(const std::string &name, const std::string &unit)
    : WKV(name, unit)
{}

void HipSensor::generateData(int frequency,
                             double jitter,
                             int duration_seconds,
                             std::optional<IWKV *> other_sensor_ptr)
{
    std::default_random_engine generator(
        static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::normal_distribution<double> jitter_distribution(1.0, jitter);
    std::normal_distribution<double> noise_distribution(0.0, 0.001); // Gaussian noise

    const uint64_t start_time_us = std::chrono::duration_cast<std::chrono::microseconds>(
                                       std::chrono::system_clock::now().time_since_epoch())
                                       .count();
    const uint64_t end_time_us = start_time_us
                                 + static_cast<uint64_t>(duration_seconds)
                                       * 1000000; // Convert seconds to microseconds
    uint64_t current_time_us = start_time_us;
    this->start_time_us_ = start_time_us; // Store the actual start time in the sensor object

    // Hip angle-specific parameters
    const double amplitude = 60.0;                 // Example amplitude in degrees
    const double max_change_per_ms = 220.0 / 1000; // Max degrees change per millisecond
    const double base_frequency = 1;               // Base frequency for hip movements

    while (current_time_us <= end_time_us) {
        const auto interval_us = static_cast<uint64_t>(
            1000000 / static_cast<double>(frequency)
            * jitter_distribution(generator)); // Apply jitter in microseconds
        const double time_in_seconds = (current_time_us - start_time_us) / 1e6;

        // Add a periodic component to simulate regular movements
        const double periodic_component = amplitude
                                          * std::sin(2 * M_PI * base_frequency * time_in_seconds);

        // Combine the components and add noise
        double sensor_value = periodic_component + noise_distribution(generator);

        addDataPoint(current_time_us, sensor_value);
        current_time_us += interval_us;
    }

    emit sensorDataReady(*this);
}
