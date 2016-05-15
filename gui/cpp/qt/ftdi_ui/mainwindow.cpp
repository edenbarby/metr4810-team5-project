#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    webcam.open(0);
    qTimer = new QTimer(this);
    qTimer->setInterval(1000/25);
    connect(qTimer, SIGNAL(timeout()), this, SLOT(displayFrame()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayFrame() {
    cv::Mat frame;

    //capture a frame from the webcam
    frame = captureFrame();
    image = getQImageFromFrame(frame);

    //set the image of the label to be the captured frame and resize the label appropriately
    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->label->resize(ui->label->pixmap()->size());
}

QImage MainWindow::getQImageFromFrame(cv::Mat frame) {
    //converts the color model of the image from RGB to BGR because OpenCV uses BGR
    cv::cvtColor(frame, frame, CV_RGB2BGR);
    return QImage((uchar*) (frame.data), frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
}

cv::Mat MainWindow::captureFrame(void)
{
    cv::Mat frame;

    //determine whether or not the webcam video stream was successfully initialized
//    if(!webcam.isOpened())
//    {
//        qDebug() << "Camera initialization failed.";
//    }

    //attempts to grab a frame from the webcam
    if (!webcam.grab()) {
//        qDebug() << "Failed to capture frame.";
    }

    //attempts to read the grabbed frame and stores it in frame
    if (!webcam.read(frame)) {
//        qDebug() << "Failed to read data from captured frame.";
    }

    return frame;
}
