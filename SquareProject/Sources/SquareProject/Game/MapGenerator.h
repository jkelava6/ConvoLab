#pragma once

class FGame;

class IMapGenerator
{
public:
	virtual int32 GetMapSize() = 0;
	virtual void PopulateMap(FGame& GameObject) = 0;
};
