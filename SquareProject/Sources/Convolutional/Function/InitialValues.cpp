#include <Convolutional/StdH.h>
#include <Convolutional/Function/InitialValues.h>

#include <Convolutional/Function/ParamBounds.h>

#include <Core/MathTools.h>

CONVOLUTION_FUNCTION_NAMESPACE_BEGIN

extern float GetInitialBiasValue()
{
	return MaxInitialBias * (-1.0f + 2.0f * FMath::RandomF());
}

extern float GetInitialWeightValue()
{
	return MaxInitialWeight * (-1.0f + 2.0f * FMath::RandomF());
}

CONVOLUTION_FUNCTION_NAMESPACE_END
