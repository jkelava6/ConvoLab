#include <Convolutional/StdH.h>
#include <Convolutional/Instance/NetworkInstance.h>

#include <Convolutional/Function/Collector.h>
#include <Convolutional/Function/Mapping.h>
#include <Convolutional/Instance/BlockInstance.h>
#include <Convolutional/Instance/ConnectionInstance.h>
#include <Convolutional/Params/Block.h>
#include <Convolutional/Params/Connection.h>
#include <Convolutional/Params/Network.h>

#include <iostream>

CONVOLUTION_INSTANCE_NAMESPACE_BEGIN

FNetwork::FNetwork(const FConvolutionParams::FNetwork& Structure, FDna* Parameters)
	: Params(Structure)
{
	const int32 NumOfBlocks = Params.Blocks.Count();
	Blocks.Prealocate(NumOfBlocks);
	for (int32 BlockIndex = 0; BlockIndex < NumOfBlocks; ++BlockIndex)
	{
		const FConvolutionParams::FBlock* BlockParams = Params.Blocks[BlockIndex];
		FBlock& Block = Blocks.Emplace(BlockParams, Parameters);
	}

	for (int32 BlockIndex = 0; BlockIndex < NumOfBlocks; ++BlockIndex)
	{
		const FConvolutionParams::FBlock* BlockParams = Params.Blocks[BlockIndex];
		FBlock& Block = Blocks[BlockIndex];

		const int32 NumOfConnections = BlockParams->Connections.Count();
		for (int32 ConnectionIndex = 0; ConnectionIndex < NumOfConnections; ++ConnectionIndex)
		{
			const FConvolutionParams::FConnection& ConnectionParams = BlockParams->Connections[ConnectionIndex];
			for (int32 FeederBlockIndex = 0; FeederBlockIndex < NumOfBlocks; ++FeederBlockIndex)
			{
				FBlock& FeederBlock = Blocks[FeederBlockIndex];
				if (&FeederBlock.GetParams() == ConnectionParams.Block)
				{
					Block.AddConnection(ConnectionParams, &FeederBlock, Parameters);
				}
			}
		}
	}
}

FNetwork::~FNetwork() = default;

void FNetwork::Evaluate(const TArray<float>& Inputs, TArray<float>& Outputs)
{
	const int32 NumOfBlocks = Blocks.Count();
	int32 NextInput = 0;
	for (int32 BlockIndex = 0; BlockIndex < NumOfBlocks; ++BlockIndex)
	{
		FBlock& Block = Blocks[BlockIndex];
		if (Block.GetParams().bIsInput)
		{
			const int32 SizeX = Block.GetSizeX();
			const int32 SizeY = Block.GetSizeY();
			const int32 NumOfFeatures = Block.GetNumOfFeatures();

			for (int32 Feature = 0; Feature < NumOfFeatures; ++Feature)
			{
				for (int32 X = 0; X < SizeX; ++X)
				{
					for (int32 Y = 0; Y < SizeY; ++Y)
					{
						Block.SetState(Feature, X, Y, Inputs[NextInput++]);
					}
				}
			}
		}
	}

	int32 NextOutput = 0;
	for (int32 BlockIndex = 0; BlockIndex < NumOfBlocks; ++BlockIndex)
	{
		FBlock& Block = Blocks[BlockIndex];
		const int32 SizeX = Block.GetSizeX();
		const int32 SizeY = Block.GetSizeY();
		const int32 NumOfFeatures = Block.GetNumOfFeatures();

		if (Block.GetParams().bIsInput)
		{
			continue;
		}

		const int32 NumOfConnections = Block.GetNumOfConnections();
		for (int32 Feature = 0; Feature < NumOfFeatures; ++Feature)
		{
			for (int32 X = 0; X < SizeX; ++X)
			{
				for (int32 Y = 0; Y < SizeY; ++Y)
				{
					FConvolutionFunction::ICollector* Collector = Block.GetParams().Collector;
					FConvolutionFunction::IFloatToFloat* Mapper = Block.GetParams().Mapper;
					Collector->Init();
					Collector->Collect(Block.GetBias(Feature, X, Y));

					for (int32 ConnectionIndex = 0; ConnectionIndex < NumOfConnections; ++ConnectionIndex)
					{
						const FConnection& Connection = Block.GetConnection(ConnectionIndex);
						const FConvolutionParams::FConnection& ConnectionParams = Connection.GetParams();
						const FBlock* Feeder = Connection.GetBlock();

						const int32 LoFeature = ConnectionParams.bAllFeatures ? 0 : Feature;
						const int32 HiFeature = ConnectionParams.bAllFeatures ? Feeder->GetNumOfFeatures() - 1 : Feature;
						const int32 LoX = ConnectionParams.LoX + (int32)(ConnectionParams.XFactor * X);
						const int32 HiX = ConnectionParams.HiX + (int32)(ConnectionParams.XFactor * X);
						const int32 LoY = ConnectionParams.LoY + (int32)(ConnectionParams.YFactor * Y);
						const int32 HiY = ConnectionParams.HiY + (int32)(ConnectionParams.YFactor * Y);
						for (int32 FeederFeature = LoFeature; FeederFeature <= HiFeature; ++FeederFeature)
						{
							for (int32 FeederX = LoX; FeederX <= HiX; ++FeederX)
							{
								for (int32 FeederY = LoY; FeederY <= HiY; ++FeederY)
								{
									// TODO: support non-neutral fillers
									if (FeederX < 0 || FeederX >= Feeder->GetSizeX()
										|| FeederY < 0 || FeederY >= Feeder->GetSizeY()
										|| FeederFeature >= Feeder->GetNumOfFeatures())
									{
										continue;
									}
									Collector->Collect(
										Feeder->GetState(FeederFeature, FeederX, FeederY)
										* Connection.GetWeight(FeederFeature, FeederX - LoX, FeederY - LoY, Feature)
									);
								}
							}
						}
					}

					Block.SetState(Feature, X, Y, Mapper->EvaluateAt(Collector->Finalize()));
				}
			}
		}

		if (Block.GetParams().bIsOutput)
		{
			for (int32 Feature = 0; Feature < NumOfFeatures; ++Feature)
			{
				for (int32 X = 0; X < SizeX; ++X)
				{
					for (int32 Y = 0; Y < SizeY; ++Y)
					{
						Outputs[NextOutput++] = Block.GetState(Feature, X, Y);
					}
				}
			}
		}
	}
}

void FNetwork::Backpropagate(const TArray<float>& ErrorDifferentials)
{
	const int32 NumOfBlocks = Blocks.Count();

	int32 NextDiff = 0;
	for (int32 BlockIndex = 0; BlockIndex < NumOfBlocks; ++BlockIndex)
	{
		FBlock& Block = Blocks[BlockIndex];
		const int32 SizeX = Block.GetSizeX();
		const int32 SizeY = Block.GetSizeY();
		const int32 NumOfFeatures = Block.GetNumOfFeatures();

		for (int32 Feature = 0; Feature < NumOfFeatures; ++Feature)
		{
			for (int32 X = 0; X < SizeX; ++X)
			{
				for (int32 Y = 0; Y < SizeY; ++Y)
				{
					Block.SetDifferential(Feature, X, Y, Block.GetParams().bIsOutput ? ErrorDifferentials[NextDiff++] : 0.0f);
				}
			}
		}
	}

	for (int32 BlockIndex = NumOfBlocks - 1; BlockIndex >= 0; --BlockIndex)
	{
		FBlock& Block = Blocks[BlockIndex];
		const int32 SizeX = Block.GetSizeX();
		const int32 SizeY = Block.GetSizeY();
		const int32 NumOfFeatures = Block.GetNumOfFeatures();
		if (Block.GetParams().bIsInput)
		{
			continue;
		}

		const int32 NumOfConnections = Block.GetNumOfConnections();
		for (int32 Feature = 0; Feature < NumOfFeatures; ++Feature)
		{
			for (int32 X = 0; X < SizeX; ++X)
			{
				for (int32 Y = 0; Y < SizeY; ++Y)
				{
					float Differential = Block.GetDifferential(Feature, X, Y);
					Differential *= Block.GetParams().Mapper->DerivateAt(Block.GetState(Feature, X, Y));
					Differential *= Block.GetParams().Collector->Derivate(Block.GetState(Feature, X, Y));

					for (int32 ConnectionIndex = 0; ConnectionIndex < NumOfConnections; ++ConnectionIndex)
					{
						FConnection& Connection = Block.GetConnection(ConnectionIndex);
						const FConvolutionParams::FConnection& ConnectionParams = Connection.GetParams();
						FBlock* Feeder = Connection.GetBlock();

						const int32 LoFeature = ConnectionParams.bAllFeatures ? 0 : Feature;
						const int32 HiFeature = ConnectionParams.bAllFeatures ? Feeder->GetNumOfFeatures() - 1 : Feature;
						const int32 LoX = ConnectionParams.LoX + (int32)(ConnectionParams.XFactor * X);
						const int32 HiX = ConnectionParams.HiX + (int32)(ConnectionParams.XFactor * X);
						const int32 LoY = ConnectionParams.LoY + (int32)(ConnectionParams.YFactor * Y);
						const int32 HiY = ConnectionParams.HiY + (int32)(ConnectionParams.YFactor * Y);
						for (int32 FeederFeature = LoFeature; FeederFeature <= HiFeature; ++FeederFeature)
						{
							for (int32 FeederX = LoX; FeederX <= HiX; ++FeederX)
							{
								for (int32 FeederY = LoY; FeederY <= HiY; ++FeederY)
								{
									// TODO: support non-neutral fillers
									if (FeederX < 0 || FeederX >= Feeder->GetSizeX()
										|| FeederY < 0 || FeederY >= Feeder->GetSizeY()
										|| FeederFeature > Feeder->GetNumOfFeatures())
									{
										continue;
									}
									Feeder->AddDifferential(FeederFeature, FeederX, FeederY,
										Differential * Connection.GetWeight(FeederFeature, FeederX - LoX, FeederY - LoY, Feature));
									Connection.AddDifferential(FeederFeature, FeederX - LoX, FeederY - LoY,
										Differential * Feeder->GetState(FeederFeature, FeederX, FeederY), Feature);
								}
							}
						}
					}
				}
			}
		}
	}

	for (int32 BlockIndex = 0; BlockIndex < NumOfBlocks; ++BlockIndex)
	{
		FBlock& Block = Blocks[BlockIndex];
		Block.SaveDifferentials();
	}
}

void FNetwork::ApplyBackProp(float Scale, float MaxStep, bool bClearDifferentials/* = true*/)
{
	for (int32 BlockIndex = 0; BlockIndex < Blocks.Count(); ++BlockIndex)
	{
		FBlock& Block = Blocks[BlockIndex];
		Block.ApplyBackProp(Scale, MaxStep);
	}

	if (bClearDifferentials)
	{
		for (int32 BI = 0; BI < Blocks.Count(); ++BI)
		{
			Blocks[BI].ClearSavedDifferentials();
		}
	}
}

void FNetwork::ExponentialBackProp(float InitialScale, float InitialMaxStep, FunctionPointer(float, ErrorFunction, void* TestContextObject),
	bool bClearDifferentials/* = true*/, void* ContextObject/* = nullptr*/)
{
	float Scale = InitialScale;
	float MaxStep = InitialMaxStep;
	float Error;
	for (int32 Iteration = 0; Iteration < 64; ++Iteration)
	{
		ApplyBackProp(Scale, MaxStep, false);

		const float NewError = ErrorFunction(ContextObject);
		if (Iteration > 1 && NewError > Error)
		{
			for (int32 BI = 0; BI < Blocks.Count(); ++BI)
			{
				Blocks[BI].RevertStates();
			}
			break;
		}
		Error = NewError;

		Scale *= 2.0f;
		MaxStep *= 2.0f;
	}

	if (bClearDifferentials)
	{
		for (int32 BI = 0; BI < Blocks.Count(); ++BI)
		{
			Blocks[BI].ClearSavedDifferentials();
		}
	}
}

CONVOLUTION_INSTANCE_NAMESPACE_END
