#pragma once

class FDataPoint;
template <class> class TArray;
//namespace std
//{
//	class string;
//}
#include <string>

class FDataSetLoader
{
public:
	static TArray<FDataPoint>* ReadDataSetFile(const std::string& FileName);
	static void WriteDataSetFile(const std::string& FileName, TArray<FDataPoint>& DataSet);
};
