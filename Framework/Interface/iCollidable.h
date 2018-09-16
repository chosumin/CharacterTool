#pragma once
#include "stdafx.h"

enum class eContainmentType
{
	Disjoint, Contains, Intersects
};

enum class ePlaneIntersectionType
{
	Front, Back, Intersecting
};

struct sLine
{
	D3DXVECTOR3 start;
	D3DXVECTOR3 end;
};

class cRectangle;
class iCollidable
{
public:
	virtual ~iCollidable() {}

	virtual eContainmentType Contains(const weak_ptr<iCollidable>& other) = 0;

	virtual eContainmentType ContainsRay(const D3DXVECTOR3 & position, const D3DXVECTOR3 & direction) = 0;
	virtual eContainmentType ContainsQuad(const vector<D3DXVECTOR3>& fourPoints) = 0;
	virtual eContainmentType ContainsDot(const D3DXVECTOR3 & point) = 0;
	virtual eContainmentType ContainsSphere(const D3DXVECTOR3 & center, float radius) = 0;
	virtual eContainmentType ContainsBox(const D3DXVECTOR3 & max, const D3DXVECTOR3 & min) = 0;
	virtual eContainmentType ContainsCylinder(const sLine & line, float radius) = 0;
	virtual ePlaneIntersectionType ContainsPlane(const D3DXVECTOR3 & normal, float d) = 0;
};