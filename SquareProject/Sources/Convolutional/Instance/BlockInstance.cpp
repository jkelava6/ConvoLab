#include <Convolutional/StdH.h>
#include <Convolutional/Instance/BlockInstance.h>

#include <Core/Dna.h>

#include <Convolutional/Function/Collector.h>
#include <Convolutional/Function/InitialValues.h>
#include <Convolutional/Function/ParamBounds.h>
#include <Convolutional/Params/Block.h>

#include <iostream>

CONVOLUTION_INSTANCE_NAMESPACE_BEGIN

FBlock::FBlock(const FConvolutionParams::FBlock* InParams, FDna* InDna/* = nullptr*/)
	: Params(*InParams)
{
	NumOfFeatures = Params.Features;
	SizeX = Params.X;
	SizeY = Params.Y;

	const int32 NumOfStates = NumOfFeatures * SizeX * SizeY;
	const int32 NumOfBiases = Params.bUseDifferentBiases ? NumOfStates : NumOfFeatures;

	for (int32 State = 0; State < NumOfStates; ++State)
	{
		States.Push();
		Differentials.Push();
		SavedDifferentials.Push() = 0.0f;
		Collectors.Add(Params.Collector ? Params.Collector->Clone() : nullptr);
	}
	for (int32 Bias = 0; Bias < NumOfBiases; ++Bias)
	{
		Biases.Add(InDna ? InDna->ReadFloat() : FConvolutionFunction::GetInitialBiasValue());
		SavedBiases.Add(Biases[Bias]);
	}
}

void FBlock::AddConnection(const FConvolutionParams::FConnection& InParams, FBlock* OtherBlock, FDna* InDna/* = nullptr*/)
{
	Connections.Emplace(&InParams, OtherBlock, this, InDna);
}

int32 FBlock::GetNumOfFeatures() const
{
	return NumOfFeatures;
}

int32 FBlock::GetSizeX() const
{
	return SizeX;
}

int32 FBlock::GetSizeY() const
{
	return SizeY;
}

const FConvolutionParams::FBlock& FBlock::GetParams() const
{
	return Params;
}

float FBlock::GetBias(int32 Feature, int32 X, int32 Y) const
{
	const int32 Index = Params.bUseDifferentBiases ? (Feature * (SizeX * SizeY) + X * (SizeY)+Y) : Feature;
	return Biases[Index];
}

void FBlock::SetBias(int32 Feature, int32 X, int32 Y, float Bias)
{
	const int32 Index = Params.bUseDifferentBiases ? (Feature * (SizeX * SizeY) + X * (SizeY)+Y) : Feature;
	Biases[Index] = FMath::ClampF(Bias, -FConvolutionFunction::MaxBiasValue, FConvolutionFunction::MaxBiasValue);
}

float FBlock::GetState(int32 Feature, int32 X, int32 Y) const
{
	const int32 Index = (Feature * (SizeX * SizeY) + X * (SizeY)+Y);
	return States[Index];
}

void FBlock::SetState(int32 Feature, int32 X, int32 Y, float Value)
{
	const int32 Index = (Feature * (SizeX * SizeY) + X * (SizeY)+Y);
	States[Index] = Value;
}

float FBlock::GetDifferential(int32 Feature, int32 X, int32 Y) const
{
	const int32 Index = (Feature * (SizeX * SizeY) + X * (SizeY)+Y);
	return Differentials[Index];
}

void FBlock::SetDifferential(int32 Feature, int32 X, int32 Y, float Value)
{
	const int32 Index = (Feature * (SizeX * SizeY) + X * (SizeY)+Y);
	Differentials[Index] = Value;
}

void FBlock::AddDifferential(int32 Feature, int32 X, int32 Y, float Value)
{
	const int32 Index = (Feature * (SizeX * SizeY) + X * (SizeY)+Y);
	Differentials[Index] += Value;
}

int32 FBlock::GetNumOfConnections() const
{
	return Connections.Count();
}

FConnection& FBlock::GetConnection(int32 ConnIndex)
{
	return Connections[ConnIndex];
}

void FBlock::SaveDifferentials()
{
	for (int32 Feature = 0; Feature < NumOfFeatures; ++Feature)
	{
		for (int32 X = 0; X < SizeX; ++X)
		{
			for (int32 Y = 0; Y < SizeY; ++Y)
			{
				const int32 Index = (Feature * (SizeX * SizeY) + X * (SizeY)+Y);
				SavedDifferentials[Index] += Differentials[Index];

				const int32 NumOfConnections = GetNumOfConnections();
				for (int32 ConnIndex = 0; ConnIndex < NumOfConnections; ++ConnIndex)
				{
					Connections[ConnIndex].SaveDifferentials();
				}
			}
		}
	}
}

void FBlock::ApplyBackProp(float Scale, float MaxStep)
{
	for (int32 Index = 0; Index < Biases.Count(); ++Index)
	{
		SavedBiases[Index] = Biases[Index];
	}

	for (int32 Feature = 0; Feature < NumOfFeatures; ++Feature)
	{
		float FeatureDifferential = 0.0f;

		for (int32 X = 0; X < SizeX; ++X)
		{
			for (int32 Y = 0; Y < SizeY; ++Y)
			{
				const int32 Index = (Feature * (SizeX * SizeY) + X * (SizeY)+Y);
				if (Params.bUseDifferentBiases)
				{
					SetBias(Feature, X, Y, GetBias(Feature, X, Y) + FMath::ClampF(Scale * SavedDifferentials[Index], -MaxStep, MaxStep));
				}
				else
				{
					FeatureDifferential += SavedDifferentials[Index];
				}
			}
		}

		if (!Params.bUseDifferentBiases)
		{
			SetBias(Feature, 0, 0, GetBias(Feature, 0, 0) + FMath::ClampF(Scale * FeatureDifferential, -MaxStep, MaxStep));
		}
	}

	const int32 NumOfConnections = GetNumOfConnections();
	for (int32 ConnIndex = 0; ConnIndex < NumOfConnections; ++ConnIndex)
	{
		Connections[ConnIndex].ApplyBackProp(Scale, MaxStep);
	}
}

void FBlock::RevertStates()
{
	for (int32 Index = 0; Index < Biases.Count(); ++Index)
	{
		Biases[Index] = SavedBiases[Index];
	}

	for (int32 Index = 0; Index < Connections.Count(); ++Index)
	{
		Connections[Index].RevertWeights();
	}
}

void FBlock::ClearSavedDifferentials()
{
	for (int32 Index = 0; Index < SavedDifferentials.Count(); ++Index)
	{
		SavedDifferentials[Index] = 0;
	}

	for (int32 Index = 0; Index < Connections.Count(); ++Index)
	{
		Connections[Index].ClearSavedDifferentials();
	}
}

CONVOLUTION_INSTANCE_NAMESPACE_END
