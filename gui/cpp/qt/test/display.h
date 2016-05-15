#ifndef DISPLAY_H
#define DISPLAY_H

#include <QApplication>
#include <QProgressBar>
#include <QPushButton>
#include <QSlider>
#include <QWidget>

class Display : public QWidget
{
    Q_OBJECT
public:
    explicit Display(QWidget *parent = 0);

signals:
    void maxPressCountReached(void);

private slots:
    void slotButtonClicked(bool checked);

private:
    int presscount;
    QPushButton *button;
};

#endif // DISPLAY_H
