#pragma once
#include "stdafx.h"

enum class ContainmentType
{
	Disjoint, Contains, Intersects
};

enum class PlaneIntersectionType
{
	Front, Back, Intersecting
};

struct sLine
{
	D3DXVECTOR3 start;
	D3DXVECTOR3 end;
};

class iCollidable
{
public:
	virtual ~iCollidable() {}

	virtual ContainmentType ContainsRay(D3DXVECTOR3 position, D3DXVECTOR3 direction) = 0;
	virtual ContainmentType ContainsPlane(D3DXVECTOR3 normal, float d) = 0;
	virtual ContainmentType ContainsDot(D3DXVECTOR3 point) = 0;
	virtual ContainmentType ContainsSphere(D3DXVECTOR3 center, float radius) = 0;
	virtual ContainmentType ContainsBox(D3DXVECTOR3 max, D3DXVECTOR3 min) = 0;

	virtual bool IntersectsWithRay(D3DXVECTOR3 position, D3DXVECTOR3 direction) = 0;
	virtual PlaneIntersectionType IntersectsWithPlane(D3DXVECTOR3 normal, float d) = 0;
	virtual bool IntersectsWithDot(D3DXVECTOR3 point) = 0;
	virtual bool IntersectsWithSphere(D3DXVECTOR3 center, float radius) = 0;
	virtual bool IntersectsWithBox(D3DXVECTOR3 min, D3DXVECTOR3 max) = 0;
	virtual bool IntersectsWithCylinder(sLine line, float radius) = 0;
};