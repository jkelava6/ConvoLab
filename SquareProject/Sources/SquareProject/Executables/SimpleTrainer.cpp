#include <SquareProject/StdH.h>
#include <SquareProject/Executables/SimpleTrainer.h>

#include <SquareProject/Tools/DataPoint.h>
#include <SquareProject/Tools/DataSetLoader.h>
#include <SquareProject/Tools/DnaLoader.h>
#include <SquareProject/Tools/ParamsLoader.h>
#include <SquareProject/Tools/TrainingTask.h>

#include <Convolutional/Instance/BlockInstance.h>
#include <Convolutional/Instance/ConnectionInstance.h>
#include <Convolutional/Instance/NetworkInstance.h>
#include <Convolutional/Params/Network.h>

#include <JsonIO/Data/JsonMap.h>
#include <JsonIO/Data/JsonText.h>
#include <JsonIO/Service/JsonReader.h>

#include <Core/Dna.h>
#include <Core/DynamicArray.h>

#include <sstream>
#include <string>

extern void SimpleTraining()
{
	FJsonMap* Settings = &FJsonReader::ReadJsonObject()->AsMap();
	std::string DataSetFileName = Settings->Data["dataset_file"]->AsText().Text;
	std::string InputDnaFileName = Settings->Data["input_file"]->AsText().Text;
	std::string OutputDnaFileName = Settings->Data["output_file"]->AsText().Text;
	std::string ParamsFileName = Settings->Data["params_file"]->AsText().Text;

	TArray<FDataPoint>* DataSet = FDataSetLoader::ReadDataSetFile(DataSetFileName);
	FDna* ConstructionDna = InputDnaFileName.length() > 0 ? FDnaLoader::ReadDnaFile(InputDnaFileName) : nullptr;

	FConvolutionParams::FNetwork* Params = FParamsLoader::ReadParamsFile(ParamsFileName);
	// load inpui
	FConvolutionInstance::FNetwork* Network = new FConvolutionInstance::FNetwork(*Params, ConstructionDna);

	FJsonMap& Training = Settings->Data["training_rounds"]->AsMap();
	int32 NumOfRounds = std::stoi(Training.Data["num_of_rounds"]->AsText().Text);
	char RoundName[10];
	for (int32 Round = 0; Round < NumOfRounds; ++Round)
	{
		sprintf_s(RoundName, "%d", Round);
		FJsonMap& RoundData = Training.Data[RoundName]->AsMap();
		int32 NumOfIterations = std::stoi(RoundData.Data["iterations"]->AsText().Text);
		float MaxStep = std::stof(RoundData.Data["max_step"]->AsText().Text);
		float Scale = std::stof(RoundData.Data["gradient_scale"]->AsText().Text);
		bool bUseGroupTraining = !!std::stoi(RoundData.Data["group_training"]->AsText().Text);
		FTrainingTask Training(NumOfIterations, MaxStep, Scale, bUseGroupTraining);
		Training.Train(*Network, *DataSet);
	}

	FDna TrainedDna;
	Network->Serialize(TrainedDna);
	TrainedDna.Seek(0);
	FDnaLoader::WriteDnaFile(OutputDnaFileName, TrainedDna);

	delete DataSet;
	if (ConstructionDna)
	{
		delete ConstructionDna;
	}
}
