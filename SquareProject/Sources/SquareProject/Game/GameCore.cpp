#include <SquareProject/StdH.h>
#include <SquareProject/Game/GameCore.h>

#include <SquareProject/Game/MapGenerator.h>

ESquareModifier operator&(const ESquareModifier& Left, const ESquareModifier& Right)
{
	return (ESquareModifier)((uint32)Left & (uint32)Right);
}

ESquareModifier operator|(const ESquareModifier& Left, const ESquareModifier& Right)
{
	return (ESquareModifier)((uint32)Left | (uint32)Right);
}

ESquareModifier operator>>(const ESquareModifier& Left, const ESquareModifier& Right)
{
	return (ESquareModifier)((uint32)Left >> (uint32)Right);
}

ESquareModifier operator<<(const ESquareModifier& Left, const ESquareModifier& Right)
{
	return (ESquareModifier)((uint32)Left << (uint32)Right);
}

FSquare::FSquare() = default;

FSquare::FSquare(ESquareModifier InModifier, int32 InSquareOwnership, int32 InUnitCount)
	: Modifier(InModifier)
	, SquareOwnership(InSquareOwnership)
	, UnitCount(InUnitCount)
{
}

FGame::FGame() = default;

void FGame::SetupGame(IMapGenerator& Generator)
{
	MapSize = Generator.GetMapSize();
	const int32 SquareCount = FMath::SquareI(MapSize);
	Squares.Prealocate(SquareCount);
	for (int32 Square = 0; Square < Squares.Count(); ++Square)
	{
		Squares[Square] = FSquare();
	}
	for (int32 Square = Squares.Count(); Square < SquareCount; ++Square)
	{
		Squares.Add(FSquare());
	}

	Generator.PopulateMap(*this);
}

int32 FGame::GetResourceAmount(int32 Player/* = -1*/) const
{
	if (Player == -1)
	{
		Player = CurrentPlayer;
	}
	int32 Resources = 0;

	for (int32 X = 0; X < MapSize; ++X)
	{
		for (int32 Y = 0; Y < MapSize; ++Y)
		{
			const FSquare& Square = GetSquare(X, Y, false);
			if (Square.SquareOwnership == Player)
			{
				Resources += (((int32)Square.Modifier & (int32)ESquareModifier::ResourceMask) >> (int32)ESquareModifier::ResourceShift);
			}
		}
	}

	return Resources;
}

void FGame::Play(int32 X, int32 Y, bool bRotateFromPlayerView/* = false*/)
{
	FSquare& TargetSquare = AccessSquare(X, Y, bRotateFromPlayerView);

	if (TargetSquare.SquareOwnership == CurrentPlayer)
	{
		TargetSquare.UnitCount += GetResourceAmount();
	}

	SkipTurn();
}

void FGame::SkipTurn()
{
	++CurrentPlayer;
	if (CurrentPlayer == 4)
	{
		CurrentPlayer = 0;
		GameTick();
	}
}

const FSquare& FGame::GetSquare(int32 X, int32 Y, bool bRotateFromPlayerView/* = false*/) const
{
	if (bRotateFromPlayerView)
	{
		for (int32 Rotation = 0; Rotation < CurrentPlayer; ++Rotation)
		{
			const int32 OldX = X;
			X = Y;
			Y = (MapSize - 1) - OldX;
		}
	}

	return Squares[X * MapSize + Y];
}

FSquare& FGame::AccessSquare(int32 X, int32 Y, bool bRotateFromPlayerView/* = false*/)
{
	if (bRotateFromPlayerView)
	{
		for (int32 Rotation = 0; Rotation < CurrentPlayer; ++Rotation)
		{
			const int32 OldX = X;
			X = Y;
			Y = (MapSize - 1) - OldX;
		}
	}

	return Squares[X * MapSize + Y];
}

void FGame::GameTick()
{
	// we will need to go through all the squares, but in random order
	TArray<int32> SquareOrder;
	const int32 SquareCount = FMath::SquareI(MapSize);
	SquareOrder.Prealocate(SquareCount);
	for (int32 Square = 0; Square < SquareCount; ++Square)
	{
		SquareOrder.Add(Square);
	}
	FMath::RandomPerm(&SquareOrder[0], SquareOrder.Count());

	// coordinate deltas for adjacent squares (will be permutated)
	int32 DX[] = { -1, 0, 1, 0 };
	int32 DY[] = { 0, -1, 0, 1 };

	// for each square....
	for (int32 SquareOrderIndex = 0; SquareOrderIndex < SquareCount; ++SquareOrderIndex)
	{
		const int32 SquareIndex = SquareOrder[SquareOrderIndex];
		const int32 X = SquareIndex / MapSize;
		const int32 Y = SquareIndex % MapSize;
		FSquare& Square = Squares[SquareIndex];

		if (Square.UnitCount < 2)
		{
			continue;
		}

		// permutate adjacency order
		for (int32 Delta = 3; Delta; --Delta)
		{
			const int32 PermIndex = FMath::RandomI(Delta + 1);
			int32 Temp = DX[PermIndex];
			DX[PermIndex] = DX[Delta];
			DX[Delta] = Temp;
			Temp = DY[PermIndex];
			DY[PermIndex] = DY[Delta];
			DY[Delta] = Temp;
		}

		// for each adjacent square
		for (int32 Direciton = 0; Direciton < 4; ++Direciton)
		{
			const int32 NX = X + DX[Direciton];
			const int32 NY = Y + DY[Direciton];

			if (NX < 0 || NX >= MapSize || NY < 0 || NY >= MapSize)
			{
				continue;
			}

			const int32 NewIndex = NX * MapSize + NY;
			FSquare& NewSquare = Squares[NewIndex];

			// move a unit if the difference in units of original square's color on these squares is two or more
			const bool bSamePlayer = (Square.SquareOwnership == NewSquare.SquareOwnership);
			if ((Square.UnitCount > 1 && !bSamePlayer) || (Square.UnitCount > NewSquare.UnitCount + 1))
			{
				// remove a unit from origin square
				--Square.UnitCount;
				// for moving to already owned square, simply add a new unit
				if (bSamePlayer)
				{
					++NewSquare.UnitCount;
				}
				// for unoccupied squares, take the ownership (and place the moved unit)
				else if (NewSquare.UnitCount == 0)
				{
					NewSquare.SquareOwnership = Square.SquareOwnership;
					NewSquare.UnitCount = 1;
				}
				// for unowned and guarded squares
				else
				{
					// check for any defense square property
					int32 DefenseLevel = (int32)((NewSquare.Modifier & ESquareModifier::DefenseMask) >> ESquareModifier::DefenseShift);
					if (FMath::RandomF() > 0.1f * DefenseLevel)
					{
						// remove a guard (and ownership if the last guard is removed)
						--NewSquare.UnitCount;
						if (NewSquare.UnitCount == 0)
						{
							NewSquare.SquareOwnership = -1;
						}
					}
				}
			}
		}
	}
}

int32 FGame::GetMapSize() const
{
	return MapSize;
}

int32 FGame::GetCurrentPlayer() const
{
	return CurrentPlayer;
}
