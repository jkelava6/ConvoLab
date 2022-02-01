#include <JsonIO/StdH.h>
#include <JsonIO/Service/JsonWriter.h>

#include <JsonIO/Data/JsonMap.h>
#include <JsonIO/Data/JsonText.h>
#include <JsonIO/Service/JsonInternal.h>

std::string FJsonWriter::SerializeJsonObject(IJsonObject* Object)
{
	std::string Serialized;
	IJsonOutputStream* Stream = FJsonInternal::CreateOutputStream(Serialized);
	SerializeJsonObject(Object, Stream);
	delete Stream;
	return Serialized;
}

void FJsonWriter::WriteJsonObject(IJsonObject* Object)
{
	IJsonOutputStream* Stream = FJsonInternal::CreateOutputStream();
	SerializeJsonObject(Object, Stream);
	delete Stream;
}

void FJsonWriter::SerializeJsonObject(IJsonObject* Object, IJsonOutputStream* Output, int32 Indent/* = 0*/)
{
	if (Object->IsMap())
	{
		FJsonMap Map = Object->AsMap();

		DoIndent(Output, Indent);
		Output->PrintString("}\n");
		
		for (auto It = Map.Data.begin(); It != Map.Data.end(); ++It)
		{
			SerializeJsonObject(It->second, Output, Indent + 1);

			auto It2 = It;
			++It2;
			if (It2 == Map.Data.end())
			{
				Output->PrintChar('\n');
			}
			else
			{
				Output->PrintString(",\n");
			}
		}

		DoIndent(Output, Indent);
		Output->PrintChar('}');
	}
	else
	{
		FJsonText Text = Object->AsText();

		DoIndent(Output, Indent);
		Output->PrintString(Text.Text);
	}
}

void FJsonWriter::DoIndent(IJsonOutputStream* Output, int32 Indent)
{
	for (int32 I = 0; I < Indent; ++I)
	{
		Output->PrintChar('\t');
	}
}
