#pragma once
#include "./Interface/iCloneable.h"

enum class ColorType
{
	AMBIENT, DIFFUSE, SPECULAR, EMISSIVE, SHININESS, DETAILFACTOR,
	COUNT
};
enum class TextureType
{
	DIFFUSE = 0, SPECULAR = 1, EMISSIVE = 2, NORMAL = 3, DETAIL = 4,
	COUNT
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

	//머티리얼 파일명
	wstring GetName() const { return _name; }
	//머티리얼 파일명 변경
	void SetName(wstring name) { _name = name; }

	//쉐이더 이름 가져옴
	wstring GetShaderName() const 
	{ 
		return _shader->GetFileName(); 
	}

	//머티리얼 쉐이더 변경
	void SetShader(string file);
	void SetShader(wstring file);
	void SetShader(shared_ptr<cShader> & shader);

	//반사 종류에 따른 색 반환
	D3DXCOLOR* GetTextureColor(ColorType eType);
	float* GetTextureColor(ColorType eType, int i);//i는 어떤값이나 넣어도 됨

	//반사 색 변경
	void SetTextureColor(ColorType eType, D3DXCOLOR color, float value);
	void SetTextureColor(ColorType eType, float r, float g, float b, float value);

	//반사 텍스쳐 컨테이너 반환
	const unordered_map<TextureType, shared_ptr<cTexture>> & GetTextureMaps() const
	{
		return _textureMap;
	}
	//반사 텍스쳐 반환
	const shared_ptr<cTexture> & GetTextureMap(TextureType eType)
	{
		return _textureMap[eType];
	}

	//반사 텍스쳐 변경
	void SetTextureMap(TextureType eType, wstring file, D3DX11_IMAGE_LOAD_INFO * info = nullptr);
	void SetTextureMap(TextureType eType, string file, D3DX11_IMAGE_LOAD_INFO * info = nullptr);

	/************
		렌더링
	*************/
	void Render(const bool & onShader = true);
private:
	void Initialize();
	D3DXCOLOR& GetColor(ColorType eType);
	float& GetColor(ColorType eType, int i);
private:
	//머티리얼 이름
	wstring _name = L"";

	//머티리얼 버퍼
	shared_ptr<cMaterialBuffer> _cbuffer = nullptr;

	//쉐이더
	shared_ptr<cShader> _shader = nullptr;

	//텍스쳐 맵
	unordered_map<TextureType, shared_ptr<cTexture>> _textureMap;
};

/******************************************************************/

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