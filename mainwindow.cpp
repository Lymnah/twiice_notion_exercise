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
    QChart *chart = new QChart();
    chart->setTitle(QString::fromStdString(wkv.getName()));

    QLineSeries *series = new QLineSeries();
    for (size_t i = 0; i < wkv.getTimestampsUs().size(); ++i) {
        series->append((wkv.getTimestampsUs()[i] - wkv.getStartTimeUs()) / 1e6, wkv.getData()[i]);
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

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setObjectName(
        QString::fromStdString(wkv.getName())); // Set object name to sensor name
    tabWidget->addTab(chartView, QString::fromStdString(wkv.getName()));
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
