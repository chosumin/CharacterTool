#pragma once
#include "./Common/cSingletone.h"

struct D3DDesc
{
	wstring		ProjectName;
	HINSTANCE	Instance;
	HWND		Hwnd;
	bool		Vsync;
	bool		FullScreen;
	float		Width;
	float		Height;
};

class D3D : public cSingletone<D3D>
{
public:
	static void					GetDesc(OUT D3DDesc* pDesc);
	static void					SetDesc(IN D3DDesc& rDesc);
	static ID3D11Device*		GetDevice();
	static ID3D11DeviceContext*	GetDC();
	static IDXGISwapChain*		GetSwapChain();
public:
	void SetRenderTarget(ID3D11RenderTargetView* rtv = nullptr
		 				 , ID3D11DepthStencilView* dsv = nullptr);
	void Clear(D3DXCOLOR color = D3DXCOLOR(0xFF555566)
		 	   , ID3D11RenderTargetView* rtv = nullptr
		 	   , ID3D11DepthStencilView* dsv = nullptr);
	void Present();
	void ResizeScreen(float width, float height);
private: 
	D3D();
	~D3D();

	void SetGpuInfo();
	void CreateSwapChainAndDevice();
	void CreateBackBuffer(float width, float height);
	void DeleteBackBuffer();
private:
	static D3DDesc				m_Desc;
	static IDXGISwapChain*		m_pSwapChain; //더블 버퍼링
	static ID3D11Device*		m_pDevice; //객체 관리
	static ID3D11DeviceContext*	m_pDeviceContext; //렌더링 관리
private:
	UINT	m_uGpuMemorySize;
	UINT	m_uNumerator;
	UINT	m_uDenominator;
	wstring	m_wsGpuDescription;

	ID3D11Texture2D*		m_pBackBuffer;
	ID3D11DepthStencilView*	m_pDepthStencilView;
	ID3D11RenderTargetView*	m_pRenderTargetView;
private:
	friend class cSingletone<D3D>;
};

