#include "stdafx.h"
#include "cBoxLineCollider.h"
#include "./Polygon/cLine.h"
#include "./Polygon/cBox.h"
#include "./Graphic/ConstBuffer/cColliderBuffer.h"

cBoxLineCollider::cBoxLineCollider(float length, D3DXCOLOR color, D3DXVECTOR3 direction, D3DXVECTOR3 min, D3DXVECTOR3 max)
	:cBoxCollider(min, max)
{
	_line = make_unique<cLine>(color, direction, length);

	D3DXVECTOR3 temp;
	if (direction.x > D3DX_16F_EPSILON)
	{
		temp.x = max.x - (max.x - min.x) * 0.25f;
		temp.y = min.y, temp.z = min.z;
		_box = make_unique<cBox>(temp, max, false, color);
	}
	else if (direction.y > D3DX_16F_EPSILON)
	{
		temp.y = max.y - (max.y - min.y) * 0.25f;
		temp.x = min.x, temp.z = min.z;
		_box = make_unique<cBox>(temp, max, false, color);
	}
	else if (direction.z > D3DX_16F_EPSILON)
	{
		temp.z = min.z + (max.z - min.z) * 0.25f;
		temp.x = max.x, temp.y = max.y;
		_box = make_unique<cBox>(min, temp, false, color);
	}
}

cBoxLineCollider::~cBoxLineCollider()
{
}

void cBoxLineCollider::Render()
{
	cBoxCollider::Render();
	_line->Render();
	_box->Render();
}