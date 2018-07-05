#include "stdafx.h"
#include "cArrow.h"
#include "MeshUtility.h"

cArrow::cArrow(D3DXCOLOR color, D3DXVECTOR3 axis, float length)
	:mColor(color), mLength(length)
{
	CreateVertex(axis, length);
	RotateAxis(axis);
}

cArrow::~cArrow()
{
	SAFE_RELEASE(mVB);
	SAFE_RELEASE(mIB);
}

void cArrow::Render()
{
	UINT stride = sizeof(VertexC);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	
	//선 그리기
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	D3D::GetDC()->DrawIndexed(2, 0, 0);

	//원뿔 그리기
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D::GetDC()->DrawIndexed(mCount, 2, 3);
}

void cArrow::CreateVertex(D3DXVECTOR3 axis, float length)
{
	const D3DXVECTOR3 apex = { 0,0,-length };

	VertexC vertex;
	vertex.color = mColor;

	//선 정점
	{
		vertex.position = { 0,0,0 }; mVertexData.push_back(vertex);
		vertex.position = apex; mVertexData.push_back(vertex);
	}

	//원뿔 정점
	{
		const int sphereResolution = 30;
		float step = 2.0f * (float)D3DX_PI / sphereResolution;

		for (float i = 0.0f; i < 2 * D3DX_PI; i += step)
		{
			vertex.position = D3DXVECTOR3{ cosf(i) * 0.25f, sinf(i) * 0.25f, -length * 0.75f };
			mVertexData.push_back(vertex);

			vertex.position = apex; 
			mVertexData.push_back(vertex);

			vertex.position = D3DXVECTOR3{ cosf(i + step) * 0.25f, sinf(i + step) * 0.25f, -length * 0.75f };
			mVertexData.push_back(vertex);
		}
	}
}

void cArrow::RotateAxis(D3DXVECTOR3 axis)
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

	UINT size = mVertexData.size();
	mCount = size - 2;

	vector<UINT> indexData(size);
	for (UINT i = 0; i < size; i++)
	{
		indexData[i] = i;
	}

	cMeshUtility<VertexC>::CreateVertexBuffer(&mVB, mVertexData);
	cMeshUtility<VertexC>::CreateIndexBuffer(&mIB, indexData);
}