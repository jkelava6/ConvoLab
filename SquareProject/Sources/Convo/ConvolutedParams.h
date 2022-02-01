#pragma once

#include <Core/DynamicArray.h>

BASE_NETWORK_NAMESPACE_BEGIN
class FNetwork;
BASE_NETWORK_NAMESPACE_END
class FDna;

class FConvolutionResolution
{
public:
	FConvolutionResolution();
	FConvolutionResolution(int32 InFromX, int32 InFromY, int32 InToX, int32 InToY);
	FConvolutionResolution(int32 X, int32 Y, int32 SizeX, int32 SizeY, const FConvolutionResolution& ReadReso);
public:
	int32 FromX = 0;
	int32 FromY = 0;
	int32 ToX = -1;
	int32 ToY = -1;
};

class FConvolutionBlockConnection
{
public:
	FConvolutionBlockConnection(int32 InFeederBlock, int32 Compression, const FConvolutionResolution& InResolution);
public:
	int32 FeederBlock = -1;
	int32 Compression = 1;
	FConvolutionResolution Resolution;
};

class FConvolutionBlockParams
{
public:
	FConvolutionBlockParams(int32 InSizeX, int32 InSizeY, int32 InConvolutionLayers,
		const FConvolutionResolution& InResolution);
public:
	int32 SizeX = -1;
	int32 SizeY = -1;
	int32 ConvolutionLayers = -1;
	FConvolutionResolution Resolution;

	TArray<FConvolutionBlockConnection> Connections;
};

class FConvolutedNetworkParams
{
public:
	FConvolutedNetworkParams();

	void SetIOCount(int32 InInputs, int32 InOutputs);
	void SetInitialRanges();
	int32 AddBlock(const FConvolutionBlockParams& Block);
	int32 AddBlock(FConvolutionBlockParams&& Block);

	void SetupNetwork(FBaseNetwork::FNetwork* TargetNetwork, FDna* Dna = nullptr, FDna* OutDna = nullptr) const;
private:
	int32 InputCount = -1;
	int32 OutputCount = -1;
	float BiasRange = 0.0f;
	float WeightRange = 0.0f;
	TArray<FConvolutionBlockParams> Blocks;
};

