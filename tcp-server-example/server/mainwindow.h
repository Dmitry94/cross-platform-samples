#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QtNetwork/QNetworkSession>
#include <QtNetwork/QTcpServer>


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
    void session_opened();
    void new_connection_established();
    void ready_read();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    QNetworkSession *session;
    QTcpServer *server;
};

#endif // MAINWINDOW_H
