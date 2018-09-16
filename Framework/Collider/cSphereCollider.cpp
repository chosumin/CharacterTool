#include "stdafx.h"
#include "cSphereCollider.h"
#include "./Helper/cMath.h"
#include "./Mesh/cSphere.h"
#include "./Graphic/ConstBuffer/cColliderBuffer.h"

cSphereCollider::cSphereCollider(weak_ptr<sTransform> parent,
								 D3DXVECTOR3 vCenter, float fRadius)
	: cCollider(parent)
	, _center(vCenter)
	, _radius(fRadius)
{
	_sphere = make_unique<cSphere>(D3DXCOLOR{ 0,1,0,1 }, _center, _radius);
}

cSphereCollider::~cSphereCollider()
{
}

void cSphereCollider::Render()
{
	cCollider::Render();
	_sphere->Render();
}

eContainmentType cSphereCollider::Contains(const weak_ptr<iCollidable>& other)
{
	if (other.expired())
		return eContainmentType::Disjoint;

	_cbuffer->Data.Intersect = 0;

	D3DXVECTOR3 transformed;
	GetTransformedCenter(transformed);

	float tempRadius = GetTransformedRadius();

	auto type = other.lock()->ContainsSphere(transformed,
											 tempRadius);

	if (type == eContainmentType::Intersects)
		_cbuffer->Data.Intersect = 1;

	return type;
}

eContainmentType cSphereCollider::ContainsRay(const D3DXVECTOR3 & position, const D3DXVECTOR3 & direction)
{
	D3DXVECTOR3 transformed;
	GetTransformedCenter(transformed);
	float tempRadius = GetTransformedRadius();

	auto type = cColliderUtility::RayAndSphere(position, direction,
											   transformed,
											   tempRadius);

	_cbuffer->Data.Intersect =
		type == eContainmentType::Disjoint ? 0 : 1;

	return type;
}

eContainmentType cSphereCollider::ContainsQuad(const vector<D3DXVECTOR3>& fourPoints)
{
	return eContainmentType();
}

eContainmentType cSphereCollider::ContainsDot(const D3DXVECTOR3 & point)
{
	return eContainmentType();
}

eContainmentType cSphereCollider::ContainsSphere(const D3DXVECTOR3 & center, float radius)
{
	D3DXVECTOR3 transformed;
	GetTransformedCenter(transformed);

	auto world = GetWorldTransform().lock();
	float tempRadius = _radius * world->GetScaleMatrix()._41;

	float dstSquared = cMath::DistanceSquared(center, transformed);
	float radiusSquared = radius * tempRadius;

	bool intersect = dstSquared <= radiusSquared;

	_cbuffer->Data.Intersect = intersect ? 1 : 0;

	//현재 거리가 반지름 합보다 짧으면 충돌 아니면 충돌X
	return intersect ?
		eContainmentType::Intersects : eContainmentType::Disjoint;
}

eContainmentType cSphereCollider::ContainsBox(const D3DXVECTOR3 & max, const D3DXVECTOR3 & min)
{
	D3DXVECTOR3 transformed;
	D3DXVec3TransformCoord(&transformed, &_center, &GetWorld());

	auto vector3 = cMath::Clamp(transformed, min, max);

	float single = cMath::DistanceSquared(transformed, vector3);

	float tempRadius = _radius * GetWorld()._41;
	if (single <= tempRadius * tempRadius)
	{
		_cbuffer->Data.Intersect = 1;
		return eContainmentType::Intersects;
	}

	_cbuffer->Data.Intersect = 0;
	return eContainmentType::Disjoint;
}

eContainmentType cSphereCollider::ContainsCylinder(const sLine & line, float radius)
{
	D3DXVECTOR3 center;
	GetTransformedCenter(center);

	auto type = cColliderUtility::CylinderAndSphere(line, radius,
													center, GetTransformedRadius());
	if(type == eContainmentType::Intersects)
		_cbuffer->Data.Intersect = 1;
	else
		_cbuffer->Data.Intersect = 0;

	return type;
}

ePlaneIntersectionType cSphereCollider::ContainsPlane(const D3DXVECTOR3 & normal, float d)
{
	return ePlaneIntersectionType();
}

float cSphereCollider::GetTransformedRadius()
{
	auto world = GetWorldTransform().lock();
	return _radius * world->GetScaleMatrix()._11;
}

void cSphereCollider::GetTransformedCenter(OUT D3DXVECTOR3& center)
{
	D3DXVec3TransformCoord(&center, &_center, &GetWorld());
}
