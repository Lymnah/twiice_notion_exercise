#include "imusensor.h"
#include <random>

IMUSensor::IMUSensor(const std::string &name, const std::string &unit)
    : WKV(name, unit)
{}

void IMUSensor::generateData(int frequency,
                             double jitter,
                             int duration_seconds,
                             std::optional<IWKV *> other_sensor_ptr)
{
    if (!other_sensor_ptr) {
        throw std::invalid_argument("IMU sensor data generation requires a reference hip sensor.");
    }
    std::default_random_engine generator(
        static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::normal_distribution<double> jitter_distribution(1.0, jitter);
    std::normal_distribution<double> noise_distribution(0.0, 0.001); // Gaussian noise

    auto &hipSensor = other_sensor_ptr.value();
    const uint64_t start_time_us = hipSensor->getStartTimeUs();
    const uint64_t end_time_us = start_time_us
                                 + static_cast<uint64_t>(duration_seconds)
                                       * 1000000; // Convert seconds to microseconds
    uint64_t current_time_us = start_time_us;
    this->start_time_us_ = start_time_us; // Store the actual start time in the sensor object

    // Generate IMU data correlated with the hip sensor data
    double previous_hip_angle = 0.0;
    for (int i = 0; i < hipSensor->getTimestampsUs().size(); ++i) {
        if (hipSensor->getTimestampsUs()[i] > end_time_us)
            break;

        while (current_time_us <= hipSensor->getTimestampsUs()[i]) {
            const auto interval_us = static_cast<uint64_t>(
                1000000 / static_cast<double>(frequency)
                * jitter_distribution(generator)); // Apply jitter in microseconds

            // Calculate angular velocity as the derivative of the hip angle
            double angular_velocity = (hipSensor->getData()[i] - previous_hip_angle) * frequency
                                      + noise_distribution(generator);
            previous_hip_angle = hipSensor->getData()[i];

            addDataPoint(current_time_us, angular_velocity);
            current_time_us += interval_us;
        }
    }
    emit sensorDataReady(*this);
}
