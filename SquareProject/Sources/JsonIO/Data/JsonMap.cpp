#include <JsonIO/StdH.h>
#include <JsonIO/Data/JsonMap.h>

bool FJsonMap::IsText()
{
	return false;
}

bool FJsonMap::IsMap()
{
	return true;
}

FJsonText& FJsonMap::AsText()
{
	throw FJsonCastException();
}

FJsonMap& FJsonMap::AsMap()
{
	return *this;
}
