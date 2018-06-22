#include "stdafx.h"
#include "cTexture.h"
#include "./Helper/cPath.h"
using namespace DirectX;

/**************************
		텍스쳐 팩토리
***************************/
shared_ptr<cTexture> cTexture::Create(wstring filePath, D3DX11_IMAGE_LOAD_INFO * loadInfo)
{
	//텍스쳐 캐시
	static unordered_map<wstring, weak_ptr<cTexture>> cache;

	//트릭 클래스
	struct make_shared_enabler : public cTexture {
		make_shared_enabler(wstring file, D3DX11_IMAGE_LOAD_INFO *loadInfo) :cTexture(file, loadInfo) {}
	};

	//캐시에서 텍스쳐 검색
	auto shaderPtr = cache[filePath].lock();
	if (shaderPtr == nullptr)
	{
		shaderPtr = make_shared<make_shared_enabler>(filePath, loadInfo);
		cache[filePath] = shaderPtr;
	}

	return shaderPtr;
}

void cTexture::SetSamplerState(D3D11_SAMPLER_DESC desc)
{
	SAFE_RELEASE(_samplerState);

	cStates::CreateSampler(&desc, &_samplerState);
}

void cTexture::SetShaderResource(UINT slot)
{
	D3D::GetDC()->PSSetShaderResources(slot, 1, &_view);
}

void cTexture::SetShaderSampler(UINT slot)
{
	D3D::GetDC()->PSSetSamplers(slot, 1, &_samplerState);
}

void cTexture::SetBlankSamplerState(UINT slot)
{
	ID3D11SamplerState* null[1]{ NULL };
	D3D::GetDC()->PSSetSamplers(slot, 1, null);
}

void cTexture::SetBlankShaderResource(UINT slot)
{
	ID3D11ShaderResourceView* null[1]{ NULL };
	D3D::GetDC()->PSSetShaderResources(slot, 1, null);
}

cTexture::cTexture(wstring file, D3DX11_IMAGE_LOAD_INFO * loadInfo)
	:_fileName(file)
{
	Load(this, loadInfo);

	D3D11_SAMPLER_DESC desc;
	cStates::GetSamplerDesc(&desc);
	cStates::CreateSampler(&desc, &_samplerState);
}

cTexture::~cTexture()
{
	SAFE_RELEASE(_samplerState);
	SAFE_RELEASE(_view);
}

void cTexture::Load(cTexture * texture, D3DX11_IMAGE_LOAD_INFO * loadInfo)
{
	ScratchImage image; //복사 이미지

	//확장자 구하기
	wstring extension = cPath::GetExtension(texture->_fileName);

	CreateData(extension, texture, &image);
	
	HRESULT hr = DirectX::CreateShaderResourceView(D3D::GetDevice(), image.GetImages(), image.GetImageCount(), _metaData, &_view);
	assert(SUCCEEDED(hr) && "리소스 뷰 생성 오류!");
}

/***********************
확장자에 따른 데이터 생성
************************/
void cTexture::CreateData(wstring extension, cTexture *texture, ScratchImage *outImage)
{
	HRESULT hr_data, hr_image;
	
	if (extension == L"tga")
	{
		hr_data = GetMetadataFromTGAFile(texture->_fileName.c_str(), _metaData);
		hr_image = LoadFromTGAFile(texture->_fileName.c_str(), &_metaData, *outImage);
	}
	else if (extension == L"dds")
	{
		hr_data = GetMetadataFromDDSFile(texture->_fileName.c_str(), DDS_FLAGS_NONE, _metaData);
		hr_image = LoadFromDDSFile(texture->_fileName.c_str(), DDS_FLAGS_NONE, &_metaData, *outImage);
	}
	else if (extension == L"hdr")
	{
		hr_data = hr_image = false;
	}
	else
	{
		hr_data = GetMetadataFromWICFile(texture->_fileName.c_str(), WIC_FLAGS_NONE, _metaData);
		hr_image = LoadFromWICFile(texture->_fileName.c_str(), WIC_FLAGS_NONE, &_metaData, *outImage);
	}

	assert(SUCCEEDED(hr_data) && "텍스쳐 생성 오류!");
	assert(SUCCEEDED(hr_image) && "이미지 복사 오류!");
}
