#include <QStandardItemModel>
#include <QtNetwork/QNetworkSession>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QAbstractSocket>
#include <QDataStream>
#include <QFileInfo>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    move(100, 100);
    setWindowTitle("Server");

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

    model = new QStandardItemModel();
    ui->listView->setModel(model);

    QStandardItem *item = new QStandardItem("Init server");
    item->setEditable(false);
    model->appendRow(item);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
}

void MainWindow::session_opened() {
    server = new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this,
            &MainWindow::new_connection_established);
    if(!server->listen(QHostAddress::Any, 45569)) {
        QString error = QString("Error: ") + server->errorString();
        QStandardItem *item = new QStandardItem(error);
        item->setEditable(false);
        model->appendRow(item);
    }
}

void MainWindow::new_connection_established() {
    QTcpSocket *client_socket = server->nextPendingConnection();
    connect(client_socket, &QAbstractSocket::disconnected, client_socket,
            &QObject::deleteLater);
    connect(client_socket, &QIODevice::readyRead, this,
            &MainWindow::ready_read);
}

void MainWindow::ready_read() {
    QTcpSocket *client_socket = qobject_cast <QTcpSocket *> (sender());
    if (!client_socket) {
        return;
    }
    QDataStream inout(client_socket);
    inout.setVersion(QDataStream::Qt_5_5);
    inout.startTransaction();
    QString file_path;
    inout >> file_path;
    bool is_wrote = false;

    QFileInfo check_file(file_path);
    if (check_file.exists() && check_file.isFile()) {
        QFile file(file_path);
        if (!file.open(QIODevice::ReadOnly))
            inout << QString("404");
        inout << QString("Good");

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_5);
        QByteArray bytes = file.readAll();
        block.append(bytes);

        file.close();
        inout << (quint32)(block.size());

        qint64 x = 0;
        while(x < block.size()) {
            qint64 y = client_socket->write(block);
            x = x + y;
        }
        qDebug() << "Wrote: " << x;
        is_wrote = true;
    } else {
        inout << QString("404");
    }
    if (!inout.commitTransaction()) {
        return;
    }

    QString message = (client_socket->peerAddress().toString() +
                       "> Request file: " + file_path);
    QStandardItem *item = new QStandardItem(message);
    item->setEditable(false);
    model->appendRow(item);

    message = "";
    if (is_wrote) {
        message = client_socket->peerAddress().toString() + "> sent file.";
    } else {
        message = (client_socket->peerAddress().toString() +
                   "> file doesn't exist.");
    }
    item = new QStandardItem(message);
    item->setEditable(false);
    model->appendRow(item);

    client_socket->disconnectFromHost();
}
