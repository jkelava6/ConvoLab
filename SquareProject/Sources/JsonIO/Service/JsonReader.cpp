#include <JsonIO/StdH.h>
#include <JsonIO/Service/JsonReader.h>

#include <JsonIO/Data/JsonMap.h>
#include <JsonIO/Data/JsonText.h>
#include <JsonIO/Service/JsonInternal.h>

IJsonObject* FJsonReader::ReadJsonObject()
{
	IJsonInputStream* InputStream = FJsonInternal::CreateInputStream();
	IJsonObject* Object = ReadJsonMap(InputStream);
	delete InputStream;
	return Object;
}

IJsonObject* FJsonReader::ReadJsonObject(const std::string& JsonString)
{
	IJsonInputStream* InputStream = FJsonInternal::CreateInputStream(JsonString);
	IJsonObject* Object = ReadJsonMap(InputStream);
	delete InputStream;
	return Object;
}

IJsonObject* FJsonReader::ReadJsonObject(IJsonInputStream* Input)
{
	const char Character = Input->NextNonWhitespace();
	if (Character == '"')
	{
		return ReadJsonText(Input);
	}
	if (Character == '{')
	{
		return ReadJsonMap(Input);
	}
	throw FJsonReadException();
}

FJsonMap* FJsonReader::ReadJsonMap(IJsonInputStream* Input)
{
	FJsonMap* Map = new FJsonMap();

	while (true)
	{
		const char Defining = Input->NextNonWhitespace();

		if (Defining == '}')
		{
			break;
		}

		if (Defining != '"')
		{
			throw FJsonReadException();
		}

		std::string Name = ReadString(Input);
		AssertChar(Input, '=');
		IJsonObject* Object = ReadJsonObject();
		Map->Data[Name] = Object;

		const char RequireCommaOrEnd = Input->NextNonWhitespace();
		if (RequireCommaOrEnd == ',')
		{
			continue;
		}
		if (RequireCommaOrEnd == '}')
		{
			break;
		}
		throw FJsonReadException();
	}

	return Map;
}

FJsonText* FJsonReader::ReadJsonText(IJsonInputStream* Input)
{
	FJsonText* Text = new FJsonText();
	Text->Text = ReadString(Input);
	return Text;
}

void FJsonReader::AssertChar(IJsonInputStream* Input, char Asserted)
{
	char Character = Input->NextNonWhitespace();
	if (Character != Asserted)
	{
		throw FJsonCastException();
	}
}

std::string FJsonReader::ReadString(IJsonInputStream* Input)
{
	std::string String = "";
	
	while (true)
	{
		char Next = Input->Next();

		if (Next == '"')
		{
			break;
		}

		if (Next == '\\')
		{
			Next = Input->Next();
		}

		String.append(&Next, sizeof(char));
	}

	return String;
}
