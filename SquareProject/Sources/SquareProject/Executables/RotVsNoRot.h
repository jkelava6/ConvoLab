#pragma once

#include <Core/DynamicArray.h>

class FConvolutedNetworkParams;
class FGameSetup;
class IArtificialIntelligence;

extern void WithVsWithoutRotation(const FConvolutedNetworkParams& Params, IArtificialIntelligence& AI,
	const TArray<FGameSetup>& Training, const TArray<FGameSetup>& Test, uint32 NetSeed);
