#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "opencv2/opencv.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QTimer *qTimer;

    cv::VideoCapture webcam;

    void displayFrame();
    QImage getQImageFromFrame(cv::Mat frame);
    cv::Mat captureFrame(void);
};

#endif // MAINWINDOW_H
