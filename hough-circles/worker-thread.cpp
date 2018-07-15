#include <cassert>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "worker-thread.h"
#include "convertions.h"

WorkerThread::WorkerThread(MainWindow *parent_window)
{
    this->parent_window = parent_window;
}

WorkerThread::~WorkerThread()
{ }

const QImage *WorkerThread::get_result_image() const
{
    assert(isFinished());
    return result.data();
}

void WorkerThread::load_file(const QString &file_path)
{
    if (isRunning())
        return;
    operation = LoadFile;
    result.reset(new QImage);
    this->file_path = file_path;
    QThread::start();
}

void WorkerThread::find_circles(const QImage &image)
{
    if (isRunning())
        return;
    operation = FindCircles;
    result.reset(new QImage(image));
    this->image = image;
    QThread::start();
}

void WorkerThread::stop()
{
    result.reset();
    requestInterruption();
    emit canceled();
}

void WorkerThread::run()
{
    switch (operation)
    {
        case LoadFile:
            if (result->load(file_path)) {
                result.reset(new QImage(*result));
            }
            this->file_path.clear();
            break;
        case FindCircles:
            if (!image.isNull()) {
                cv::Mat image_mat = conv::cvmat_from_qimage(image);
                cv::Mat result_mat = image_mat.clone();

                cv::cvtColor(image_mat, image_mat, CV_BGR2GRAY);
                cv::GaussianBlur(image_mat, image_mat, cv::Size(9, 9), 2, 2);
                cv::vector<cv::Vec3f> circles;
                cv::HoughCircles(image_mat, circles, CV_HOUGH_GRADIENT, 1,
                                 image_mat.rows / 8, 200, 100, 0, 0 );
                for(size_t i = 0; i < circles.size(); i++) {
                    cv::Point center(cvRound(circles[i][0]),
                            cvRound(circles[i][1]));
                    int radius = cvRound(circles[i][2]);
                    circle(result_mat, center, 3, cv::Scalar(0, 255, 0),
                           5, 8, 0);
                    circle(result_mat, center, radius, cv::Scalar(0, 0, 255),
                           5, 8, 0);
                }
                result.reset(new QImage(conv::cvmat_to_qimage(result_mat)));
            }
            break;
    }
}
