#pragma once

#include <Core/DynamicArray.h>
#include <Convolutional/Instance/ConnectionInstance.h>

class FDna;

CONVOLUTION_FUNCTION_NAMESPACE_BEGIN
class ICollector;
CONVOLUTION_FUNCTION_NAMESPACE_END

CONVOLUTION_PARAMS_NAMESPACE_BEGIN
class FBlock;
class FConnection;
CONVOLUTION_PARAMS_NAMESPACE_END

CONVOLUTION_INSTANCE_NAMESPACE_BEGIN

class FConnection;

class FBlock
{
public:
	FBlock(const FConvolutionParams::FBlock* InParams, FDna* InDna = nullptr);
	void AddConnection(const FConvolutionParams::FConnection& InParams, FBlock* OtherBlock, FDna* InDna = nullptr);

	int32 GetNumOfFeatures() const;
	int32 GetSizeX() const;
	int32 GetSizeY() const;
	const FConvolutionParams::FBlock& GetParams() const;
	float GetBias(int32 Feature, int32 X, int32 Y) const;
	void SetBias(int32 Feature, int32 X, int32 Y, float Bias);
	float GetState(int32 Feature, int32 X, int32 Y) const;
	void SetState(int32 Feature, int32 X, int32 Y, float Value);
	float GetDifferential(int32 Feature, int32 X, int32 Y) const;
	void SetDifferential(int32 Feature, int32 X, int32 Y, float Value);
	void AddDifferential(int32 Feature, int32 X, int32 Y, float Value);
	int32 GetNumOfConnections() const;
	FConnection& GetConnection(int32 ConnIndex);
	void SaveDifferentials();
	void ApplyBackProp(float Scale, float MaxStep);
	void RevertStates();
	void ClearSavedDifferentials();
private:
	const FConvolutionParams::FBlock& Params;

	int32 NumOfFeatures;
	int32 SizeX;
	int32 SizeY;
	TArray<float> Biases;
	TArray<float> SavedBiases;
	TArray<FConvolutionFunction::ICollector*> Collectors;
	TArray<float> States;
	TArray<float> Differentials;
	TArray<float> SavedDifferentials;
	TArray<FConnection> Connections;
};

CONVOLUTION_INSTANCE_NAMESPACE_END
