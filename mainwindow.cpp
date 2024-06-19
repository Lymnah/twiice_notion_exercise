#include "mainwindow.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateUI(const IWKV &wkv)
{
    updateUIWithVelocities(wkv, std::vector<double>(), std::vector<double>());
}

void MainWindow::updateUIWithVelocities(const IWKV &wkv,
                                        const std::vector<double> &velocities,
                                        const std::vector<double> &accelerations)
{
    QString sensorId = QString::fromStdString(wkv.getName());
    QChartView *chartView = tabWidget->findChild<QChartView *>(sensorId);

    if (!chartView) {
        // If the tab does not exist, create a new chart
        QChart *chart = new QChart();
        chart->setTitle(sensorId);

        QLineSeries *series = new QLineSeries();
        for (size_t i = 0; i < wkv.getTimestampsUs().size(); ++i) {
            series->append((wkv.getTimestampsUs()[i] - wkv.getStartTimeUs()) / 1e6,
                           wkv.getData()[i]);
        }

        chart->addSeries(series);

        QValueAxis *axisX = new QValueAxis;
        axisX->setTitleText("Time (s)");
        axisX->setTickCount(10);
        chart->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis *axisY = new QValueAxis;
        axisY->setTitleText("Value");
        chart->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);

        chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setObjectName(sensorId);
        tabWidget->addTab(chartView, sensorId);
    } else {
        // If the tab exists, update the existing chart
        QChart *chart = chartView->chart();
        chart->removeAllSeries();

        QLineSeries *series = new QLineSeries();
        for (size_t i = 0; i < wkv.getTimestampsUs().size(); ++i) {
            series->append((wkv.getTimestampsUs()[i] - wkv.getStartTimeUs()) / 1e6,
                           wkv.getData()[i]);
        }

        chart->addSeries(series);
        series->attachAxis(chart->axes(Qt::Horizontal).first());
        series->attachAxis(chart->axes(Qt::Vertical).first());
    }

    // Create velocity chart
    QString velocitySensorId = "velocity_" + sensorId;
    QChartView *velocityChartView = tabWidget->findChild<QChartView *>(velocitySensorId);

    if (!velocities.empty()) {
        // If the velocity tab does not exist, create a new chart
        QChart *velocityChart = new QChart();
        velocityChart->setTitle("velocity_" + sensorId);

        QLineSeries *velocitySeries = new QLineSeries();
        for (size_t i = 0; i < velocities.size(); ++i) {
            velocitySeries->append((wkv.getTimestampsUs()[i] - wkv.getStartTimeUs()) / 1e6,
                                   velocities[i]);
        }

        velocityChart->addSeries(velocitySeries);

        QValueAxis *velocityAxisX = new QValueAxis;
        velocityAxisX->setTitleText("Time (s)");
        velocityAxisX->setTickCount(10);
        velocityChart->addAxis(velocityAxisX, Qt::AlignBottom);
        velocitySeries->attachAxis(velocityAxisX);

        QValueAxis *velocityAxisY = new QValueAxis;
        velocityAxisY->setTitleText("Velocity");
        velocityChart->addAxis(velocityAxisY, Qt::AlignLeft);
        velocitySeries->attachAxis(velocityAxisY);

        velocityChartView = new QChartView(velocityChart);
        velocityChartView->setRenderHint(QPainter::Antialiasing);
        velocityChartView->setObjectName(velocitySensorId);
        tabWidget->addTab(velocityChartView, velocitySensorId);
    }

    // Create acceleration chart
    QString accelerationSensorId = "acceleration_" + sensorId;
    QChartView *accelerationChartView = tabWidget->findChild<QChartView *>(accelerationSensorId);

    if (!accelerations.empty()) {
        // If the acceleration tab does not exist, create a new chart
        QChart *accelerationChart = new QChart();
        accelerationChart->setTitle("acceleration_" + sensorId);

        QLineSeries *accelerationSeries = new QLineSeries();
        for (size_t i = 0; i < accelerations.size(); ++i) {
            accelerationSeries->append((wkv.getTimestampsUs()[i] - wkv.getStartTimeUs()) / 1e6,
                                       accelerations[i]);
        }

        accelerationChart->addSeries(accelerationSeries);

        QValueAxis *accelerationAxisX = new QValueAxis;
        accelerationAxisX->setTitleText("Time (s)");
        accelerationAxisX->setTickCount(10);
        accelerationChart->addAxis(accelerationAxisX, Qt::AlignBottom);
        accelerationSeries->attachAxis(accelerationAxisX);

        QValueAxis *accelerationAxisY = new QValueAxis;
        accelerationAxisY->setTitleText("Acceleration");
        accelerationChart->addAxis(accelerationAxisY, Qt::AlignLeft);
        accelerationSeries->attachAxis(accelerationAxisY);

        accelerationChartView = new QChartView(accelerationChart);
        accelerationChartView->setRenderHint(QPainter::Antialiasing);
        accelerationChartView->setObjectName(accelerationSensorId);
        tabWidget->addTab(accelerationChartView, accelerationSensorId);
    }
}

void MainWindow::updateUIWithPeaks(const IWKV &wkv,
                                   const std::vector<uint64_t> &peaks,
                                   const std::string &sensorId)
{
    QString qSensorId = QString::fromStdString(sensorId);
    QChartView *chartView = tabWidget->findChild<QChartView *>(qSensorId);
    if (chartView) {
        QChart *chart = chartView->chart();

        // Safely cast the axis to QValueAxis
        QValueAxis *axisY = dynamic_cast<QValueAxis *>(chart->axisY());
        if (!axisY) {
            qDebug() << "Error: Y-axis is not a QValueAxis.";
            return;
        }

        for (auto peakTime : peaks) {
            double timeInSeconds = (peakTime - wkv.getStartTimeUs()) / 1e6;
            QLineSeries *lineSeries = new QLineSeries();
            lineSeries->append(timeInSeconds, axisY->min());
            lineSeries->append(timeInSeconds, axisY->max());
            lineSeries->setName("Peak at " + QString::number(timeInSeconds) + "s");

            chart->addSeries(lineSeries);
            lineSeries->attachAxis(chart->axes(Qt::Horizontal).first());
            lineSeries->attachAxis(axisY);
        }
    }
}
