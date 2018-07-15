#include <QApplication>
#include "mainwindow.h"


int main(int nArgC, char *apszArgV[]) {
    QApplication app(nArgC, apszArgV);
    MainWindow w;
    w.show();
    app.exec();
    return 0;
}
