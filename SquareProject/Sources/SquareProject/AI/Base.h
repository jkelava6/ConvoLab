#pragma once

#include <Core/DynamicArray.h>

class FGame;

class FPlayedMove
{
public:
	FPlayedMove();
	FPlayedMove(int32 InX, int32 InY);
public:
	int32 X = -1;
	int32 Y = -1;
};

class IArtificialIntelligence
{
public:
	virtual FPlayedMove ChooseMove(const FGame& GameObject) = 0;
	virtual FPlayedMove ChooseMove(const FGame& GameObject, const TArray<float>& Grades) = 0;
	virtual void GradeMoves(const FGame& GameObject, TArray<float>& OutGrades, bool bRotateFromPlayersView = false) = 0;
};
