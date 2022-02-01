#pragma once

#include <string>

class IJsonOutputStream;
class IJsonObject;

class FJsonWriter
{
public:
	static std::string SerializeJsonObject(IJsonObject* Object);
	static void WriteJsonObject(IJsonObject* Object);
private:
	static void SerializeJsonObject(IJsonObject* Object, IJsonOutputStream* Output, int32 Indent = 0);
	static void DoIndent(IJsonOutputStream* Output, int32 Indent);
};
