#ifndef WORKERTHREAD
#define WORKERTHREAD

#include <QImage>
#include <QThread>

#include "mainwindow.h"

class WorkerThread : public QThread
{
    Q_OBJECT

public:
    WorkerThread(MainWindow *window);
    ~WorkerThread();

public:
    const QImage *get_result_image() const;
    void load_file(const QString &file_path);
    void find_circles(const QImage &image);
    void stop();

    enum Operation {
        LoadFile,
        FindCircles
    };
    Operation operation;
    bool is_canceled;

signals:
    void canceled();

private:
    MainWindow *parent_window;
    QScopedPointer <QImage> result;
    QString file_path;
    QImage image;

    virtual void run();
};

#endif // WORKERTHREAD

