#pragma once
#include "./Interface/iCloneable.h"

enum class ColorType
{
	AMBIENT, DIFFUSE, SPECULAR, EMISSIVE, SHININESS, DETAILFACTOR
};
enum class TextureType
{
	DIFFUSE = 0, SPECULAR = 1, EMISSIVE = 2, NORMAL = 3, DETAIL = 4
};

class cMaterialBuffer;
class cTexture;
class cMaterial : public iCloneable<cMaterial>
{
public:
	/******************
		생성자 소멸자
	*******************/
	cMaterial();
	cMaterial(wstring shaderFile);
	~cMaterial();
public:
	// iClonable을(를) 통해 상속됨
	virtual unique_ptr<cMaterial> Clone() const override;
public:
	/********************
		Getter Setter
	*********************/
	wstring GetName() const { return _name; }
	void SetName(wstring name) { _name = name; }

	void SetShader(string file);
	void SetShader(wstring file);
	void SetShader(shared_ptr<cShader> shader);

	D3DXCOLOR* GetTextureColor(ColorType eType);
	float* GetTextureColor(ColorType eType, int i);//i는 어떤값이나 넣어도 됨
	void SetTextureColor(ColorType eType, D3DXCOLOR color, float value);
	void SetTextureColor(ColorType eType, float r, float g, float b, float value);

	weak_ptr<cTexture> GetTextureMap(TextureType eType);
	void SetTextureMap(TextureType eType, wstring file, D3DX11_IMAGE_LOAD_INFO * info = nullptr);
	void SetTextureMap(TextureType eType, string file, D3DX11_IMAGE_LOAD_INFO * info = nullptr);

	/************
		렌더링
	*************/
	void Render();
private:
	void Initialize();
	D3DXCOLOR& GetColor(ColorType eType);
	float& GetColor(ColorType eType, int i);
private:
	//머터리얼 이름
	wstring _name = L"";

	//머터리얼 버퍼
	shared_ptr<cMaterialBuffer> _cbuffer = nullptr;

	//쉐이더
	bool _hasShader = false;
	shared_ptr<cShader> _shader = nullptr;

	//텍스쳐 맵
	unordered_map<TextureType, shared_ptr<cTexture>> _textureMap;
};

/**************************************************
***************************************************/

class cMaterialBuffer : public cCBuffer
{
public:
	cMaterialBuffer() : cCBuffer(&Data, sizeof(Data))
	{}

	struct Buffer
	{
		D3DXCOLOR Ambient = { 0,0,0,1 };
		D3DXCOLOR Diffuse = { 1,1,1,1 };
		D3DXCOLOR Specular = { 1,1,1,1 };
		D3DXCOLOR Emissive = { 0,0,0,1 };

		float Shininess = 1.0f;
		float DetailFactor = 1.0f;

		float Padding[2];
	}Data;
};