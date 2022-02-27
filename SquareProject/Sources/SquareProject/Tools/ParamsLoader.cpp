#include <SquareProject/StdH.h>
#include <SquareProject/Tools/ParamsLoader.h>

#include <Convolutional/Function/Collector.h>
#include <Convolutional/Function/Mapping.h>
#include <Convolutional/Params/Block.h>
#include <Convolutional/Params/Connection.h>
#include <Convolutional/Params/Network.h>

#include <fstream>
#include <string>

static FConvolutionFunction::ICollector* SavableCollectors[] =
{
	new FConvolutionFunction::FSum(),
	new FConvolutionFunction::FAverage(),
};

static FConvolutionFunction::IFloatToFloat* SavableMappers[] =
{
	new FConvolutionFunction::FLinear(),
	new FConvolutionFunction::FReLu(),
	new FConvolutionFunction::FLeakyReLu(1e-3f),
	new FConvolutionFunction::FLeakyReLu(1e-2f),
	new FConvolutionFunction::FLeakyReLu(1e-1f),
	new FConvolutionFunction::FLeakyReLu(0.5f),
	new FConvolutionFunction::FTangens(),
};

static int32 SaveCollector(FConvolutionFunction::ICollector* Collector)
{
	for (int32 Index = 0; Index < ARRAY_SIZE(SavableCollectors); ++Index)
	{
		if (Collector->Equals(*SavableCollectors[Index]))
		{
			return Index;
		}
	}
	return 0;
}

static FConvolutionFunction::ICollector* LoadCollector(int32 SavedIndex)
{
	return SavableCollectors[SavedIndex];
}

static int32 SaveMapper(FConvolutionFunction::IFloatToFloat* Collector)
{
	for (int32 Index = 0; Index < ARRAY_SIZE(SavableMappers); ++Index)
	{
		if (Collector->Equals(*SavableMappers[Index]))
		{
			return Index;
		}
	}
	return 0;
}

static FConvolutionFunction::IFloatToFloat* LoadMapper(int32 SavedIndex)
{
	return SavableMappers[SavedIndex];
}

FConvolutionParams::FNetwork* FParamsLoader::ReadParamsFile(const std::string& FileName)
{
	std::ifstream File;
	File.open(FileName, std::ios::in);
	FConvolutionParams::FNetwork* Params = new FConvolutionParams::FNetwork();

	int32 NumOfBlocks;
	File >> NumOfBlocks;
	Params->Blocks.Prealocate(NumOfBlocks);
	
	for (int32 BlockIndex = 0; BlockIndex < NumOfBlocks; ++BlockIndex)
	{
		int32 SizeX;
		int32 SizeY;
		int32 NumOfFeatures;
		int32 CollectorCode;
		int32 MapperCode;
		File >> SizeX >> SizeY >> NumOfFeatures >> CollectorCode >> MapperCode;
		FConvolutionParams::FBlock Block(SizeX, SizeY, NumOfFeatures, LoadCollector(CollectorCode), LoadMapper(MapperCode));
		int32 NumOfConnections;
		File >> NumOfConnections;
		for (int32 ConnectionIndex = 0; ConnectionIndex < NumOfConnections; ++ConnectionIndex)
		{
			int32 BlockIndex;
			float ScaleX;
			int32 LoX;
			int32 LoY;
			float ScaleY;
			int32 HiX;
			int32 HiY;
			bool bAllFeatures;
			File >> BlockIndex >> ScaleX >> LoX >> HiX >> ScaleY >> LoX >> HiY >> bAllFeatures;
			// TODO: This does not support recurrent networks.
			Block.Connections.Emplace(Params->Blocks[BlockIndex], ScaleX, LoX, HiX, ScaleY, LoY, HiY, bAllFeatures);
		}
	}

	File.close();
	return Params;
}

void FParamsLoader::WriteParamsFile(const std::string& FileName, FConvolutionParams::FNetwork& Network)
{
	std::ofstream File;
	File.open(FileName, std::ios::out);
	const int32 NumOfBlocks = Network.Blocks.Count();
	File << NumOfBlocks << '\n';

	for (int32 BlockIndex = 0; BlockIndex < NumOfBlocks; ++BlockIndex)
	{
		FConvolutionParams::FBlock* Block = Network.Blocks[BlockIndex];
		File << Block->X << " " << Block->Y << " " << Block->Features << " " << SaveCollector(Block->Collector) << " " << SaveMapper(Block->Mapper) << "\n";
		
		const int32 NumOfConnections = Block->Connections.Count();
		File << NumOfConnections << "\n";

		for (int32 ConnectionIndex = 0; ConnectionIndex < NumOfConnections; ++ConnectionIndex)
		{
			FConvolutionParams::FConnection& Connection = Block->Connections[ConnectionIndex];
			File << Network.Blocks.IndexOf(&Connection.Block) << " "
				<< Connection.XFactor << " " << Connection.LoX << " " << Connection.HiX << " "
				<< Connection.YFactor << " " << Connection.LoY << " " << Connection.HiY << " "
				<< Connection.bAllFeatures << "\n";
		}
	}
	
	File.close();
}
