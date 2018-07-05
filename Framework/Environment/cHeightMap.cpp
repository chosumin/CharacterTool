#include "stdafx.h"
#include "cHeightMap.h"
#include "./Graphic/cMaterial.h"

cHeightMap::cHeightMap(wstring file)
	:cBasicMap(file)
{
	m_pMaterial = new cMaterial(Shader + L"Terrain.hlsl");
	m_pMaterial->SetTextureMap(TextureType::Diffuse, Content + L"Landscape/Dirt.png");
	m_pWorld = new cWorldBuffer();
}

cHeightMap::~cHeightMap()
{
	SAFE_DELETE(m_pMaterial);

	SAFE_DELETE(m_pWorld);
}

void cHeightMap::Update()
{
}

void cHeightMap::Render()
{
	m_pMaterial->Render();
	m_pWorld->SetVSBuffer(1);
	cBasicMap::Render();
}

void cHeightMap::PostRender()
{
}

D3DXVECTOR2 cHeightMap::GetCellNumber(const D3DXVECTOR3& start, const D3DXVECTOR3& direction)
{
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT index0 = (width + 1) * z + x;
			UINT index1 = (width + 1) * (z + 1) + x;
			UINT index2 = (width + 1) * z + x + 1;
			UINT index3 = (width + 1) * (z + 1) + x + 1;

			D3DXVECTOR3 v0 = vertexData[index0].position;
			D3DXVECTOR3 v1 = vertexData[index1].position;
			D3DXVECTOR3 v2 = vertexData[index2].position;
			D3DXVECTOR3 v3 = vertexData[index3].position;

			float u, v, distance;
			if (D3DXIntersectTri(&v0, &v1, &v2, &start, &direction, &u, &v, &distance) == TRUE)
				return{ (float)x, (float)z };

			if (D3DXIntersectTri(&v3, &v1, &v2, &start, &direction, &u, &v, &distance) == TRUE)
				return{ (float)x, (float)z };
		}//for(x)
	}//for(z)

	return{ -1, -1 };
}