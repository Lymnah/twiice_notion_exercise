#ifndef WKV_H
#define WKV_H

#include <string>
#include <vector>

/**
 * @brief The WKV class stores sensor data along with their corresponding timestamps.
 */
class WKV
{
private:
    std::string name_;                    ///< Name of the sensor.
    std::string unit_;                    ///< Unit of measurement for the data.
    std::vector<uint64_t> timestamps_us_; ///< List of timestamps in microseconds.
    std::vector<double> data_;            ///< Data values corresponding to the timestamps.
    uint64_t start_time_us_;              ///< Start time of the data series in microseconds.

public:
    /**
     * @brief Construct a new WKV object.
     * 
     * @param name The name of the sensor.
     * @param unit The measurement unit of the data.
     */
    WKV(const std::string &name, const std::string &unit);

    /**
     * @brief Get the Name of the sensor.
     * 
     * @return std::string The name of the sensor.
     */
    std::string getName() const;

    /**
     * @brief Get the Unit of measurement.
     * 
     * @return std::string The unit of measurement.
     */
    std::string getUnit() const;

    /**
     * @brief Get the Start Time in microseconds.
     * 
     * @return uint64_t The start time in microseconds.
     */
    uint64_t getStartTimeUs() const;

    /**
     * @brief Set the Name of the sensor.
     * 
     * @param name The new name of the sensor.
     */
    void setName(const std::string &name);

    /**
     * @brief Set the Unit of measurement.
     * 
     * @param unit The new unit of measurement.
     */
    void setUnit(const std::string &unit);

    /**
     * @brief Set the Start Time in microseconds.
     * 
     * @param start_time_us The new start time in microseconds.
     */
    void setStartTimeUs(uint64_t start_time_us);

    /**
     * @brief Add a data point to the series.
     * 
     * @param epoch_us The timestamp in microseconds.
     * @param value The data value.
     */
    void addDataPoint(const uint64_t epoch_us, const double value);

    /**
     * @brief Get the Timestamps in microseconds.
     * 
     * @return const std::vector<uint64_t>& A constant reference to the timestamps vector.
     */
    const std::vector<uint64_t> &getTimestampsUs() const;

    /**
     * @brief Get the Data series values.
     * 
     * @return const std::vector<double>& A constant reference to the data vector.
     */
    const std::vector<double> &getData() const;
};

#endif // WKV_H
