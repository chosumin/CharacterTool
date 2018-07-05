#include "stdafx.h"
#include "cLine.h"
#include "MeshUtility.h"

cLine::cLine(D3DXCOLOR color, D3DXVECTOR3 axis, float length)
	:mColor(color), mLength(length)
{
	CreateVertex(axis, length);
	RotateAxis(axis);
}

cLine::~cLine()
{
	SAFE_RELEASE(mVB);
}

void cLine::Render()
{
	UINT stride = sizeof(VertexC);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	D3D::GetDC()->Draw(2, 0);
}

void cLine::CreateVertex(D3DXVECTOR3 axis, float length)
{
	const D3DXVECTOR3 apex = { 0,0,-length };

	VertexC vertex;
	vertex.color = mColor;

	//선 정점
	{
		vertex.position = { 0,0,0 }; mVertexData.push_back(vertex);
		vertex.position = apex; mVertexData.push_back(vertex);
	}
}

void cLine::RotateAxis(D3DXVECTOR3 axis)
{
	//축에 따른 정점 위치 변환
	D3DXMATRIX matrix;
	D3DXMatrixIdentity(&matrix);
	float radian = 90.0f * PI / 180.0f;
	if (axis.y > 0.0f)
		D3DXMatrixRotationX(&matrix, radian);
	else if (axis.x > 0.0f)
		D3DXMatrixRotationY(&matrix, -radian);

	for (auto&& vertex : mVertexData)
	{
		D3DXVec3TransformCoord(&vertex.position, &vertex.position, &matrix);
	}

	cMeshUtility<VertexC>::CreateVertexBuffer(&mVB, mVertexData);
}