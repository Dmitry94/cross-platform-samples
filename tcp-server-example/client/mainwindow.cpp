#include <QtNetwork/QNetworkSession>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QDataStream>
#include <QMessageBox>
#include <QFileInfo>


#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    move(200, 200);
    setWindowTitle("Client");
    connect(ui->pushButton_get_file, SIGNAL(clicked()), this,
            SLOT(send_clicked()));

    QNetworkSession *session = Q_NULLPTR;
    QNetworkConfigurationManager manager;
    if (manager.capabilities() &
            QNetworkConfigurationManager::NetworkSessionRequired) {
        QNetworkConfiguration config = manager.defaultConfiguration();
        session = new QNetworkSession(config, this);
    }
    if (session) {
        connect(session, &QNetworkSession::opened, this,
                &MainWindow::session_opened);
        session->open();
    } else {
        session_opened();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete session;
    delete socket;
}

void MainWindow::session_opened() {
    socket = new QTcpSocket(this);
    connect(socket, &QAbstractSocket::connected, this, &MainWindow::connected);
    connect(socket, &QIODevice::readyRead, this, &MainWindow::ready_to_read);
}

void MainWindow::send_clicked() {
    ui->pushButton_get_file->setEnabled(false);
    socket->connectToHost(ui->lineEdit_address->text(),
                          ui->spinBox_port->value());
}

void MainWindow::connected() {
    QDataStream inout(socket);
    inout << ui->lineEdit_fpath->text();
}

void MainWindow::ready_to_read() {
    QDataStream inout(socket);
    inout.setVersion(QDataStream::Qt_5_5);
    inout.startTransaction();
    QString message;
    inout >> message;

    if (message == "404") {
        QMessageBox Msgbox;
        Msgbox.setText("Requested file isn't exist.");
        Msgbox.exec();
    } else {
        quint32 block_size = 0;
        inout >> block_size;
        if (block_size == 0) {
            ui->pushButton_get_file->setEnabled(true);
            return;
        }

        QFileInfo fileInfo(ui->lineEdit_fpath->text());
        QString filename(fileInfo.fileName());
        QFile target(filename);
        if(!(target.open(QIODevice::WriteOnly)))
            QMessageBox::information(this, tr("File"),
                                     tr("File cannot be opened."));
        QDataStream dataStrmForFile(&target);
        quint8 byte;
        for (quint32 i = 0; i < block_size; i++) {
            inout >> byte;
            dataStrmForFile << byte;
        }
        target.close();
    }

    if (!inout.commitTransaction()) {
        ui->pushButton_get_file->setEnabled(true);
        return;
    }
    qDebug() << "Get :" << message;
    socket->disconnectFromHost();
    ui->pushButton_get_file->setEnabled(true);
}
