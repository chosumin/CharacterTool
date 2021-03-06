#pragma once

class cBinaryWriter
{
public:
	static unique_ptr<cBinaryWriter> Open(wstring filePath, UINT openOption = CREATE_ALWAYS);
	~cBinaryWriter();
public:
	void Close();

	void Bool(bool data);
	void Word(WORD data);
	void Int(int data);
	void UInt(UINT data);
	void Float(float data);
	void Double(double data);

	void Vector2(const D3DXVECTOR2& data);
	void Vector3(const D3DXVECTOR3& data);
	void Vector4(const D3DXVECTOR4& data);
	void Vector4(const D3DXQUATERNION& data);

	void Color3f(const D3DXCOLOR& data);
	void Color4f(const D3DXCOLOR& data);
	void Matrix(const D3DXMATRIX& data);

	void String(const string& data);
	void Byte(void* data, UINT dataSize);
private:
	cBinaryWriter();
private:
	HANDLE fileHandle = nullptr;
	DWORD size = 0;
};

/*******************************************************************
*******************************************************************/

class cBinaryReader
{
public:
	static unique_ptr<cBinaryReader> Open(wstring filePath);
	~cBinaryReader();
public:
	void Close();

	bool Bool();
	WORD Word();
	int Int();
	UINT UInt();
	float Float();
	double Double();

	D3DXVECTOR2 Vector2();
	D3DXVECTOR3 Vector3();
	D3DXVECTOR4 Vector4();
	D3DXQUATERNION Quaternion();

	D3DXCOLOR Color3f();
	D3DXCOLOR Color4f();
	D3DXMATRIX Matrix();

	string String();
	void Byte(void** data, UINT dataSize);
private:
	cBinaryReader();
private:
	HANDLE fileHandle = nullptr;
	DWORD size = 0;
};