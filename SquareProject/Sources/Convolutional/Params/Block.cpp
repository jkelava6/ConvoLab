#include <Convolutional/StdH.h>
#include <Convolutional/Params/Block.h>

CONVOLUTION_PARAMS_NAMESPACE_BEGIN

FBlock::FBlock(int32 InX, int32 InY, int32 InFeatures, FConvolutionFunction::ICollector* InCollector, FConvolutionFunction::IFloatToFloat* InMapper)
	: X(InX)
	, Y(InY)
	, Features(InFeatures)
	, Collector(InCollector)
	, Mapper(InMapper)
{
}

CONVOLUTION_PARAMS_NAMESPACE_END
