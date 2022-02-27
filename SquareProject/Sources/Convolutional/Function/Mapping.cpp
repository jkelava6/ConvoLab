#include <Convolutional/StdH.h>
#include <Convolutional/Function/Mapping.h>

#include <Core/MathTools.h>

CONVOLUTION_FUNCTION_NAMESPACE_BEGIN

IFloatToFloat* IFloatToFloat::Derivation()
{
	return nullptr;
}

float IFloatToFloat::DerivateAt(float X)
{
	const float IdealDelta = 1e-6f;
	const float Delta = FMath::Max(IdealDelta, X * IdealDelta);
	return (EvaluateAt(X + Delta) - EvaluateAt(X)) / Delta;
}

bool IFloatToFloat::Equals(IFloatToFloat& Other)
{
	return GetUID() == Other.GetUID();
}

float FLinear::EvaluateAt(float X)
{
	return X;
}

float FLinear::DerivateAt(float X)
{
	return 1;
}

float FReLu::EvaluateAt(float X)
{
	return X > 0.0f ? X : 0.0f;
}

float FReLu::DerivateAt(float X)
{
	return X > 0.0f ? 1.0f : 0.0f;
}

float FLeakyReLu::EvaluateAt(float X)
{
	return X > 0.0f ? X : Leak * X;
}

float FLeakyReLu::DerivateAt(float X)
{
	return X > 0.0f ? 1.0f : Leak;
}

bool FLeakyReLu::Equals(IFloatToFloat& Other)
{
	if (GetUID() != Other.GetUID())
	{
		return false;
	}
	FLeakyReLu* OtherReLu = (FLeakyReLu*)&Other;
	return Leak == OtherReLu->Leak;
}

float FTangens::EvaluateAt(float X)
{
	return FMath::Tanh(X);
}

CONVOLUTION_FUNCTION_NAMESPACE_END
