#include "sliding_window.h"
#include <exception>

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
    if (!IsKeyExists(faceId)) {
        m_windowMap[faceId] = {};
        m_windowMapIdx[faceId] = 0;
    }
    if (m_windowMap[faceId].size() < m_windowWidth) {
        m_windowMap[faceId].push_back(features);
        m_windowMapIdx[faceId]++;
    } else {
        m_windowMapIdx[faceId] = (m_windowMapIdx[faceId] < m_windowWidth - 1) ? m_windowMapIdx[faceId] + 1 : 0;
        m_windowMap[faceId][m_windowMapIdx[faceId]] = features;
    }
}

tvm::runtime::NDArray SlidingWindow::GetSequence(int64_t faceId) {
    if (!IsKeyExists(faceId) || m_windowMap[faceId].size() < m_windowWidth) {
        throw std::runtime_error("The sequence for faceId " + std::to_string(faceId) + " is not ready");
        return {};
    }


    return {};
}
