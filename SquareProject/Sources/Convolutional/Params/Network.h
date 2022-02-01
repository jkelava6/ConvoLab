#pragma once

#include <Core/DynamicArray.h>

CONVOLUTION_PARAMS_NAMESPACE_BEGIN

class FBlock;

class FNetwork
{
public:
	FNetwork();
	~FNetwork();
	DECLARE_NOCOPY(FNetwork);
	DECLARE_NOMOVE(FNetwork);

	void AddBlock(FBlock* Block);
	FBlock* NewBlock();
public:
	TArray<FBlock*> Blocks;
	bool bDestroyBlocksOnDestruction = true;
};

CONVOLUTION_PARAMS_NAMESPACE_END
