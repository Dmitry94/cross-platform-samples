#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlFileSelector>
#include <QQuickView>
#include <QQuickItem>

#include "time-trigger.h"

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc,argv);

    QQuickView view;
    view.connect(view.engine(), &QQmlEngine::quit, &app, &QCoreApplication::quit);
    new QQmlFileSelector(view.engine(), &view);
    view.setSource(QUrl("../clocks.qml"));
    if (view.status() == QQuickView::Error)
        return -1;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    QObject *item = view.rootObject()->childItems()[0];
    TimeTrigger time_trigger(item);
    QObject::connect(item, SIGNAL(qmlSignal()),
                     &time_trigger, SLOT(update_time()));
    view.show();
    return app.exec();
}
