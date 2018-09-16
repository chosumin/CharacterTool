#include "stdafx.h"
#include "cMaterial.h"
#include "cTexture.h"
#include "./Helper/cString.h"

cMaterial::cMaterial()
{
	Initialize();
}

cMaterial::cMaterial(wstring shaderFile)
{
	assert(shaderFile.length() > 0 && "파일명을 입력하세요!");

	Initialize();

	_shader = cShader::Create(shaderFile);
}

cMaterial::~cMaterial()
{
}

unique_ptr<cMaterial> cMaterial::Clone() const
{
	auto material = make_unique<cMaterial>();

	material->_name = _name;
	material->_cbuffer = make_shared<cMaterialBuffer>(*_cbuffer);
	material->_shader = cShader::Create(_shader->GetFileName());

	for (auto&& texture : _textureMap)
	{
		if(texture.second)
			material->_textureMap[texture.first] = cTexture::Create(texture.second->GetFile());
	}

	return material;
}

void cMaterial::SetShader(string file)
{
	SetShader(cString::Wstring(file));
}

void cMaterial::SetShader(wstring file)
{
	_shader = cShader::Create(file);
}

void cMaterial::SetShader(shared_ptr<cShader> shader)
{
	_shader = shader;
}

D3DXCOLOR * cMaterial::GetTextureColor(ColorType eType)
{
	return &GetColor(eType);
}

float * cMaterial::GetTextureColor(ColorType eType, int i)
{
	return &GetColor(eType, i);
}

void cMaterial::SetTextureColor(ColorType eType, D3DXCOLOR color, float value)
{
	GetColor(eType) = color;
	GetColor(eType, 0) = value;
}

void cMaterial::SetTextureColor(ColorType eType, float r, float g, float b, float value)
{
	SetTextureColor(eType, { r,g,b,1 }, value);
}

unordered_map<TextureType, shared_ptr<cTexture>> cMaterial::GetTextureMaps() const
{
	return _textureMap;
}

weak_ptr<cTexture> cMaterial::GetTextureMap(TextureType eType)
{
	return _textureMap[eType];
}

void cMaterial::SetTextureMap(TextureType eType, wstring file, D3DX11_IMAGE_LOAD_INFO * info)
{
	if(file.length() > 0)
		_textureMap[eType] = cTexture::Create(file, info);
}

void cMaterial::SetTextureMap(TextureType eType, string file, D3DX11_IMAGE_LOAD_INFO * info)
{
	SetTextureMap(eType, cString::Wstring(file), info);
}

void cMaterial::Render()
{
	if(_shader)
		_shader->Render();

	//텍스쳐 세팅
	for (auto&& texture : _textureMap)
	{
		auto slot = static_cast<UINT>(texture.first);
		
		if (texture.second)
		{
			texture.second->SetShaderResource(slot);
			texture.second->SetShaderSampler(slot);
		}
		else
		{
			cTexture::SetBlankShaderResource(slot);
			cTexture::SetBlankSamplerState(slot);
		}
	}

	_cbuffer->SetPSBuffer(1);
}

void cMaterial::Initialize()
{
	_textureMap[TextureType::DIFFUSE] = nullptr;
	_textureMap[TextureType::SPECULAR] = nullptr;
	_textureMap[TextureType::EMISSIVE] = nullptr;
	_textureMap[TextureType::NORMAL] = nullptr;
	_textureMap[TextureType::DETAIL] = nullptr;

	_cbuffer = make_shared<cMaterialBuffer>();
}

D3DXCOLOR & cMaterial::GetColor(ColorType eType)
{
	static D3DXCOLOR color{ 0,0,0,0 };
	switch (eType)
	{
		case ColorType::AMBIENT:
			return _cbuffer->Data.Ambient;
		case ColorType::DIFFUSE:
			return _cbuffer->Data.Diffuse;
		case ColorType::SPECULAR:
			return _cbuffer->Data.Specular;
		case ColorType::EMISSIVE:
			return _cbuffer->Data.Emissive;
	}

	return color;
}

float & cMaterial::GetColor(ColorType eType, int i)
{
	static float temp = 0.0f;
	switch (eType)
	{
		case ColorType::SHININESS:
			return _cbuffer->Data.Shininess;
		case ColorType::DETAILFACTOR:
			return _cbuffer->Data.DetailFactor;
	}
	return temp;
}
