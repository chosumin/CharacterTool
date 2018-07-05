#include "stdafx.h"
#include "cQuadCollider.h"
#include "./Mesh/cRectangle.h"
#include "./Graphic/ConstBuffer/cColliderBuffer.h"

cQuadCollider::cQuadCollider(D3DXVECTOR3 min, D3DXVECTOR3 max, D3DXCOLOR color)
{
	_rect = make_unique<cRectangle>(min, max, color);
	
	_shader = cShader::Create(Shader + L"002_Collider.hlsl");
	_cbuffer = make_unique<cColliderBuffer>();
}

cQuadCollider::~cQuadCollider()
{
}

ContainmentType cQuadCollider::ContainsRay(D3DXVECTOR3 position, D3DXVECTOR3 direction)
{
	return ContainmentType();
}

ContainmentType cQuadCollider::ContainsPlane(D3DXVECTOR3 normal, float d)
{
	return ContainmentType();
}

ContainmentType cQuadCollider::ContainsDot(D3DXVECTOR3 point)
{
	return ContainmentType();
}

ContainmentType cQuadCollider::ContainsSphere(D3DXVECTOR3 center, float radius)
{
	return ContainmentType();
}

ContainmentType cQuadCollider::ContainsBox(D3DXVECTOR3 max, D3DXVECTOR3 min)
{
	return ContainmentType();
}

bool cQuadCollider::IntersectsWithRay(D3DXVECTOR3 position, D3DXVECTOR3 direction)
{
	bool intersect = _rect->IntersectWithRay(position, direction);

	if (intersect)
		_cbuffer->Data.Intersect = 1;
	else
		_cbuffer->Data.Intersect = 0;

	return intersect;
}

PlaneIntersectionType cQuadCollider::IntersectsWithPlane(D3DXVECTOR3 normal, float d)
{
	return PlaneIntersectionType();
}

bool cQuadCollider::IntersectsWithDot(D3DXVECTOR3 point)
{
	return false;
}

bool cQuadCollider::IntersectsWithSphere(D3DXVECTOR3 center, float radius)
{
	return false;
}

bool cQuadCollider::IntersectsWithBox(D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	return false;
}

bool cQuadCollider::IntersectsWithCylinder(sLine line, float radius)
{
	return false;
}

void cQuadCollider::Render()
{
	_shader->Render();
	_cbuffer->SetPSBuffer(2);
	_rect->Render();
}

void cQuadCollider::ResetState()
{
	_cbuffer->Data.Intersect = 0;
}
