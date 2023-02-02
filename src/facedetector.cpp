#include "facedetector.h"

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/opencv.h>

#include <QPixmap>
#include <QDebug>

FaceDetector::FaceDetector()
{

}

std::vector<dlib::rectangle> FaceDetector::detect(const QPixmap& input)
{
    qDebug() << 11111111111111;
    QImage qImg = input.toImage();
    qImg.convertTo(QImage::Format_RGB888);
    /*cv::Mat img(qImg.height(), qImg.width(), CV_8UC3, (cv::Scalar*)qImg.rgbSwapped().scanLine(0));
    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
    //dlib::array2d<unsigned char> dlibImg;
    dlib::cv_image<dlib::bgr_pixel> cimg(img);*/
    qDebug() << 2222222222222222222;
    dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
    qDebug() << 333333333333333333;
    dlib::array2d<unsigned char> img;
    load_image(img, "/Users/echuraev/tmp.jpg");
    qDebug() << 44444444444444;
    while(img.size() < 1800*1800)
     pyramid_up(img);
    qDebug() << 444444555555;
    auto d = detector(img);
    qDebug() << 555555555555555555;
    return d;
}
