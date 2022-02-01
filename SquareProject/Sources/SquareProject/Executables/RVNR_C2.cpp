#include <SquareProject/StdH.h>
#include <SquareProject/Executables/RVNR_C2.h>

#include <SquareProject/AI/Base.h>
#include <SquareProject/Game/GameCore.h>
#include <SquareProject/Game/GameSetup.h>
#include <SquareProject/Game/MapGenerator.h>

#include <Convolutional/Instance/NetworkInstance.h>

#include <iostream>

class FDataPoint
{
public:
	FDataPoint() = default;
	FDataPoint& operator=(const FDataPoint& Copied)
	{
		Inputs = Copied.Inputs;
		Outputs = Copied.Inputs;
		return *this;
	}
	FDataPoint(const FDataPoint& Copied)
	{
		*this = Copied;
	}
	FDataPoint& operator=(FDataPoint&& Copied) noexcept
	{
		Inputs = Move(Copied.Inputs);
		Outputs = Move(Copied.Outputs);
		return *this;
	}
	FDataPoint(FDataPoint&& Copied) noexcept
	{
		*this = Move(Copied);
	}
public:
	TArray<float> Inputs;
	TArray<float> Outputs;
};

float GetTargetStep(int32 Iteration, int32 NumOfIterations, float SlowdownStartRatio = 0.1f, float SlowdownEndRatio = 0.8f)
{
	constexpr float MaxStep = 3e-2f;
	constexpr float MinStep = 1e-4f;
	const int32 SDSI = (int32)(SlowdownStartRatio * NumOfIterations);
	const int32 SDEI = (int32)(SlowdownEndRatio * NumOfIterations);
	return FMath::LerpF(MaxStep, MinStep, FMath::ClampF((Iteration - SDSI) / (float)(SDEI - SDSI), 0, 1));
}

float GetTargetScale(int32 Iteration, int32 NumOfIterations, float SlowdownStartRatio = 0.2f, float SlowdownEndRatio = 0.8f)
{
	constexpr float MaxStep = -1.0f;
	constexpr float MinStep = -1e-4f;
	const int32 SDSI = (int32)(SlowdownStartRatio * NumOfIterations);
	const int32 SDEI = (int32)(SlowdownEndRatio * NumOfIterations);
	return FMath::LerpF(MaxStep, MinStep, FMath::ClampF((Iteration - SDSI) / (float)(SDEI - SDSI), 0, 1));
}

static void TrainNetwork(FConvolutionInstance::FNetwork& Network, TArray<FDataPoint>& DataSet)
{
	const FDataPoint& FirstDataPoint = DataSet[0];
	const int32 NumOfInputs = FirstDataPoint.Inputs.Count();
	const int32 NumOfOutputs = FirstDataPoint.Outputs.Count();

	constexpr int32 NumOfIterations = 500;
	const int32 NumOfReports = 500;
	int32 LastReport = -1;
	float NextStep = GetTargetStep(0, NumOfIterations);
	double LastErrorSum = 0.0;
	double LastSquaredSum = 0.0;
	for (int32 Iteration = 0; Iteration < NumOfIterations; ++Iteration)
	{
		TArray<float> Gradient;
		TArray<float> NetworkOutputs;
		for (int32 Output = 0; Output < NumOfOutputs; ++Output)
		{
			Gradient.Add(0.0f);
			NetworkOutputs.Push();
		}

		double ErrorSum = 0.0;
		double ErrorSquaredSum = 0.0;
		for (int32 DataPointIndex = 0; DataPointIndex < DataSet.Count(); ++DataPointIndex)
		{
			const FDataPoint& Data = DataSet[DataPointIndex];
			Network.Evaluate(Data.Inputs, NetworkOutputs);
			for (int32 Output = 0; Output < NumOfOutputs; ++Output)
			{
				const float Error = NetworkOutputs[Output] - Data.Outputs[Output];
				//std::cout << Error << " ";
				Gradient[Output] = Error; // should this be plus or minus?

				ErrorSum += FMath::AbsF(Error);
				ErrorSquaredSum += FMath::SquareF(Error);
			}
			Network.Backpropagate(Gradient);
			//std::cout << "\n";
		}

		const float ThisStep = NextStep;
		Network.ApplyBackProp(GetTargetScale(Iteration, NumOfIterations), ThisStep);
		if (FMath::AbsF((float)(ErrorSum - LastErrorSum)) < 1e-3f && FMath::AbsF((float)(ErrorSquaredSum - LastSquaredSum)) < 1e-3f)
		{
			NextStep *= 6.0f;
			NextStep = FMath::Min(0.1f, NextStep);
		}
		else
		{
			NextStep = GetTargetStep(Iteration, NumOfIterations);
		}
		//std::cout << "\n\n\n";

		const int32 ReportIndex = (Iteration * NumOfReports) / NumOfIterations;
		if (ReportIndex > LastReport)
		{
			std::cout << "Iteration " << Iteration << " (" << ((100 * Iteration) / NumOfIterations)
				<< "%) Error Sum: " << ErrorSum << " Squared Error Sum: " << ErrorSquaredSum
				<< "\nStep Size: " << ThisStep << " Error Sum Diff: " << ErrorSum - LastErrorSum << " Square Sum Diff: " << ErrorSquaredSum - LastSquaredSum << "\n";
			LastReport = ReportIndex;
		}

		LastErrorSum = ErrorSum;
		LastSquaredSum = ErrorSquaredSum;
	}
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
		//const int32 MapSize = GameObject.GetMapSize();
		//for (int32 X = 0; X < MapSize; ++X)
		//{
		//	for (int32 Y = 0; Y < MapSize; ++Y)
		//	{
		//		const FSquare& Square = GameObject.GetSquare(X, Y);
		//		std::cout << (Square.SquareOwnership == 0 ? Square.UnitCount : -Square.UnitCount) << ' ';
		//	}
		//	std::cout << '\n';
		//}
		//std::cout << '\n';

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

					WithNetwork.Evaluate(Inputs, Outputs);
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

					if (Turn % 6 == 0)
					{
						FDataPoint& With = WithDataset.Push();
						FDataPoint& Without = WithoutDataset.Push();
						GenerateInputs(GameObject, With.Inputs, true);
						GenerateInputs(GameObject, Without.Inputs, false);

						AI.GradeMoves(GameObject, With.Outputs, true);
						AI.GradeMoves(GameObject, Without.Outputs, false);
						FPlayedMove Move = AI.ChooseMove(GameObject, Without.Outputs);
						GameObject.Play(Move.X, Move.Y, false);
					}
					else
					{
						FPlayedMove Move = AI.ChooseMove(GameObject);
						GameObject.Play(Move.X, Move.Y, false);
					}
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

	// training
	TrainNetwork(WithNetwork, WithDataset);
	TrainNetwork(WithoutNetwork, WithoutDataset);

	// testing
	int32 WithWins = 0;
	int32 WithoutWins = 0;
	for (int32 TestGameIndex = 0; TestGameIndex < Test.Count(); ++TestGameIndex)
	{
		const FGameSetup& TestSetup = Test[TestGameIndex];

		FMath::SetRandomSeed(TestSetup.Seed);
		GameObject.SetupGame(*TestSetup.MapGen);
		int32 Winner = EvalTestGame(GameObject, 0, TestSetup, WithNetwork, WithoutNetwork, OutputCount, AI);
		WithWins += (Winner == 0);
		WithoutWins += (Winner == 2);

		FMath::SetRandomSeed(TestSetup.Seed);
		GameObject.SetupGame(*TestSetup.MapGen);
		Winner = EvalTestGame(GameObject, 2, TestSetup, WithNetwork, WithoutNetwork, OutputCount, AI);
		WithWins += (Winner == 0);
		WithoutWins += (Winner == 2);
	}

	std::cout << "With rotation wins: " << WithWins << "\n";
	std::cout << "Without rotation wins: " << WithoutWins << "\n";
}
