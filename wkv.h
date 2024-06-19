#ifndef WKV_H
#define WKV_H
#include "iwkv.h"

/**
 * @brief The WKV class stores sensor data along with their corresponding timestamps.
 */
class WKV : public IWKV
{
protected:
    std::string name_;                    ///< Name of the sensor.
    std::string unit_;                    ///< Unit of measurement for the data.
    int frequency_;                       ///< Frequency in Hertz
    std::vector<uint64_t> timestamps_us_; ///< List of timestamps in microseconds.
    std::vector<double> data_;            ///< Data values corresponding to the timestamps.
    uint64_t start_time_us_;              ///< Start time of the data series in microseconds.

public:
    /**
     * @brief Construct a new WKV object.
     * 
     * @param name The name of the sensor.
     * @param unit The measurement unit of the dat  a.
     */
    WKV(const std::string &name, const std::string &unit);

    virtual ~WKV() {}
    /**
     * @brief Get the Name of the sensor.
     * 
     * @return std::string The name of the sensor.
     */
    std::string getName() const override;

    /**
     * @brief Get the frequency of the sensor.
     * 
     * @return int the frequency in Hertz.
     */
    virtual int getFrequency() const override;

    /**
     * @brief Get the Unit of measurement.
     * 
     * @return std::string The unit of measurement.
     */
    std::string getUnit() const override;

    /**
     * @brief Get the Start Time in microseconds.
     * 
     * @return uint64_t The start time in microseconds.
     */
    uint64_t getStartTimeUs() const override;

    /**
     * @brief Set the Name of the sensor.
     * 
     * @param name The new name of the sensor.
     */
    void setName(const std::string &name) override;

    /**
     * @brief Set the Unit of measurement.
     * 
     * @param unit The new unit of measurement.
     */
    void setUnit(const std::string &unit) override;

    /**
     * @brief Set the Start Time in microseconds.
     * 
     * @param start_time_us The new start time in microseconds.
     */
    void setStartTimeUs(uint64_t start_time_us) override;

    /**
     * @brief Add a data point to the series.
     * 
     * @param epoch_us The timestamp in microseconds.
     * @param value The data value.
     */
    void addDataPoint(const uint64_t epoch_us, const double value) override;

    /**
     * @brief Get the Timestamps in microseconds.
     * 
     * @return const std::vector<uint64_t>& A constant reference to the timestamps vector.
     */
    const std::vector<uint64_t> &getTimestampsUs() const override;

    /**
     * @brief Get the Data series values.
     * 
     * @return const std::vector<double>& A constant reference to the data vector.
     */
    const std::vector<double> &getData() const override;

    /**
     * @brief Set the data series values.
     */
    virtual void setData(const std::vector<double> &data) override;

    /**
     * @brief Copy data from one iwkv instance to another. Was force to do so as copy constructor are deleted in Q_Object classes...
     */
    virtual void copyFrom(const IWKV &other) override;
};

#endif // WKV_H
