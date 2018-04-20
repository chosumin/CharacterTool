#include "stdafx.h"
#include "D3D.h"

D3DDesc D3D::m_Desc;
ID3D11Device* D3D::m_pDevice = nullptr;
ID3D11DeviceContext* D3D::m_pDeviceContext = nullptr;
IDXGISwapChain* D3D::m_pSwapChain = nullptr;

D3D::D3D()
	:m_uNumerator(0)
	,m_uDenominator(0)
{
	SetGpuInfo();

	CreateSwapChainAndDevice();
	CreateBackBuffer(m_Desc.Width, m_Desc.Height);
}

D3D::~D3D()
{
	DeleteBackBuffer();

	if (m_pSwapChain)
		m_pSwapChain->SetFullscreenState(false, nullptr);

	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pDeviceContext);
	HRESULT hr = m_pDevice->Release();
	assert(SUCCEEDED(hr) && "디바이스이용 생성된 객체중 해체되지 않는 객체 있음");
}

void D3D::GetDesc(OUT D3DDesc * pDesc)
{
	*pDesc = m_Desc;
}

void D3D::SetDesc(IN D3DDesc & rDesc)
{
	m_Desc = rDesc;
}

ID3D11Device * D3D::GetDevice()
{
	return m_pDevice;
}

ID3D11DeviceContext * D3D::GetDC()
{
	return m_pDeviceContext;
}

IDXGISwapChain * D3D::GetSwapChain()
{
	return m_pSwapChain;
}

void D3D::SetRenderTarget(ID3D11RenderTargetView * rtv, ID3D11DepthStencilView * dsv)
{
	if (rtv == NULL)
		rtv = m_pRenderTargetView;

	if (dsv == NULL)
		dsv = m_pDepthStencilView;

	D3D::GetDC()->OMSetRenderTargets(1, &rtv, dsv);
}

void D3D::Clear(D3DXCOLOR color, ID3D11RenderTargetView * rtv, ID3D11DepthStencilView * dsv)
{
	if (rtv == NULL)
		rtv = m_pRenderTargetView;

	if (dsv == NULL)
		dsv = m_pDepthStencilView;

	m_pDeviceContext->ClearRenderTargetView(rtv, color);
	m_pDeviceContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1, 0);
}

void D3D::Present()
{
	m_pSwapChain->Present(m_Desc.Vsync == true ? 1 : 0, 0);
}

void D3D::ResizeScreen(float width, float height)
{
	m_Desc.Width = width;
	m_Desc.Height = height;

	DeleteBackBuffer();
	{
		HRESULT hr = m_pSwapChain->ResizeBuffers(0, (UINT)width, (UINT)height
												 , DXGI_FORMAT_UNKNOWN, 0);
		assert(SUCCEEDED(hr));
	}
	CreateBackBuffer(width, height);
}

void D3D::SetGpuInfo()
{
	HRESULT hr;

	IDXGIFactory* factory;
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&factory);
	assert(SUCCEEDED(hr));

	IDXGIAdapter* adapter;
	hr = factory->EnumAdapters(0, &adapter);
	assert(SUCCEEDED(hr));

	IDXGIOutput* adapterOutput;
	hr = adapter->EnumOutputs(0, &adapterOutput);
	assert(SUCCEEDED(hr));

	{
		UINT modeCount;
		hr = adapterOutput->GetDisplayModeList
		(
			DXGI_FORMAT_R8G8B8A8_UNORM
			, DXGI_ENUM_MODES_INTERLACED
			, &modeCount
			, NULL
		);
		assert(SUCCEEDED(hr));

		DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[modeCount];
		hr = adapterOutput->GetDisplayModeList
		(
			DXGI_FORMAT_R8G8B8A8_UNORM
			, DXGI_ENUM_MODES_INTERLACED
			, &modeCount
			, displayModeList
		);
		assert(SUCCEEDED(hr));

		for (UINT i = 0; i < modeCount; i++)
		{
			bool isCheck = true;
			isCheck &= displayModeList[i].Width == m_Desc.Width;
			isCheck &= displayModeList[i].Height == m_Desc.Height;

			if (isCheck == true)
			{
				m_uNumerator = displayModeList[i].RefreshRate.Numerator;
				m_uDenominator = displayModeList[i].RefreshRate.Denominator;
			}
		}

		DXGI_ADAPTER_DESC adapterDesc;
		hr = adapter->GetDesc(&adapterDesc);
		assert(SUCCEEDED(hr));

		m_uGpuMemorySize = adapterDesc.DedicatedVideoMemory / 1024 / 1024;
		m_wsGpuDescription = adapterDesc.Description;

		SAFE_DELETE_ARRAY(displayModeList);
	}

	SAFE_RELEASE(adapterOutput);
	SAFE_RELEASE(adapter);
	SAFE_RELEASE(factory);
}

void D3D::CreateSwapChainAndDevice()
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (m_Desc.Vsync == true)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = m_uNumerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = m_uDenominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = m_Desc.Hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !m_Desc.FullScreen;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

	//포함할 다이렉트 버전
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		NULL
		, D3D_DRIVER_TYPE_HARDWARE
		, NULL
		, creationFlags
		, featureLevels
		, 1
		, D3D11_SDK_VERSION
		, &swapChainDesc
		, &m_pSwapChain
		, &m_pDevice
		, NULL
		, &m_pDeviceContext
	);
	assert(SUCCEEDED(hr));
}

void D3D::CreateBackBuffer(float width, float height)
{
	HRESULT hr;

	/********************
		백버퍼 텍스쳐 생성
	*********************/
	{
		D3D11_TEXTURE2D_DESC desc = { 0 };
		desc.Width = (UINT)width;
		desc.Height = (UINT)height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		hr = D3D::GetDevice()->CreateTexture2D(&desc, NULL, &m_pBackBuffer);
		assert(SUCCEEDED(hr));
	}

	/********************
		렌더 타겟 생성
	*********************/
	{
		ID3D11Texture2D* backbufferPointer;
		hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&backbufferPointer);
		assert(SUCCEEDED(hr));

		hr = D3D::GetDevice()->CreateRenderTargetView(backbufferPointer, NULL, &m_pRenderTargetView);
		assert(SUCCEEDED(hr));

		SAFE_RELEASE(backbufferPointer);
	}

	/********************
		깊이 스텐실 생성
	*********************/
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipSlice = 0;

		hr = D3D::GetDevice()->CreateDepthStencilView(m_pBackBuffer, &desc, &m_pDepthStencilView);
		assert(SUCCEEDED(hr));

		SetRenderTarget(m_pRenderTargetView, m_pDepthStencilView);
	}
}

void D3D::DeleteBackBuffer()
{
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pBackBuffer);
}
