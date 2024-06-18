#include "mainwindow.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
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
    for (int i = 0; i < wkv.getTimestampsUs().size(); ++i) {
        series->append((wkv.getTimestampsUs()[i] - wkv.getStartTimeUs()) / 1e6, wkv.getData()[i]);
    }

    chart->addSeries(series);
    QValueAxis *axisX = new QValueAxis;
    axisX->setTitleText("Time (s)");
    axisX->setTickCount(21);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setTitleText("Value");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    tabWidget->addTab(chartView, chart->title());
}
