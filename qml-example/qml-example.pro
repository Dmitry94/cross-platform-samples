TEMPLATE     = app

QT          += qml quick

SOURCES     += main.cpp \
    time-trigger.cpp
RESOURCES   += clocks.qrc

OTHER_FILES  += clocks.qml \
                content/*.png

HEADERS += \
    time-trigger.h

