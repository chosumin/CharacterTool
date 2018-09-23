#include "stdafx.h"
#include "cRenderTarget.h"

cRenderTarget::cRenderTarget()
	: _texture(nullptr)
	, _rtv(nullptr)
	, _srv(nullptr)
{
	D3DDesc desc;
	D3D::GetDesc(&desc);

	Create();
}

cRenderTarget::~cRenderTarget()
{
	Delete();
}

void cRenderTarget::Set(D3DXCOLOR color)
{
	D3D::Get()->SetRenderTarget(_rtv);
	D3D::Get()->Clear(color, _rtv);
}

void cRenderTarget::Create(UINT width, UINT height)
{
	Delete();

	D3DDesc desc;
	D3D::GetDesc(&desc);

	if (width < 1)
		_width = static_cast<UINT>(desc.Width);
	else
		_width = width;

	if (height < 1)
		_height = static_cast<UINT>(desc.Height);
	else
		_height = height;

	D3D11_TEXTURE2D_DESC textureDesc;
	CreateTexture(textureDesc);
	CreateRTV(textureDesc);
	CreateSRV(textureDesc);
}

void cRenderTarget::Delete()
{
	SAFE_RELEASE(_texture);
	SAFE_RELEASE(_rtv);
	SAFE_RELEASE(_srv)
}

void cRenderTarget::CreateTexture(D3D11_TEXTURE2D_DESC& textureDesc)
{
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.Width = _width; //ȭ�� ������� ����
	textureDesc.Height = _height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1; //��Ƽ ���ø� ����
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags
		//���̴� ���ҽ� �並 ���� �׸��ڴ�. ���� ����Ÿ�� �뵵�̴�.
		= D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

	HRESULT hr;
	hr = D3D::GetDevice()->CreateTexture2D(&textureDesc, nullptr, &_texture);
	assert(SUCCEEDED(hr) && "�ؽ��� ���� ����!");
}

void cRenderTarget::CreateRTV(D3D11_TEXTURE2D_DESC& textureDesc)
{
	D3D11_RENDER_TARGET_VIEW_DESC renderDesc;
	ZeroMemory(&renderDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	renderDesc.Format = textureDesc.Format;
	renderDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D; //���� Ÿ�� �뵵�� ����� ���̴�.
	renderDesc.Texture2D.MipSlice = 0;

	HRESULT hr = D3D::GetDevice()->CreateRenderTargetView(_texture, &renderDesc, &_rtv);
	assert(SUCCEEDED(hr) && "����Ÿ�ٺ� ���� ����!");
}

void cRenderTarget::CreateSRV(D3D11_TEXTURE2D_DESC & textureDesc)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	viewDesc.Format = textureDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;

	HRESULT hr = D3D::GetDevice()->CreateShaderResourceView(_texture, &viewDesc, &_srv);
	assert(SUCCEEDED(hr) && "���̴����ҽ��� ���� ����!");
}
