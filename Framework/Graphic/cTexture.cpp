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
	LoadJson(this, loadInfo);

	D3D11_SAMPLER_DESC desc;
	cStates::GetSamplerDesc(&desc);
	cStates::CreateSampler(&desc, &_samplerState);
}

cTexture::~cTexture()
{
	SAFE_RELEASE(_samplerState);
	SAFE_RELEASE(_view);
}

void cTexture::SaveFile(wstring file, ID3D11Texture2D * src)
{
	D3D11_TEXTURE2D_DESC srcDesc;
	src->GetDesc(&srcDesc);

	ID3D11Texture2D* dest;
	D3D11_TEXTURE2D_DESC destDesc;
	ZeroMemory(&destDesc, sizeof(D3D11_TEXTURE2D_DESC));
	destDesc.Width = srcDesc.Width;
	destDesc.Height = srcDesc.Height;
	destDesc.MipLevels = 1;
	destDesc.ArraySize = 1;
	destDesc.Format = srcDesc.Format;
	destDesc.SampleDesc = srcDesc.SampleDesc;
	destDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	destDesc.Usage = D3D11_USAGE_STAGING;

	HRESULT hr;
	hr = D3D::GetDevice()->CreateTexture2D(&destDesc, NULL, &dest);
	assert(SUCCEEDED(hr));

	hr = D3DX11LoadTextureFromTexture(D3D::GetDC(), src, NULL, dest);
	assert(SUCCEEDED(hr));

	hr = D3DX11SaveTextureToFile(D3D::GetDC(), dest, D3DX11_IFF_PNG, file.c_str());
	assert(SUCCEEDED(hr));

	SAFE_RELEASE(dest);
}

D3D11_TEXTURE2D_DESC cTexture::ReadPixels(DXGI_FORMAT readFormat, vector<D3DXCOLOR>* pixels)
{
	HRESULT hr;

	ID3D11Texture2D* srcTexture;
	_view->GetResource((ID3D11Resource **)&srcTexture);

	D3D11_TEXTURE2D_DESC srcDesc;
	srcTexture->GetDesc(&srcDesc);


	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Width = srcDesc.Width;
	desc.Height = srcDesc.Height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = readFormat;
	desc.SampleDesc = srcDesc.SampleDesc;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.Usage = D3D11_USAGE_STAGING;

	ID3D11Texture2D* texture;
	hr = D3D::GetDevice()->CreateTexture2D(&desc, NULL, &texture);
	assert(SUCCEEDED(hr));

	hr = D3DX11LoadTextureFromTexture(D3D::GetDC(), srcTexture, NULL, texture);
	assert(SUCCEEDED(hr));


	D3D11_MAPPED_SUBRESOURCE map;
	UINT* colors = new UINT[desc.Width * desc.Height];
	D3D::GetDC()->Map(texture, 0, D3D11_MAP_READ, NULL, &map);
	{
		memcpy(colors, map.pData, sizeof(UINT) * desc.Width * desc.Height);
	}
	D3D::GetDC()->Unmap(texture, 0);


	pixels->reserve(desc.Width * desc.Height);
	for (UINT y = 0; y < desc.Height; y++)
	{
		for (UINT x = 0; x < desc.Width; x++)
		{
			UINT index = desc.Width * y + x;

			CONST FLOAT f = 1.0f / 255.0f;
			float r = f * (float)((0xFF000000 & colors[index]) >> 24);
			float g = f * (float)((0x00FF0000 & colors[index]) >> 16);
			float b = f * (float)((0x0000FF00 & colors[index]) >> 8);
			float a = f * (float)((0x000000FF & colors[index]) >> 0);

			pixels->push_back(D3DXCOLOR(a, b, g, r));
		}
	}

	SAFE_DELETE_ARRAY(colors);
	SAFE_RELEASE(srcTexture);
	SAFE_RELEASE(texture);

	return desc;
}

void cTexture::SaveFile(wstring file)
{
	ID3D11Texture2D* srcTexture;
	_view->GetResource((ID3D11Resource **)&srcTexture);

	SaveFile(file, srcTexture);
}

void cTexture::LoadJson(cTexture * texture, D3DX11_IMAGE_LOAD_INFO * loadInfo)
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
