#include <SquareProject/StdH.h>
#include <SquareProject/Executables/RVNR_C2.h>

#include <SquareProject/AI/Base.h>
#include <SquareProject/Game/GameCore.h>
#include <SquareProject/Game/GameSetup.h>
#include <SquareProject/Game/MapGenerator.h>
#include <SquareProject/Tools/DataPoint.h>
#include <SquareProject/Tools/TrainingTask.h>

#include <Convolutional/Instance/NetworkInstance.h>
#include <Convolutional/Instance/BlockInstance.h>

#include <fstream>
#include <iostream>

#define ONLY_GENERATE_INITIAL_TRAINING_PARAMETERS true
#if ONLY_GENERATE_INITIAL_TRAINING_PARAMETERS
#include <SquareProject/Tools/DataSetLoader.h>
#include <SquareProject/Tools/ParamsLoader.h>
#include <SquareProject/Tools/DnaLoader.h>
#include <Core/Dna.h>
#endif

static void TrainNetwork(FConvolutionInstance::FNetwork& Network, TArray<FDataPoint>& DataSet)
{
	FTrainingTask Training = FTrainingTask(2000, 1e-6f, -1e-6f, true);
	Training.Train(Network, DataSet);
}

static void GenerateInputs(const FGame& GameObject, TArray<float>& OutInputs, bool bRotateFromPlayersView)
{
	const int32 MapSize = GameObject.GetMapSize();
	const int32 SquareCount = FMath::SquareI(MapSize);

	for (int32 Player = 0; Player < 4; ++Player)
	{
		const int32 ScannedPlayer = (GameObject.GetCurrentPlayer() + Player) % 4;
		for (int32 X = 0; X < MapSize; ++X)
		{
			for (int32 Y = 0; Y < MapSize; ++Y)
			{
				const FSquare& Square = GameObject.GetSquare(X, Y, bRotateFromPlayersView);
				OutInputs.Push() = Square.SquareOwnership == ScannedPlayer ? Square.UnitCount : 0.0f;
			}
		}
	}
	for (int32 X = 0; X < MapSize; ++X)
	{
		for (int32 Y = 0; Y < MapSize; ++Y)
		{
			const FSquare& Square = GameObject.GetSquare(X, Y, bRotateFromPlayersView);
			const int32 ValueWith = (int32)
				((Square.Modifier & ESquareModifier::ResourceMask) >> ESquareModifier::ResourceShift);
			OutInputs.Push() = 1.0f * ValueWith;
		}
	}
	for (int32 X = 0; X < MapSize; ++X)
	{
		for (int32 Y = 0; Y < MapSize; ++Y)
		{
			const FSquare& Square = GameObject.GetSquare(X, Y, bRotateFromPlayersView);
			const int32 ValueWith = (int32)
				((Square.Modifier & ESquareModifier::DefenseMask) >> ESquareModifier::DefenseShift);
			OutInputs.Push() = 1.0f * ValueWith;
		}
	}
}

int32 EvalTestGame(FGame& GameObject, int32 WithRotationPlayer, const FGameSetup& TestSetup,
	FConvolutionInstance::FNetwork& WithNetwork, FConvolutionInstance::FNetwork& WithoutNetwork,
	int32 OutputCount, IArtificialIntelligence& AI)
{
	constexpr int32 MaxTurns = 512;
	int32 Turn;
	for (Turn = 0; Turn < MaxTurns; ++Turn)
	{
		int32 NumOfPlayersAlive = 0;
		for (int32 Player = 0; Player < 4; ++Player)
		{
			if (TestSetup.Players[Player])
			{
				if (!GameObject.GetResourceAmount())
				{
					GameObject.SkipTurn();
					continue;
				}
				++NumOfPlayersAlive;

				FPlayedMove Move;
				const bool bRotate = (Player == -2);
				const FConvolutionInstance::FNetwork& Network = bRotate ? WithNetwork : WithoutNetwork;

				if (Player == 0 || Player == 2)
				{
					TArray<float> Inputs;
					GenerateInputs(GameObject, Inputs, bRotate);

					TArray<float> Outputs;
					Outputs.Prealocate(OutputCount);
					for (int32 Output = 0; Output < OutputCount; ++Output)
					{
						Outputs.Push();
					}

					if (Player == WithRotationPlayer)
					{
						WithNetwork.Evaluate(Inputs, Outputs);
					}
					else
					{
						WithoutNetwork.Evaluate(Inputs, Outputs);
					}
					Move = AI.ChooseMove(GameObject, Outputs);
				}
				else
				{
					Move = AI.ChooseMove(GameObject);
				}

				GameObject.Play(Move.X, Move.Y, bRotate);
			}
			else
			{
				GameObject.SkipTurn();
			}
		}

		if (NumOfPlayersAlive < 2)
		{
			break;
		}
	}

	const int32 MapSize = GameObject.GetMapSize();
	bool bPlayersAlive[4] = { 0, 0, 0, 0 };
	int32 NumOfPlayersAlive = 0;
	for (int32 Player = 0; Player < 4; ++Player)
	{
		bPlayersAlive[Player] = GameObject.GetResourceAmount(Player);
		NumOfPlayersAlive += bPlayersAlive[Player];
	}

	if (NumOfPlayersAlive == 1)
	{
		for (int32 Player = 0; Player < 4; ++Player)
		{
			if (bPlayersAlive[Player])
			{
				return Player;
			}
		}
	}

	return -1;
}

static void GenerateVisualizationData(FConvolutionInstance::FNetwork& Network, TArray<FDataPoint>& Dataset, std::string Destination)
{
	std::fstream File;
	char FileName[20];
	const int32 MapSize = Network.Blocks[0].GetSizeX();

	for (int32 TestIndex = 0; TestIndex < Dataset.Count(); ++TestIndex)
	{
		FDataPoint& DP = Dataset[TestIndex];
		TArray<float> DummyOutputs;
		DummyOutputs.Prealocate(DP.Outputs.Count());
		for (int32 DOI = 0; DOI < DP.Outputs.Count(); ++DOI)
		{
			DummyOutputs.Push();
		}
		Network.Evaluate(DP.Inputs, DummyOutputs);

		sprintf_s(FileName, "Test%d.txt", TestIndex);
		File.open(Destination + FileName, std::ios_base::out);

		const int32 NumOfBlocks = Network.Blocks.Count();
		File << NumOfBlocks << "\n";

		for (int32 BlockIndex = 0; BlockIndex < NumOfBlocks; ++BlockIndex)
		{
			FConvolutionInstance::FBlock& Block = Network.Blocks[BlockIndex];
			const int32 SizeX = Block.GetSizeX();
			const int32 SizeY = Block.GetSizeY();
			const int32 NumOfFeatures = Block.GetNumOfFeatures();
			if (BlockIndex + 1 == NumOfBlocks)
			{
				File << SizeX << " " << SizeY << " " << NumOfFeatures + 1 << "\n";
			}
			else
			{
				File << SizeX << " " << SizeY << " " << NumOfFeatures << "\n";
			}

			for (int32 Feature = 0; Feature < NumOfFeatures; ++Feature)
			{
				for (int32 X = 0; X < SizeX; ++X)
				{
					for (int32 Y = 0; Y < SizeY; ++Y)
					{
						File << Block.GetState(Feature, X, Y) << " ";
					}
					File << "\n";
				}
				File << "\n";
			}

			if (BlockIndex + 1 == NumOfBlocks)
			{
				for (int32 Feature = 0; Feature < 1; ++Feature)
				{
					for (int32 X = 0; X < MapSize; ++X)
					{
						for (int32 Y = 0; Y < MapSize; ++Y)
						{
							File << DP.Outputs[Feature * MapSize * MapSize + X * MapSize + Y] << " ";
						}
						File << "\n";
					}
					File << "\n";
				}
			}
		}

		File.close();
	}
}

extern void WithVsWithoutRotation2(const FConvolutionParams::FNetwork& Params, IArtificialIntelligence& AI,
	const TArray<FGameSetup>& Training, const TArray<FGameSetup>& Test, uint32 NetSeed)
{
	// setup networks
	IMapGenerator* FirstMapGen = Training[0].MapGen;
	const int32 MapSize = FirstMapGen->GetMapSize();
	const int32 SquareCount = FMath::SquareI(MapSize);
	const int32 InputCount = 6 * SquareCount;
	const int32 OutputCount = 1 * SquareCount;

	FMath::SetRandomSeed(NetSeed);
	FConvolutionInstance::FNetwork WithNetwork(Params, nullptr);
	FMath::SetRandomSeed(NetSeed);
	FConvolutionInstance::FNetwork WithoutNetwork(Params, nullptr);

	// prepare training dataset
	FGame GameObject;
	TArray<FDataPoint> WithDataset;
	TArray<FDataPoint> WithoutDataset;
	for (int32 GameIndex = 0; GameIndex < Training.Count(); ++GameIndex)
	{
		const FGameSetup& GameSetup = Training[GameIndex];
		IMapGenerator* Generator = GameSetup.MapGen;

		FMath::SetRandomSeed(GameSetup.Seed);
		GameObject.SetupGame(*Generator);

		constexpr int32 MaxTurns = 512;
		int32 Turn;
		for (Turn = 0; Turn < MaxTurns; ++Turn)
		{
			int32 NumOfPlayersAlive = 0;
			for (int32 Player = 0; Player < 4; ++Player)
			{
				if (GameSetup.Players[Player])
				{
					if (!GameObject.GetResourceAmount())
					{
						GameObject.SkipTurn();
						continue;
					}
					++NumOfPlayersAlive;

					FDataPoint& With = WithDataset.Push();
					FDataPoint& Without = WithoutDataset.Push();
					GenerateInputs(GameObject, With.Inputs, true);
					GenerateInputs(GameObject, Without.Inputs, false);

					AI.GradeMoves(GameObject, With.Outputs, true);
					AI.GradeMoves(GameObject, Without.Outputs, false);
					FPlayedMove Move = AI.ChooseMove(GameObject, Without.Outputs);
					GameObject.Play(Move.X, Move.Y, false);

					With.bProvideDifferentialFeedback = !(Turn % 4);
					Without.bProvideDifferentialFeedback = !(Turn % 4);
				}
				else
				{
					GameObject.SkipTurn();
				}
			}
			if (NumOfPlayersAlive < 2)
			{
				break;
			}
		}
		std::cout << "Training board " << GameIndex << ", moves played : " << Turn << "\n";
	}

#if ONLY_GENERATE_INITIAL_TRAINING_PARAMETERS
	FDna WithInitialDna;
	WithNetwork.Serialize(WithInitialDna);
	FDnaLoader::WriteDnaFile("DataFiles/WithRotDna_0.txt", WithInitialDna);
	FDna WithoutInitialDna;
	WithoutNetwork.Serialize(WithoutInitialDna);
	FDnaLoader::WriteDnaFile("DataFiles/WithoutRotDna_0.txt", WithoutInitialDna);
	FParamsLoader::WriteParamsFile("DataFiles/WVsWoRotNetParams.txt", Params);
	FDataSetLoader::WriteDataSetFile("DataFiles/WithRotDataset.txt", WithDataset);
	FDataSetLoader::WriteDataSetFile("DataFiles/WithoutRotDataset.txt", WithoutDataset);
#else

	// training
	TrainNetwork(WithNetwork, WithDataset);
	TrainNetwork(WithoutNetwork, WithoutDataset);

	// generate visualizaiton data
	GenerateVisualizationData(WithNetwork, WithDataset, "WithRot_");
	GenerateVisualizationData(WithoutNetwork, WithoutDataset, "WithoutRot_");

	// testing
	int32 WithWins = 0;
	int32 WithoutWins = 0;
	for (int32 TestGameIndex = 0; TestGameIndex < Test.Count(); ++TestGameIndex)
	{
		const FGameSetup& TestSetup = Test[TestGameIndex];

		int32 GS_WW = 0;
		int32 GS_WOW = 0;

		FMath::SetRandomSeed(TestSetup.Seed);
		GameObject.SetupGame(*TestSetup.MapGen);
		int32 Winner = EvalTestGame(GameObject, 0, TestSetup, WithNetwork, WithoutNetwork, OutputCount, AI);
		WithWins += (Winner == 0);
		WithoutWins += (Winner == 2);
		GS_WW += (Winner == 0);
		GS_WOW += (Winner == 2);

		FMath::SetRandomSeed(TestSetup.Seed);
		GameObject.SetupGame(*TestSetup.MapGen);
		Winner = EvalTestGame(GameObject, 2, TestSetup, WithNetwork, WithoutNetwork, OutputCount, AI);
		WithWins += (Winner == 2);
		WithoutWins += (Winner == 0);
		GS_WW += (Winner == 2);
		GS_WOW += (Winner == 0);

		std::cout << "Game setup #" << TestGameIndex + 1 << ": " << GS_WW << " - " << GS_WOW << "\n";
	}

	std::cout << "With rotation wins: " << WithWins << "\n";
	std::cout << "Without rotation wins: " << WithoutWins << "\n";
#endif // ONLY_GENERATE_INITIAL_TRAINING_PARAMETERS
}
