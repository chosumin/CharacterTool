#pragma once

class cModel;
class cSky
{
public:
	class Buffer : public cCBuffer
	{
	public:
		Buffer() : cCBuffer(&Data, sizeof(Data))
		{
			Data.Apex = D3DXCOLOR(0xffffffff);
			Data.Center = D3DXCOLOR(0xff0080ff);
			Data.Height = 2.0f;
		}

		struct Struct
		{
			D3DXCOLOR Center;
			D3DXCOLOR Apex;
			float Height;
			float Padding[3];
		}Data;
	};

	cSky(weak_ptr<sGlobalVariable> global);
	~cSky();

	void Update();
	void Render();

	void PostRender();
private:
	weak_ptr<sGlobalVariable> _global;

	wstring shaderFile;
	wstring modelFile;

	unique_ptr<cModel> model;

	unique_ptr<Buffer> buffer;

	ID3D11RasterizerState* rasterizer[2];
	ID3D11DepthStencilState* depthStencilState[2];
};