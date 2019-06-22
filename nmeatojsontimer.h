#ifndef NMEATOJSONTIMER_H
#define NMEATOJSONTIMER_H

#include <QTimer>

class MyTimer : public QObject
{
    Q_OBJECT
public:
    MyTimer();
    QTimer *timer;

public slots:
    void MyTimerSlot();
};

#endif // NMEATOJSONTIMER_H
