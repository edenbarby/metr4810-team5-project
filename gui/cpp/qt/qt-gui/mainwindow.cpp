#include "mainwindow.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // Open camera capture device.
    this->camera_ = new cv::VideoCapture(0);

    cv::Mat frame;
    this->camera_->read(frame);

    // Resize MainWindow to accomadate frames.
    this->resize(frame.cols+50, frame.rows+50);

    // Setup the label as the display window for the webcam frames.
    this->frame_display_ = new QLabel(this);
    this->frame_display_->setGeometry(25, 25, frame.cols, frame.rows);

    // Setup timer.
    this->timer_ = new QTimer(this);
    connect(this->timer_, SIGNAL(timeout()), this, SLOT(displayFrame()));
    this->timer_->start(40);
}

void MainWindow::displayFrame(void)
{
    cv::Mat frame;
    this->camera_->read(frame);

    cv::cvtColor(frame, frame, CV_RGB2BGR);

    QImage image = QImage((uchar *)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

    this->frame_display_->setPixmap(QPixmap::fromImage(image));
}
