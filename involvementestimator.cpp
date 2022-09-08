#include "enetb08bestafewwrapper.h"
#include "facedetectionwrapper.h"
#include "involvementestimator.h"
#include "modelexecutor.h"
#include "screencapture.h"

#include <iostream>

/*
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
*/
#include <QDebug>
#include <QGuiApplication>
#include <QPixmap>

#include "mtcnn/detector.h"

InvolvementEstimator::InvolvementEstimator(QObject *parent)
    : QObject(parent)
{
}

void InvolvementEstimator::run(int x, int y, int width, int height)
{
    static int counter = 0;
    counter++;
    try {
        auto screenPixmap = ScreenCapture::capture(x, y, width, height);
        /*ModelExecutor executor;
        executor.loadModel(EnetB08BestAfewWrapper::getModelPath());
        QPixmap face("/Users/echuraev/Workspace/HSE/InvolvementEstimator/InvolvementEstimator/models/face_224_224.png");
        auto input = EnetB08BestAfewWrapper::getInputTensor(face);
        auto output = EnetB08BestAfewWrapper::getOutputTensor();
        executor.run(input, output);
        auto emotion = EnetB08BestAfewWrapper::classifyEmition(output);
        qDebug() << "Result: " << emotion.c_str();*/
        /*ModelExecutor executor;
        executor.loadModel(FaceDetectionWrapper::getModelPath());
        auto input = FaceDetectionWrapper::getInputTensor(screenPixmap);
        auto output = EnetB08BestAfewWrapper::getOutputTensor();
        executor.run(input, output);*/

        std::string modelDir = "/Users/echuraev/Workspace/HSE/InvolvementEstimator/InvolvementEstimator/opencv-mtcnn/data/models/";
        ProposalNetwork::Config pConfig;
          pConfig.caffeModel = modelDir + "det1.caffemodel";
          pConfig.protoText = modelDir + "det1.prototxt";
          pConfig.threshold = 0.6f;

          RefineNetwork::Config rConfig;
          rConfig.caffeModel = modelDir + "det2.caffemodel";
          rConfig.protoText = modelDir + "det2.prototxt";
          rConfig.threshold = 0.7f;

          OutputNetwork::Config oConfig;
          oConfig.caffeModel = modelDir + "det3.caffemodel";
          oConfig.protoText = modelDir + "det3.prototxt";
          oConfig.threshold = 0.7f;

          MTCNNDetector detector(pConfig, rConfig, oConfig);
          //QImage test("/Users/echuraev/Nextcloud/pic.jpg");
          //test.convertTo(QImage::Format_RGB888);
          //cv::Mat img(test.height(), test.width(), CV_8UC3, (cv::Scalar*)test.rgbSwapped().scanLine(0));
          //cv::Mat img = cv::imread("/Users/echuraev/Nextcloud/pic.jpg");
          //std::cout << img << std::endl;
          //exit(123);
          QImage qImg = screenPixmap.toImage();
          qImg.convertTo(QImage::Format_RGB888);
          cv::Mat img(qImg.height(), qImg.width(), CV_8UC3, (cv::Scalar*)qImg.rgbSwapped().scanLine(0));
          //qDebug() << "img channels: " << img.channels() << ", w: " << img;
          std::vector<Face> faces;
          faces = detector.detect(img, 20.f, 0.709f);


        /*auto face_cascade_name = "/usr/local/share/opencv4/haarcascades/haarcascade_frontalface_alt2.xml";
        cv::CascadeClassifier face_cascade;
        if(!face_cascade.load(face_cascade_name))
            {
                qDebug() << "--(!)Error loading face cascade\n";
                throw std::runtime_error("bbbb");
            };
        std::vector<cv::Rect>faces;
        //cv::Mat img(screenPixmap.height(), screenPixmap.width(), CV_8UC3, (cv::Scalar*)screenPixmap.toImage().rgbSwapped().scanLine(0));

        cv::Mat img = cv::imread("/Users/echuraev/Nextcloud/pic.jpg");
        face_cascade.detectMultiScale(img, faces, 1.1, 4, cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));*/


          qDebug() << "Result: " << faces.size();
    } catch (...) {
    }
}
