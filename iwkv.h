#ifndef IWKV_H
#define IWKV_H
#include <QObject>
#include "qtmetamacros.h"
#include <string>
#include <vector>

/**
 * @brief The IWKV interface defines the required functionalities for sensor data handling classes.
 * This includes data generation, retrieval, and setting of sensor attributes.
 */
class IWKV : public QObject
{
    Q_OBJECT

signals:
    void sensorDataReady(const IWKV &wkv);

public:
    virtual ~IWKV() {}

    /**
     * @brief Pure virtual function to generate sensor data based on the specified parameters.
     * 
     * @param frequency The frequency at which data points should be generated.
     * @param jitter The percentage of jitter to apply to the timing of data generation.
     * @param duration_seconds The duration in seconds for which data should be generated.
     */
    virtual void generateData(int frequency,
                              double jitter,
                              int duration_seconds,
                              std::optional<IWKV *> other_sensor_ptr = std::nullopt)
        = 0;

    /**
     * @brief Get the name of the sensor.
     * 
     * @return std::string The name of the sensor.
     */
    virtual std::string getName() const = 0;

    /**
     * @brief Get the frequency of the sensor.
     * 
     * @return int the frequency in Hertz.
     */
    virtual int getFrequency() const = 0;

    /**
     * @brief Get the unit of measurement for the sensor data.
     * 
     * @return std::string The unit of measurement.
     */
    virtual std::string getUnit() const = 0;

    /**
     * @brief Get the start time of the data series in microseconds.
     * 
     * @return uint64_t The start time in microseconds.
     */
    virtual uint64_t getStartTimeUs() const = 0;

    /**
     * @brief Set the name of the sensor.
     * 
     * @param name The new name of the sensor.
     */
    virtual void setName(const std::string &name) = 0;

    /**
     * @brief Set the unit of measurement for the sensor data.
     * 
     * @param unit The new unit of measurement.
     */
    virtual void setUnit(const std::string &unit) = 0;

    /**
     * @brief Set the start time for the data series in microseconds.
     * 
     * @param start_time_us The new start time in microseconds.
     */
    virtual void setStartTimeUs(uint64_t start_time_us) = 0;

    /**
     * @brief Add a data point to the series with a specific timestamp and value.
     * 
     * @param epoch_us The timestamp in microseconds when the data was recorded.
     * @param value The value of the data point.
     */
    virtual void addDataPoint(const uint64_t epoch_us, const double value) = 0;

    /**
     * @brief Get the timestamps for the data points in microseconds.
     * 
     * @return const std::vector<uint64_t>& A constant reference to the vector of timestamps.
     */
    virtual const std::vector<uint64_t> &getTimestampsUs() const = 0;

    /**
     * @brief Get the data series values.
     * 
     * @return const std::vector<double>& A constant reference to the vector of data values.
     */
    virtual const std::vector<double> &getData() const = 0;

    /**
     * @brief Set the data series values.
     */
    virtual void setData(const std::vector<double> &data) = 0;

    /**
     * @brief Copy data from one iwkv instance to another. Was force to do so as copy constructor are deleted in Q_Object classes...
     */
    virtual void copyFrom(const IWKV &other) = 0;
};

#endif // IWKV_H
