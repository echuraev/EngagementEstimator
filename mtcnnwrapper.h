#ifndef MTCNNWRAPPER_H
#define MTCNNWRAPPER_H

#include "mtcnn/detector.h"

#include <vector>

class QPixmap;

class MTCNNWrapper
{
public:
    MTCNNWrapper();
    std::vector<Face> detect(const QPixmap input);

private:
    const std::string m_modelDir = "opencv-mtcnn/data/models/";
    const ProposalNetwork::Config m_pConfig = {m_modelDir + "det1.prototxt", m_modelDir + "det1.caffemodel", 0.6f};
    const RefineNetwork::Config m_rConfig = {m_modelDir + "det2.prototxt", m_modelDir + "det2.caffemodel", 0.7f};
    const OutputNetwork::Config m_oConfig = {m_modelDir + "det3.prototxt", m_modelDir + "det3.caffemodel", 0.7f};
    MTCNNDetector m_detector;
};

#endif // MTCNNWRAPPER_H
