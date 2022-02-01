#include <SquareProject/StdH.h>
#include <SquareProject/AI/Base.h>

FPlayedMove::FPlayedMove() = default;

FPlayedMove::FPlayedMove(int32 InX, int32 InY)
	: X(InX)
	, Y(InY)
{
}
