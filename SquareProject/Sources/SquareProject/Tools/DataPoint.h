#pragma once

#include <Core/DynamicArray.h>

class FDataPoint
{
public:
	FDataPoint();
	DECLARE_COPY(FDataPoint);
	DECLARE_MOVE(FDataPoint);
public:
	TArray<float> Inputs;
	TArray<float> Outputs;
	bool bProvideDifferentialFeedback = false;
};
