#include <SquareProject/StdH.h>
#include <SquareProject/Game/GameSetup.h>

FGameSetup::FGameSetup() = default;

FGameSetup::FGameSetup(IMapGenerator* InMapGen, uint32 InSeed)
	: MapGen(InMapGen)
	, Seed(InSeed)
{
	Players.Prealocate(4);
	for (int32 Player = 0; Player < 4; ++Player)
	{
		Players.Add(!(Player & 1));
	}
}
