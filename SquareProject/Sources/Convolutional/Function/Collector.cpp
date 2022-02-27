#include <Convolutional/StdH.h>
#include <Convolutional/Function/Collector.h>

CONVOLUTION_FUNCTION_NAMESPACE_BEGIN

bool ICollector::Equals(ICollector& Other)
{
	return GetUID() == Other.GetUID();
}

void FSum::Init()
{
	Sum = 0.0f;
}

void FSum::Collect(float X)
{
	Sum += X;
}

float FSum::Finalize()
{
	return Sum;
}

ICollector* FSum::Clone() const
{
	return new FSum();
}

float FSum::Derivate(float Input) const
{
	return 1.0f;
}

void FAverage::Init()
{
	Sum = 0.0f;
	Count = 0;
}

void FAverage::Collect(float X)
{
	Sum += X;
	++Count;
}

float FAverage::Finalize()
{
	return Sum / Count;
}

ICollector* FAverage::Clone() const
{
	return new FAverage();
}

float FAverage::Derivate(float Input) const
{
	return 1.0f / Count;
}

CONVOLUTION_FUNCTION_NAMESPACE_END
