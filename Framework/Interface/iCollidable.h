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

__interface iCollidable
{
	ContainmentType ContainsRay(D3DXVECTOR3 position, D3DXVECTOR3 direction);
	ContainmentType ContainsPlane(D3DXVECTOR3 normal, float d);
	ContainmentType ContainsDot(D3DXVECTOR3 point);
	ContainmentType ContainsSphere(D3DXVECTOR3 center, float radius);
	ContainmentType ContainsBox(D3DXVECTOR3 max, D3DXVECTOR3 min);

	bool IntersectsWithRay(D3DXVECTOR3 position, D3DXVECTOR3 direction);
	PlaneIntersectionType IntersectsWithPlane(D3DXVECTOR3 normal, float d);
	bool IntersectsWithDot(D3DXVECTOR3 point);
	bool IntersectsWithSphere(D3DXVECTOR3 center, float radius);
	bool IntersectsWithBox(D3DXVECTOR3 min, D3DXVECTOR3 max);
};