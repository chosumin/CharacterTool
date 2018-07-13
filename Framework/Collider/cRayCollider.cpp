#include "stdafx.h"
#include "cRayCollider.h"
#include "cQuadCollider.h"
#include "cColliderUtility.h"
#include "./Mesh/cRectangle.h"

cRayCollider::cRayCollider(weak_ptr<sTransform> parent, D3DXVECTOR3 origin, D3DXVECTOR3 direction)
	:cCollider(parent)
	,_origin(origin)
	,_direction(direction)
{
}

cRayCollider::~cRayCollider()
{
}

bool cRayCollider::IntersectsWith(weak_ptr<iCollidable> other)
{
	if (other.expired())
		return false;

	return other.lock()->IntersectsWithRay
	(
		GetTransformedOrigin(),			   
		GetTransformedDirection()
	);
}

ContainmentType cRayCollider::ContainsRay(D3DXVECTOR3 position, D3DXVECTOR3 direction)
{
	return ContainmentType();
}

ContainmentType cRayCollider::ContainsPlane(D3DXVECTOR3 normal, float d)
{
	return ContainmentType();
}

ContainmentType cRayCollider::ContainsDot(D3DXVECTOR3 point)
{
	return ContainmentType();
}

ContainmentType cRayCollider::ContainsSphere(D3DXVECTOR3 center, float radius)
{
	return ContainmentType();
}

ContainmentType cRayCollider::ContainsBox(D3DXVECTOR3 max, D3DXVECTOR3 min)
{
	return ContainmentType();
}

bool cRayCollider::IntersectsWithRay(D3DXVECTOR3 position, D3DXVECTOR3 direction)
{
	return false;
}

PlaneIntersectionType cRayCollider::IntersectsWithPlane(D3DXVECTOR3 normal, float d)
{
	return PlaneIntersectionType();
}

bool cRayCollider::IntersectsWithQuad(const cRectangle & rect)
{
	return rect.IntersectWithRay
	(
		GetTransformedOrigin(),
		GetTransformedDirection()
	);
}

bool cRayCollider::IntersectsWithDot(D3DXVECTOR3 point)
{
	return false;
}

bool cRayCollider::IntersectsWithSphere(D3DXVECTOR3 center, float radius)
{
	return false;
}

bool cRayCollider::IntersectsWithBox(D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	if (cColliderUtility::BoxAndRay(max, min, GetTransformedOrigin(), GetTransformedDirection()))
		return true;

	return false;
}

bool cRayCollider::IntersectsWithCylinder(sLine line, float radius)
{
	return false;
}

D3DXVECTOR3 cRayCollider::GetTransformedOrigin()
{
	D3DXVECTOR3 temp;
	D3DXVec3TransformCoord(&temp, &_origin, &GetWorld());

	return temp;
}

D3DXVECTOR3 cRayCollider::GetTransformedDirection()
{
	D3DXVECTOR3 temp;
	D3DXVec3TransformCoord(&temp, &_direction, &GetWorld());

	return temp;
}