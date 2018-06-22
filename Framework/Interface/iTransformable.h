#pragma once
#include "stdafx.h"
#include "./Transform/sTransform.h"

__interface iTransformable
{
	void Transform(const D3DXMATRIX& matrix);
	void Transform(const sTransform& transform);
};