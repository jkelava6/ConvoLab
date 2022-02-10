#include <SquareProject/StdH.h>

#include <SquareProject/AI/Heuristics/ExponentialFalloff.h>
#include <SquareProject/Executables/RVNR_C2.h>
#include <SquareProject/Game/GameSetup.h>
#include <SquareProject/MapGen/AlternateRing.h>

#include <Convolutional/Function/Collector.h>
#include <Convolutional/Function/Mapping.h>
#include <Convolutional/Params/Block.h>
#include <Convolutional/Params/Connection.h>
#include <Convolutional/Params/Network.h>

#include <Core/MathTools.h>

#include <iostream>

int main()
{
	const int32 MapSize = 10;
	const int32 SquareCount = FMath::SquareI(MapSize);

	//FConvolutedNetworkParams NetworkParams;
	//NetworkParams.AddBlock(FConvolutionBlockParams(MapSize, MapSize, 0, FConvolutionResolution())); // 0
	//NetworkParams.AddBlock(FConvolutionBlockParams(MapSize, MapSize, 0, FConvolutionResolution())); // 1
	//NetworkParams.AddBlock(FConvolutionBlockParams(MapSize, MapSize, 0, FConvolutionResolution())); // 2
	//NetworkParams.AddBlock(FConvolutionBlockParams(MapSize, MapSize, 0, FConvolutionResolution())); // 3

	//NetworkParams.AddBlock(FConvolutionBlockParams(MapSize, MapSize, 0, FConvolutionResolution())); // 4
	//NetworkParams.AddBlock(FConvolutionBlockParams(MapSize, MapSize, 0, FConvolutionResolution())); // 5

	//FConvolutionResolution ConvolutionResolution(-2, -2, 3, 3);
	//FConvolutionResolution ConnectionResolution(0, 0, 1, 1);

	//FConvolutionBlockParams ConvolutionBlock(MapSize, MapSize, 5, ConvolutionResolution);
	//
	//const int32 LowBlock[] = { 0, 6, 9, 12, 15 };
	//const int32 HighBlock[] = { 6, 9, 12, 15, 16 };
	//const int32 NumOfConnectionLayers = ARRAY_SIZE(LowBlock);
	//ARRAY_MATCH(LowBlock, HighBlock);

	//for (int32 ConnectionLayer = 1; ConnectionLayer < NumOfConnectionLayers; ++ConnectionLayer)
	//{
	//	const int32 FromLow = LowBlock[ConnectionLayer - 1];
	//	const int32 FromHigh = HighBlock[ConnectionLayer - 1];
	//	const int32 ToLow = LowBlock[ConnectionLayer];
	//	const int32 ToHigh = HighBlock[ConnectionLayer];

	//	for (int32 To = ToLow; To < ToHigh; ++To)
	//	{
	//		ConvolutionBlock.Connections.PopAll();
	//		for (int32 From = FromLow; From < FromHigh; ++From)
	//		{
	//			ConvolutionBlock.Connections.Add(FConvolutionBlockConnection(From, 1, ConnectionResolution));
	//		}
	//		NetworkParams.AddBlock(ConvolutionBlock);
	//	}
	//}

	typedef FConvolutionParams::FBlock CPBlock;
	typedef FConvolutionParams::FConnection CPConnection;
	typedef FConvolutionParams::FNetwork CPNetwork;
	FConvolutionParams::FNetwork TrainingParams;
	CPBlock* InputBlock = new CPBlock(MapSize, MapSize, 6, nullptr, nullptr);
	InputBlock->bIsInput = true;
	TrainingParams.AddBlock(InputBlock);
	FConvolutionFunction::FSum SumCollector;
	FConvolutionFunction::FAverage AvgCollector;
	FConvolutionFunction::FReLu ReLuMapper;
	FConvolutionFunction::FLeakyReLu LeakyMapper(0.1f);

	CPBlock* LastBlock = InputBlock;

	//int32 FeatureCounts[] = { 8, 12, 16, 16, 20 };
	//int32 ConvolutionRadius[] = { 2, 1, 1, 2, 2 };
	//float Scale[] = { 1, 1, 1, 1, 1 };
	//int32 Size[] = { MapSize, MapSize, MapSize, MapSize, MapSize };
	//bool bUseAllFeatures[] = { true, true, true, false, true };
	
	//int32 FeatureCounts[] = { 3, 2, 2 };
	//int32 ConvolutionRadius[] = { 2, 2, 1 };
	//float Scale[] = { 1, 1, 1 };
	//int32 Size[] = { MapSize, MapSize, MapSize, MapSize, MapSize };
	//bool bUseAllFeatures[] = { true, true, true };

	//int32 FeatureCounts[] = { 16, 16, 20, 5, 20, 6 };
	//int32 ConvolutionRadius[] = { 2, 3, 2, 2, 1, 1 };
	//int32 Size[] = { MapSize, MapSize, MapSize, MapSize / 2, MapSize, MapSize };
	//float Scale[] = { 1, 1, 1, 2, 0.5f, 1 };
	//bool bUseAllFeatures[] = { true, true, true, true, true, true };

	int32 FeatureCounts[] = { 8, 12, 12, 8 };
	int32 ConvolutionRadius[] = { 1, 2, 1, 2 };
	int32 Size[] = { MapSize, MapSize, MapSize, MapSize};
	float Scale[] = { 1, 1, 1, 1 };
	bool bUseAllFeatures[] = { true, true, true, true };

	ARRAY_MATCH(FeatureCounts, ConvolutionRadius);
	ARRAY_MATCH(ConvolutionRadius, bUseAllFeatures);
	TArray<CPBlock*> AddedBlocks;
	AddedBlocks.Prealocate(ARRAY_SIZE(FeatureCounts));
	for (int32 CBI = 0; CBI < ARRAY_SIZE(FeatureCounts); ++CBI)
	{
		CPBlock* ConvBlock = new CPBlock(Size[CBI], Size[CBI], FeatureCounts[CBI], &SumCollector, &LeakyMapper);
		int32 ConvRad = ConvolutionRadius[CBI];
		ConvBlock->Connections.Add(CPConnection(LastBlock, Scale[CBI], -ConvRad, ConvRad, Scale[CBI], -ConvRad, ConvRad, bUseAllFeatures[CBI]));

		if (CBI > 0)
		{
			ConvBlock->Connections.Add(CPConnection(InputBlock, Scale[CBI], -ConvRad, ConvRad, Scale[CBI], -ConvRad, ConvRad, bUseAllFeatures[CBI]));
		}

		TrainingParams.AddBlock(ConvBlock);
		LastBlock = ConvBlock;
		AddedBlocks.Add(ConvBlock);
	}

	//AddedBlocks[4]->Connections.Add(CPConnection(AddedBlocks[2], 1.0f, -2, 2, 1.0f, -2, 2, true));
	//AddedBlocks[3]->Connections.Add(CPConnection(InputBlock, 0.5f, -1, 1, 0.5f, -1, 1, true));

	CPBlock* FinalBlock = new CPBlock(MapSize, MapSize, 1, &SumCollector, &LeakyMapper);
	FinalBlock->Connections.Add(CPConnection(LastBlock, 1.0f, 0, 0, 1.0f, 0, 0, true));
	FinalBlock->Connections.Add(CPConnection(InputBlock, 1.0f, 0, 0, 1.0f, 0, 0, true));
	FinalBlock->bIsOutput = true;
	TrainingParams.AddBlock(FinalBlock);

	TArray<CPBlock*> BlocksLeft;
	BlocksLeft.Add(InputBlock);
	TArray<CPBlock*> BlocksRight;
	BlocksRight.Add(InputBlock);

	/*int32 LeftScale[] = {1, 2, 4, 2, 1};
	int32 LeftFeatures[] = { 8, 8, 8, 8, 8 };
	int32 LeftLow[] = { -1, 0, 0, -1, -1 };
	int32 LeftHigh[] = { 1, 1, 1, 1, 1 };
	for (int32 X = 0; X < ARRAY_SIZE(LeftScale); ++X)
	{
		const int32 Size = MapSize / LeftScale[X];
		CPBlock* Added = new CPBlock(Size, Size, LeftFeatures[X], &AvgCollector, &ReLuMapper);
		for (CPBlock* )
	}*/
	
	TArray<FGameSetup> TrainingSet;
	TArray<bool> TwoPlayers;
	TwoPlayers.Prealocate(4);
	TwoPlayers.Push() = 1;
	TwoPlayers.Push() = 0;
	TwoPlayers.Push() = 1;
	TwoPlayers.Push() = 0;
	FAlteranateRingGenerator RingGen(MapSize, TwoPlayers);
	for (int32 Game = 0; Game < 5; ++Game)
	{
		TrainingSet.Add(FGameSetup(&RingGen, Game));
	}

	TArray<FGameSetup> TestSet;
	for (int32 Game = 0; Game < 1; ++Game)
	{
		TestSet.Add(FGameSetup(&RingGen, 1000 + Game));
	}

	FExponentialFalloffAI AI;
	//WithVsWithoutRotation(NetworkParams, AI, TrainingSet, TestSet, 0);
	WithVsWithoutRotation2(TrainingParams, AI, TrainingSet, TestSet, 5);

	return 0;
}
