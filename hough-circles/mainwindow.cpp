#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QFileDialog>
#include <QStandardPaths>
#include <QImageReader>
#include <QImageWriter>
#include <QMessageBox>
#include <QCloseEvent>

#include <convertions.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "worker-thread.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    move(100, 100);
    resize(800, 600);

    connect(ui->actionOpen, SIGNAL(triggered(bool)), this,
            SLOT(open_image()));
    connect(ui->actionSave, SIGNAL(triggered(bool)), this,
            SLOT(save_image()));
    connect(ui->actionExit, SIGNAL(triggered(bool)), this,
            SLOT(exit()));
    connect(ui->actionFind_circles, SIGNAL(triggered(bool)), this,
            SLOT(process_image()));
    connect(ui->actionStop, SIGNAL(triggered(bool)), this,
            SLOT(cancel()));

    ui->originalImage->setStyleSheet("QLabel { background-color : white; }");
    ui->processedImage->setStyleSheet("QLabel { background-color : white; }");
    ui->actionStop->setEnabled(false);

    this->worker = new WorkerThread(this);
    connect(worker, SIGNAL(started()), this, SLOT(worker_started()));
    connect(worker, SIGNAL(finished()), this, SLOT(worker_finished()));
    connect(worker, SIGNAL(canceled()), this, SLOT(worker_canceled()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    worker->stop();
}

static void init_image_file_dialog(QFileDialog &dialog,
                                   QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = (
                    QStandardPaths::standardLocations(
                        QStandardPaths::PicturesLocation));
        dialog.setDirectory(picturesLocations.isEmpty() ?
                            QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = (
                acceptMode == QFileDialog::AcceptOpen
                ? QImageReader::supportedMimeTypes() :
                  QImageWriter::supportedMimeTypes());
    foreach (const QByteArray &mimeTypeName, supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}

void MainWindow::open_image() {
    QFileDialog dialog(this, tr("Open File"));
    init_image_file_dialog(dialog, QFileDialog::AcceptOpen);
    int code = dialog.exec();
    if (code != QFileDialog::Accepted) {
        return;
    }
    QString file_path = dialog.selectedFiles().first();
    if (!file_path.isEmpty()) {
        worker->load_file(file_path);
        this->result_image = QImage();
        ui->processedImage->setPixmap(QPixmap::fromImage(result_image));
    }
}

void MainWindow::save_image() {
    if (result_image.isNull()) {
        return;
    }

    QFileDialog dialog(this, tr("Save File As"));
    init_image_file_dialog(dialog, QFileDialog::AcceptSave);
    int code = dialog.exec();
    if (code != QFileDialog::Accepted) {
        return;
    }
    QString file_path = dialog.selectedFiles().first();
    QImageWriter writer(file_path);
    if (!writer.write(result_image)) {
       QMessageBox::information(
                   this, QGuiApplication::applicationDisplayName(),
                   tr("Cannot write %1: %2").arg(QDir::toNativeSeparators(
                                                     file_path)),
                   writer.errorString());
       return;
    }
    const QString message = tr("Wrote \"%1\"").arg(
                QDir::toNativeSeparators(file_path));
    statusBar()->showMessage(message);
}

void MainWindow::exit() {
    close();
}

void MainWindow::process_image() {
    if (!original_image.isNull()) {
        worker->find_circles(original_image);
    }
}

void MainWindow::worker_started() {
    ui->actionOpen->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->actionFind_circles->setEnabled(false);
    ui->actionStop->setEnabled(true);
    QApplication::setOverrideCursor(Qt::WaitCursor);
}

void MainWindow::worker_finished() {
    ui->actionOpen->setEnabled(true);
    ui->actionSave->setEnabled(true);
    ui->actionFind_circles->setEnabled(true);
    ui->actionStop->setEnabled(false);
    QApplication::restoreOverrideCursor();

    if (worker->is_canceled) {
        worker->is_canceled = false;
        return;
    }

    const QImage *r_image = worker->get_result_image();
    if (r_image) {
        cv::Mat image_mat = conv::cvmat_from_qimage(*r_image);
        cv::resize(image_mat, image_mat, cv::Size(640, 480));
        QImage show_image = conv::cvmat_to_qimage(image_mat);
        if (worker->operation == WorkerThread::LoadFile) {
            original_image = *r_image;
            ui->originalImage->setPixmap(QPixmap::fromImage(show_image));
        } else {
            this->result_image = *r_image;
            ui->processedImage->setPixmap(QPixmap::fromImage(show_image));
        }
    }
}

void MainWindow::cancel()
{
    worker->stop();
}

void MainWindow::worker_canceled()
{
    worker->is_canceled = true;
}
