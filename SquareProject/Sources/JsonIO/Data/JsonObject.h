#pragma once

class FJsonMap;
class FJsonText;

class IJsonObject
{
public:
	virtual bool IsText() = 0;
	virtual bool IsMap() = 0;
	virtual FJsonText& AsText() = 0;
	virtual FJsonMap& AsMap() = 0;
};

class IJsonException
{
protected:
	IJsonException() = default;
};

class FJsonCastException : public IJsonException
{
public:
	FJsonCastException() = default;
};

class FJsonReadException : public IJsonException
{
public:
	FJsonReadException() = default;
};
