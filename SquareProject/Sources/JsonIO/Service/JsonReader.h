#pragma once

#include <string>

class FJsonMap;
class FJsonText;
class IJsonInputStream;
class IJsonObject;

class FJsonReader
{
public:
	static IJsonObject* ReadJsonObject();
	static IJsonObject* ReadJsonObject(const std::string& JsonString);

private:
	static IJsonObject* ReadJsonObject(IJsonInputStream* Input);
	static FJsonMap* ReadJsonMap(IJsonInputStream* Input);
	static FJsonText* ReadJsonText(IJsonInputStream* Input);
	static void AssertChar(IJsonInputStream* Input, char Asserted);
	static std::string ReadString(IJsonInputStream* Input);

	FJsonReader() = default;
};
