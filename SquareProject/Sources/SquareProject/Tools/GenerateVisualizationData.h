#pragma once

class FDataPoint;
CONVOLUTION_INSTANCE_NAMESPACE_BEGIN
class FNetwork;
CONVOLUTION_INSTANCE_NAMESPACE_END
template <class T> class TArray;

#include <string>

extern void GenerateVisualizationData(FConvolutionInstance::FNetwork& Network, TArray<FDataPoint>& Dataset, std::string Destination);
