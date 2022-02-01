#pragma once

#include <Core/DynamicArray.h>

CONVOLUTION_PARAMS_NAMESPACE_BEGIN
class FNetwork;
CONVOLUTION_PARAMS_NAMESPACE_END
class FGameSetup;
class IArtificialIntelligence;

extern void WithVsWithoutRotation2(const FConvolutionParams::FNetwork& Params, IArtificialIntelligence& AI,
	const TArray<FGameSetup>& Training, const TArray<FGameSetup>& Test, uint32 NetSeed);
