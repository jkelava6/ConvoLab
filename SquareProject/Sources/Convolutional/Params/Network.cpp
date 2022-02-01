#include <Convolutional/StdH.h>
#include <Convolutional/Params/Network.h>

#include <Convolutional/Params/Block.h>

CONVOLUTION_PARAMS_NAMESPACE_BEGIN

FNetwork::FNetwork() = default;

FNetwork::~FNetwork()
{
	if (bDestroyBlocksOnDestruction)
	{
		for (int32 BI = 0; BI < Blocks.Count(); ++BI)
		{
			delete Blocks[BI];
		}
		Blocks.Clear();
	}
}

void FNetwork::AddBlock(FBlock* Block)
{
	Blocks.Add(Block);
}

FBlock* FNetwork::NewBlock()
{
	return Blocks.Push();
}

CONVOLUTION_PARAMS_NAMESPACE_END
