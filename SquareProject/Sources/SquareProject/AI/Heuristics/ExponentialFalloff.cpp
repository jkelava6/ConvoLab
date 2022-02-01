#include <SquareProject/StdH.h>
#include <SquareProject/AI/Heuristics/ExponentialFalloff.h>

#include <SquareProject/Game/GameCore.h>

#include <Core/DynamicArray.h>

FPlayedMove FExponentialFalloffAI::ChooseMove(const FGame& GameObject)
{
	TArray<float> Grades;
	GradeMoves(GameObject, Grades, false);
	return ChooseMove(GameObject, Grades);
}

FPlayedMove FExponentialFalloffAI::ChooseMove(const FGame& GameObject, const TArray<float>& OutGrades)
{
	const int32 MapSize = GameObject.GetMapSize();
	const int32 SquareCount = FMath::SquareI(MapSize);

	int32 BestSquare = 0;
	float BestGrade = OutGrades[0];
	for (int32 Square = 1; Square < SquareCount; ++Square)
	{
		if (OutGrades[Square] > BestGrade)
		{
			BestSquare = Square;
			BestGrade = OutGrades[Square];
		}
	}

	return FPlayedMove(BestSquare / MapSize, BestSquare % MapSize);
}

void FExponentialFalloffAI::GradeMoves(const FGame& GameObject, TArray<float>& OutGrades, bool bRotateFromPlayersView/* = false*/)
{
	const int32 MapSize = GameObject.GetMapSize();
	const int32 SquareCount = FMath::SquareI(MapSize);
	OutGrades.Prealocate(SquareCount);
	OutGrades.PopAll();
	for (int32 Square = 0; Square < SquareCount; ++Square)
	{
		OutGrades.Push() = 0.0f;
	}

	for (int32 SourceX = 0; SourceX < MapSize; ++SourceX)
	{
		for (int32 SourceY = 0; SourceY < MapSize; ++SourceY)
		{
			const FSquare SourceSquare = GameObject.GetSquare(SourceX, SourceY, bRotateFromPlayersView);
			const bool bSquareOwned = (SourceSquare.SquareOwnership == GameObject.GetCurrentPlayer());

			const int32 Resource = (int32)((SourceSquare.Modifier & ESquareModifier::ResourceMask) >> ESquareModifier::ResourceShift);
			float Incentive = (float)Resource;
			if (bSquareOwned)
			{
				Incentive = 0.5f * Incentive * FMath::PowerNat(-3 * ResourceSteepness) / FMath::PowerNat((float)SourceSquare.UnitCount);
			}

			const float Avoidance = (bSquareOwned
				? 0.0f
				: 0.1f * FMath::Tanh(SourceSquare.UnitCount / 5.0f));

			for (int32 X = 0; X < MapSize; ++X)
			{
				for (int32 Y = 0; Y < MapSize; ++Y)
				{
					const FSquare& Square = GameObject.GetSquare(X, Y, bRotateFromPlayersView);

					const int32 Distance = FMath::AbsI(X - SourceX) + FMath::AbsI(Y - SourceY);
					OutGrades[X * MapSize + Y] += Incentive * FMath::PowerNat(-ResourceSteepness * Distance)
						- Avoidance * FMath::PowerNat(-DefenseSteepness * Distance);
				}
			}
		}
	}

	for (int32 X = 0; X < MapSize; ++X)
	{
		for (int32 Y = 0; Y < MapSize; ++Y)
		{
			const FSquare& Square = GameObject.GetSquare(X, Y, bRotateFromPlayersView);

			if (Square.SquareOwnership != GameObject.GetCurrentPlayer())
			{
				OutGrades[X * MapSize + Y] = 0.0f;
			}
			else
			{
				OutGrades[X * MapSize + Y] = FMath::Tanh(OutGrades[X * MapSize + Y] * FMath::PowerNat(-0.3f * Square.UnitCount));
			}
		}
	}
}

void FExponentialFalloffAI::SetFalloffSteepness(float InResourceSteepness, float InDefenseSteepness)
{
	ResourceSteepness = InResourceSteepness;
	DefenseSteepness = InDefenseSteepness;
}
