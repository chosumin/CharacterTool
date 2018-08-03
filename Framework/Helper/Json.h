#pragma once

namespace Json
{
	void GetValue(Json::Value& parent, string name, bool& value);
	void SetValue(Json::Value& parent, string name, bool& value);

	void GetValue(Json::Value& parent, string name, float& value);
	void SetValue(Json::Value& parent, string name, float& value);

	void GetValue(Json::Value& parent, string name, D3DXVECTOR2& vec);
	void SetValue(Json::Value& parent, string name, D3DXVECTOR2& vec);

	void GetValue(Json::Value& parent, string name, D3DXVECTOR3& vec);
	void SetValue(Json::Value& parent, string name, D3DXVECTOR3& vec);

	void GetValue(Json::Value& parent, string name, D3DXMATRIX& mat);
	void SetValue(Json::Value& parent, string name, D3DXMATRIX& mat);

	void GetValue(Json::Value& parent, string name, D3DXCOLOR& vec);
	void SetValue(Json::Value& parent, string name, D3DXCOLOR& vec);

	void GetValue(Json::Value& parent, string name, string& value);
	void SetValue(Json::Value& parent, string name, string& value);

	void GetValue(Json::Value& parent, string name, int& value);
	void SetValue(Json::Value& parent, string name, int& value);

	void GetValue(Json::Value& parent, string name, UINT& value);
	void SetValue(Json::Value& parent, string name, UINT& value);

	void ReadData(wstring file, Json::Value* root);
	void WriteData(wstring file, Json::Value* root);
}