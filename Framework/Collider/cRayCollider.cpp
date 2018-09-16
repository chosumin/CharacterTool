#include "stdafx.h"
#include "cRayCollider.h"
#include "cQuadCollider.h"
#include "./Graphic/ConstBuffer/cColliderBuffer.h"
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

eContainmentType cRayCollider::Contains(const weak_ptr<iCollidable>& other)
{
	if (other.expired())
		return eContainmentType::Disjoint;

	return other.lock()->ContainsRay
	(
		GetTransformedOrigin(),			   
		GetTransformedDirection()
	);
}

eContainmentType cRayCollider::ContainsRay(const D3DXVECTOR3 & position, const D3DXVECTOR3 & direction)
{
	return eContainmentType();
}

eContainmentType cRayCollider::ContainsQuad(const vector<D3DXVECTOR3>& fourPoints)
{
	return cColliderUtility::RayAndQuad(fourPoints,
										GetTransformedOrigin(),
										GetTransformedDirection());
}

eContainmentType cRayCollider::ContainsDot(const D3DXVECTOR3 & point)
{
	return eContainmentType();
}

eContainmentType cRayCollider::ContainsSphere(const D3DXVECTOR3 & center, float radius)
{
	auto type = cColliderUtility::RayAndSphere(GetTransformedOrigin(), GetTransformedDirection(),
											   center,
											   radius);

	_cbuffer->Data.Intersect =
		type == eContainmentType::Disjoint ? 0 : 1;

	return type;
}

eContainmentType cRayCollider::ContainsBox(const D3DXVECTOR3 & max, const D3DXVECTOR3 & min)
{
	return cColliderUtility::BoxAndRay(max, min,
									   GetTransformedOrigin(),
									   GetTransformedDirection());
}

eContainmentType cRayCollider::ContainsCylinder(const sLine & line, float radius)
{
	return eContainmentType();
}

ePlaneIntersectionType cRayCollider::ContainsPlane(const D3DXVECTOR3 & normal, float d)
{
	return ePlaneIntersectionType();
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