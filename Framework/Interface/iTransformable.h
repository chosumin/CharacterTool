#pragma once
#include "stdafx.h"
#include "./Transform/sTransform.h"

class iTransformable
{
public:
	virtual ~iTransformable() {}
	virtual void Transform(const D3DXMATRIX& matrix) = 0;
	virtual void Transform(const sTransform& transform) = 0;
};