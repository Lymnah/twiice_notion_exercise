#ifndef SENSORDATAPROCESSOR_H
#define SENSORDATAPROCESSOR_H

#include <QObject>
#include "iwkv.h"

class SensorDataProcessor : public QObject
{
    Q_OBJECT

public:
    static void resampleData(IWKV *base_sensor,
                             IWKV *resampled_sensor,
                             int target_rate,
                             double start_time_s,
                             double end_time_s);
};

#endif // SENSORDATAPROCESSOR_H
