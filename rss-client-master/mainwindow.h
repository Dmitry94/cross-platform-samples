#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void update();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    QNetworkAccessManager manager;
    QString hostName;
};

#endif // MAINWINDOW_H
