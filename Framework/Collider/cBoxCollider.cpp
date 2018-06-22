#include "stdafx.h"
#include "cBoxCollider.h"
#include "./Helper/cMath.h"
#include "./Polygon/cBox.h"
#include "./Graphic/ConstBuffer/cColliderBuffer.h"

cBoxCollider::cBoxCollider(D3DXVECTOR3 min, D3DXVECTOR3 max)
	:_max(max), _min(min)
	, _originMin(min), _originMax(max)
{
	_box = make_unique<cBox>(min, max, true);

	_shader = cShader::Create(Shader + L"002_Collider.hlsl");
	_cbuffer = make_unique < cColliderBuffer>();
}

cBoxCollider::~cBoxCollider()
{
}

ContainmentType cBoxCollider::ContainsRay(D3DXVECTOR3 position, D3DXVECTOR3 direction)
{
	return ContainmentType();
}

bool cBoxCollider::IntersectsWithRay(D3DXVECTOR3 position, D3DXVECTOR3 direction)
{
	float single = 0.0f;
	float single1 = cMath::FloatMaxValue;

	if (!CheckBasis(single, single1, 0, position, direction))
		return false;
	if (!CheckBasis(single, single1, 1, position, direction))
		return false;
	if (!CheckBasis(single, single1, 2, position, direction))
		return false;

	_cbuffer->Data.Intersect = 1;
	return true;
}

ContainmentType cBoxCollider::ContainsPlane(D3DXVECTOR3 normal, float d)
{
	return ContainmentType();
}

PlaneIntersectionType cBoxCollider::IntersectsWithPlane(D3DXVECTOR3 normal, float d)
{
	D3DXVECTOR3 vector3_1;
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
		return PlaneIntersectionType::Intersecting;
}

ContainmentType cBoxCollider::ContainsDot(D3DXVECTOR3 point)
{
	return ContainmentType();
}

bool cBoxCollider::IntersectsWithDot(D3DXVECTOR3 point)
{
	return false;
}

ContainmentType cBoxCollider::ContainsSphere(D3DXVECTOR3 center, float radius)
{
	return ContainmentType();
}

bool cBoxCollider::IntersectsWithSphere(D3DXVECTOR3 center, float radius)
{
	/*Vector3 result1 = Vector3::Clamp(sphere.Center, Min, Max);
	float result2 = Vector3::DistanceSquared(sphere.Center, result1);

	if ((double)result2 <= (double)sphere.Radius * (double)sphere.Radius)
		return true;
	else*/
		return false;
}

ContainmentType cBoxCollider::ContainsBox(D3DXVECTOR3 max, D3DXVECTOR3 min)
{
	return ContainmentType();
}

bool cBoxCollider::IntersectsWithBox(D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	if ((_max.x < min.x || _min.x > max.x) ||
		(_max.y < min.y || _min.y > max.y) ||
		(_max.z < min.z || _min.z > max.z))
		return false;
	
	return true;
}

void cBoxCollider::Render()
{
	_cbuffer->SetPSBuffer(2);
	_shader->Render();
	//_box->Render();
}

void cBoxCollider::ResetState()
{
	_cbuffer->Data.Intersect = 0;
}

bool cBoxCollider::CheckBasis(float& single, float& single1, int i, D3DXVECTOR3 pos, D3DXVECTOR3 dir)
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