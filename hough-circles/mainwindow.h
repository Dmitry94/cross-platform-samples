#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class WorkerThread;

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
    void open_image();
    void save_image();
    void process_image();
    void cancel();
    void exit();
    void worker_started();
    void worker_finished();
    void worker_canceled();

private:
    Ui::MainWindow *ui;
    QImage original_image, result_image;
    WorkerThread *worker;

protected:
    virtual void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
