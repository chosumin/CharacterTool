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

bool cSphereCollider::IntersectsWith(weak_ptr<iCollidable> other)
{
	if (other.expired())
		return false;

	_cbuffer->Data.Intersect = 0;

	D3DXVECTOR3 transformed;
	D3DXVec3TransformCoord(&transformed, &_center, &GetWorld());

	auto world = GetWorldTransform().lock();
	float tempRadius = _radius * world->GetScaleMatrix()._41;

	bool intersect = other.lock()->IntersectsWithSphere(transformed, tempRadius);

	if (intersect)
		_cbuffer->Data.Intersect = 1;

	return intersect;
}

ContainmentType cSphereCollider::ContainsRay(D3DXVECTOR3 position, D3DXVECTOR3 direction)
{
	return ContainmentType();
}

ContainmentType cSphereCollider::ContainsPlane(D3DXVECTOR3 normal, float d)
{
	return ContainmentType();
}

ContainmentType cSphereCollider::ContainsDot(D3DXVECTOR3 point)
{
	return ContainmentType();
}

ContainmentType cSphereCollider::ContainsSphere(D3DXVECTOR3 center, float radius)
{
	return ContainmentType();
}

ContainmentType cSphereCollider::ContainsBox(D3DXVECTOR3 max, D3DXVECTOR3 min)
{
	return ContainmentType();
}

bool cSphereCollider::IntersectsWithRay(D3DXVECTOR3 position, D3DXVECTOR3 direction)
{
	return false;
}

bool cSphereCollider::IntersectsWithQuad(const vector<D3DXVECTOR3>& fourPoints)
{
	return false;
}

PlaneIntersectionType cSphereCollider::IntersectsWithPlane(D3DXVECTOR3 normal, float d)
{
	return PlaneIntersectionType();
}

bool cSphereCollider::IntersectsWithDot(D3DXVECTOR3 point)
{
	return false;
}

bool cSphereCollider::IntersectsWithSphere(D3DXVECTOR3 center, float radius)
{
	D3DXVECTOR3 transformed;
	D3DXVec3TransformCoord(&transformed, &_center, &GetWorld());

	auto world = GetWorldTransform().lock();
	float tempRadius = _radius * world->GetScaleMatrix()._41;

	float dstSquared = cMath::DistanceSquared(center, transformed);
	float radiusSquared = radius * tempRadius;

	return dstSquared <= radiusSquared;
}

bool cSphereCollider::IntersectsWithBox(D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	D3DXVECTOR3 transformed;
	D3DXVec3TransformCoord(&transformed, &_center, &GetWorld());

	auto vector3 = cMath::Clamp(transformed, min, max);

	float single = cMath::DistanceSquared(transformed, vector3);

	float tempRadius = _radius * GetWorld()._41;
	if (single <= tempRadius * tempRadius)
	{
		_cbuffer->Data.Intersect = 1;
		return true;
	}

	_cbuffer->Data.Intersect = 0;
	return false;
}

bool cSphereCollider::IntersectsWithCylinder(sLine line, float radius)
{
	return false;
}