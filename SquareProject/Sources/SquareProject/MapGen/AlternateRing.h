#pragma once

#include <SquareProject/Game/MapGenerator.h>

#include <Core/DynamicArray.h>

class FAlteranateRingGenerator : public IMapGenerator
{
public:
	FAlteranateRingGenerator(int32 InMapSize, const TArray<bool>& bPlayerExists);

	virtual int32 GetMapSize() override;
	virtual void PopulateMap(FGame& GameObject) override;

private:
	int32 MapSize;
	TArray<bool> GeneratePlayers;
};
