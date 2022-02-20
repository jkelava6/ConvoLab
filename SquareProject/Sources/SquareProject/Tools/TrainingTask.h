#pragma once

CONVOLUTION_INSTANCE_NAMESPACE_BEGIN
class FNetwork;
CONVOLUTION_INSTANCE_NAMESPACE_END
class FDataPoint;
template <class T> class TArray;

class FTrainingTask
{
public:
	FTrainingTask();
	FTrainingTask(int32 InNumOfIterations, float InMaxStep, float InGradientScale, bool bInUseGroupTraining);
	void Train(FConvolutionInstance::FNetwork& Network, TArray<FDataPoint>& DataSet,
		FunctionPointer(float, ErrorFunction, void* TestContextObject) = nullptr, void* ContextObject = nullptr);

private:
	static float CalculateError(void* ContextObject);

public:
	int32 NumOfIterations = 100;
	float MaxStep = 1e-3f;
	float GradientScale = -1e0f;
	bool bUseGroupTraining;

	int32 NumOfReports = 100;

private:
	FConvolutionInstance::FNetwork* TestNetwork = nullptr;
	TArray<FDataPoint>* TestDataSet = nullptr;
};
