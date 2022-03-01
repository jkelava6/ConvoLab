#include <SquareProject/StdH.h>
#include <SquareProject/Tools/GenerateVisualizationData.h>

#include <SquareProject/Tools/DataPoint.h>

#include <Convolutional/Instance/BlockInstance.h>
#include <Convolutional/Instance/NetworkInstance.h>

#include <Core/DynamicArray.h>

#include <fstream>

extern void GenerateVisualizationData(FConvolutionInstance::FNetwork& Network, TArray<FDataPoint>& Dataset, std::string Destination)
{
	std::fstream File;
	char FileName[20];
	const int32 MapSize = Network.Blocks[0].GetSizeX();

	for (int32 TestIndex = 0; TestIndex < Dataset.Count(); ++TestIndex)
	{
		FDataPoint& DP = Dataset[TestIndex];
		TArray<float> DummyOutputs;
		DummyOutputs.Prealocate(DP.Outputs.Count());
		for (int32 DOI = 0; DOI < DP.Outputs.Count(); ++DOI)
		{
			DummyOutputs.Push();
		}
		Network.Evaluate(DP.Inputs, DummyOutputs);

		sprintf_s(FileName, "Test%d.txt", TestIndex);
		File.open(Destination + FileName, std::ios_base::out);

		const int32 NumOfBlocks = Network.Blocks.Count();
		File << NumOfBlocks << "\n";

		for (int32 BlockIndex = 0; BlockIndex < NumOfBlocks; ++BlockIndex)
		{
			FConvolutionInstance::FBlock& Block = Network.Blocks[BlockIndex];
			const int32 SizeX = Block.GetSizeX();
			const int32 SizeY = Block.GetSizeY();
			const int32 NumOfFeatures = Block.GetNumOfFeatures();
			if (BlockIndex + 1 == NumOfBlocks)
			{
				File << SizeX << " " << SizeY << " " << NumOfFeatures + 1 << "\n";
			}
			else
			{
				File << SizeX << " " << SizeY << " " << NumOfFeatures << "\n";
			}

			for (int32 Feature = 0; Feature < NumOfFeatures; ++Feature)
			{
				for (int32 X = 0; X < SizeX; ++X)
				{
					for (int32 Y = 0; Y < SizeY; ++Y)
					{
						File << Block.GetState(Feature, X, Y) << " ";
					}
					File << "\n";
				}
				File << "\n";
			}

			if (BlockIndex + 1 == NumOfBlocks)
			{
				for (int32 Feature = 0; Feature < 1; ++Feature)
				{
					for (int32 X = 0; X < MapSize; ++X)
					{
						for (int32 Y = 0; Y < MapSize; ++Y)
						{
							File << DP.Outputs[Feature * MapSize * MapSize + X * MapSize + Y] << " ";
						}
						File << "\n";
					}
					File << "\n";
				}
			}
		}

		File.close();
	}
}
