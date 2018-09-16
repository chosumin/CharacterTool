#include "stdafx.h"
#include "cBoxCollider.h"
#include "cRayCollider.h"
#include "./Helper/cMath.h"
#include "./Mesh/cBox.h"
#include "./Graphic/ConstBuffer/cColliderBuffer.h"
#include "./Transform/sTransform.h"

cBoxCollider::cBoxCollider(weak_ptr<sTransform> parent,
						   D3DXVECTOR3 min, D3DXVECTOR3 max)
	:cCollider(parent),
	 _max(max), _min(min)
	,_originMin(min), _originMax(max)
{
	_box = make_unique<cBox>(min, max, true, D3DXCOLOR{ 0,1,0,1 });
}

cBoxCollider::~cBoxCollider()
{
}

void cBoxCollider::Render()
{
	cCollider::Render();

	_box->Render();
}

void cBoxCollider::RenderGizmo()
{
	cCollider::Render();
}

eContainmentType cBoxCollider::Contains(const weak_ptr<iCollidable>& other)
{
	if(other.expired())
		return eContainmentType::Disjoint;

	_cbuffer->Data.Intersect = 0;

	D3DXVECTOR3 transformedMax, transformedMin;
	D3DXVec3TransformCoord(&transformedMax, &_max, &GetWorld());
	D3DXVec3TransformCoord(&transformedMin, &_min, &GetWorld());

	auto type = other.lock()->ContainsBox(transformedMin,
										  transformedMax);

	if (type == eContainmentType::Intersects)
		_cbuffer->Data.Intersect = 1;

	return type;
}

eContainmentType cBoxCollider::ContainsRay(const D3DXVECTOR3 & position, const D3DXVECTOR3 & direction)
{
	return cColliderUtility::BoxAndRay(_max, _min, position, direction);
}

eContainmentType cBoxCollider::ContainsQuad(const vector<D3DXVECTOR3>& fourPoints)
{
	/*D3DXVECTOR3 vector3_1;
	vector3_1.x = normal.x >= 0.0 ? _min.x : _max.x;
	vector3_1.y = normal.y >= 0.0 ? _min.y : _max.y;
	vector3_1.z = normal.z >= 0.0 ? _min.z : _max.z;

	D3DXVECTOR3 vector3_2;
	vector3_2.x = normal.x >= 0.0 ? _max.x : _min.x;
	vector3_2.y = normal.y >= 0.0 ? _max.y : _min.y;
	vector3_2.z = normal.z >= 0.0 ? _max.z : _min.z;

	if (normal.x * vector3_1.x + normal.y * vector3_1.y + normal.z * vector3_1.z + d > 0.0)
	return PlaneIntersectionType::Front;
	else if (normal.x * vector3_2.x + normal.y * vector3_2.y + normal.z * vector3_2.z + d < 0.0)
	return PlaneIntersectionType::Back;
	else
	return PlaneIntersectionType::Intersecting;*/
	return eContainmentType();
}

eContainmentType cBoxCollider::ContainsDot(const D3DXVECTOR3 & point)
{
	return eContainmentType();
}

eContainmentType cBoxCollider::ContainsSphere(const D3DXVECTOR3 & center, float radius)
{
	D3DXVECTOR3 transformedMax, transformedMin;
	D3DXVec3TransformCoord(&transformedMax, &_max, &GetWorld());
	D3DXVec3TransformCoord(&transformedMin, &_min, &GetWorld());

	auto vector3 = cMath::Clamp(center, transformedMin, transformedMax);

	float single = cMath::DistanceSquared(center, vector3);

	if (single <= radius * radius)
	{
		return eContainmentType::Intersects;
	}

	return eContainmentType::Disjoint;
}

eContainmentType cBoxCollider::ContainsBox(const D3DXVECTOR3 & max, const D3DXVECTOR3 & min)
{
	//todo : OBB·Î ±¸Çö
	/*if ((_max.x < min.x || _min.x > max.x) ||
	(_max.y < min.y || _min.y > max.y) ||
	(_max.z < min.z || _min.z > max.z))
	return false;*/
	return eContainmentType();
}

eContainmentType cBoxCollider::ContainsCylinder(const sLine & line, float radius)
{
	return eContainmentType();
}

ePlaneIntersectionType cBoxCollider::ContainsPlane(const D3DXVECTOR3 & normal, float d)
{
	return ePlaneIntersectionType();
}

bool cBoxCollider::CheckBasis(float& single, float& single1, int i, D3DXVECTOR3 pos, D3DXVECTOR3 dir) const
{
	if (abs(dir[i]) >= 1E-06f)
	{
		float x = 1.0f / dir[i];
		float x1 = (_min[i] - pos[i]) * x;
		float x2 = (_max[i] - pos[i]) * x;
		if (x1 > x2)
		{
			float single2 = x1;
			x1 = x2;
			x2 = single2;
		}
		single = max(x1, single);
		single1 = min(x2, single1);
		if (single > single1)
		{
			_cbuffer->Data.Intersect = 0;
			return false;
		}
	}
	else if (pos[i] < _min[i] || pos[i] > _max[i])
	{
		_cbuffer->Data.Intersect = 0;
		return false;
	}

	return true;
}