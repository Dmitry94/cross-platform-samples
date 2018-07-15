#include <QImage>
#include <QPixmap>
#include <QDebug>

#include <opencv2/core/core.hpp>

#include "convertions.h"

namespace conv {

QImage cvmat_to_qimage(const cv::Mat& image) {
    switch (image.type()) {
         case CV_8UC4: {
            QImage qimage(image.data, image.cols, image.rows,
                         static_cast<int>(image.step),
                         QImage::Format_ARGB32);
            return qimage.copy();
         }

         case CV_8UC3: {
            QImage qimage(image.data, image.cols, image.rows,
                         static_cast<int>(image.step),
                         QImage::Format_RGB888);
            return qimage.rgbSwapped().copy();
         }

         case CV_8UC1: {
            QImage qimage(image.data, image.cols, image.rows,
                         static_cast<int>(image.step),
                         QImage::Format_Grayscale8);
            return qimage.copy();
         }

         default:
            qWarning() << "GUI::cvmat_to_qimage() - cv::Mat image type not "
                          "handled in switch:" << image.type();
            break;
      }

      return QImage();
}


QPixmap cvmat_to_qpixmap(const cv::Mat& image) {
    return QPixmap::fromImage(cvmat_to_qimage(image));
}


cv::Mat cvmat_from_qimage(const QImage& qimage) {
    switch (qimage.format()) {

       // 8-bit, 4 channel
       case QImage::Format_ARGB32:
       case QImage::Format_ARGB32_Premultiplied: {
          cv::Mat mat(qimage.height(), qimage.width(), CV_8UC4,
                      const_cast<uchar*>(qimage.bits()),
                      static_cast<size_t>(qimage.bytesPerLine()));

          return mat.clone();
       }

       // 8-bit, 3 channel
       case QImage::Format_RGB32:
       case QImage::Format_RGB888: {
          QImage swapped = qimage;
          if ( qimage.format() == QImage::Format_RGB32 ) {
             swapped = swapped.convertToFormat(QImage::Format_RGB888);
          }
          swapped = swapped.rgbSwapped();

          return cv::Mat(swapped.height(), swapped.width(), CV_8UC3,
                         const_cast<uchar*>(swapped.bits()),
                         static_cast<size_t>(swapped.bytesPerLine())).clone();
       }

       // 8-bit, 1 channel
       case QImage::Format_Indexed8: {
          cv::Mat mat(qimage.height(), qimage.width(), CV_8UC1,
                      const_cast<uchar*>(qimage.bits()),
                      static_cast<size_t>(qimage.bytesPerLine()));

          return mat.clone();
       }

       default:
          qWarning() << "GUI::cvmat_from_qimage() - QImage format "
                        "not handled in switch:" << qimage.format();
          break;
    }

    return cv::Mat();
}


cv::Mat cvmat_from_qpixmap(const QPixmap& qpixmap) {
    return cvmat_from_qimage(qpixmap.toImage());
}

} //namespace conv
