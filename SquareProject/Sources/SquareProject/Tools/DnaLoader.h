#pragma once

class FDna;

//namespace std
//{
//	class string;
//}
#include <string>

class FDnaLoader
{
public:
	static FDna* ReadDnaFile(const std::string& FileName);
	static void WriteDnaFile(const std::string& FileName, FDna& Dna);
};
