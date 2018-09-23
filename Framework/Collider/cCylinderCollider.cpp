#include "stdafx.h"
#include "cCylinderCollider.h"
#include "./Mesh/cCapsule.h"
#include "./Graphic/ConstBuffer/cColliderBuffer.h"
#include "./Transform/sTransform.h"
#include "./Helper/cMath.h"

cCylinderCollider::cCylinderCollider(weak_ptr<sTransform> parentTransform,
									 float height, float radius)
	: cCollider(parentTransform)
	, _height(height)
	, _radius(radius)
{
	_capsule = make_unique<cCapsule>(height, radius, D3DXCOLOR{ 0,1,0,1 });
}

cCylinderCollider::~cCylinderCollider()
{
	
}

void cCylinderCollider::Update()
{
	cCollider::Update();
}

void cCylinderCollider::Render()
{
	cCollider::Render();
	_capsule->Render();
}

eContainmentType cCylinderCollider::Contains(const weak_ptr<iCollidable>& other)
{
	if (other.expired())
		return eContainmentType::Disjoint;

	_cbuffer->Data.Intersect = 0;
	
	auto type = other.lock()->ContainsCylinder
	(
		GetTransformedLine(), GetTransformedRadius()
	);

	if(type == eContainmentType::Intersects)
		_cbuffer->Data.Intersect = 1;

	return type;
}

eContainmentType cCylinderCollider::ContainsRay(const D3DXVECTOR3 & position, const D3DXVECTOR3 & direction)
{
	return eContainmentType();
}

eContainmentType cCylinderCollider::ContainsQuad(const vector<D3DXVECTOR3>& fourPoints)
{
	return eContainmentType();
}

eContainmentType cCylinderCollider::ContainsDot(const D3DXVECTOR3 & point)
{
	return eContainmentType();
}

eContainmentType cCylinderCollider::ContainsSphere(const D3DXVECTOR3 & center, float radius)
{
	auto type = cColliderUtility::CylinderAndSphere(
		GetTransformedLine(),
		GetTransformedRadius(),
		center, radius);

	_cbuffer->Data.Intersect = type == eContainmentType::Disjoint ? 0 : 1;

	return type;
}

eContainmentType cCylinderCollider::ContainsBox(const D3DXVECTOR3 & max, const D3DXVECTOR3 & min)
{
	return eContainmentType();
}

eContainmentType cCylinderCollider::ContainsCylinder(const sLine & line, float radius)
{
	float dist2 = ClosestPtSegmentSegment(line);

	float radius2 = GetTransformedRadius() +
		radius;

	bool intersect = dist2 <= radius2 * radius2;
	
	_cbuffer->Data.Intersect = intersect ? 1 : 0;

	return intersect ? eContainmentType::Intersects : eContainmentType::Disjoint;
}

ePlaneIntersectionType cCylinderCollider::ContainsPlane(const D3DXVECTOR3 & normal, float d)
{
	return ePlaneIntersectionType();
}

sLine cCylinderCollider::GetTransformedLine() const
{
	sLine line;
	line.start = { 0, _height * 0.5f, 0 };
	line.end = { 0, -_height * 0.5f, 0 };

	D3DXMATRIX world;
	D3DXVec3TransformCoord(&line.start, &line.start, &GetWorld());
	D3DXVec3TransformCoord(&line.end, &line.end, &GetWorld());

	return line;
}

float cCylinderCollider::GetTransformedRadius() const
{
	auto& world = GetWorldTransform();
	return _radius * world->GetScaleMatrix()._11;
}

float cCylinderCollider::ClosestPtSegmentSegment(sLine line2)
{
	auto line1 = GetTransformedLine();
	D3DXVECTOR3 d1 = line1.start - line1.end;
	D3DXVECTOR3 d2 = line2.start - line2.end;
	D3DXVECTOR3 r = line1.end - line2.end;
	float a = D3DXVec3Dot(&d1, &d1);
	float e = D3DXVec3Dot(&d2, &d2);
	float f = D3DXVec3Dot(&d2, &r);

	float s, t;
	D3DXVECTOR3 c1, c2;
	if (a <= 0.001f && e <= 0.001f)
	{
		s = t = 0.0f;
		c1 = line1.end;
		c2 = line2.end;
		return D3DXVec3Dot(&(c1 - c2), &(c1 - c2));
	}

	if (a <= 0.001f)
	{
		s = 0.0f;
		t = f / e;
		return cMath::Clamp(t, 0.0f, 1.0f);
	}
	else
	{
		float c = D3DXVec3Dot(&d1, &r);

		if (e <= 0.001f)
		{
			t = 0.0f;
			s = cMath::Clamp(-c / a, 0.0f, 1.0f);
		}
		else
		{
			float b = D3DXVec3Dot(&d1, &d2);
			float denom = a*e - b*b;

			if (denom != 0.0f)
			{
				s = cMath::Clamp((b*f - c*e) / denom, 0.0f, 1.0f);
			}
			else s = 0.0f;

			t = (b*s + f) / e;

			if (t < 0.0f)
			{
				t = 0.0f;
				s = cMath::Clamp(-c / a, 0.0f, 1.0f);
			}
			else if (t > 1.0f)
			{
				t = 1.0f;
				s = cMath::Clamp((b - c) / a, 0.0f, 1.0f);
			}
		}
	}

	c1 = line1.end + d1 * s;
	c2 = line2.end + d2 * t;
	return D3DXVec3Dot(&(c1 - c2), &(c1 - c2));
}