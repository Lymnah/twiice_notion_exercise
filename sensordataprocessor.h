#ifndef SENSORDATAPROCESSOR_H
#define SENSORDATAPROCESSOR_H

#include <QObject>
#include "iwkv.h"

class SensorDataProcessor : public QObject
{
    Q_OBJECT

public:
    void resampleData(IWKV *base_sensor,
                      IWKV *resampled_sensor,
                      int target_rate,
                      double start_time_s,
                      double end_time_s);

    std::vector<uint64_t> findPeaks(const IWKV *sensor, double start_time_s, double end_time_s);
    void calculateVelocityAndAcceleration(const IWKV &sensor,
                                          double start_time_s,
                                          double end_time_s,
                                          std::vector<double> &velocities,
                                          std::vector<double> &accelerations);
    void applyGaussianSmoothing(IWKV &sensor, int kernel_size, double sigma);

signals:
    void peaksDataReady(const IWKV &sensor,
                        const std::vector<uint64_t> &peaks,
                        const std::string &sensorId);
};

#endif // SENSORDATAPROCESSOR_H
