#pragma once

#include <Core/DynamicArray.h>
#include <Convolutional/Params/Connection.h>

CONVOLUTION_FUNCTION_NAMESPACE_BEGIN
class ICollector;
class IFloatToFloat;
CONVOLUTION_FUNCTION_NAMESPACE_END

CONVOLUTION_PARAMS_NAMESPACE_BEGIN

class FBlock
{
public:
	FBlock(int32 InX, int32 InY, int32 InFeatures, FConvolutionFunction::ICollector* InCollector, FConvolutionFunction::IFloatToFloat* InMapper);
public:
	int32 X;
	int32 Y;
	int32 Features;
	bool bUseDifferentBiases = false;

	bool bIsInput = false;
	bool bIsOutput = false;
	
	FConvolutionFunction::ICollector* Collector;
	FConvolutionFunction::IFloatToFloat* Mapper;
	TArray<FConnection> Connections;
};

CONVOLUTION_PARAMS_NAMESPACE_END
