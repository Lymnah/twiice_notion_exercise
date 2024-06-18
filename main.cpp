#include <QApplication>
#include "boost_example.h"
#include "mainwindow.h"
#include "wkv.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    testBoostLib();

    WKV hip_angle_sensor("hip_sensor", "deg");
    WKV imu_sensor("3-axis IMU", "deg/s");

    w.show();
    return a.exec();
}
