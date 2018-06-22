#include "stdafx.h"
#include "cArrowCollider.h"
#include "./Polygon/cArrow.h"
#include "./Graphic/ConstBuffer/cColliderBuffer.h"

cArrowCollider::cArrowCollider(float length, D3DXCOLOR color, D3DXVECTOR3 direction, D3DXVECTOR3 min, D3DXVECTOR3 max)
	:cBoxCollider(min, max)
{
	_arrow = make_unique<cArrow>(color, direction, length);
}

cArrowCollider::~cArrowCollider()
{
}

void cArrowCollider::Render()
{
	cBoxCollider::Render();
	_arrow->Render();
}