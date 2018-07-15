#include <QLoggingCategory>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QEventLoop>
#include <QtXml/QDomDocument>
#include <QMessageBox>
#include <QAbstractItemView>

#include <iostream>

#include "mainwindow.h"
#include "ui_mainwindow.h"

QString outText(const QDomElement &rcParent, const QString &rcChildName) {
    QDomElement element = rcParent.firstChildElement(rcChildName);
    if (element.isNull())
        return "";

    QDomNode node = element.firstChild();
    if (!node.isText())
        return "";
    QDomText section = node.toText();
    if (section.isNull())
        return "";

    return section.data();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    move(100, 100);

    hostName = "http://feeds.bbci.co.uk/news/rss.xml";
    manager.connectToHost(hostName);

    model = new QStandardItemModel();
    model->setHeaderData(0, Qt::Horizontal, "http://feeds.bbci.co.uk/news");
    ui->treeView->setModel(model);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->pushButton_update, SIGNAL(clicked()), this,
            SLOT(update()));
    update();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
}

void MainWindow::update() {
    QUrl url { hostName };
    QNetworkRequest request(url);

    QNetworkReply *pReply = manager.get(request);
    QEventLoop loop;
    QObject::connect(pReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    const int cnError = pReply->error();
    if (cnError != 0) {
        QString message = "HTTP error: " + cnError;
        QMessageBox::information(this, "Error", message);
        return;
    }

    QByteArray resData = pReply->readAll();
    QString strErrorMsg;
    int nErrorRow, nErrorCol;
    QDomDocument document;
    const bool cbSuccess = document.setContent(
                resData, true, &strErrorMsg, &nErrorRow, &nErrorCol);

    if (!cbSuccess) {
        QString message = ("XML error at " + QString(nErrorRow) +
                           " : " + QString(nErrorCol) +
                           ": " + QString(strErrorMsg));
        QMessageBox::information(this, "Error", message);
        return;
    }

    QDomElement elementRss = document.firstChildElement("rss");

    if (!elementRss.isNull()) {
        QDomElement elementChannel = elementRss.firstChildElement("channel");
        while (!elementChannel.isNull()) {
            QString ch_title = outText(elementChannel, "title");
            QDomElement elementItem = elementChannel.firstChildElement(
                        "item");

            setWindowTitle("Channel title: " + ch_title);
            while (!elementItem.isNull()) {
                auto title = outText(elementItem, "title");
                auto descr = outText(elementItem, "description");
                auto pub_date = outText(elementItem, "pubDate");
                auto link = outText(elementItem, "link");
                QStandardItem *titleitem = new QStandardItem(title);
                QStandardItem *descritem = new QStandardItem(
                            "Description: " + descr);
                QStandardItem *dateitem = new QStandardItem(
                            "Date: " + pub_date);
                QStandardItem *linkitem = new QStandardItem(
                            "Link: " + link);
                titleitem->appendRow(descritem);
                titleitem->appendRow(dateitem);
                titleitem->appendRow(linkitem);
                model->appendRow(titleitem);

                elementItem = elementItem.nextSiblingElement("item");
            }
            elementChannel = elementChannel.nextSiblingElement("channel");
        }
    }
    ui->treeView->setWordWrap(true);
    ui->treeView->setTextElideMode(Qt::ElideMiddle);
}
