#pragma once

#include <SquareProject/AI/Base.h>

class FExponentialFalloffAI : public IArtificialIntelligence
{
public:
	virtual FPlayedMove ChooseMove(const FGame& GameObject) override;
	virtual FPlayedMove ChooseMove(const FGame& GameObject, const TArray<float>& Grades) override;
	virtual void GradeMoves(const FGame& GameObject, TArray<float>& OutGrades, bool bRotateFromPlayersView = false) override;

	void SetFalloffSteepness(float InResourceSteepness, float InDefenseSteepness);

private:
	float ResourceSteepness = 0.3f;
	float DefenseSteepness = 0.7f;
};
