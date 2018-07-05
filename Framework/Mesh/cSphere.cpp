#include "stdafx.h"
#include "cSphere.h"
#include "MeshUtility.h"

cSphere::cSphere(D3DXCOLOR color, D3DXVECTOR3 center, float radius)
	:_color(color)
{
	CreateVertex(center, radius);
}

cSphere::~cSphere()
{
	SAFE_RELEASE(_vb);
	SAFE_RELEASE(_ib);
}

void cSphere::Render()
{
	UINT stride = sizeof(VertexC);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &_vb, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(_ib, DXGI_FORMAT_R32_UINT, 0);

	//원 그리기
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D::GetDC()->DrawIndexed(_vertexData.size(), 0, 0);
}

void cSphere::CreateVertex(D3DXVECTOR3 center, float radius)
{
	VertexC vertex;
	vertex.color = _color;

	//원 정점
	const int sphereResolution = 30;
	float step = 2.0f * (float)D3DX_PI / sphereResolution;

	for (float i = 0.0f; i < 2 * D3DX_PI; i += step)
	{
		vertex.position = D3DXVECTOR3{ cosf(i) * radius, sinf(i) * radius, 0 };
		_vertexData.push_back(vertex);

		vertex.position = { 0,0,radius };
		_vertexData.push_back(vertex);

		vertex.position = D3DXVECTOR3{ cosf(i + step) * radius, sinf(i + step) * radius, 0 };
		_vertexData.push_back(vertex);
	}

	for (float i = 0.0f; i < 2 * D3DX_PI; i += step)
	{
		vertex.position = D3DXVECTOR3{ cosf(i) * radius, sinf(i) * radius, 0 };
		_vertexData.push_back(vertex);

		vertex.position = { 0,0,-radius };
		_vertexData.push_back(vertex);

		vertex.position = D3DXVECTOR3{ cosf(i + step) * radius, sinf(i + step) * radius, 0 };
		_vertexData.push_back(vertex);
	}

	for (auto&& vertex : _vertexData)
		vertex.position += center;

	UINT size = _vertexData.size();
	vector<UINT> indexData(size);
	for (UINT i = 0; i < size; i++)
		indexData[i] = i;

	cMeshUtility<VertexC>::CreateVertexBuffer(&_vb, _vertexData);
	cMeshUtility<VertexC>::CreateIndexBuffer(&_ib, indexData);
}