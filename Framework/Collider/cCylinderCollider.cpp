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

bool cCylinderCollider::IntersectsWith(weak_ptr<iCollidable> other)
{
	if (other.expired())
		return false;

	_cbuffer->Data.Intersect = 0;
	
	bool intersect = other.lock()->IntersectsWithCylinder
	(
		GetTransformedLine(), GetTransformedRadius()
	);

	if(intersect)
		_cbuffer->Data.Intersect = 1;

	return intersect;
}

ContainmentType cCylinderCollider::ContainsRay(D3DXVECTOR3 position, D3DXVECTOR3 direction)
{
	return ContainmentType();
}

ContainmentType cCylinderCollider::ContainsPlane(D3DXVECTOR3 normal, float d)
{
	return ContainmentType();
}

ContainmentType cCylinderCollider::ContainsDot(D3DXVECTOR3 point)
{
	return ContainmentType();
}

ContainmentType cCylinderCollider::ContainsSphere(D3DXVECTOR3 center, float radius)
{
	return ContainmentType();
}

ContainmentType cCylinderCollider::ContainsBox(D3DXVECTOR3 max, D3DXVECTOR3 min)
{
	return ContainmentType();
}

bool cCylinderCollider::IntersectsWithRay(D3DXVECTOR3 position, D3DXVECTOR3 direction)
{
	return false;
}

PlaneIntersectionType cCylinderCollider::IntersectsWithPlane(D3DXVECTOR3 normal, float d)
{
	return PlaneIntersectionType();
}

bool cCylinderCollider::IntersectsWithQuad(const vector<D3DXVECTOR3>& fourPoints)
{
	return false;
}

bool cCylinderCollider::IntersectsWithDot(D3DXVECTOR3 point)
{
	return false;
}

bool cCylinderCollider::IntersectsWithSphere(D3DXVECTOR3 center, float radius)
{
	return true;
}

bool cCylinderCollider::IntersectsWithBox(D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	return false;
}

bool cCylinderCollider::IntersectsWithCylinder(sLine line, float radius)
{
	float dist2 = ClosestPtSegmentSegment(line);

	float radius2 = GetTransformedRadius() +
		radius;

	return dist2 <= radius2 * radius2;
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
	auto world = GetWorldTransform();
	return _radius * world.lock()->GetScaleMatrix()._11;
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