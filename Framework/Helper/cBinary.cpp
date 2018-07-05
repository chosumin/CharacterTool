#include "stdafx.h"
#include "cBinary.h"

unique_ptr<cBinaryWriter> cBinaryWriter::Open(wstring filePath, UINT openOption)
{
	assert(filePath.size() > 0 && "파일명을 입력하세요!");

	//트릭 클래스
	struct make_unique_enabler : public cBinaryWriter {};
	auto writer = make_unique<make_unique_enabler>();

	writer->fileHandle = CreateFile
	(
		filePath.c_str(),
		GENERIC_WRITE,
		0,
		nullptr,
		openOption,
		FILE_ATTRIBUTE_NORMAL,
		nullptr
	);

	assert(writer->fileHandle != INVALID_HANDLE_VALUE && "유효하지 않은 파일!");

	return move(writer);
}

void cBinaryWriter::Close()
{
	if (fileHandle != nullptr)
	{
		CloseHandle(fileHandle);
		fileHandle = nullptr;
	}
}

void cBinaryWriter::Bool(bool data)
{
	WriteFile(fileHandle, &data, sizeof(bool), &size, nullptr);
}

void cBinaryWriter::Word(WORD data)
{
	WriteFile(fileHandle, &data, sizeof(WORD), &size, nullptr);
}

void cBinaryWriter::Int(int data)
{
	WriteFile(fileHandle, &data, sizeof(int), &size, nullptr);
}

void cBinaryWriter::UInt(UINT data)
{
	WriteFile(fileHandle, &data, sizeof(UINT), &size, nullptr);
}

void cBinaryWriter::Float(float data)
{
	WriteFile(fileHandle, &data, sizeof(float), &size, nullptr);
}

void cBinaryWriter::Double(double data)
{
	WriteFile(fileHandle, &data, sizeof(double), &size, nullptr);
}

void cBinaryWriter::Vector2(const D3DXVECTOR2 & data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXVECTOR2), &size, nullptr);
}

void cBinaryWriter::Vector3(const D3DXVECTOR3 & data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXVECTOR3), &size, nullptr);
}

void cBinaryWriter::Vector4(const D3DXVECTOR4 & data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXVECTOR4), &size, nullptr);
}

void cBinaryWriter::Vector4(const D3DXQUATERNION & data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXQUATERNION), &size, nullptr);
}

void cBinaryWriter::Color3f(const D3DXCOLOR & data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXCOLOR) - 4, &size, nullptr);
}

void cBinaryWriter::Color4f(const D3DXCOLOR & data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXCOLOR), &size, nullptr);
}

void cBinaryWriter::Matrix(const D3DXMATRIX & data)
{
	WriteFile(fileHandle, &data, sizeof(D3DXMATRIX), &size, nullptr);
}

void cBinaryWriter::String(const string & data)
{
	UInt(data.size());

	const char* str = data.c_str();
	WriteFile(fileHandle, str, data.size(), &size, nullptr);
}

void cBinaryWriter::Byte(void * data, UINT dataSize)
{
	WriteFile(fileHandle, data, dataSize, &size, nullptr);
}

cBinaryWriter::cBinaryWriter()
{
}

cBinaryWriter::~cBinaryWriter()
{
}

/*******************************************************************
********************************************************************/

unique_ptr<cBinaryReader> cBinaryReader::Open(wstring filePath)
{
	assert(filePath.size() > 0 && "파일명을 입력하세요!");

	//트릭 객체
	struct make_unique_enabler : public cBinaryReader {};
	auto reader = make_unique<make_unique_enabler>();

	reader->fileHandle = CreateFile
	(
		filePath.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ,
		nullptr,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		nullptr
	);

	assert(reader->fileHandle != INVALID_HANDLE_VALUE && "유효하지 않은 파일!");

	return move(reader);
}

void cBinaryReader::Close()
{
	if (fileHandle != nullptr)
	{
		CloseHandle(fileHandle);
		fileHandle = nullptr;
	}
}

bool cBinaryReader::Bool()
{
	bool temp = false;
	ReadFile(fileHandle, &temp, sizeof(bool), &size, NULL);

	return temp;
}

WORD cBinaryReader::Word()
{
	WORD temp = 0;
	ReadFile(fileHandle, &temp, sizeof(WORD), &size, NULL);

	return temp;
}

int cBinaryReader::Int()
{
	int temp = 0;
	ReadFile(fileHandle, &temp, sizeof(int), &size, NULL);

	return temp;
}

UINT cBinaryReader::UInt()
{
	UINT temp = 0;
	ReadFile(fileHandle, &temp, sizeof(UINT), &size, NULL);

	return temp;
}

float cBinaryReader::Float()
{
	float temp = 0.0f;
	ReadFile(fileHandle, &temp, sizeof(float), &size, NULL);

	return temp;
}

double cBinaryReader::Double()
{
	double temp = 0.0f;
	ReadFile(fileHandle, &temp, sizeof(double), &size, NULL);

	return temp;
}

D3DXVECTOR2 cBinaryReader::Vector2()
{
	float x = Float();
	float y = Float();

	return{ x, y };
}

D3DXVECTOR3 cBinaryReader::Vector3()
{
	float x = Float();
	float y = Float();
	float z = Float();

	return{ x, y, z };
}

D3DXVECTOR4 cBinaryReader::Vector4()
{
	float x = Float();
	float y = Float();
	float z = Float();
	float w = Float();

	return{ x, y, z, w };
}

D3DXQUATERNION cBinaryReader::Quaternion()
{
	float x = Float();
	float y = Float();
	float z = Float();
	float w = Float();

	return{ x,y,z,w };
}

D3DXCOLOR cBinaryReader::Color3f()
{
	float r = Float();
	float g = Float();
	float b = Float();

	return{ r, g, b, 1.0f };
}

D3DXCOLOR cBinaryReader::Color4f()
{
	float r = Float();
	float g = Float();
	float b = Float();
	float a = Float();

	return{ r, g, b, a };
}

D3DXMATRIX cBinaryReader::Matrix()
{
	D3DXMATRIX matrix;
	matrix._11 = Float();	matrix._12 = Float();	matrix._13 = Float();	matrix._14 = Float();
	matrix._21 = Float();	matrix._22 = Float();	matrix._23 = Float();	matrix._24 = Float();
	matrix._31 = Float();	matrix._32 = Float();	matrix._33 = Float();	matrix._34 = Float();
	matrix._41 = Float();	matrix._42 = Float();	matrix._43 = Float();	matrix._44 = Float();

	return matrix;
}

string cBinaryReader::String()
{
	UINT size = Int();

	char* temp = new char[size + 1];
	ReadFile(fileHandle, temp, sizeof(char) * size, &this->size, nullptr); //데이터 읽기
	temp[size] = '\0';

	return temp;
}

void cBinaryReader::Byte(void ** data, UINT dataSize)
{
	ReadFile(fileHandle, *data, dataSize, &size, nullptr);
}

cBinaryReader::cBinaryReader()
{
}

cBinaryReader::~cBinaryReader()
{
}
