#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "../../lib/ftd2xx.h"
#include "opencv2/opencv.hpp"

#include <QLabel>
#include <QMainWindow>
#include <QTimer>

#include <iostream>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:

private slots:

    void displayFrame(void);

private:

    cv::VideoCapture *camera_;
    QLabel *frame_display_;
    QTimer *timer_;
};

#endif // MAINWINDOW_HPP
