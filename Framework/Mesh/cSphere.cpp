#include "stdafx.h"
#include "cSphere.h"

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
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	D3D::GetDC()->DrawIndexed(_vertexData.size(), 0, 0);
}

void cSphere::CreateVertex(D3DXVECTOR3 center, float radius)
{
	const int sphereResolution = 30;
	const int lineCount = (sphereResolution + 1) * 3;
	const int vertexCount = lineCount * 2;
	const int indexCount = vertexCount;

	_vertexData.resize(vertexCount);

	float step = 2.0f * (float)D3DX_PI / sphereResolution;

	int index = 0;
	//xy
	for (float i = 0.0f; i < 2 * D3DX_PI; i += step)
	{
		_vertexData[index++].position = D3DXVECTOR3{ cosf(i), sinf(i), 0.0f };
		_vertexData[index++].position = D3DXVECTOR3{ cosf(i + step), sinf(i + step), 0.0f };
	}

	//xz
	for (float i = 0.0f; i < 2 * D3DX_PI; i += step)
	{
		_vertexData[index++].position = D3DXVECTOR3{ cosf(i), 0.0f, sinf(i) };
		_vertexData[index++].position = D3DXVECTOR3{ cosf(i + step), 0.0f, sinf(i + step) };
	}

	//yz
	for (float i = 0; i < 2 * D3DX_PI; i += step)
	{
		_vertexData[index++].position = D3DXVECTOR3{ 0.0f, cosf(i), sinf(i) };
		_vertexData[index++].position = D3DXVECTOR3{ 0.0f, cosf(i + step), sinf(i + step) };
	}

	for (auto& vertex : _vertexData)
		vertex.color = _color;

	vector<UINT> indexData(indexCount);
	for (UINT i = 0; i < indexCount; i++)
	{
		indexData[i] = i;
	}

	cMeshUtility<VertexC>::CreateIndexBuffer(&_ib, indexData);
	cMeshUtility<VertexC>::CreateVertexBuffer(&_vb, _vertexData);
}