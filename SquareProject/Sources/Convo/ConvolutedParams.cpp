#include <Convo/StdH.h>
#include <Convo/ConvolutedParams.h>

#include <NeuNet/BaseNetwork.h>

#include <Core/Dna.h>

FConvolutionResolution::FConvolutionResolution() = default;

FConvolutionResolution::FConvolutionResolution(int32 InFromX, int32 InFromY, int32 InToX, int32 InToY)
	: FromX(InFromX)
	, FromY(InFromY)
	, ToX(InToX)
	, ToY(InToY)
{
}

FConvolutionResolution::FConvolutionResolution(int32 X, int32 Y, int32 SizeX, int32 SizeY, const FConvolutionResolution& ReadReso)
{
	FromX = FMath::Max(X + ReadReso.FromX, 0);
	FromY = FMath::Max(Y + ReadReso.FromY, 0);
	ToX = FMath::Min(X + ReadReso.ToX, SizeX);
	ToY = FMath::Min(Y + ReadReso.ToY, SizeY);
}

FConvolutionBlockConnection::FConvolutionBlockConnection(int32 InFeederBlock, int32 InCompression, const FConvolutionResolution& InResolution)
	: FeederBlock(InFeederBlock)
	, Compression(InCompression)
	, Resolution(InResolution)
{
}

FConvolutionBlockParams::FConvolutionBlockParams(int32 InSizeX, int32 InSizeY, int32 InConvolutionLayers,
	const FConvolutionResolution& InResolution)
	: SizeX(InSizeX)
	, SizeY(InSizeY)
	, ConvolutionLayers(InConvolutionLayers)
	, Resolution(InResolution)
{
}

FConvolutedNetworkParams::FConvolutedNetworkParams() = default;

void FConvolutedNetworkParams::SetIOCount(int32 InInputs, int32 InOutputs)
{
	InputCount = InInputs;
	OutputCount = InOutputs;
}

int32 FConvolutedNetworkParams::AddBlock(const FConvolutionBlockParams& Block)
{
	const int32 AddedIndex = Blocks.Count();
	Blocks.Add(Block);
	return AddedIndex;
}

int32 FConvolutedNetworkParams::AddBlock(FConvolutionBlockParams&& Block)
{
	const int32 AddedIndex = Blocks.Count();
	Blocks.Add(Move(Block));
	return AddedIndex;
}

void FConvolutedNetworkParams::SetupNetwork(FBaseNetwork::FNetwork* TargetNetwork, FDna* InDna, FDna* OutDna) const
{
	TArray<int32> StartingNodes;
	StartingNodes.Prealocate(Blocks.Count());

	const int32 NumOfBlocks = Blocks.Count();
	for (int32 CurrentBlock = 0; CurrentBlock < NumOfBlocks; ++CurrentBlock)
	{
		const FConvolutionBlockParams& Block = Blocks[CurrentBlock];
		const float Start = (float)TargetNetwork->GetNeuronCount();
		const int32 SizeX = Block.SizeX;
		const int32 SizeY = Block.SizeY;
		for (int32 X = 0; X < SizeX; ++X)
		{
			for (int32 Y = 0; Y < SizeY; ++Y)
			{
				const float Bias = InDna ? InDna->ReadFloat() : BiasRange * (-1.0f + 2.0f * FMath::RandomF());
				if (TargetNetwork)
				{
					TargetNetwork->AddNode(Bias);
				}
				if (OutDna)
				{
					OutDna->PushFloat(Bias);
				}
			}
		}

		const TArray<FConvolutionBlockConnection>& Feeders = Block.Connections;
		const int32 NumOfFeeders = Feeders.Count();
		for (int32 ConnectionIndex = 0; ConnectionIndex < NumOfFeeders; ++ConnectionIndex)
		{
			const FConvolutionBlockConnection& Conn = Block.Connections[ConnectionIndex];
			const int32 FeederIndex = Conn.FeederBlock;
			const FConvolutionBlockParams& Feeder = Blocks[FeederIndex];
			for (int32 X = 0; X < SizeX; ++X)
			{
				for (int32 Y = 0; Y < SizeY; ++Y)
				{
					const int32 AddedIndex = X * SizeY + Y;
					const float Bias = InDna ? InDna->ReadFloat() : BiasRange * (-1.0f + 2.0f * FMath::RandomF());
					if (TargetNetwork)
					{
						TargetNetwork->AddNode(Bias);
					}
					if (OutDna)
					{
						OutDna->PushFloat(Bias);
					}
					const FConvolutionResolution FeedReso(X, Y, Feeder.SizeX, Feeder.SizeY, Conn.Resolution);

					for (int32 FeedX = FeedReso.FromX; FeedX < FeedReso.ToX; ++FeedX)
					{
						for (int32 FeedY = FeedReso.FromY; FeedY < FeedReso.ToY; ++FeedY)
						{
							const int32 FeedIndex = FeedX * Feeder.SizeY + FeedY;
							const float Weight = InDna ? InDna->ReadFloat() : WeightRange * (-1.0f + 2.0f * FMath::RandomF());
							if (TargetNetwork)
							{
								TargetNetwork->AddEdge(FeedIndex, AddedIndex, Weight);
							}
							if (OutDna)
							{
								OutDna->PushFloat(Weight);
							}
						}
					}
				}
			}
		}

		const int32 LayerSize = SizeX * SizeY;
		for (int32 CL = 0; CL < Block.ConvolutionLayers; ++CL)
		{
			for (int32 X = 0; X < SizeX; ++X)
			{
				for (int32 Y = 0; Y < SizeY; ++Y)
				{
					const int32 AddedIndex = X * SizeY + Y;
					const float Bias = InDna ? InDna->ReadFloat() : BiasRange * (-1.0f + 2.0f * FMath::RandomF());
					if (Bias)
					{
						TargetNetwork->AddNode(Bias);
					}
					if (OutDna)
					{
						OutDna->PushFloat(Bias);
					}
					const FConvolutionResolution FeedReso(X, Y, SizeX, SizeY, Block.Resolution);

					for (int32 FeedX = FeedReso.FromX; FeedX < FeedReso.ToX; ++FeedX)
					{
						for (int32 FeedY = FeedReso.FromY; FeedY < FeedReso.ToY; ++FeedY)
						{
							const int32 FeedIndex = CL * LayerSize + FeedX * SizeY + FeedY;
							const float Weight = InDna ? InDna->ReadFloat() : WeightRange * (-1.0f + 2.0f * FMath::RandomF());
							if (TargetNetwork)
							{
								TargetNetwork->AddEdge(FeedIndex, AddedIndex, Weight);
							}
							if (OutDna)
							{
								OutDna->PushFloat(Weight);
							}
						}
					}
				}
			}
		}
	}
}

