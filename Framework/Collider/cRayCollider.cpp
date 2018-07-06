#include "stdafx.h"
#include "cRayCollider.h"
#include "cQuadCollider.h"

cRayCollider::cRayCollider(weak_ptr<sTransform> parent, D3DXVECTOR3 origin, D3DXVECTOR3 direction)
	:cCollider(parent, eColliderShape::Ray)
	,_origin(origin)
	,_direction(direction)
{
}

cRayCollider::~cRayCollider()
{
}

D3DXVECTOR3 cRayCollider::GetTransformedOrigin()
{
	D3DXVECTOR3 temp;
	D3DXVec3TransformCoord(&temp, &_origin, &_world);

	return temp;
}

D3DXVECTOR3 cRayCollider::GetTransformedDirection()
{
	D3DXVECTOR3 temp;
	D3DXVec3TransformCoord(&temp, &_direction, &_world);

	return temp;
}

ContainmentType cRayCollider::ContainsRay(weak_ptr<cCollider> other)
{
	return ContainmentType();
}

ContainmentType cRayCollider::ContainsPlane(weak_ptr<cCollider> other)
{
	return ContainmentType();
}

ContainmentType cRayCollider::ContainsDot(weak_ptr<cCollider> other)
{
	return ContainmentType();
}

ContainmentType cRayCollider::ContainsSphere(weak_ptr<cCollider> other)
{
	return ContainmentType();
}

ContainmentType cRayCollider::ContainsBox(weak_ptr<cCollider> other)
{
	return ContainmentType();
}

bool cRayCollider::IntersectsWithRay(weak_ptr<cCollider> other) {
	return false;
}

bool cRayCollider::IntersectsWithQuad(weak_ptr<cCollider> other)
{
	auto plane = reinterpret_cast<cQuadCollider*>(other.lock().get());

	return plane->IntersectsWithRay(shared_from_this());
}

bool cRayCollider::IntersectsWithDot(weak_ptr<cCollider> other)
{
	return false;
}

bool cRayCollider::IntersectsWithSphere(weak_ptr<cCollider> other)
{
	return false;
}

bool cRayCollider::IntersectsWithBox(weak_ptr<cCollider> other)
{
	return false;
}

bool cRayCollider::IntersectsWithCylinder(weak_ptr<cCollider> other)
{
	return false;
}
