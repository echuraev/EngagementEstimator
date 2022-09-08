#ifndef ENETB08BESTAFEWWRAPPER_H
#define ENETB08BESTAFEWWRAPPER_H

class QPixmap;

#undef slots
#include <tvm/runtime/module.h>
#define slots Q_SLOTS

class EnetB08BestAfewWrapper
{
public:
    EnetB08BestAfewWrapper();
    static tvm::runtime::NDArray getInputTensor(const QPixmap input);
    static tvm::runtime::NDArray getOutputTensor();
    static std::string classifyEmition(const tvm::runtime::NDArray& output);
    static std::string getModelPath();
};

#endif // ENETB08BESTAFEWWRAPPER_H
