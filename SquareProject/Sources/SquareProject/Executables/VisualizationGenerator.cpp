#include <SquareProject/StdH.h>
#include <SquareProject/Executables/VisualizationGenerator.h>

#include <SquareProject/Tools/DataPoint.h>
#include <SquareProject/Tools/DataSetLoader.h>
#include <SquareProject/Tools/DnaLoader.h>
#include <SquareProject/Tools/GenerateVisualizationData.h>
#include <SquareProject/Tools/ParamsLoader.h>

#include <Convolutional/Instance/NetworkInstance.h>
#include <Convolutional/Params/Network.h>

#include <JsonIO/Service/JsonReader.h>
#include <JsonIO/Data/JsonMap.h>
#include <JsonIO/Data/JsonText.h>

#include <Core/Dna.h>
#include <Core/DynamicArray.h>

extern void GenerateVisualizationFilesProgram()
{
	FJsonMap* Setup = &FJsonReader::ReadJsonObject()->AsMap();

	std::string DatasetFileName = Setup->Data["dataset_file"]->AsText().Text;
	std::string NetworkParamsFileName = Setup->Data["params_file"]->AsText().Text;
	std::string DnaFileName = Setup->Data["dna_file"]->AsText().Text;
	std::string OutputTarget = Setup->Data["output_target"]->AsText().Text;

	TArray<FDataPoint>* DataSet = FDataSetLoader::ReadDataSetFile(DatasetFileName);
	FDna* Dna = FDnaLoader::ReadDnaFile(DnaFileName);
	FConvolutionParams::FNetwork* Params = FParamsLoader::ReadParamsFile(NetworkParamsFileName);
	FConvolutionInstance::FNetwork* Network = new FConvolutionInstance::FNetwork(*Params, Dna);

	GenerateVisualizationData(*Network, *DataSet, OutputTarget);

	delete Network;
	delete Params;
	delete Dna;
	delete DataSet;

	delete Setup;
}
