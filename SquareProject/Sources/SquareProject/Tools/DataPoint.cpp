#include <SquareProject/StdH.h>
#include <SquareProject/Tools/DataPoint.h>

FDataPoint::FDataPoint() = default;

FDataPoint& FDataPoint::operator=(const FDataPoint& Copied)
{
	Inputs = Copied.Inputs;
	Outputs = Copied.Inputs;
	return *this;
}

FDataPoint::FDataPoint(const FDataPoint& Copied)
{
	*this = Copied;
}

FDataPoint& FDataPoint::operator=(FDataPoint&& Copied) noexcept
{
	Inputs = Move(Copied.Inputs);
	Outputs = Move(Copied.Outputs);
	return *this;
}

FDataPoint::FDataPoint(FDataPoint&& Copied) noexcept
{
	*this = Move(Copied);
}
