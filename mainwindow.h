#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "iwkv.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTabWidget *tabWidget;

public slots:
    void updateUI(const IWKV &wkv);
    void updateUIWithVelocities(const IWKV &wkv,
                                const std::vector<double> &velocities,
                                const std::vector<double> &accelerations);
    void updateUIWithPeaks(const IWKV &wkv,
                           const std::vector<uint64_t> &peaks,
                           const std::string &sensorId);
};
#endif // MAINWINDOW_H
