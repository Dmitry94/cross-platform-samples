#ifndef TIMETRIGGER_H
#define TIMETRIGGER_H

#include <QObject>

class TimeTrigger : public QObject
{
    Q_OBJECT
public:
    TimeTrigger(QObject *obj);
public slots:
    void update_time();

private:
    QObject *obj;
};

#endif // TIMETRIGGER_H
