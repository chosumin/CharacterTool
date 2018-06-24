#pragma once
#include "stdafx.h"

class cModelBoneBuffer : public cCBuffer
{
public:
	cModelBoneBuffer() : cCBuffer(&data, sizeof(data))
	{
		for (int i = 0; i < 128; i++)
			D3DXMatrixIdentity(&data.Bones[i]);
	}

	void SetBones(D3DXMATRIX* m, UINT count)
	{
		memcpy(data.Bones, m, sizeof(D3DXMATRIX) * count);

		for (UINT i = 0; i < count; i++)
			D3DXMatrixTranspose(&data.Bones[i], &data.Bones[i]);
	}

private:
	struct Struct
	{
		D3DXMATRIX Bones[128];
	} data;
};