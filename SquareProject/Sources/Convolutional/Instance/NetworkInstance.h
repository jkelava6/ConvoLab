#pragma once

#include <Core/DynamicArray.h>

class FDna;
CONVOLUTION_PARAMS_NAMESPACE_BEGIN
class FNetwork;
CONVOLUTION_PARAMS_NAMESPACE_END

CONVOLUTION_INSTANCE_NAMESPACE_BEGIN

class FBlock;

class FNetwork
{
public:
	FNetwork(const FConvolutionParams::FNetwork& Structure, FDna* Parameters);
	~FNetwork();
	DECLARE_NOCOPY(FNetwork);
	DECLARE_NOMOVE(FNetwork);

	void Evaluate(const TArray<float>& Inputs, TArray<float>& Outputs);
	void Backpropagate(const TArray<float>& ErrorDifferentials);
	void ApplyBackProp(float Scale, float MaxStep);

private:
	const FConvolutionParams::FNetwork& Params;

public: // access to Blocks for debug purposes
	TArray<FBlock> Blocks;
};

CONVOLUTION_INSTANCE_NAMESPACE_END
