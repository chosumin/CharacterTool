#include "stdafx.h"
#include "cQuadCollider.h"
#include "cRayCollider.h"
#include "./Mesh/cRectangle.h"
#include "./Graphic/ConstBuffer/cColliderBuffer.h"

cQuadCollider::cQuadCollider(weak_ptr<sTransform> parent,
							 D3DXVECTOR3 min, D3DXVECTOR3 max,
							 D3DXCOLOR color)
	:cCollider(parent, eColliderShape::Quad)
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

void cQuadCollider::ResetState()
{
	_cbuffer->Data.Intersect = 0;
}

ContainmentType cQuadCollider::ContainsRay(weak_ptr<cCollider> other)
{
	return ContainmentType();
}

ContainmentType cQuadCollider::ContainsPlane(weak_ptr<cCollider> other)
{
	return ContainmentType();
}

ContainmentType cQuadCollider::ContainsDot(weak_ptr<cCollider> other)
{
	return ContainmentType();
}

ContainmentType cQuadCollider::ContainsSphere(weak_ptr<cCollider> other)
{
	return ContainmentType();
}

ContainmentType cQuadCollider::ContainsBox(weak_ptr<cCollider> other)
{
	return ContainmentType();
}

bool cQuadCollider::IntersectsWithRay(weak_ptr<cCollider> other)
{
	auto ray = reinterpret_cast<cRayCollider*>(other.lock().get());
	auto position = ray->GetTransformedOrigin();
	auto direction = ray->GetTransformedDirection();
	bool intersect = _rect->IntersectWithRay(position, direction);

	if (intersect)
		_cbuffer->Data.Intersect = 1;
	else
		_cbuffer->Data.Intersect = 0;

	return intersect;
}

bool cQuadCollider::IntersectsWithQuad(weak_ptr<cCollider> other)
{
	return false;
}

bool cQuadCollider::IntersectsWithDot(weak_ptr<cCollider> other)
{
	return false;
}

bool cQuadCollider::IntersectsWithSphere(weak_ptr<cCollider> other)
{
	return false;
}

bool cQuadCollider::IntersectsWithBox(weak_ptr<cCollider> other)
{
	return false;
}

bool cQuadCollider::IntersectsWithCylinder(weak_ptr<cCollider> other)
{
	return false;
}
