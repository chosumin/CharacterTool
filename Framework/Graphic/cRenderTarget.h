#pragma once

class cOrthogonal;
class cRenderTarget
{
public:
	cRenderTarget();
	~cRenderTarget();

	ID3D11RenderTargetView* GetRTV() { return _rtv; }
	ID3D11ShaderResourceView* GetSRV() { return _srv; }
	ID3D11Texture2D* GetTexture() { return _texture; }

	UINT GetWidth() { return _width; }
	UINT GetHeight() { return _height; }

	void Set(D3DXCOLOR color = 0xFFFFFFFF);
	void Create(UINT width = 0, UINT height = 0);
private:
	void Delete();

	//�ؽ��� ����
	void CreateTexture(D3D11_TEXTURE2D_DESC& textureDesc);

	//���� Ÿ�� �� ����
	void CreateRTV(D3D11_TEXTURE2D_DESC& textureDesc);

	//���̴� ���ҽ� �� ����
	void CreateSRV(D3D11_TEXTURE2D_DESC& textureDesc);
private:
	UINT _width, _height;

	ID3D11Texture2D* _texture;
	ID3D11RenderTargetView* _rtv;
	ID3D11ShaderResourceView* _srv;
};