#ifndef SLIDINGWINDOW_H
#define SLIDINGWINDOW_H

#include <vector>
#include <unordered_map>

#undef slots
#include <tvm/runtime/module.h>
#define slots Q_SLOTS

class SlidingWindow
{
public:
    SlidingWindow();
    inline int GetWindowWidth() { return m_windowWidth; }
    inline bool IsKeyExists(int64_t faceId) { return m_windowMap.count(faceId) > 0; };
    bool IsReady(int64_t faceId);
    void AddFeatureVector(int64_t faceId, const tvm::runtime::NDArray& features);
    tvm::runtime::NDArray GetSequence(int64_t faceId);

private:
    const static int m_windowWidth = 120;
    std::unordered_map<int64_t, std::vector<std::vector<float>>> m_windowMap;
    std::unordered_map<int64_t, size_t> m_windowMapIdx;
};

#endif // SLIDINGWINDOW_H
