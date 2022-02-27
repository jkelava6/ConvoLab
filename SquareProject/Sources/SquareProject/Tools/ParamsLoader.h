#pragma once

CONVOLUTION_PARAMS_NAMESPACE_BEGIN
class FNetwork;
CONVOLUTION_PARAMS_NAMESPACE_END

//namespace std
//{
//	class string;
//}
#include <string>

class FParamsLoader
{
public:
	static FConvolutionParams::FNetwork* ReadParamsFile(const std::string& FileName);
	static void WriteParamsFile(const std::string& FileName, FConvolutionParams::FNetwork& Network);
};
