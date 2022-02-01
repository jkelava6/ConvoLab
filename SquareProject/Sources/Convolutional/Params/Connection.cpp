#include <Convolutional/StdH.h>
#include <Convolutional/Params/Connection.h>

#include <Convolutional/Params/Block.h>

#include <Core/Dna.h>

CONVOLUTION_PARAMS_NAMESPACE_BEGIN

FConnection::FConnection(FBlock* InBlock,
	float InXFactor, int32 InLoX, int32 InHiX, float InYFactor, int32 InLoY, int32 InHiY,
	bool bInAllFeatures)
	: Block(InBlock)
	, XFactor(InXFactor)
	, LoX(InLoX)
	, HiX(InHiX)
	, YFactor(InYFactor)
	, LoY(InLoY)
	, HiY(InHiY)
	, bAllFeatures(bInAllFeatures)
{
}

CONVOLUTION_PARAMS_NAMESPACE_END
