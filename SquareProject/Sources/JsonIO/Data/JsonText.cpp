#include <JsonIO/StdH.h>
#include <JsonIO/Data/JsonText.h>

bool FJsonText::IsText()
{
	return true;
}

bool FJsonText::IsMap()
{
	return false;
}

FJsonText& FJsonText::AsText()
{
	return *this;
}

FJsonMap& FJsonText::AsMap()
{
	throw FJsonCastException();
}
