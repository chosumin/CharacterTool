#include "stdafx.h"
#include "cCapsule.h"

cCapsule::cCapsule(float height, float radius, D3DXCOLOR color)
	:_color(color)
{
	CreateVertex(height, radius, color);
	CreateIndices();

	cMeshUtility<VertexC>::CreateVertexBuffer(&_mesh.VertexBuffer, _mesh.Vertices);
	cMeshUtility<VertexC>::CreateIndexBuffer(&_mesh.IndexBuffer, _mesh.Indices);
}

cCapsule::~cCapsule()
{
	SAFE_RELEASE(_mesh.VertexBuffer);
	SAFE_RELEASE(_mesh.IndexBuffer);
}

void cCapsule::Render()
{
	cMeshUtility<VertexC>::Render(_mesh, D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void cCapsule::CreateVertex(float height, float radius, D3DXCOLOR color)
{
	VertexC vertex;
	vertex.color = color;

	float rotY = 0;
	//원 2개
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			rotY = PI * 2 / 32 * j;
			if (i == 0)
				vertex.position = D3DXVECTOR3(sinf(rotY) * radius, height / 2, cosf(rotY) * radius);
			else
				vertex.position = D3DXVECTOR3(sinf(rotY) * radius, -height / 2, cosf(rotY) * radius);

			_mesh.Vertices.push_back(vertex);
		}
	}

	//반원 4개
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 32; j++)
		{
			if (j >= 17) break;

			rotY = PI * 2 / 32 * j;

			switch (i)
			{
				case 0:
					vertex.position = D3DXVECTOR3(0, cosf(rotY - PI / 2) / 2 + radius, sinf(rotY - PI / 2) / 2);
					break;
				case 1:
					vertex.position = D3DXVECTOR3(sinf(rotY - PI / 2) / 2, cosf(rotY - PI / 2) / 2 + radius, 0);
					break;
				case 2:
					vertex.position = D3DXVECTOR3(0, cosf(rotY + PI / 2) / 2 - radius, sinf(rotY + PI / 2) / 2);
					break;
				case 3:
					vertex.position = D3DXVECTOR3(sinf(rotY + PI / 2) / 2, cosf(rotY + PI / 2) / 2 - radius, 0);
					break;
			}

			_mesh.Vertices.push_back(vertex);
		}
	}

	//선 4개
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (j == 0) rotY = height / 2;
			else rotY = -height / 2;
			switch (i)
			{
				case 0: vertex.position = D3DXVECTOR3(0, rotY, radius); break;
				case 1: vertex.position = D3DXVECTOR3(radius, rotY, 0); break;
				case 2: vertex.position = D3DXVECTOR3(0, rotY, -radius); break;
				case 3: vertex.position = D3DXVECTOR3(-radius, rotY, 0); break;
			}
			_mesh.Vertices.push_back(vertex);
		}
	}
}

void cCapsule::CreateIndices()
{
	int start = 0;
	int index = 64;
	//원 2개
	for (int i = start; i < index; i++, start++)
	{
		if (i % 32 == 31)
		{
			_mesh.Indices.push_back(i);
			_mesh.Indices.push_back(i - 31);
		}
		else
		{
			_mesh.Indices.push_back(i);
			_mesh.Indices.push_back(i + 1);
		}
	}

	//반원 4개
	index += 17 * 4;
	for (int i = start, j = 0; i < index; i++, j++, start++)
	{
		if (j % 17 == 16) continue;

		_mesh.Indices.push_back(i);
		_mesh.Indices.push_back(i + 1);
	}

	//선 4개
	index += 8;
	for (int i = start; i < index; i++)
		_mesh.Indices.push_back(i);
}
