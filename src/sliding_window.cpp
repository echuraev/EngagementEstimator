#include "sliding_window.h"
#include <exception>
#include <limits>
#include <string>

namespace {
std::vector<float> max_func(const std::vector<std::vector<float>>& vec) {
    size_t vecNums = vec.size();
    size_t elemNums = vec[0].size();
    std::vector<float> res(elemNums, std::numeric_limits<float>::min());
    for (size_t i = 0; i < vecNums; ++i) {
        for (size_t j = 0; j < elemNums; ++j) {
            res[j] = std::max(res[j], vec[i][j]);
        }
    }
    return res;
}
}

SlidingWindow::SlidingWindow()
{

}

bool SlidingWindow::IsReady(int64_t faceId)
{
    if (!IsKeyExists(faceId))
        return false;
    if (m_windowMap[faceId].size() < m_windowWidth)
        return false;
    return true;
}

void SlidingWindow::AddFeatureVector(int64_t faceId, const tvm::runtime::NDArray& features) {
    const float* fData = static_cast<float*>(features->data);
    std::vector<float> data(fData, fData + 1280);
    if (!IsKeyExists(faceId)) {
        m_windowMap[faceId].resize(m_windowWidth, data);
        m_windowMapIdx[faceId] = 1;
        return;
    }
    if (m_windowMap[faceId].size() < m_windowWidth) {
        m_windowMap[faceId].push_back(data);
        m_windowMapIdx[faceId]++;
    } else {
        m_windowMapIdx[faceId] = (m_windowMapIdx[faceId] < m_windowWidth - 1) ? m_windowMapIdx[faceId] + 1 : 0;
        m_windowMap[faceId][m_windowMapIdx[faceId]] = data;
    }
}



tvm::runtime::NDArray SlidingWindow::GetSequence(int64_t faceId) {
    if (!IsKeyExists(faceId) || m_windowMap[faceId].size() < m_windowWidth) {
        throw std::runtime_error("The sequence for faceId " + std::to_string(faceId) + " is not ready");
    }
    const size_t elemsNum = 2560;
    const size_t resSize = m_windowWidth * elemsNum;

    auto stat_vec = max_func(m_windowMap[faceId]);
    //std::vector<float>> res(m_windowWidth, stat_vec);
    std::vector<float> res;
    res.reserve(resSize);
    for (size_t i = 0; i < m_windowWidth; ++i) {
        size_t idx = m_windowMapIdx[faceId] + i;
        idx = (idx < m_windowWidth) ? idx : idx - m_windowWidth;
        res.insert(res.end(), stat_vec.begin(), stat_vec.end());
        res.insert(res.end(), m_windowMap[faceId][idx].begin(), m_windowMap[faceId][idx].end());
        //res[i].insert(res[i].end(), m_windowMap[faceId][idx].begin(), m_windowMap[faceId][idx].end());
    }

    DLDevice devCPU{kDLCPU, 0};
    auto resTensor = tvm::runtime::NDArray::Empty({1, m_windowWidth, elemsNum}, DLDataType{kDLFloat, 32, 1}, devCPU);
    resTensor.CopyFromBytes(res.data(), resSize * sizeof(float));
    return resTensor;
}
