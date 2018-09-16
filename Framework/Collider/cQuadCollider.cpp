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

eContainmentType cQuadCollider::Contains(const weak_ptr<iCollidable> & other)
{
	auto colPtr = other.lock();
	if (!colPtr)
		return eContainmentType::Disjoint;

	_cbuffer->Data.Intersect = 0;

	vector<D3DXVECTOR3> vertices;
	_rect->GetVertexVector(vertices);

	Transformed(vertices);

	auto type = colPtr->ContainsQuad(vertices);
	if(type == eContainmentType::Intersects)
		_cbuffer->Data.Intersect = 1;

	return type;
}

eContainmentType cQuadCollider::ContainsRay(const D3DXVECTOR3 & position, const D3DXVECTOR3 & direction)
{
	bool intersect = _rect->IntersectWithRay(position, direction);

	if (intersect)
		_cbuffer->Data.Intersect = 1;
	else
		_cbuffer->Data.Intersect = 0;

	return intersect ? eContainmentType::Intersects : eContainmentType::Disjoint;
}

eContainmentType cQuadCollider::ContainsQuad(const vector<D3DXVECTOR3>& fourPoints)
{
	return eContainmentType();
}

eContainmentType cQuadCollider::ContainsDot(const D3DXVECTOR3 & point)
{
	return eContainmentType();
}

eContainmentType cQuadCollider::ContainsSphere(const D3DXVECTOR3 & center, float radius)
{
	return eContainmentType();
}

eContainmentType cQuadCollider::ContainsBox(const D3DXVECTOR3 & max, const D3DXVECTOR3 & min)
{
	return eContainmentType();
}

eContainmentType cQuadCollider::ContainsCylinder(const sLine & line, float radius)
{
	return eContainmentType();
}

ePlaneIntersectionType cQuadCollider::ContainsPlane(const D3DXVECTOR3 & normal, float d)
{
	return ePlaneIntersectionType();
}

void cQuadCollider::Transformed(OUT vector<D3DXVECTOR3>& vertices)
{
	for (auto&& vertex : vertices)
	{
		D3DXVec3TransformCoord(&vertex, &vertex, &GetWorld());
	}
}
