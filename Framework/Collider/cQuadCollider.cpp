#include "stdafx.h"
#include "cQuadCollider.h"
#include "cRayCollider.h"
#include "./Mesh/cRectangle.h"
#include "./Graphic/ConstBuffer/cColliderBuffer.h"

cQuadCollider::cQuadCollider(weak_ptr<sTransform> parent,
							 D3DXVECTOR3 min, D3DXVECTOR3 max,
							 D3DXCOLOR color)
	: cCollider(parent)
	, _min(min)
	, _max(max)
{
	_rect = make_unique<cRectangle>(min, max, color);
}

cQuadCollider::~cQuadCollider()
{
}

void cQuadCollider::Render()
{
	cCollider::Render();
	_rect->Render();
}

bool cQuadCollider::IntersectsWith(weak_ptr<iCollidable> other)
{
	auto colPtr = other.lock();
	if (!colPtr)
		return false;

	_cbuffer->Data.Intersect = 0;

	vector<D3DXVECTOR3> vertices;
	_rect->GetVertexVector(vertices);

	Transformed(vertices);

	bool intersect = colPtr->IntersectsWithQuad(vertices);
	if(intersect)
		_cbuffer->Data.Intersect = 1;
	return intersect;
}

ContainmentType cQuadCollider::ContainsRay(D3DXVECTOR3 position, D3DXVECTOR3 direction)
{
	return ContainmentType();
}

ContainmentType cQuadCollider::ContainsPlane(D3DXVECTOR3 normal, float d)
{
	return ContainmentType();
}

ContainmentType cQuadCollider::ContainsDot(D3DXVECTOR3 point)
{
	return ContainmentType();
}

ContainmentType cQuadCollider::ContainsSphere(D3DXVECTOR3 center, float radius)
{
	return ContainmentType();
}

ContainmentType cQuadCollider::ContainsBox(D3DXVECTOR3 max, D3DXVECTOR3 min)
{
	return ContainmentType();
}

bool cQuadCollider::IntersectsWithRay(D3DXVECTOR3 position, D3DXVECTOR3 direction)
{
	bool intersect = _rect->IntersectWithRay(position, direction);

	if (intersect)
		_cbuffer->Data.Intersect = 1;
	else
		_cbuffer->Data.Intersect = 0;

	return intersect;
}

PlaneIntersectionType cQuadCollider::IntersectsWithPlane(D3DXVECTOR3 normal, float d)
{
	return PlaneIntersectionType();
}

bool cQuadCollider::IntersectsWithQuad(const vector<D3DXVECTOR3>& fourPoints)
{
	return false;
}

bool cQuadCollider::IntersectsWithDot(D3DXVECTOR3 point)
{
	return false;
}

bool cQuadCollider::IntersectsWithSphere(D3DXVECTOR3 center, float radius)
{
	return false;
}

bool cQuadCollider::IntersectsWithBox(D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	return false;
}

bool cQuadCollider::IntersectsWithCylinder(sLine line, float radius)
{
	return false;
}

void cQuadCollider::Transformed(OUT vector<D3DXVECTOR3>& vertices)
{
	for (auto&& vertex : vertices)
	{
		D3DXVec3TransformCoord(&vertex, &vertex, &GetWorld());
	}
}
