#pragma once

#include <Core/DynamicArray.h>

class IMapGenerator;

enum class ESquareModifier : uint32
{
	None = 0,

	ResourceShift = 0,
	ResourcePlusOne = 1 << ResourceShift,
	ResourcePlusTwo = 2 << ResourceShift,
	ResourcePlusThree = 3 << ResourceShift,
	ResourcePlusFour = 4 << ResourceShift,
	ResourcePlusFive = 5 << ResourceShift,
	ResourcePlusSix = 6 << ResourceShift,
	ResourcePlusSeven = 7 << ResourceShift,
	ResourceMask = 0x7 << ResourceShift,

	DefenseShift = 3,
	DefensePlusTen = 1 << DefenseShift,
	DefensePlusTwenty = 2 << DefenseShift,
	DefensePlusThirty = 3 << DefenseShift,
	DefensePlusFourty = 4 << DefenseShift,
	DefensePlusFifty = 5 << DefenseShift,
	DefensePlusSixty = 6 << DefenseShift,
	DefensePlusSeventy = 7 << DefenseShift,
	DefenseMask = 0x7 << DefenseShift,
};

ESquareModifier operator&(const ESquareModifier& Left, const ESquareModifier& Right);
ESquareModifier operator|(const ESquareModifier& Left, const ESquareModifier& Right);
ESquareModifier operator>>(const ESquareModifier& Left, const ESquareModifier& Right);
ESquareModifier operator<<(const ESquareModifier& Left, const ESquareModifier& Right);

class FSquare
{
public:
	FSquare();
	FSquare(ESquareModifier InModifier, int32 InSquareOwnership, int32 InUnitCount);
public:
	ESquareModifier Modifier = ESquareModifier::None;
	int32 SquareOwnership = -1;
	int32 UnitCount = 0;
};

class FGame
{
public:
	FGame();
	void SetupGame(IMapGenerator& Generator);
	int32 GetResourceAmount(int32 Player = -1) const;
	void Play(int32 X, int32 Y, bool bRotateFromPlayerView = false);
	void SkipTurn();

	const FSquare& GetSquare(int32 X, int32 Y, bool bRotateFromPlayerView = false) const;
	FSquare& AccessSquare(int32 X, int32 Y, bool bRotateFromPlayerView = false);
	int32 GetMapSize() const;
	int32 GetCurrentPlayer() const;
private:
	void GameTick();
private:
	int32 CurrentPlayer = 0;
	int32 MapSize = -1;
	TArray<FSquare> Squares;
};
