#pragma once

#include <JsonIO/Data/JsonObject.h>

#include <string>

class FJsonText : public IJsonObject
{
public:
	virtual bool IsText() override;
	virtual bool IsMap() override;
	virtual FJsonText& AsText() override;
	virtual FJsonMap& AsMap() override;

public:
	std::string Text;
};
