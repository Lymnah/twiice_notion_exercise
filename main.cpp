#include <QApplication>
#include "boost_example.h"
#include "wkvfactory.h"

#include "mainwindow.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // Create sensors using the factory method
    auto hip_angle_sensor = WKVFactory::createSensor("Hip");
    auto imu_sensor = WKVFactory::createSensor("IMU");

    // Ensure sensors were created successfully
    if (!hip_angle_sensor || !imu_sensor) {
        std::cerr << "Failed to create sensors." << std::endl;
        return -1; // Exit if sensors weren't created
    }

    w.show();
    return a.exec();
}
