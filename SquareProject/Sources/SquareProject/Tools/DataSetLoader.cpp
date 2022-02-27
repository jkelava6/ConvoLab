#include <SquareProject/StdH.h>
#include <SquareProject/Tools/DataSetLoader.h>

#include <SquareProject/Tools/DataPoint.h>

#include <fstream>
#include <string>


TArray<FDataPoint>* FDataSetLoader::ReadDataSetFile(const std::string& FileName)
{
	std::ifstream File;
	File.open(FileName, std::ios::in);
	TArray<FDataPoint>* DataSet = new TArray<FDataPoint>();

	int32 TestCount;
	int32 InputSize;
	int32 OutputSize;
	File >> TestCount >> InputSize >> OutputSize;

	DataSet->Prealocate(TestCount);
	for (int32 TestIndex = 0; TestIndex < TestCount; ++TestIndex)
	{
		FDataPoint& Test = DataSet->Push();
		File >> Test.bProvideDifferentialFeedback;

		float Value;
		Test.Inputs.Prealocate(InputSize);
		for (int32 Input = 0; Input < InputSize; ++Input)
		{
			File >> Value;
			Test.Inputs.Add(Value);
		}
		Test.Outputs.Prealocate(OutputSize);
		for (int32 Output = 0; Output < OutputSize; ++Output)
		{
			File >> Value;
			Test.Outputs.Add(Value);
		}
	}

	File.close();
	return DataSet;
}

void FDataSetLoader::WriteDataSetFile(const std::string& FileName, TArray<FDataPoint>& DataSet)
{
	std::ofstream File;
	File.open(FileName, std::ios::out);

	int32 TestCount = DataSet.Count();
	int32 InputSize = DataSet[0].Inputs.Count();
	int32 OutputSize = DataSet[0].Outputs.Count();
	File << TestCount << " " << InputSize << " " << OutputSize << "\n";

	for (int32 TestIndex = 0; TestIndex < TestCount; ++TestIndex)
	{
		FDataPoint& Test = DataSet[TestIndex];
		File << Test.bProvideDifferentialFeedback << "\n";
		for (int32 Input = 0; Input < InputSize; ++Input)
		{
			File << Test.Inputs[Input] << " ";
		}
		File << "\n";
		for (int32 Output = 0; Output < OutputSize; ++Output)
		{
			File << Test.Outputs[Output] << " ";
		}
		File << "\n";
	}

	File.close();
}
