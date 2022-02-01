#pragma once

#include <Core/DynamicArray.h>

class IMapGenerator;

class FGameSetup
{
public:
	FGameSetup();
	FGameSetup(IMapGenerator* InMapGen, uint32 InSeed);
public:
	IMapGenerator* MapGen = nullptr;
	TArray<bool> Players;
	uint32 Seed = 0;
};
