#pragma once

#include <JsonIO/Data/JsonObject.h>

#include <map>
#include <string>

class FJsonMap : public IJsonObject
{
public:
	virtual bool IsText() override;
	virtual bool IsMap() override;
	virtual FJsonText& AsText() override;
	virtual FJsonMap& AsMap() override;

public:
	std::map<std::string, IJsonObject*> Data;
};
