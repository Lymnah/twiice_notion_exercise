#include <QApplication>
#include "mainwindow.h"
#include "sensordataprocessor.h"
#include "wkvfactory.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // Create sensors using the factory method
    auto hip_angle_sensor = WKVFactory::createSensor("HIP", "hip_sensor");
    auto imu_sensor = WKVFactory::createSensor("IMU", "3-axis-IMU");

    // Ensure sensors were created successfully
    if (!hip_angle_sensor || !imu_sensor) {
        std::cerr << "Failed to create sensors." << std::endl;
        return -1; // Exit if sensors weren't created
    }

    QObject::connect(hip_angle_sensor.get(), &IWKV::sensorDataReady, &w, &MainWindow::updateUI);
    QObject::connect(imu_sensor.get(), &IWKV::sensorDataReady, &w, &MainWindow::updateUI);

    // Sensor data generation
    hip_angle_sensor->generateData(1000, 0.02, 20);
    imu_sensor->generateData(400, 0.03, 20, hip_angle_sensor.get());

    // Resampling to 100Hz
    auto hip_angle_resampled_sensor = WKVFactory::createSensor("HIP", "hip_sensor_resampled");
    auto imu_resampled_sensor = WKVFactory::createSensor("IMU", "3-axis-IMU-resampled");
    // Ensure sensors were created successfully
    if (!hip_angle_resampled_sensor || !imu_resampled_sensor) {
        std::cerr << "Failed to create sensors." << std::endl;
        return -1; // Exit if sensors weren't created
    }
    QObject::connect(hip_angle_resampled_sensor.get(),
                     &IWKV::sensorDataReady,
                     &w,
                     &MainWindow::updateUI);
    QObject::connect(imu_resampled_sensor.get(), &IWKV::sensorDataReady, &w, &MainWindow::updateUI);

    SensorDataProcessor processor;

    processor.resampleData(hip_angle_sensor.get(), hip_angle_resampled_sensor.get(), 100, 2.7, 4.8);
    processor.resampleData(imu_sensor.get(), imu_resampled_sensor.get(), 100, 2.7, 4.8);

    // Finding peaks
    QObject::connect(&processor,
                     &SensorDataProcessor::peaksDataReady,
                     &w,
                     &MainWindow::updateUIWithPeaks);

    const auto hip_peaks_window = processor.findPeaks(hip_angle_resampled_sensor.get(), 2.7, 4.8);

    w.show();
    return a.exec();
}
