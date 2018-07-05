#include "stdafx.h"
#include "cBox.h"
#include "MeshUtility.h"
#include "cRectangle.h"

cBox::cBox(D3DXVECTOR3 min, D3DXVECTOR3 max, bool wireMode, D3DXCOLOR color)
	:m_bWireMode(wireMode)
{
	_rectangles.emplace_back(make_unique<cRectangle>(D3DXVECTOR3{ min.x,max.y,min.z }, D3DXVECTOR3{ max.x,min.y,min.z }, color));
	_rectangles.emplace_back(make_unique<cRectangle>(D3DXVECTOR3{ max.x, max.y, max.z }, D3DXVECTOR3{ min.x,min.y, max.z }, color));
	_rectangles.emplace_back(make_unique<cRectangle>(D3DXVECTOR3{ min.x,max.y,max.z }, D3DXVECTOR3{ max.x, max.y,min.z }, color));
	_rectangles.emplace_back(make_unique<cRectangle>(D3DXVECTOR3{ min.x, min.y, min.z }, D3DXVECTOR3{ max.x,min.y, max.z }, color));
	_rectangles.emplace_back(make_unique<cRectangle>(D3DXVECTOR3{ max.x,max.y,min.z }, D3DXVECTOR3{ max.x,min.y, max.z }, color));
	_rectangles.emplace_back(make_unique<cRectangle>(D3DXVECTOR3{ min.x,max.y,max.z }, D3DXVECTOR3{ min.x,min.y,min.z }, color));

	if (m_bWireMode)
	{
		D3D11_RASTERIZER_DESC desc;
		cStates::GetRasterizerDesc(&desc);
		desc.FillMode = D3D11_FILL_WIREFRAME;
		cStates::CreateRasterizer(&desc, &m_pRasterizer);
	}
}

cBox::~cBox()
{
	if(m_bWireMode)
		SAFE_RELEASE(m_pRasterizer);
}

void cBox::UpdateVertexBuffer()
{
	for (auto&& rect : _rectangles)
		rect->UpdateVertexBuffer();
}

void cBox::Render()
{
	if (m_bWireMode)
		D3D::GetDC()->RSSetState(m_pRasterizer);

	for (auto&& rect : _rectangles)
		rect->Render();

	if (m_bWireMode)
		D3D::GetDC()->RSSetState(nullptr);
}

void cBox::UpdateData(D3DXVECTOR3 start, D3DXVECTOR3 end)
{
	_rectangles[0]->UpdateData(D3DXVECTOR3{ start.x,end.y,start.z }, D3DXVECTOR3{ end.x,start.y,start.z });
	_rectangles[1]->UpdateData(D3DXVECTOR3{ end.x, end.y, end.z }, D3DXVECTOR3{ start.x,start.y, end.z });
	_rectangles[2]->UpdateData(D3DXVECTOR3{ start.x,end.y,end.z }, D3DXVECTOR3{ end.x, end.y,start.z });
	_rectangles[3]->UpdateData(D3DXVECTOR3{ start.x, start.y, start.z }, D3DXVECTOR3{ end.x,start.y, end.z });
	_rectangles[4]->UpdateData(D3DXVECTOR3{ end.x,end.y,start.z }, D3DXVECTOR3{ end.x,start.y, end.z });
	_rectangles[5]->UpdateData(D3DXVECTOR3{ start.x,end.y,end.z }, D3DXVECTOR3{ start.x,start.y,start.z });

	/*for (auto&& rect : _rectangles)
		rect->UpdateData(start, end);*/
}