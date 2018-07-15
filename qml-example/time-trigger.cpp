#include <QQmlComponent>
#include <QTime>

#include "time-trigger.h"

TimeTrigger::TimeTrigger(QObject *obj) {
    this->obj = obj;
}

void TimeTrigger::update_time() {
    QTime cur_time = QTime::currentTime();

    obj->setProperty("hours", cur_time.hour());
    obj->setProperty("minutes", cur_time.minute());
    obj->setProperty("seconds", cur_time.second());
}
