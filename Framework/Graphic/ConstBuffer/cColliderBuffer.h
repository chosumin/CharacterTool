#pragma once
#include "stdafx.h"

class cColliderBuffer : public cCBuffer
{
public:
	cColliderBuffer() : cCBuffer(&Data, sizeof(Data))
	{
		Data.Intersect = false;
	}

	struct Struct
	{
		UINT Intersect;
		float Padding[3];
	}Data;
};