#include <SquareProject/StdH.h>
#include <SquareProject/MapGen/AlternateRing.h>

#include <SquareProject/Game/GameCore.h>

FAlteranateRingGenerator::FAlteranateRingGenerator(int32 InMapSize, const TArray<bool>& bPlayerExists)
	: MapSize(InMapSize)
{
	GeneratePlayers.Prealocate(4);
	for (int Player = 0; Player < 4; ++Player)
	{
		GeneratePlayers.Add(bPlayerExists[Player]);
	}
}

int32 FAlteranateRingGenerator::GetMapSize()
{
	return MapSize;
}

void FAlteranateRingGenerator::PopulateMap(FGame& GameObject)
{
	const float HalfSize = MapSize / 2.0f;
	const float RingRadius = 0.7f * HalfSize;
	const float PlayerRadius = 0.9f * HalfSize;

	const float RingLength = 2.0f * FMath::PI * RingRadius;
	const int32 FeatureCount = ((int32)((0.4f + FMath::RandomF() * 0.4f) * RingLength) & ~1);

	for (int32 Feature = 0; Feature < FeatureCount; ++Feature)
	{
		const float Angle = Feature * 2.0f * FMath::PI / FeatureCount;
		const int32 X = (int32)(HalfSize + RingRadius * FMath::Cos(Angle));
		const int32 Y = (int32)(HalfSize + RingRadius * FMath::Sin(Angle));

		FSquare& Square = GameObject.AccessSquare(X, Y);
		Square.Modifier = (Feature & 1) ? ESquareModifier::ResourcePlusOne : ESquareModifier::DefensePlusTen;
	}

	const int32 ResourceLevel = 3 + (int32)(3.0f * FMath::RandomF());
	const ESquareModifier ResourceMod = ((ESquareModifier)ResourceLevel << ESquareModifier::ResourceMask);
	const int32 DefenseLevel = 1 + (int32)(2.0f * FMath::RandomF());
	const ESquareModifier DefenseMod = ((ESquareModifier)DefenseLevel << ESquareModifier::DefenseMask);
	GameObject.AccessSquare((int32)HalfSize, (int32)HalfSize).Modifier = (ResourceMod | DefenseMod);

	for (int32 Player = 0; Player < 4; ++Player)
	{
		if (GeneratePlayers[Player])
		{
			const float Angle = Player * 2.0f * FMath::PI / 4;
			const int32 X = (int32)(HalfSize + PlayerRadius * FMath::Cos(Angle));
			const int32 Y = (int32)(HalfSize + PlayerRadius * FMath::Sin(Angle));

			FSquare& Square = GameObject.AccessSquare(X, Y);
			Square.UnitCount = 5;
			Square.SquareOwnership = Player;
			Square.Modifier = ESquareModifier::ResourcePlusTwo | ESquareModifier::DefensePlusTen;
		}
	}

}
