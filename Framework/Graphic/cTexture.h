#pragma once

class cTexture
{
public:
	//텍스쳐 팩토리
	static shared_ptr<cTexture> Create(wstring filePath, D3DX11_IMAGE_LOAD_INFO *loadInfo = nullptr);

	static void SetBlankSamplerState(UINT slot);
	static void SetBlankShaderResource(UINT slot);

	//소멸자
	~cTexture();
public:
	D3D11_TEXTURE2D_DESC ReadPixels(DXGI_FORMAT readFormat, vector<D3DXCOLOR>* pixels);
public:
	/************
	Getter Setter
	*************/
	wstring GetFile() { return _fileName; }

	UINT GetWidth() { return _metaData.width; }
	UINT GetHeight() { return _metaData.height; }

	void GetImageInfo(OUT DirectX::TexMetadata *outData) const { *outData = _metaData; }

	ID3D11ShaderResourceView* GetView() { return _view; }

	void SetSamplerState(D3D11_SAMPLER_DESC desc);
	void SetShaderResource(UINT slot);
	void SetShaderSampler(UINT slot);
private:
	/************
		생성자
	*************/
	cTexture(wstring file, D3DX11_IMAGE_LOAD_INFO *loadInfo = nullptr);
private:
	//메타데이터, 쉐이더리소스뷰 생성
	void Load(cTexture *texture, D3DX11_IMAGE_LOAD_INFO *loadInfo = NULL);

	//확장자에 따른 데이터 생성
	void CreateData(wstring extension, cTexture *texture, DirectX::ScratchImage *outImage);
private:
	wstring _fileName;

	DirectX::TexMetadata _metaData;
	ID3D11ShaderResourceView *_view;
	ID3D11SamplerState *_samplerState;
};