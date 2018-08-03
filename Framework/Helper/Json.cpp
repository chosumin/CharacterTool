#include "stdafx.h"
#include "Json.h"
#include "./Helper/cString.h"

void Json::GetValue(Json::Value & parent, string name, bool & value)
{
	value = parent[name.c_str()].asBool();
}

void Json::SetValue(Json::Value & parent, string name, bool & value)
{
	parent[name.c_str()] = value;
}

void Json::GetValue(Json::Value & parent, string name, float & value)
{
	value = parent[name.c_str()].asFloat();
}

void Json::SetValue(Json::Value & parent, string name, float & value)
{
	parent[name.c_str()] = value;
}

void Json::GetValue(Json::Value & parent, string name, D3DXVECTOR2 & vec)
{
	for (int i = 0; i < 2; i++)
		vec[i] = parent[name.c_str()][i].asFloat();
}

void Json::SetValue(Json::Value & parent, string name, D3DXVECTOR2 & vec)
{
	for (int i = 0; i < 2; i++)
		parent[name.c_str()].append(vec[i]);
}

void Json::GetValue(Json::Value & parent, string name, D3DXVECTOR3 & vec)
{
	for (int i = 0; i < 3; i++)
		vec[i] = parent[name.c_str()][i].asFloat();
}

void Json::SetValue(Json::Value & parent, string name, D3DXVECTOR3 & vec)
{
	for (int i = 0; i < 3; i++)
		parent[name.c_str()].append(vec[i]);
}

void Json::GetValue(Json::Value & parent, string name, D3DXMATRIX & mat)
{
	for (int i = 0; i < 16; i++)
		mat[i] = parent[name.c_str()][i].asFloat();
}

void Json::SetValue(Json::Value & parent, string name, D3DXMATRIX & mat)
{
	for (int i = 0; i < 16; i++)
		parent[name.c_str()].append(mat[i]);
}

void Json::GetValue(Json::Value & parent, string name, D3DXCOLOR & vec)
{
	for (int i = 0; i < 4; i++)
		vec[i] = parent[name.c_str()][i].asFloat();
}

void Json::SetValue(Json::Value & parent, string name, D3DXCOLOR & vec)
{
	for (int i = 0; i < 4; i++)
		parent[name.c_str()].append(vec[i]);
}

void Json::GetValue(Json::Value & parent, string name, string & value)
{
	value = "";

	for (size_t i = 0; i < parent[name.c_str()].size(); i++)
		value.append(parent[name.c_str()][i].asString());
}

void Json::SetValue(Json::Value & parent, string name, string & value)
{
	parent[name.c_str()].append(value);
}

void Json::GetValue(Json::Value & parent, string name, int & value)
{
	value = parent[name.c_str()].asInt();
}

void Json::SetValue(Json::Value & parent, string name, int & value)
{
	parent[name.c_str()] = value;
}

void Json::GetValue(Json::Value & parent, string name, UINT & value)
{
	value = parent[name.c_str()].asUInt();
}

void Json::SetValue(Json::Value & parent, string name, UINT & value)
{
	parent[name.c_str()] = value;
}

void Json::ReadData(wstring file, Json::Value * root)
{
	ifstream stream;

	stream.open(file);
	{
		Json::Reader reader;
		reader.parse(stream, *root);
	}
	stream.close();
}

void Json::WriteData(wstring file, Json::Value * root)
{
	ofstream stream;

	string temp = cString::String(file);
	stream.open(temp);
	{
		Json::StyledWriter writer;

		stream << writer.write(*root);
	}
	stream.close();
}