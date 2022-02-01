#include <JsonIO/StdH.h>
#include <JsonIO/Service/JsonInternal.h>

#include <iostream>
#include <string>

char IJsonInputStream::NextNonWhitespace()
{
	char Character;
	do
	{
		Character = Next();
	} while (Character == ' ' || Character == '\n' || Character == '\t' || Character == '\r');
	return Character;
}

bool FJsonStandardInputStream::HasNext()
{
	return true;
}

char FJsonStandardInputStream::Next()
{
	char Next;
	std::cin >> Next;
	return Next;
}

FJsonStringInputStream::FJsonStringInputStream(const std::string& InputString)
	: String(InputString)
{
}

bool FJsonStringInputStream::HasNext()
{
	return NextIndex < (int32)String.length();
}

char FJsonStringInputStream::Next()
{
	return String.at(NextIndex++);
}

void FJsonStandardOutputStream::PrintChar(char Char)
{
	std::cout << Char;
}

void FJsonStandardOutputStream::PrintString(const std::string& String)
{
	std::cout << String;
}

void FJsonStringOutputStream::PrintChar(char Char)
{
	Target.append(&Char, sizeof(char));
}

void FJsonStringOutputStream::PrintString(const std::string& String)
{
	Target.append(String);
}

IJsonInputStream* FJsonInternal::CreateInputStream()
{
	return new FJsonStandardInputStream();
}

IJsonInputStream* FJsonInternal::CreateInputStream(const std::string& Json)
{
	return new FJsonStringInputStream(Json);
}

IJsonOutputStream* FJsonInternal::CreateOutputStream()
{
	return new FJsonStandardOutputStream();
}

IJsonOutputStream* FJsonInternal::CreateOutputStream(std::string& Target)
{
	return new FJsonStringOutputStream(Target);
}
