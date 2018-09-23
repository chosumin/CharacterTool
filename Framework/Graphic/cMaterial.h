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
		������ �Ҹ���
	*******************/
	cMaterial();
	cMaterial(wstring shaderFile);
	~cMaterial();
public:
	// iClonable��(��) ���� ��ӵ�
	virtual unique_ptr<cMaterial> Clone() const override;
public:
	/********************
		Getter Setter
	*********************/

	//��Ƽ���� ���ϸ�
	wstring GetName() const { return _name; }
	//��Ƽ���� ���ϸ� ����
	void SetName(wstring name) { _name = name; }

	//���̴� �̸� ������
	wstring GetShaderName() const 
	{ 
		return _shader->GetFileName(); 
	}

	//��Ƽ���� ���̴� ����
	void SetShader(string file);
	void SetShader(wstring file);
	void SetShader(shared_ptr<cShader> & shader);

	//�ݻ� ������ ���� �� ��ȯ
	D3DXCOLOR* GetTextureColor(ColorType eType);
	float* GetTextureColor(ColorType eType, int i);//i�� ����̳� �־ ��

	//�ݻ� �� ����
	void SetTextureColor(ColorType eType, D3DXCOLOR color, float value);
	void SetTextureColor(ColorType eType, float r, float g, float b, float value);

	//�ݻ� �ؽ��� �����̳� ��ȯ
	const unordered_map<TextureType, shared_ptr<cTexture>> & GetTextureMaps() const
	{
		return _textureMap;
	}
	//�ݻ� �ؽ��� ��ȯ
	const shared_ptr<cTexture> & GetTextureMap(TextureType eType)
	{
		return _textureMap[eType];
	}

	//�ݻ� �ؽ��� ����
	void SetTextureMap(TextureType eType, wstring file, D3DX11_IMAGE_LOAD_INFO * info = nullptr);
	void SetTextureMap(TextureType eType, string file, D3DX11_IMAGE_LOAD_INFO * info = nullptr);

	/************
		������
	*************/
	void Render(const bool & onShader = true);
private:
	void Initialize();
	D3DXCOLOR& GetColor(ColorType eType);
	float& GetColor(ColorType eType, int i);
private:
	//��Ƽ���� �̸�
	wstring _name = L"";

	//��Ƽ���� ����
	shared_ptr<cMaterialBuffer> _cbuffer = nullptr;

	//���̴�
	shared_ptr<cShader> _shader = nullptr;

	//�ؽ��� ��
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