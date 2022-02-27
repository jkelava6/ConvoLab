#include <SquareProject/StdH.h>
#include <SquareProject/Tools/DnaLoader.h>

#include <SquareProject/Tools/DataPoint.h>

#include <Core/Dna.h>

#include <fstream>
#include <string>

FDna* FDnaLoader::ReadDnaFile(const std::string& FileName)
{
	std::ifstream File;
	File.open(FileName, std::ios::in);

	FDna* Dna = new FDna();

	int32 Size;
	File >> Size;
	int32 Value;
	for (int32 I = 0; I < Size; ++I)
	{
		File >> Value;
		Dna->PushInt(Value);
	}

	File.close();
	Dna->Seek(0);
	return Dna;
}

void FDnaLoader::WriteDnaFile(const std::string& FileName, FDna& Dna)
{
	std::ofstream File;
	File.open(FileName, std::ios::out);

	const int32 Size = Dna.Size();
	File << Size << "\n";
	for (int32 I = 0; I < Size; ++I)
	{
		File << Dna.ReadInt() << " ";
	}

	File.close();
}
