#include <Core/StdH.h>
#include <Core/Dna.h>

FDna::FDna(int32 PreparedSize/* = 256*/)
	: Genes(PreparedSize)
{
}

void FDna::Seek(int32 Position)
{
	Pointer = Position;
}

int32 FDna::GetPointer()
{
	return Pointer;
}

int32 FDna::Size()
{
	return Genes.Count();
}

void FDna::PushFloat(float Pushed)
{
	void* Written = Pointer == Genes.Count() ? &Genes.Push() : &Genes[Pointer];
	*(float*)Written = Pushed;
	++Pointer;
}

void FDna::PushInt(int32 Pushed)
{
	void* Written = Pointer == Genes.Count() ? &Genes.Push() : &Genes[Pointer];
	*(int32*)Written = Pushed;
	++Pointer;
}

float FDna::ReadFloat()
{
	void* Read = Pointer == Genes.Count() ? &Genes.Push() : &Genes[Pointer];
	++Pointer;
	return *(float*)Read;
}

int32 FDna::ReadInt()
{
	void* Read = Pointer == Genes.Count() ? &Genes.Push() : &Genes[Pointer];
	++Pointer;
	return *(int32*)Read;
}

void FDna::Trim(int32 Trimmed)
{
	Genes.Prealocate(Genes.Count() - Trimmed);
}

void FDna::TrimTo(int32 TrimmedTo)
{
	Genes.Prealocate(TrimmedTo);
	Pointer = FMath::Min(Pointer, Genes.Count());
}

