#include <Convolutional/StdH.h>
#include <Convolutional/Instance/ConnectionInstance.h>

#include <Core/Dna.h>

#include <Convolutional/Function/InitialValues.h>
#include <Convolutional/Function/ParamBounds.h>
#include <Convolutional/Instance/BlockInstance.h>
#include <Convolutional/Params/Block.h>
#include <Convolutional/Params/Connection.h>

#include <iostream>

CONVOLUTION_INSTANCE_NAMESPACE_BEGIN

FConnection::FConnection(const FConvolutionParams::FConnection* InParams, FBlock* FeederBlock, FBlock* TargetBlock, FDna* Dna = nullptr)
	: Params(*InParams)
	, Block(FeederBlock)
{
	SizeX = Params.HiX + 1 - Params.LoX;
	SizeY = Params.HiY + 1 - Params.LoY;
	SizeFeatures = Params.bAllFeatures ? Params.Block->Features : 1;
	SizeDestFeat = TargetBlock->GetNumOfFeatures();

	const int32 NumOfWeights = SizeDestFeat * SizeX * SizeY * SizeFeatures;
	Weights.Prealocate(NumOfWeights);
	for (int32 WI = 0; WI < NumOfWeights; ++WI)
	{
		Weights.Add(Dna ? Dna->ReadFloat() : FConvolutionFunction::GetInitialWeightValue());
		Differentials.Add(0.0f);
		SavedDifferentials.Add(0.0f);
	}
}

int32 FConnection::GetNumOfFeatures() const
{
	return SizeFeatures;
}

int32 FConnection::GetSizeX() const
{
	return SizeX;
}

int32 FConnection::GetSizeY() const
{
	return SizeY;
}

float FConnection::GetWeight(int32 Feature, int32 X, int32 Y, int32 DestinationFeature) const
{
	const int32 Index = DestinationFeature * (SizeFeatures * SizeX * SizeY) + Feature * (SizeX * SizeY) + X * (SizeY) + Y;
	return Weights[Index];
}

void FConnection::SetWeight(int32 Feature, int32 X, int32 Y, float Value, int32 DestinationFeature)
{
	const int32 Index = DestinationFeature * (SizeFeatures * SizeX * SizeY) + Feature * (SizeX * SizeY) + X * (SizeY) + Y;
	Weights[Index] = FMath::ClampF(Value, -FConvolutionFunction::MaxWeightValue, FConvolutionFunction::MaxWeightValue);
}

float FConnection::GetDifferential(int32 Feature, int32 X, int32 Y, int32 DestinationFeature) const
{
	const int32 Index = DestinationFeature * (SizeFeatures * SizeX * SizeY) + Feature * (SizeX * SizeY) + X * (SizeY)+Y;
	return Differentials[Index];
}

void FConnection::SetDifferential(int32 Feature, int32 X, int32 Y, float Value, int32 DestinationFeature)
{
	const int32 Index = DestinationFeature * (SizeFeatures * SizeX * SizeY) + Feature * (SizeX * SizeY) + X * (SizeY)+Y;
	Differentials[Index] = Value;
}

void FConnection::AddDifferential(int32 Feature, int32 X, int32 Y, float Value, int32 DestinationFeature)
{
	const int32 Index = DestinationFeature * (SizeFeatures * SizeX * SizeY) + Feature * (SizeX * SizeY) + X * (SizeY)+Y;
	Differentials[Index] += Value;
}

const FConvolutionParams::FConnection& FConnection::GetParams() const
{
	return Params;
}

FBlock* FConnection::GetBlock() const
{
	return Block;
}

void FConnection::SaveDifferentials()
{
	for (int32 Feature = 0; Feature < SizeFeatures; ++Feature)
	{
		for (int32 X = 0; X < SizeX; ++X)
		{
			for (int32 Y = 0; Y < SizeY; ++Y)
			{
				const int32 Index = (Feature * (SizeX * SizeY) + X * (SizeY)+Y);
				SavedDifferentials[Index] += Differentials[Index];
			}
		}
	}
}

void FConnection::ApplyBackProp(float Scale, float MaxStep)
{
	for (int32 Feature = 0; Feature < SizeFeatures; ++Feature)
	{
		for (int32 X = 0; X < SizeX; ++X)
		{
			for (int32 Y = 0; Y < SizeY; ++Y)
			{
				const int32 Index = (Feature * (SizeX * SizeY) + X * (SizeY)+Y);
				//std::cout << SavedDifferentials[Index] << " ";
				Weights[Index] += FMath::ClampF(Scale * SavedDifferentials[Index], -MaxStep, MaxStep);
				SavedDifferentials[Index] = 0.0f;
			}
		}
	}
}

CONVOLUTION_INSTANCE_NAMESPACE_END
