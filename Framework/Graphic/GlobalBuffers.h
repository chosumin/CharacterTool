#pragma once
#include "stdafx.h"

class cWorldBuffer : public cCBuffer
{
public:
	cWorldBuffer() : cCBuffer(&Data, sizeof(Struct))
	{
		D3DXMatrixIdentity(&Data.World);
	}

	void SetMatrix(D3DXMATRIX matrix)
	{
		memcpy(&Data.World, &matrix, sizeof(D3DXMATRIX));
		D3DXMatrixTranspose(&Data.World, &Data.World);
	}
private:
	struct Struct
	{
		D3DXMATRIX World;
	};

	Struct Data;
};

class cViewProjectionBuffer : public cCBuffer
{
public:
	cViewProjectionBuffer() : cCBuffer(&Data, sizeof(Struct))
	{
		D3DXMatrixIdentity(&Data.View);
		D3DXMatrixIdentity(&Data.Projection);
	}

	void SetView(D3DXMATRIX& matrix)
	{
		Data.View = matrix;
		D3DXMatrixInverse(&Data.ViewInverse, nullptr, &Data.View);
		D3DXMatrixTranspose(&Data.View, &Data.View);
		D3DXMatrixTranspose(&Data.ViewInverse, &Data.ViewInverse);
	}

	void SetProjection(D3DXMATRIX& matrix)
	{
		Data.Projection = matrix;
		D3DXMatrixTranspose(&Data.Projection, &Data.Projection);
	}
private:
	struct Struct
	{
		D3DXMATRIX View;
		D3DXMATRIX Projection;
		D3DXMATRIX ViewInverse;
	};

	Struct Data;
};

class cLightBuffer : public cCBuffer
{
public:
	cLightBuffer() : cCBuffer(&Data, sizeof(Struct))
	{
		Data.Direction = D3DXVECTOR3(-1, -1, -1);
		Data.Overcast = 1.0f;
		Data.Intensity = 1.0f;
		Data.Color = D3DXCOLOR{ 1,1,1,1 };
	}
public:
	struct Struct
	{
		D3DXVECTOR3 Direction;
		float Padding;

		D3DXCOLOR Color;

		float Overcast;
		float Intensity;
		float Padding2[2];
	};

	Struct Data;
};