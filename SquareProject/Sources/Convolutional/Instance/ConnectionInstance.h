#pragma once

#include <Core/DynamicArray.h>

CONVOLUTION_PARAMS_NAMESPACE_BEGIN
class FBlock;
class FConnection;
CONVOLUTION_PARAMS_NAMESPACE_END

class FDna;

CONVOLUTION_INSTANCE_NAMESPACE_BEGIN

class FBlock;

class FConnection
{
public:
	FConnection(const FConvolutionParams::FConnection* InParams, FBlock* FeederBlock, FBlock* TargetBlock, FDna* Dna/* = nullptr*/);

	int32 GetNumOfFeatures() const;
	int32 GetSizeX() const;
	int32 GetSizeY() const;
	float GetWeight(int32 Feature, int32 X, int32 Y, int32 DestinationFeature) const;
	void SetWeight(int32 Feature, int32 X, int32 Y, float Value, int32 DestinationFeature);
	float GetDifferential(int32 Feature, int32 X, int32 Y, int32 DestinationFeature) const;
	void SetDifferential(int32 Feature, int32 X, int32 Y, float Value, int32 DestinationFeature);
	void AddDifferential(int32 Feature, int32 X, int32 Y, float Value, int32 DestinationFeature);
	const FConvolutionParams::FConnection& GetParams() const;
	FBlock* GetBlock() const;
	void SaveDifferentials();
	void ApplyBackProp(float Scale, float MaxStep);
private:
	const FConvolutionParams::FConnection& Params;
	int32 SizeFeatures;
	int32 SizeX;
	int32 SizeY;
	int32 SizeDestFeat;
	TArray<float> Weights;
	TArray<float> Differentials;
	TArray<float> SavedDifferentials;
	FBlock* Block;
};

CONVOLUTION_INSTANCE_NAMESPACE_END
