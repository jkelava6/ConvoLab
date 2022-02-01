#pragma once

#include <Core/DynamicArray.h>

class FDna;

CONVOLUTION_PARAMS_NAMESPACE_BEGIN

class FBlock;

class FConnection
{
public:
	FConnection() = default;
	FConnection(FBlock* InBlock,
		float InXFactor, int32 InLoX, int32 InHiX, float InYFactor, int32 InLoY, int32 InHiY,
		bool bInAllFeatures);

	void InitArray(FDna* Dna = nullptr);
public:
	FBlock* Block = nullptr;
	float XFactor = 1;
	int32 LoX = -1;
	int32 HiX = 1;
	float YFactor = 1;
	int32 LoY = -1;
	int32 HiY = 1;
	bool bAllFeatures = true;
};

CONVOLUTION_PARAMS_NAMESPACE_END
