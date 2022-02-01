#pragma once

#include <Core/DynamicArray.h>

class FDna
{
public:
	FDna(int32 PreparedSize = 256);

	void Seek(int32 Position);
	int32 GetPointer();
	int32 Size();

	void PushFloat(float Pushed);
	void PushInt(int32 Pushed);
	float ReadFloat();
	int32 ReadInt();
	void Trim(int32 Trimmed);
	void TrimTo(int32 TrimmedTo);

private:
	TArray<int32> Genes;
	int32 Pointer = 0;
};
