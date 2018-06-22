#include "stdafx.h"
#include "cWorldGrid.h"
#include "./Polygon/PolygonUtility.h"

cWorldGrid::cWorldGrid()
{
	_world = make_unique<cWorldBuffer>();
	_shader = cShader::Create(Shader + L"001_VertexC.hlsl");

	CreateVertices();
}

cWorldGrid::~cWorldGrid()
{
	SAFE_RELEASE(_vb);
}

void cWorldGrid::Render()
{
	if (cDebug::GetDebugMode() == false)
		return;

	_world->SetVSBuffer(1);
	_shader->Render();

	cPolygon<VertexType>::Render(_vb, _vertices.size(), D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void cWorldGrid::CreateVertices(int nNumHalfTile, float fInterval)
{
	float fMax = nNumHalfTile * fInterval;
	float fMin = -nNumHalfTile * fInterval;

	float normal = 0.7f;
	float middle = 0.4f;
	VertexType v;
	for (int i = 1; i <= nNumHalfTile; i++)
	{
		if (i % 5 == 0)
			v.color = { normal, normal, normal, normal };
		else
			v.color = { middle, middle, middle, middle };

		v.position = D3DXVECTOR3(fMin, 0, i * fInterval); _vertices.push_back(v);
		v.position = D3DXVECTOR3(fMax, 0, i * fInterval); _vertices.push_back(v);

		v.position = D3DXVECTOR3(fMin, 0, -i * fInterval); _vertices.push_back(v);
		v.position = D3DXVECTOR3(fMax, 0, -i * fInterval); _vertices.push_back(v);

		v.position = D3DXVECTOR3(i * fInterval, 0, fMin); _vertices.push_back(v);
		v.position = D3DXVECTOR3(i * fInterval, 0, fMax); _vertices.push_back(v);

		v.position = D3DXVECTOR3(-i * fInterval, 0, fMin); _vertices.push_back(v);
		v.position = D3DXVECTOR3(-i * fInterval, 0, fMax); _vertices.push_back(v);
	}

	v.color = { 1,0,0,1 };
	v.position = D3DXVECTOR3(fMin, 0, 0); _vertices.push_back(v);
	v.position = D3DXVECTOR3(fMax, 0, 0); _vertices.push_back(v);

	v.color = { 0, 1, 0, 1 };
	v.position = D3DXVECTOR3(0, fMin, 0); _vertices.push_back(v);
	v.position = D3DXVECTOR3(0, fMax, 0); _vertices.push_back(v);

	v.color = { 0, 0, 1, 1 };
	v.position = D3DXVECTOR3(0, 0, fMin); _vertices.push_back(v);
	v.position = D3DXVECTOR3(0, 0, fMax); _vertices.push_back(v);

	cPolygon<VertexType>::CreateVertexBuffer(&_vb, _vertices);
}