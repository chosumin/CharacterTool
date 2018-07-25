#include "stdafx.h"
#include "cRectangle.h"
#include "MeshUtility.h"

cRectangle::cRectangle(D3DXVECTOR3 min, D3DXVECTOR3 max, D3DXCOLOR color)
	: m_color(color)
{
	//정점 자료 입력
	m_vertexData.resize(4);

	UpdateData(min, max);

	//색인 자료 입력
	m_arrIndexData = new UINT[6]{ 0,1,2,2,1,3 };

	CreateBuffer();
}

cRectangle::~cRectangle()
{
	SAFE_DELETE_ARRAY(m_arrIndexData);

	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
}

void cRectangle::GetVertexVector(OUT vector<D3DXVECTOR3>& vec)
{
	vec.clear();
	
	for (auto&& vertex : m_vertexData)
		vec.emplace_back(vertex.position);
}

bool cRectangle::IntersectWithRay(const D3DXVECTOR3 & position, const D3DXVECTOR3 & direction) const
{
	auto v0 = m_vertexData[0].position;
	auto v1 = m_vertexData[1].position;
	auto v2 = m_vertexData[2].position;
	auto v3 = m_vertexData[3].position;

	float u, v, distance;
	if (D3DXIntersectTri(&v0, &v1, &v2, &position, &direction, &u, &v, &distance) == TRUE)
		return true;

	if (D3DXIntersectTri(&v3, &v1, &v2, &position, &direction, &u, &v, &distance) == TRUE)
		return true;

	return false;
}

void cRectangle::UpdateVertexBuffer()
{
	D3D::GetDC()->UpdateSubresource
	(
		m_pVB, 0, nullptr, &m_vertexData[0], sizeof(VertexC) * 4, 0
	);
}

void cRectangle::Render()
{
	UINT stride = sizeof(VertexC);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D::GetDC()->DrawIndexed(6, 0, 0);
}

void cRectangle::UpdateData(D3DXVECTOR3 start, D3DXVECTOR3 end)
{
	auto gap = start - end;

	//좌, 우
	if (abs(gap.x) < D3DX_16F_EPSILON)
	{
		m_vertexData[0].position = { start.x, end.y, start.z };
		m_vertexData[1].position = { start.x, start.y, start.z };
		m_vertexData[2].position = { end.x, end.y, end.z };
		m_vertexData[3].position = { end.x, start.y, end.z };
	}
	//상, 하
	else if (abs(gap.y) < D3DX_16F_EPSILON)
	{
		m_vertexData[0].position = { start.x, end.y, end.z };
		m_vertexData[1].position = { start.x, start.y, start.z };
		m_vertexData[2].position = { end.x, end.y, end.z };
		m_vertexData[3].position = { end.x, start.y, start.z };
	}
	//전, 후
	else if (abs(gap.z) < D3DX_16F_EPSILON)
	{
		m_vertexData[0].position = { start.x, end.y, end.z };
		m_vertexData[1].position = { start.x, start.y, start.z };
		m_vertexData[2].position = { end.x, end.y, end.z };
		m_vertexData[3].position = { end.x, start.y, start.z };
	}

	for (auto&& vertex : m_vertexData)
		vertex.color = m_color;
}

void cRectangle::CreateBuffer()
{
	cMeshUtility<VertexC>::CreateVertexBuffer(&m_pVB, m_vertexData);
	cMeshUtility<VertexC>::CreateIndexBuffer(&m_pIB, m_arrIndexData, 6);
}