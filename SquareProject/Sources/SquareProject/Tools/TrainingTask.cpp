#include <SquareProject/StdH.h>
#include <SquareProject/Tools/TrainingTask.h>

#include <SquareProject/Tools/DataPoint.h>

#include <Convolutional/Instance/NetworkInstance.h>

#include <iostream>

FTrainingTask::FTrainingTask() = default;

FTrainingTask::FTrainingTask(int32 InNumOfIterations, float InMaxStep, float InGradientScale, bool bInUseGroupTraining)
	: NumOfIterations(InNumOfIterations)
	, MaxStep(InMaxStep)
	, GradientScale(InGradientScale)
	, bUseGroupTraining(bInUseGroupTraining)
	, NumOfReports(InNumOfIterations)
{
}

void FTrainingTask::Train(FConvolutionInstance::FNetwork& Network, TArray<FDataPoint>& DataSet,
	FunctionPointer(float, ErrorFunction, void* TestContextObject)/* = nullptr*/, void* ContextObject/* = nullptr*/)
{
	const FDataPoint& FirstDataPoint = DataSet[0];
	const int32 NumOfInputs = FirstDataPoint.Inputs.Count();
	const int32 NumOfOutputs = FirstDataPoint.Outputs.Count();

	int32 LastReport = -1;
	double LastErrorSum = 0.0;
	double LastSquaredSum = 0.0;
	for (int32 Iteration = 0; Iteration < NumOfIterations; ++Iteration)
	{
		TArray<float> Gradient;
		TArray<float> NetworkOutputs;
		for (int32 Output = 0; Output < NumOfOutputs; ++Output)
		{
			Gradient.Add(0.0f);
			NetworkOutputs.Push();
		}

		double ErrorSum = 0.0;
		double ErrorSquaredSum = 0.0;
		for (int32 DataPointIndex = 0; DataPointIndex < DataSet.Count(); ++DataPointIndex)
		{
			const FDataPoint& Data = DataSet[DataPointIndex];
#if true
			if (!Data.bProvideDifferentialFeedback)
			{
				continue;
			}
#endif
			Network.Evaluate(Data.Inputs, NetworkOutputs);
			for (int32 Output = 0; Output < NumOfOutputs; ++Output)
			{
				const float Error = NetworkOutputs[Output] - Data.Outputs[Output];
				Gradient[Output] = Error; // should this be plus or minus?

				ErrorSum += FMath::AbsF(Error);
				ErrorSquaredSum += FMath::SquareF(Error);
			}
			if (Data.bProvideDifferentialFeedback)
			{
				Network.Backpropagate(Gradient);
				if (!bUseGroupTraining)
				{
					Network.ApplyBackProp(MaxStep, GradientScale);
				}
			}
		}

		if (bUseGroupTraining)
		{
			TestNetwork = &Network;
			TestDataSet = &DataSet;
			if (ErrorFunction != nullptr && ContextObject != nullptr)
			{
				Network.ExponentialBackProp(GradientScale, MaxStep, ErrorFunction, true, ContextObject);
			}
			else
			{
				Network.ExponentialBackProp(GradientScale, MaxStep, CalculateError, true, this);
			}
		}

		const int32 ReportIndex = (Iteration * NumOfReports) / NumOfIterations;
		if (ReportIndex > LastReport)
		{
			std::cout << "Iteration " << Iteration << " (" << ((100 * Iteration) / NumOfIterations)
				<< "%) Error Sum: " << ErrorSum << " Squared Error Sum: " << ErrorSquaredSum
				<< "\nError Sum Diff: " << ErrorSum - LastErrorSum << " Square Sum Diff: " << ErrorSquaredSum - LastSquaredSum << "\n";
			LastReport = ReportIndex;
		}

		LastErrorSum = ErrorSum;
		LastSquaredSum = ErrorSquaredSum;
	}
}

float FTrainingTask::CalculateError(void* ContextObject)
{
	FTrainingTask* Task = (FTrainingTask*)ContextObject;

	TArray<FDataPoint>& DataSet = *Task->TestDataSet;
	const FDataPoint& FirstDataPoint = DataSet[0];
	const int32 NumOfInputs = FirstDataPoint.Inputs.Count();
	const int32 NumOfOutputs = FirstDataPoint.Outputs.Count();
	TArray<float> NetworkOutputs;
	for (int32 Output = 0; Output < NumOfOutputs; ++Output)
	{
		NetworkOutputs.Push();
	}

	double ErrorSquaredSum = 0.0;
	for (int32 DataPointIndex = 0; DataPointIndex < DataSet.Count(); ++DataPointIndex)
	{
		const FDataPoint& Data = DataSet[DataPointIndex];
		if (!Data.bProvideDifferentialFeedback)
		{
			continue;
		}
		Task->TestNetwork->Evaluate(Data.Inputs, NetworkOutputs);
		for (int32 Output = 0; Output < NumOfOutputs; ++Output)
		{
			const float Error = NetworkOutputs[Output] - Data.Outputs[Output];
			ErrorSquaredSum += FMath::SquareF(Error);
		}
	}

	return (float)ErrorSquaredSum;
}
