#pragma once
#include "stdafx.h"

class cModelBoneBuffer : public cCBuffer
{
public:
	cModelBoneBuffer() : cCBuffer(&data, sizeof(data))
	{
		for (int i = 0; i < 256; i++)
		{
			D3DXMatrixIdentity(&data.Bones[i]);
		}
		data.IsPlayAnim = 0;
	}

	void SetBones(D3DXMATRIX* m, UINT count)
	{
		memcpy(data.Bones, m, sizeof(D3DXMATRIX) * count);
	}

	void SetIsPlayAnim(bool isPlayAnim)
	{
		data.IsPlayAnim = isPlayAnim ? 1 : 0;
	}
private:
	struct Struct
	{
		D3DXMATRIX Bones[256];
		int IsPlayAnim;
		float Padding[3];
	} data;
};