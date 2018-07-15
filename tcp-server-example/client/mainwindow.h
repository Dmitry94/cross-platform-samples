#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkSession>
#include <QtNetwork/QTcpSocket>

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
    void send_clicked();
    void connected();
    void ready_to_read();

private:
    Ui::MainWindow *ui;
    QNetworkSession *session;
    QTcpSocket *socket;
};

#endif // MAINWINDOW_H
