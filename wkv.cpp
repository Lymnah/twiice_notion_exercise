#include "WKV.h"

/**
 * @brief Construct a new WKV object.
 * 
 * Initializes the sensor name and unit of the data series and sets the start time to zero.
 * 
 * @param name The name of the sensor.
 * @param unit The unit of measurement for the data series.
 */
WKV::WKV(const std::string &name, const std::string &unit)
    : name_(name)
    , unit_(unit)
    , start_time_us_(0)
{}

/**
 * @brief Get the sensor name.
 * 
 * @return std::string Returns the sensor name.
 */
std::string WKV::getName() const
{
    return name_;
}

/**
 * @brief Get the unit of measurement of the data series.
 * 
 * @return std::string Returns the unit of measurement of the data series.
 */
std::string WKV::getUnit() const
{
    return unit_;
}

/**
 * @brief Get the start time of the data series in microseconds.
 * 
 * @return uint64_t Returns the start time of the data series in microseconds.
 */
uint64_t WKV::getStartTimeUs() const
{
    return start_time_us_;
}

/**
 * @brief Set the name of the data series.
 * 
 * @param name The new name to set for the data series.
 */
void WKV::setName(const std::string &name)
{
    name_ = name;
}

/**
 * @brief Set the unit of measurement for the data series.
 * 
 * @param unit The new unit to set for the data series.
 */
void WKV::setUnit(const std::string &unit)
{
    unit_ = unit;
}

/**
 * @brief Set the start time of the data series in microseconds.
 * 
 * @param start_time_us The new start time to set for the data series.
 */
void WKV::setStartTimeUs(uint64_t start_time_us)
{
    start_time_us_ = start_time_us;
}

/**
 * @brief Adds a data point to the data series.
 * 
 * This method appends the provided epoch time and value to the timestamps and data vectors, respectively.
 * 
 * @param epoch_us The epoch time of the data point in microseconds.
 * @param value The value of the data point.
 */
void WKV::addDataPoint(const uint64_t epoch_us, const double value)
{
    timestamps_us_.push_back(epoch_us);
    data_.push_back(value);
}

/**
 * @brief Get a constant reference to the vector of timestamps.
 * 
 * @return const std::vector<uint64_t>& Returns a constant reference to the vector of timestamps in microseconds.
 */
const std::vector<uint64_t> &WKV::getTimestampsUs() const
{
    return timestamps_us_;
}

/**
 * @brief Get a constant reference to the vector of data values.
 * 
 * @return const std::vector<double>& Returns a constant reference to the vector of data values.
 */
const std::vector<double> &WKV::getData() const
{
    return data_;
}
