#pragma once
#include "stdafx.h"

class cModelBoneBuffer : public cCBuffer
{
public:
	cModelBoneBuffer() : cCBuffer(&data, sizeof(data))
	{
		for (int i = 0; i < 128; i++)
		{
			D3DXMatrixIdentity(&data.Bones[i]);
			D3DXMatrixIdentity(&data.Bones2[i]);
		}
		data.IsPlayAnim = 0;
	}

	void SetBones(D3DXMATRIX* m, UINT count)
	{
		memcpy(data.Bones, m, sizeof(D3DXMATRIX) * count);

		if(count > 128)
			memcpy(data.Bones2, m[128], sizeof(D3DXMATRIX) * (count- 128));
		/*for (UINT i = 0; i < count; i++)
			D3DXMatrixTranspose(&data.Bones[i], &data.Bones[i]);*/
	}

	void SetIsPlayAnim(bool isPlayAnim)
	{
		data.IsPlayAnim = isPlayAnim ? 1 : 0;
	}
private:
	struct Struct
	{
		D3DXMATRIX Bones[128];
		D3DXMATRIX Bones2[128];
		int IsPlayAnim;
		float Padding[3];
	} data;
};