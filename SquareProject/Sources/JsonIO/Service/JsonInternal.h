#pragma once

#include <string>

class IJsonInputStream
{
protected:
	IJsonInputStream() = default;
public:
	virtual bool HasNext() = 0;
	virtual char Next() = 0;
	virtual char NextNonWhitespace();
};

class FJsonStandardInputStream : public IJsonInputStream
{
private:
	FJsonStandardInputStream() = default;
public:
	virtual bool HasNext() override;
	virtual char Next() override;
private:
	friend class FJsonInternal;
};

class FJsonStringInputStream : public IJsonInputStream
{
private:
	FJsonStringInputStream(const std::string& InputString);
public:
	virtual bool HasNext() override;
	virtual char Next() override;
private:
	std::string String;
	int32 NextIndex = 0;

	friend class FJsonInternal;
};

class IJsonOutputStream
{
protected:
	IJsonOutputStream() = default;
public:
	virtual void PrintChar(char Char) = 0;
	virtual void PrintString(const std::string& String) = 0;
};

class FJsonStandardOutputStream : public IJsonOutputStream
{
private:
	FJsonStandardOutputStream() = default;
public:
	virtual void PrintChar(char Char) override;
	virtual void PrintString(const std::string& String) override;
private:
	friend class FJsonInternal;
};

class FJsonStringOutputStream : public IJsonOutputStream
{
private:
	FJsonStringOutputStream(std::string& InTarget) : Target(InTarget) {}
public:
	virtual void PrintChar(char Char) override;
	virtual void PrintString(const std::string& String) override;
private:
	std::string& Target;

	friend class FJsonInternal;
};

class FJsonInternal
{
private:
	static IJsonInputStream* CreateInputStream();
	static IJsonInputStream* CreateInputStream(const std::string& Json);
	static IJsonOutputStream* CreateOutputStream();
	static IJsonOutputStream* CreateOutputStream(std::string& Target);

private:
	friend class FJsonReader;
	friend class FJsonWriter;
};
