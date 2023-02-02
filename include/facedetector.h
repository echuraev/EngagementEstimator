#ifndef FACEDETECTOR_H
#define FACEDETECTOR_H

#include <dlib/image_io.h>

#include <vector>

class QPixmap;

class FaceDetector
{
public:
    FaceDetector();
    std::vector<dlib::rectangle> detect(const QPixmap& input);
};

#endif // FACEDETECTOR_H
